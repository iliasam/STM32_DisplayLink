
/* Define to prevent recursive  ----------------------------------------------*/
#ifndef __USBH_DI_LINK_SEND_DATA_H
#define __USBH_DI_LINK_SEND_DATA_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "usbh_core.h"
  
#define XRES                    640
#define YRES                    480
  
// start-of-command token
#define DL_SOC                                                  0xAF 
// set register
#define DL_SREG                                                 0x20 
// sync/flush
#define DL_SYNC                                                 0xA0
 // 0 = hold register updates, 0xFF = resume
#define DL_REG_SYNC                                             0xFF
// 0 = normal operation, 1 = blank screen
#define DL_REG_BLANK_SCREEN                                     0x1F 
  
#define DL_ADDR_FB16_START      0x20 // 16-bit mode, color MSBs, RGB 565
#define DL_ADDR_FB16_STRIDE     0x23
#define DL_ADDR_FB8_START       0x26 // additional 8 bit for 24-bit mode, color LSBs, RGB 323?
#define DL_ADDR_FB8_STRIDE      0x29

#define DL_GFX_BASE             0x60 // base graphics command
#define DL_GFX_WORD             0x08 // word-mode flag
#define DL_GFX_COMP             0x10 // compressed-mode flag

#define DL_GFX_WRITE            (DL_GFX_BASE | 0x00) // write memory
#define DL_GFX_RLE              (DL_GFX_BASE | 0x01) // write RLE-encoded data
#define DL_GFX_COPY             (DL_GFX_BASE | 0x02) // internal copy
  
typedef struct 
{
  uint8_t count;
  uint16_t value;
} rle_word;

void test_fill(USBH_HandleTypeDef *phost);
USBH_StatusTypeDef USBH_DI_LINK_SendConfiguration(USBH_HandleTypeDef *phost);

void DI_LINK_FillDisplayColor16(USBH_HandleTypeDef *phost, uint16_t color);
void DI_LINK_DrawImage16bit(USBH_HandleTypeDef *phost, 
  uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint8_t* data);
void DI_LINK_DrawRLEImage16bit(USBH_HandleTypeDef *phost, 
  uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint8_t* data);

uint8_t *displaylink_draw_pixel_16_bit(
  uint8_t *bufptr, uint16_t x, uint16_t y, uint16_t color);


#ifdef __cplusplus
}
#endif

#endif /* __USBH_DI_LINK_SEND_DATA_H */


