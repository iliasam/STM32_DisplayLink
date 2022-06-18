
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
  
#define DL_TX_BUFF_SIZE         (16*1024)
#define DL_TX_UPPER_LEVEL       (15*1024)
  
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

//Called from "di_link_additional.c"
uint8_t *displaylink_set_registers(uint8_t *bufptr, uint8_t* values, uint8_t size);
uint8_t *displaylink_set_register(uint8_t *bufptr, uint8_t reg, uint8_t val);
uint8_t *displaylink_set_offsets(
  uint8_t *bufptr, int start16, int stride16, int start8, int stride8);
uint8_t *displaylink_sync_command(uint8_t *bufptr);
uint8_t *displaylink_set_address(uint8_t *bufptr, uint8_t reg, int address);
uint16_t RGB_24to16(int red, int grn, int blu);

USBH_StatusTypeDef USBH_DI_LINK_SendConfiguration(USBH_HandleTypeDef *phost);
void DI_LINK_DrawRLEImage16bit(USBH_HandleTypeDef *phost, 
  uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint8_t* data);


#ifdef __cplusplus
}
#endif

#endif /* __USBH_DI_LINK_SEND_DATA_H */


