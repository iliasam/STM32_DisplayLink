/**
 * @file lv_port_mouse.h
 *
 */



#ifndef LV_PORT_MOUSE_H
#define LV_PORT_MOUSE_H

#ifdef __cplusplus
extern "C" {
#endif

/*********************
 *      INCLUDES
 *********************/
#include "lvgl.h"

/*********************
 *      DEFINES
 *********************/

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 * GLOBAL PROTOTYPES
 **********************/

/**********************
 *      MACROS
 **********************/
void lv_port_indev_init(void);
void mouse_cursor_update(void);
  
#ifdef __cplusplus
} /*extern "C"*/
#endif


#endif /*LV_PORT_MOUSE_H*/


