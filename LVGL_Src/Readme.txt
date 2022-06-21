Firmware that is displaying LVGL demo.

UI is stored in "LVGL_demo" folder.

This firmware is based on FreeRTOS.
3 threads are used:
LVGL_Task - LVGL_TaskHandler() - used to update GUI
defaultTask - StartDefaultTask() - used to send data to USB.
USBH_Thread - USBH_Process_OS() - hidden part of HAL, used to handle working with USB.

Fist part of demo is displaying static GUI image - during first 12s from startup.
Second part of demo is displaying GUI with working: keyboard, text editing field, gauge meter,  progress bar, slider, label with moving text.
Mouse is emulated using accelerometer installed at the STM32F4-Discovery board.

Memory usage:
  226 001 bytes of readonly  code memory (Flash)
  130 954 bytes of readonly  data memory  (Flash)
  124 699 bytes of readwrite data memory (RAM)

CCM memory is used here to store LVGL image buffer "buf_1" which have size (640 * 45 * 2 = 57600) bytes.
Commands buffer for DisplayLink "di_link_data_buf" is taking 16KB of RAM.
LV_MEM_SIZE is defined as (20U * 1024U) - memory for LVGL internal heap - in bytes.
FreeRTOS heap is defined as configTOTAL_HEAP_SIZE ((size_t)15360) - in bytes 

How GUI drawing is working:
* User create UI objects, they are stored in LVGL heap RAM (see ui_init() in ui.c file).
* lv_task_handler() function is periodically called in LVGL_Task (main.c). This function check that parts of the image should be redrawn and render them.
    buf_1[] buffer is used to store rendered image parts.
    lv_task_handler() call disp_flush() function, dimensions of updated zone and image data are passed to this function.
* It is possible to send data to USB just in disp_flush() - blocking, but I save zone data and image data pointer.
* disp_update_handler() is periodically called from defaultTask. 
    This function check if there are new image data and starting to send them, calling DI_LINK_DrawRLEImage16bit()
* DI_LINK_DrawRLEImage16bit() function (di_link_send_data.c) is processing source image data line by line, producing RLE commands, that are stored in di_link_data_buf[] buffer.
    When buffer is filled, data from this buffer is send to USB stack - see USBH_DI_LINK_Transmit() in usbh_di_link.c.
    USB transfers are called from DI_LINK_ProcessTransmission() (called by USB stack) -> USBH_BulkSendData()
    DI_LINK_DrawRLEImage16bit() is waiting until all data will be send to USB.
    When all data is send to USB, disp_update_handler() call lv_disp_flush_ready(), to tell LVGL that display is updated.



