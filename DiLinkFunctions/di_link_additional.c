/*
Code is taken partially from: 
https://github.com/magoroku15/DisplayLink-driver-for-android
https://github.com/floe/tubecable
https://sven.killig.de/openwrt/slugterm_dl.html
https://github.com/torvalds/linux/blob/master/drivers/video/fbdev/udlfb.c
*/


/* Includes ------------------------------------------------------------------*/
#include "di_link_send_data.h"
#include "di_link_additional.h"
#include "usbh_di_link.h"
#include "image1.h"
#include "image2.h"

extern uint8_t di_link_data_buf[DL_TX_BUFF_SIZE];
volatile uint16_t color = 0;
volatile uint32_t duration_ms = 0;//debug

static uint8_t *displaylink_gfx_command(
  uint8_t *bufptr, uint8_t cmd, int addr, uint8_t count);
static uint8_t *displaylink_gfx_rle(
  uint8_t *bufptr, int addr, uint8_t count, rle_word* rs);
static uint8_t *displaylink_gfx_write(
  uint8_t *bufptr, int addr, uint16_t count, uint8_t* data);
uint8_t *displaylink_draw_line16_bit(
  uint8_t *bufptr, uint16_t x, uint16_t y, uint16_t length_pix, uint8_t* data);

//#define HTONS(n) (((((unsigned short)(n) & 0xFF)) << 8) | (((unsigned short)(n) & 0xFF00) >> 8))
#define HTONS(n) __REV16(n)

//*****************************************************************************

#pragma inline=forced
 uint8_t *displaylink_insert_addr(uint8_t *bufptr, uint32_t address) 
{
  *bufptr++ = (uint8_t)((address >> 16) & 0xFF);
  *bufptr++ = (uint8_t)((address >>  8) & 0xFF);
  *bufptr++ = (uint8_t)((address      ) & 0xFF);
  return bufptr;
}

//Test function - send image to USB. Long duration!
void test_fill(USBH_HandleTypeDef *phost)
{
  DI_LINK_HandleTypeDef *DI_LINK_Handle = 
    (DI_LINK_HandleTypeDef *) phost->pActiveClass->pData;
  if (DI_LINK_Handle->state != DI_LINK_IDLE_STATE)
    return;

  DI_LINK_FillDisplayColor16(phost, RGB_24to16(0, 0, 255));
  uint32_t start_ms = HAL_GetTick();
  //Image is really 320x240
  //DI_LINK_DrawImage16bit(phost, 0, 0, 320, 240, (uint8_t*)image2_map);
  DI_LINK_DrawRLEImage16bit(phost, 0, 0, 320, 240, (uint8_t*)image1_map);
  
  duration_ms = HAL_GetTick()- start_ms;
  //DI_LINK_testing_draw(phost);
    
  osDelay(1000);
}

//Draw line of pixels
void DI_LINK_testing_draw(USBH_HandleTypeDef *phost)
{
  DI_LINK_HandleTypeDef *DI_LINK_Handle = 
    (DI_LINK_HandleTypeDef *) phost->pActiveClass->pData;
  uint8_t *pos = &di_link_data_buf[0];
  uint32_t length;
  
  for (uint16_t i = 0; i < 200; i++)
    pos = displaylink_draw_pixel_16_bit(pos, i, i, HTONS(RGB_24to16(0, 0, 255)));

  length = (uint32_t)(pos - di_link_data_buf);

  USBH_DI_LINK_Transmit(phost, di_link_data_buf, length);
  while (DI_LINK_Handle->state != DI_LINK_IDLE_STATE)
  {
    osDelay(1);
  }
}


//Fill all display with one color
void DI_LINK_FillDisplayColor16(USBH_HandleTypeDef *phost, uint16_t color)
{
  DI_LINK_HandleTypeDef *DI_LINK_Handle = (DI_LINK_HandleTypeDef *) phost->pActiveClass->pData;
  uint8_t *pos = &di_link_data_buf[0];
  uint32_t pixels_left = XRES * YRES;
  uint32_t address = 0;//in bytes
  uint32_t length = 0;//data in tx buffer - in bytes
  
  rle_word color_w = { 0x00, 0x0000 };
  color_w.value = HTONS(color);//
  
  while (pixels_left > 256)
  {
    pos = displaylink_gfx_rle(pos, address, 0x00, &color_w ); //write 256 pixels
    address += 256 * 2;
    pixels_left -= 256;
    
    length = (uint32_t)(pos - di_link_data_buf);
    if (length > DL_TX_UPPER_LEVEL)
    {
      pos = displaylink_sync_command(pos);
      USBH_DI_LINK_Transmit(phost, di_link_data_buf, length);
      while (DI_LINK_Handle->state != DI_LINK_IDLE_STATE)
        osDelay(1);
      pos = &di_link_data_buf[0];
    }
  }
  length = (uint32_t)(pos - di_link_data_buf);
  if (length > 0)
  {
    USBH_DI_LINK_Transmit(phost, di_link_data_buf, length);
      while (DI_LINK_Handle->state != DI_LINK_IDLE_STATE)
        osDelay(1);
  }
}


//Draw line without encoding
//bufptr - command buffer
//length_pix - line length
//data - image data, 2 bytes per pixel, little-endian
uint8_t *displaylink_draw_line16_bit(
  uint8_t *bufptr, uint16_t x, uint16_t y, uint16_t length_pix, uint8_t* data)
{
  int start_addr = (y * XRES + x) * 2;
  uint16_t words_to_write = 0;
  if ((x + length_pix) >= XRES)
    length_pix = XRES - x - 1;
  if (y >= YRES)
    return bufptr;
      
  while (length_pix > 0)
  {
    if (length_pix > 256)
    {
      length_pix -= 256;
      words_to_write = 256;
    }
    else
    {
      words_to_write = length_pix;
      length_pix = 0;
    }
    
    bufptr = displaylink_gfx_write(bufptr, start_addr, words_to_write, data);
    start_addr += words_to_write * 2;
    data += words_to_write*2;
  }
  return bufptr;
}

//Draw pixel of certain color at given position
//bufptr - command buffer
//color - Big-endian
uint8_t *displaylink_draw_pixel_16_bit(uint8_t *bufptr, uint16_t x, uint16_t y, uint16_t color)
{
  if ((x >= XRES) || (y >= YRES))
    return bufptr;
  int start_addr = (y * XRES + x) * 2;
  bufptr = displaylink_gfx_write(bufptr, start_addr, 1, (uint8_t*)&color);
  return bufptr;
}


static uint8_t *displaylink_gfx_command(uint8_t *bufptr, uint8_t cmd, int addr, uint8_t count) //add 6
{
  *bufptr++ = DL_SOC;
  *bufptr++ = cmd;
  bufptr = displaylink_insert_addr(bufptr, addr);//add 3
  *bufptr++ = count;
  return bufptr;
}

//Write 16-bit words without compressing
//addr - address in DisplayLink
//count - in words!
static uint8_t *displaylink_gfx_write(uint8_t *bufptr, int addr, uint16_t count, uint8_t* data) 
{
  if (count > 256)
    return bufptr;//error
  uint16_t pcount = count * 2; //in bytes
  bufptr = displaylink_gfx_command(bufptr, DL_GFX_WRITE | DL_GFX_WORD, addr, count); //count==256 -> 0 by compiler rounding
  memcpy(bufptr, data, pcount);
  bufptr += pcount;
  return bufptr;
}

//Write with compressing
static uint8_t *displaylink_gfx_rle(uint8_t *bufptr, int addr, uint8_t count, rle_word* rs ) 
{
  bufptr = displaylink_gfx_command(bufptr, DL_GFX_RLE | DL_GFX_WORD, addr, count );
  
  int pcount = (count == 0) ? 256 : count;
  int i = 0;
  rle_word* cur = rs;
  
  while (i < pcount)
  {
    *bufptr++ = cur->count;
    //orig. file - > MSB than LSB
    *bufptr++ = ((cur->value     ) & 0xFF);
    *bufptr++ = ((cur->value >> 8) & 0xFF);
    
    i += (cur->count == 0) ? 256: cur->count;
    cur++;
  }
  return bufptr;
}