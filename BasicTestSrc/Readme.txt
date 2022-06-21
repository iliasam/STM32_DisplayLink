Firmware that is displaying simple drawing static image using DisplayLink.
Source image ("image1_map") is sored in Flash, its size is 320x240, 16 bit color.
Image is send to USB using RLE coding.

This firmware is based on FreeRTOS.
2 threads are used:
defaultTask - StartDefaultTask() - used to send data to USB.
USBH_Thread - USBH_Process_OS() - hidden part of HAL, used to handle working with USB.

See test_fill() function in di_link_additional.c file see see how drawing is called.

Memory usage:
  183 234 bytes of readonly  code memory (Flash)
      142 bytes of readonly  data memory (Flash)
   36 697 bytes of readwrite data memory (RAM)

Commands buffer for DisplayLink "di_link_data_buf" is taking 16KB of RAM.
FreeRTOS heap is defined as configTOTAL_HEAP_SIZE ((size_t)15360) - in bytes 

Image (1/4 of the screen area) is drawn in 65 ms.
Screen is filled with blue color before drawing image, so image is blinking.
