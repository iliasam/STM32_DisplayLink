/*
Code is taken partially from: 
https://github.com/magoroku15/DisplayLink-driver-for-android
https://github.com/floe/tubecable
https://sven.killig.de/openwrt/slugterm_dl.html
https://github.com/torvalds/linux/blob/master/drivers/video/fbdev/udlfb.c

Here we have function: 
* USBH_DI_LINK_SendConfiguration - called during USB init
* DI_LINK_DrawRLEImage16bit - send image to DisplayLink with RLE encoding
*/


/* Includes ------------------------------------------------------------------*/
#include "di_link_send_data.h"
#include "usbh_di_link.h"

uint8_t *displaylink_set_registers(uint8_t *bufptr, uint8_t* values, uint8_t size);
uint8_t *displaylink_set_register(uint8_t *bufptr, uint8_t reg, uint8_t val);

uint8_t *displaylink_draw_rle_line16_bit(
  uint8_t *bufptr, uint16_t x, uint16_t y, uint16_t length_pix, uint8_t* data);
uint8_t *displaylink_set_address(uint8_t *bufptr, uint8_t reg, int address);

static void dlfb_compress_hline(
  uint16_t **pixel_start_ptr,
  uint16_t *const pixel_end,
  uint32_t *device_address_ptr,
  uint8_t **command_buffer_ptr,
  uint8_t *const cmd_buffer_end,
  unsigned long back_buffer_offset);
                                             
const uint8_t dl_register_init_640x480[]   = 
// col   maybe horizontal clock   maybe vertical clock                                             xres                                              yres
{ 0x00,  0x99, 0x30, 0x26, 0x94,  0x60, 0xa9, 0xce, 0x60,  0x07, 0xb3, 0x0f, 0x79, 0xff, 0xff,  0x02, 0x80,  0x83, 0xbc, 0xff, 0xfc, 0xff, 0xff,  0x01, 0xe0,  0x01, 0x02,  0xab, 0x13 };

//For RLE encoding
#define RLX_HEADER_BYTES        7
#define MIN_RLX_PIX_BYTES       4
#define MIN_RLX_CMD_BYTES       (RLX_HEADER_BYTES + MIN_RLX_PIX_BYTES)
#define MAX_CMD_PIXELS          255
#define BPP                     2
#define min3(x, y, z) (x < y ? (x < z ? x : z) : (y < z ? y : z))

//Commands buffer that is send by USB
uint8_t di_link_data_buf[DL_TX_BUFF_SIZE];

//*****************************************************************************

#pragma inline=forced
void put_unaligned_be16(uint16_t val, uint8_t *p)
{
  *p++ = val >> 8;
  *p++ = val;
}

// Called during initialization
USBH_StatusTypeDef USBH_DI_LINK_SendConfiguration(USBH_HandleTypeDef *phost)
{
  USBH_StatusTypeDef status = USBH_FAIL;
  
  uint8_t* pos = &di_link_data_buf[0];
  
  pos = displaylink_set_registers(
      pos, (uint8_t*)dl_register_init_640x480, sizeof(dl_register_init_640x480));
  
  // offset 1byte is to make little-endian images suitable
  pos = displaylink_set_offsets(pos, 0x000000, 0x000A00, 0x555555, 0x000500 );
  pos = displaylink_set_register(pos, DL_REG_BLANK_SCREEN, 0x00 ); // enable output
  pos = displaylink_set_register(pos, DL_REG_SYNC, 0xFF );
  pos = displaylink_sync_command(pos);
  
  DI_LINK_HandleTypeDef *DI_LINK_Handle = (DI_LINK_HandleTypeDef *) phost->pActiveClass->pData;
  uint16_t length = (uint16_t)(pos - di_link_data_buf);
  USBH_DI_LINK_Transmit(phost, di_link_data_buf, length);//send will be in DI_LINK_ProcessTransmission()
  //We can't send other data until settings will be send
  DI_LINK_Handle->WaitForSettings = 1;
  return status;
}

//Draw image with RLE encoding
//Image data must be in Little-endian format
void DI_LINK_DrawRLEImage16bit(USBH_HandleTypeDef *phost, 
  uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint8_t* data)
{
  while (phost->pActiveClass == NULL)
  {
    osThreadYield();
  }
  
  //now DI_LINK_Handle is correct
  DI_LINK_HandleTypeDef *DI_LINK_Handle = (DI_LINK_HandleTypeDef *) phost->pActiveClass->pData;
  while (DI_LINK_Handle->state != DI_LINK_IDLE_STATE)
  {
    osThreadYield();
  }
  
  uint8_t *pos = &di_link_data_buf[0];
  uint32_t length;
  
  for (uint16_t line_y = y; line_y < (y + height); line_y++)
  {
    pos = displaylink_draw_rle_line16_bit(pos, x, line_y, width, data);
    data += width * 2;//width in words
    length = (uint32_t)(pos - di_link_data_buf);
    if (length > DL_TX_UPPER_LEVEL)
    {
      pos = displaylink_sync_command(pos);
      USBH_DI_LINK_Transmit(phost, di_link_data_buf, length);//not blocking, see DI_LINK_ProcessTransmission
      while (DI_LINK_Handle->state != DI_LINK_IDLE_STATE)
      {
        //osDelay(1);
        osThreadYield();
      }
      pos = &di_link_data_buf[0];
    }
  }
  pos = displaylink_sync_command(pos);
  length = (uint32_t)(pos - di_link_data_buf);
  USBH_DI_LINK_Transmit(phost, di_link_data_buf, length);
}

//Draw line with RLE encoding (really - just place data to command buffer)
//bufptr - command buffer
//length_pix - line length
//data - image data, 2 bytes per pixel, little-endian
uint8_t *displaylink_draw_rle_line16_bit(
  uint8_t *bufptr, uint16_t x, uint16_t y, uint16_t length_pix, uint8_t* data)
{
  uint32_t start_addr = (y * XRES + x) * 2;
  if ((x + length_pix) >= XRES)
    length_pix = XRES - x - 1;
  if (y >= YRES)
    return bufptr;
  
  //unsigned long back_buffer_offset = 0;
  uint16_t* pixel_ptr = (uint16_t*)data;
  uint16_t* pixel_end = (uint16_t*)(&data[length_pix * 2]);
  
  dlfb_compress_hline(
    &pixel_ptr, pixel_end,
    &start_addr,
    &bufptr,//will change
    &di_link_data_buf[DL_TX_BUFF_SIZE - 1],
    0);
  
  return bufptr;
}

//Taken from "udlfb.c"
static void dlfb_compress_hline(
  uint16_t **pixel_start_ptr,
  uint16_t *const pixel_end,
  uint32_t *device_address_ptr,
  uint8_t **command_buffer_ptr,
  uint8_t *const cmd_buffer_end,
  unsigned long back_buffer_offset)
{
  uint16_t *pixel = *pixel_start_ptr;
  uint32_t dev_addr  = *device_address_ptr;
  uint8_t *cmd = *command_buffer_ptr;
  
  while ((pixel_end > pixel) &&
    (cmd_buffer_end - MIN_RLX_CMD_BYTES > cmd)) 
  {
    uint8_t *raw_pixels_count_byte = NULL;
    uint8_t *cmd_pixels_count_byte = NULL;
    const uint16_t *raw_pixel_start = NULL;
    const uint16_t *cmd_pixel_start, *cmd_pixel_end = NULL;
           
    if (back_buffer_offset &&
        *pixel == *(uint16_t *)((uint8_t *)pixel + back_buffer_offset))
    {
      pixel++;
      dev_addr += BPP;
      //(*ident_ptr)++;
      continue;
    }
    
    *cmd++ = 0xAF;
    *cmd++ = 0x6B;
    *cmd++ = dev_addr >> 16;
    *cmd++ = dev_addr >> 8;
    *cmd++ = dev_addr;
    
    cmd_pixels_count_byte = cmd++; /*  we'll know this later */
    cmd_pixel_start = pixel;
    
    raw_pixels_count_byte = cmd++; /*  we'll know this later */
    raw_pixel_start = pixel;
           
    cmd_pixel_end = pixel + min3(MAX_CMD_PIXELS + 1UL,
                                 (unsigned long)(pixel_end - pixel),
                                 (unsigned long)(cmd_buffer_end - 1 - cmd) / BPP);
           
    if (back_buffer_offset) {
      /* note: the framebuffer may change under us, so we must test for underflow */
      while (cmd_pixel_end - 1 > pixel &&
             *(cmd_pixel_end - 1) == *(uint16_t *)((uint8_t *)(cmd_pixel_end - 1) + back_buffer_offset))
        cmd_pixel_end--;
    }
           
    while (pixel < cmd_pixel_end) 
    {
      const uint16_t * const repeating_pixel = pixel;
      uint16_t pixel_value = *pixel;
      
      put_unaligned_be16(pixel_value, cmd);
      if (back_buffer_offset)
        *(uint16_t *)((uint8_t *)pixel + back_buffer_offset) = pixel_value;
      cmd += 2;
      pixel++;
      
      //if (unlikely((pixel < cmd_pixel_end) &&
      if (((pixel < cmd_pixel_end) && (*pixel == pixel_value))) 
      {
        /* go back and fill in raw pixel count */
        *raw_pixels_count_byte = ((repeating_pixel -
                                   raw_pixel_start) + 1) & 0xFF;
        
        do {
          if (back_buffer_offset)
            *(uint16_t *)((uint8_t *)pixel + back_buffer_offset) = pixel_value;
          pixel++;
        } while ((pixel < cmd_pixel_end) &&
                 (*pixel == pixel_value));
        
        /* immediately after raw data is repeat byte */
        *cmd++ = ((pixel - repeating_pixel) - 1) & 0xFF;
        
        /* Then start another raw pixel span */
        raw_pixel_start = pixel;
        raw_pixels_count_byte = cmd++;
      }
    }
           
    if (pixel > raw_pixel_start) {
      /* finalize last RAW span */
      *raw_pixels_count_byte = (pixel-raw_pixel_start) & 0xFF;
    } else {
      /* undo unused byte */
      cmd--;
    }
           
    *cmd_pixels_count_byte = (pixel - cmd_pixel_start) & 0xFF;
    dev_addr += (uint8_t *)pixel - (uint8_t *)cmd_pixel_start;
  }
  
  if (cmd_buffer_end - MIN_RLX_CMD_BYTES <= cmd) {
    /* Fill leftover bytes with no-ops */
    if (cmd_buffer_end > cmd)
      memset(cmd, 0xAF, cmd_buffer_end - cmd);
    cmd = (uint8_t *) cmd_buffer_end;
  }
  
  *command_buffer_ptr = cmd;
  *pixel_start_ptr = pixel;
  *device_address_ptr = dev_addr;
}


uint8_t *displaylink_set_register(uint8_t *bufptr, uint8_t reg, uint8_t val)
{
  *bufptr++ = DL_SOC;
  *bufptr++ = DL_SREG;
  *bufptr++ = reg;
  *bufptr++ = val;
  
  return bufptr;
}

uint8_t *displaylink_sync_command(uint8_t *bufptr) 
{
  *bufptr++ = DL_SOC;
  *bufptr++ = DL_SYNC;
  return bufptr;
}

uint8_t *displaylink_set_registers(uint8_t *bufptr, uint8_t* values, uint8_t size)
{
  bufptr = displaylink_set_register(bufptr, DL_REG_SYNC, 0x00);
  for (int i = 0; i < size; i++)
    bufptr = displaylink_set_register(bufptr, i, values[i]);
  bufptr = displaylink_set_register(bufptr, DL_REG_SYNC, 0xFF );
  return bufptr;
}

uint8_t *displaylink_set_offsets(
  uint8_t *bufptr, int start16, int stride16, int start8, int stride8) 
{
  bufptr = displaylink_set_register(bufptr, DL_REG_SYNC, 0x00);
  bufptr = displaylink_set_address(bufptr, DL_ADDR_FB16_START,  start16);
  bufptr = displaylink_set_address(bufptr, DL_ADDR_FB16_STRIDE, stride16);
  bufptr = displaylink_set_address(bufptr, DL_ADDR_FB8_START,   start8);
  bufptr = displaylink_set_address(bufptr, DL_ADDR_FB8_STRIDE,  stride8);
  bufptr = displaylink_set_register(bufptr, DL_REG_SYNC, 0xFF);
  return bufptr;
}

uint8_t *displaylink_set_address(uint8_t *bufptr, uint8_t reg, int address) 
{
  bufptr = displaylink_set_register( bufptr, reg+0, (address >> 16) & 0xFF );
  bufptr = displaylink_set_register( bufptr, reg+1, (address >>  8) & 0xFF );
  bufptr = displaylink_set_register( bufptr, reg+2, (address      ) & 0xFF );
  return bufptr;
}


// little-endian result
uint16_t RGB_24to16(int red, int grn, int blu)
{
  return ( ((red >> 3) << 11) | ((grn >> 2) << 5) | (blu >> 3) ); //orig
}