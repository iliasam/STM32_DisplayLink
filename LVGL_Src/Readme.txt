Firmware that is displaying LVGL demo.

UI is stored in "LVGL_demo" folder.

This firmware is based on FreeRTOS.
3 threads are used:
LVGL_Task - LVGL_TaskHandler() - used to update GUI
defaultTask - StartDefaultTask() - used to send data to USB.
USBH_Thread - USBH_Process_OS() - hidden part of HAL, used to handle working with USB.

Fist part of demo is displaying static GUI image - during first 12s from startup.
Second part of demo is displaying GUI with working: keyboard, text editing field, gage meter,  progress bar, slider, label with moving text.
Mouse is emulated using accelerometer installed at STM32F4-Discovery board.

Memory usage:
  226 001 bytes of readonly  code memory (Flash)
  130 954 bytes of readonly  data memory  (Flash)
  124 699 bytes of readwrite data memory (RAM)

CCM memory is used here to store LVGL image buffer "buf_1" which have size (640 * 45 * 2 = 57600) bytes.
Commands buffer for DisplayLink "di_link_data_buf" is taking 16KB of RAM.
LV_MEM_SIZE is defined as (20U * 1024U) - memory for LVGL internal heap - in bytes.
FreeRTOS heap is defined as configTOTAL_HEAP_SIZE ((size_t)15360) - in bytes 

