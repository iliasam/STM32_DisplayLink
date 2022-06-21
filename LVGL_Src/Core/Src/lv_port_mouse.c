/**
 * @file lv_port_mouse.c
 * This is LVGL mouse driver.
 * Mouse is emulated using LIS302dl accelerometer, installed at PCB.
 */

/*********************
 *      INCLUDES
 *********************/
#include "lv_port_mouse.h"
#include "lvgl.h"
#include "lis302dl_hal.h"
#include "di_link_send_data.h"

/*********************
 *      DEFINES
 *********************/

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 *  STATIC PROTOTYPES
 **********************/
static void mouse_read(lv_indev_drv_t * indev_drv, lv_indev_data_t * data);
static bool mouse_is_pressed(void);

int16_t cursor_pos_x = 0;
int16_t cursor_pos_y = 0;

int16_t accelerometer_offset_x = 25;
int16_t accelerometer_offset_y = 0;
int16_t accelerometer_threshold = 5;

/**********************
 *  STATIC VARIABLES
 **********************/
lv_indev_t * indev_mouse;


void lv_port_indev_init(void)
{
  static lv_indev_drv_t indev_drv;
  
  /*Register a mouse input device*/
  lv_indev_drv_init(&indev_drv);
  indev_drv.type = LV_INDEV_TYPE_POINTER;
  indev_drv.read_cb = mouse_read;
  indev_mouse = lv_indev_drv_register(&indev_drv);
  
  LV_IMG_DECLARE(cursor_icon); 
  lv_obj_t *mouse_cursor = lv_img_create(lv_scr_act());
  //lv_img_set_src(mouse_cursor, LV_SYMBOL_HOME);
  lv_img_set_src(mouse_cursor, &cursor_icon);
  lv_indev_set_cursor(indev_mouse, mouse_cursor);
}

static void mouse_read(lv_indev_drv_t *indev_drv, lv_indev_data_t *data)
{
    data->point.x = cursor_pos_x;
    data->point.y = cursor_pos_y;
  
   // data->point.x = 90;
   // data->point.y = 300;

    /*Get whether the mouse button is pressed or released*/
    if(mouse_is_pressed()) {
        data->state = LV_INDEV_STATE_PR;
    }
    else {
        data->state = LV_INDEV_STATE_REL;
    }
}


/*Return true is the mouse button is pressed*/
static bool mouse_is_pressed(void)
{
    /*Your code comes here*/
  if (B1_GPIO_Port->IDR & B1_Pin)
    return true;
  else
    return false;
    
}

//Read values from accelerometer and calculate mouse pos
void mouse_cursor_update(void)
{
  AccelerometerData_t axis_data;
  AccelerometerReadAxes(&axis_data);
  
  int speed_x  = 0;
  int speed_y  = 0;
  
  axis_data.X -= accelerometer_offset_x;
  axis_data.Y -= accelerometer_offset_y;
  
  if (axis_data.X > accelerometer_threshold)
    axis_data.X-= accelerometer_threshold;
  else if (axis_data.X < -accelerometer_threshold)
    axis_data.X+= accelerometer_threshold;
  
  if (axis_data.Y > accelerometer_threshold)
    axis_data.Y-= accelerometer_threshold;
  else if (axis_data.Y < -accelerometer_threshold)
    axis_data.Y+= accelerometer_threshold;
    
  
  
  speed_x = -(axis_data.X / 2);
  speed_y = (axis_data.Y / 4);
  
  if (speed_x < 0)
    speed_x = -(speed_x * speed_x);
  else
    speed_x = (speed_x * speed_x);
  
  if (speed_y < 0)
    speed_y = -(speed_y * speed_y);
  else
    speed_y = (speed_y * speed_y);
  
  //swap
  cursor_pos_x += speed_y;
  cursor_pos_y += speed_x;
  
  if (cursor_pos_x < 0)
    cursor_pos_x = 0;
  if (cursor_pos_x >= (XRES - 1))
    cursor_pos_x = (XRES - 1);
  
  if (cursor_pos_y < 0)
    cursor_pos_y = 0;
  if (cursor_pos_y >= (YRES - 1))
    cursor_pos_y = (YRES - 1);
}