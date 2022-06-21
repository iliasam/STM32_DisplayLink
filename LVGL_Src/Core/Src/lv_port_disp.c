/**
 * @file lv_port_disp.c
 * This is LVGL display driver
 */

/*********************
 *      INCLUDES
 *********************/
#include "lv_port_disp.h"
#include "di_link_send_data.h"
#include "lvgl.h"
#include "cmsis_os.h"

/*********************
 *      DEFINES
 *********************/
//Use double buffering
#define DISP_USE_DOUBLE_BUF     0
#define DISP_BUFF_LINES         45

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 *  STATIC PROTOTYPES
 **********************/

static void disp_flush(lv_disp_drv_t * disp_drv, const lv_area_t * area, lv_color_t * color_p);

static lv_area_t disp_curr_fill_area;
static lv_color_t *disp_curr_fill_color_p;
static uint8_t disp_need_update = 0;

/**********************
 *  STATIC VARIABLES
 **********************/
extern USBH_HandleTypeDef hUsbHostFS;

static lv_disp_drv_t disp_drv; /*Descriptor of a display driver*/

/**********************
 *      MACROS
 **********************/

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

void lv_port_disp_init(void)
{
/*-----------------------------
 * Create a buffer for drawing
 *----------------------------*/
  
// Note that "place in CCMRAM_region {section .ccmram};" line is needed in stm32f407xx_flash.icf
//To enable CCM RAM
  
#if (DISP_USE_DOUBLE_BUF)
  static lv_disp_draw_buf_t draw_buf_dsc;
#pragma location = ".ccmram"
  static lv_color_t buf_2_1[XRES * DISP_BUFF_LINES];  //A buffer for 10 rows
#pragma location = ".ccmram"
  static lv_color_t buf_2_2[XRES * DISP_BUFF_LINES];  //An other buffer for 10 rows
  lv_disp_draw_buf_init(&draw_buf_dsc, buf_2_1, buf_2_2, XRES * DISP_BUFF_LINES);   //Initialize the display buffer
#else
  static lv_disp_draw_buf_t draw_buf_dsc;
#pragma location = ".ccmram"
  static lv_color_t buf_1[XRES * DISP_BUFF_LINES]; //A buffer for 10 rows
  lv_disp_draw_buf_init(&draw_buf_dsc, buf_1, NULL, XRES * DISP_BUFF_LINES);   //Initialize the display buffer
#endif
  
  
  /*-----------------------------------
  * Register the display in LVGL
  *----------------------------------*/
  
  
  lv_disp_drv_init(&disp_drv); /*Basic initialization*/
  
  /*Set up the functions to access to your display*/
  
  /*Set the resolution of the display*/
  disp_drv.hor_res = XRES;
  disp_drv.ver_res = YRES;
  
  /*Used to copy the buffer's content to the display*/
  disp_drv.flush_cb = disp_flush;
  
  /*Set a display buffer*/
  disp_drv.draw_buf = &draw_buf_dsc;
  
  /*Finally register the driver*/
  lv_disp_drv_register(&disp_drv);
}

/**********************
 *   STATIC FUNCTIONS
 **********************/


/*Flush the content of the internal buffer the specific area on the display
 *You can use DMA or any hardware acceleration to do this operation in the background but
 *'lv_disp_flush_ready()' has to be called when finished.*/
static void disp_flush(lv_disp_drv_t * disp_drv, const lv_area_t * area, lv_color_t * color_p)
{
  /*
  //This is working too, but suitable only for single framebuffer
  DI_LINK_DrawRLEImage16bit(
    &hUsbHostFS, 
    area->x1, area->y1,
    area->x2 - area->x1 + 1, area->y2 - area->y1 + 1, (uint8_t*)color_p);
  
  //Inform the graphics library that you are ready with the flushing
  lv_disp_flush_ready(&disp_drv);
  return;
  */
  
  //Store are to local variable
  memcpy(&disp_curr_fill_area, area, sizeof(disp_curr_fill_area));
  disp_curr_fill_color_p = color_p;
  disp_need_update = 1;
}

//Send data to USB, called from StartDefaultTask() task handler
void disp_update_handler(void)
{
  if (disp_need_update)
  {
    lv_area_t *area = &disp_curr_fill_area;
    
    DI_LINK_DrawRLEImage16bit(
      &hUsbHostFS, 
      area->x1, area->y1, 
      area->x2 - area->x1 + 1, area->y2 - area->y1 + 1, 
      (uint8_t*)disp_curr_fill_color_p);
    disp_need_update = 0;
    
    lv_disp_flush_ready(&disp_drv);
  }
}

