
/* Define to prevent recursive  ----------------------------------------------*/
#ifndef __USBH_DI_LINK_ADDITIONAL_H
#define __USBH_DI_LINK_ADDITIONAL_H

#ifdef __cplusplus
extern "C" {
#endif

#include "usbh_core.h"
  
/* Includes ------------------------------------------------------------------*/
void DI_LINK_DrawImage16bit(USBH_HandleTypeDef *phost, 
  uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint8_t* data);
void DI_LINK_FillDisplayColor16(USBH_HandleTypeDef *phost, uint16_t color);

uint8_t *displaylink_draw_pixel_16_bit(
  uint8_t *bufptr, uint16_t x, uint16_t y, uint16_t color);

void test_fill(USBH_HandleTypeDef *phost);

#ifdef __cplusplus
}
#endif

#endif /* __USBH_DI_LINK_ADDITIONAL_H */


