/* Define to prevent recursive  ----------------------------------------------*/
#ifndef __USBH_DI_LINK_H
#define __USBH_DI_LINK_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "usbh_core.h"


/** @addtogroup USBH_LIB
* @{
*/

/** @addtogroup USBH_CLASS
* @{
*/

/** @addtogroup USBH_DI_LINK_CLASS
* @{
*/

/** @defgroup USBH_DI_LINK_CORE
* @brief This file is the Header file for usbh_core.c
* @{
*/


/*Communication Class codes*/
#define USB_DI_LINK_CLASS                                       0xFFU
#define COMMUNICATION_INTERFACE_CLASS_CODE                      0xFFU

/*Data Interface Class Codes*/
#define DATA_INTERFACE_CLASS_CODE                               0x0AU

/*Communication Interface Class Control Protocol Codes*/
#define NO_CLASS_SPECIFIC_PROTOCOL_CODE                         0x00U
#define VENDOR_SPECIFIC                                         0xFFU
/**
  * @}
  */

/** @defgroup USBH_DI_LINK_CORE_Exported_Types
* @{
*/

/* States for DI_LINK State Machine */
typedef enum
{
  DI_LINK_IDLE = 0U,
  DI_LINK_SEND_DATA,
  DI_LINK_SEND_DATA_WAIT,
  DI_LINK_RECEIVE_DATA,
  DI_LINK_RECEIVE_DATA_WAIT,
}
DI_LINK_DataStateTypeDef;

typedef enum
{
  DI_LINK_IDLE_STATE = 0U,
  DI_LINK_TRANSFER_DATA,
  DI_LINK_ERROR_STATE,
  DI_LINK_NEED_EDID,
  DI_LINK_NEED_CONFIGURE,
}
DI_LINK_StateTypeDef;


typedef struct
{
  uint8_t              OutPipe;
  uint8_t              OutEp;
  uint8_t              buff[8];
  uint16_t             OutEpSize;
}
DI_LINK_DataItfTypedef;

/* Structure for DI_LINK process */
typedef struct _DI_LINK_Process
{
  DI_LINK_DataItfTypedef                DataItf;
  uint8_t                               *pTxData;
  uint32_t                              TxDataLength;
  DI_LINK_StateTypeDef                  state;
  DI_LINK_DataStateTypeDef              data_tx_state;
  
  uint8_t                               WaitForSettings;
  uint8_t                               EDID_Table[128];
}
DI_LINK_HandleTypeDef;




extern USBH_ClassTypeDef        DI_LINK_Class;
#define USBH_DI_LINK_CLASS      &DI_LINK_Class

/**
* @}
*/

/** @defgroup USBH_DI_LINK_CORE_Exported_FunctionsPrototype
* @{
*/


USBH_StatusTypeDef  USBH_DI_LINK_Transmit(USBH_HandleTypeDef *phost,
                                      uint8_t *pbuff,
                                      uint32_t length);


uint16_t            USBH_DI_LINK_GetLastReceivedDataSize(USBH_HandleTypeDef *phost);

USBH_StatusTypeDef  USBH_DI_LINK_Stop(USBH_HandleTypeDef *phost);


void USBH_DI_LINK_TransmitCallback(USBH_HandleTypeDef *phost);

/**
* @}
*/

#ifdef __cplusplus
}
#endif

#endif /* __USBH_DI_LINK_H */

/**
* @}
*/

/**
* @}
*/

/**
* @}
*/

/**
* @}
*/
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

