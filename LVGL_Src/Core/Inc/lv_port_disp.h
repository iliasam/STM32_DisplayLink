/**
 * @file lv_port_disp_templ.h
 *
 */
#ifndef LV_PORT_DISP_H
#define LV_PORT_DISP_H

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
void lv_port_disp_init(void);
void disp_update_handler(void);
  
#ifdef __cplusplus
} /*extern "C"*/
#endif


#endif /*LV_PORT_DISP_TEMPL_H*/
