# STM32_DisplayLink
Connecting USB-VGA adapter to STM32  
  
This is an example how to connect VGA display to STM32 using cheap DisplayLink adapter.  
DisplayLink is a kind of USB video card with internal framebuffer RAM, 
so you can connect such adapter to the USB HOST of your MCU and get a big display for your STM32.  
  
See video: https://www.youtube.com/watch?v=bcdXBnMFIVg  
  
![Alt text](Photo/Photo1.jpg?raw=true "Image")  
  
Only USB 2.0 version of adapters is supported in code.  
Of course, your STM32 will need to have USB HOST hardware. STM32 MCUs have only Full-Speed internal USB, so data transfer rate is limited ~1 Mbyte/s.  
For updating VGA screen with 640x480 resolution you will need to send ~0.6 MByte of data, which means that FPS will be very low.  
DiplayLink adapter supports RLE data coding, which can be helpful to increase FPS.  

All code is written for STM32F4-Discovery (MB997B), HAL is used here. Notice that newer PCB revisions have other accelerometer model.  
  
Only one resolution is supported: 640x480, if you need another you need to change (XRES, YRES) defines and take another configuration 
array data from here: https://github.com/floe/tubecable/blob/master/doc/dldemo.c instead of "dl_register_init_640x480".  

There are two projects in this repo: BasicTestSrc and LVGL_Src.  
One is simple drawing static image and another is displaying GUI using LVGL.  
See more information about those projects in their Readme files.  
  
You can set USBH_DEBUG_LEVEL define in "usbh_conf.h" to 3U value to get USB debug information (using Semihosting in IDE).  
  
Tested only with 0x17E9 & 0x024B device.


