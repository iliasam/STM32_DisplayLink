
/* Includes ------------------------------------------------------------------*/
#include "usbh_di_link.h"
#include "di_link_send_data.h"

static USBH_StatusTypeDef USBH_DI_LINK_InterfaceInit(USBH_HandleTypeDef *phost);
static USBH_StatusTypeDef USBH_DI_LINK_InterfaceDeInit(USBH_HandleTypeDef *phost);
static USBH_StatusTypeDef USBH_DI_LINK_Process(USBH_HandleTypeDef *phost);
static USBH_StatusTypeDef USBH_DI_LINK_SOFProcess(USBH_HandleTypeDef *phost);
static USBH_StatusTypeDef USBH_DI_LINK_ClassRequest(USBH_HandleTypeDef *phost);
static void DI_LINK_ProcessTransmission(USBH_HandleTypeDef *phost);

static USBH_StatusTypeDef USBH_DI_LINK_GetEDID(USBH_HandleTypeDef *phost);
static USBH_StatusTypeDef USBH_DI_LINK_ReadEDIDByte(
  USBH_HandleTypeDef *phost, uint8_t offset, uint8_t* result);
static USBH_StatusTypeDef USBH_DI_LINK_SendKey(USBH_HandleTypeDef *phost);

USBH_StatusTypeDef CommonUSBControlTransfer(USBH_HandleTypeDef *phost,
   uint8_t bRequest, uint8_t request_type, uint16_t wValue, uint16_t wIndex,
  unsigned char* data, uint16_t wLength, unsigned int timeout);


USBH_ClassTypeDef  DI_LINK_Class =
{
  "DisplayLink", //pActiveClass->Name
  USB_DI_LINK_CLASS,
  USBH_DI_LINK_InterfaceInit,
  USBH_DI_LINK_InterfaceDeInit,
  USBH_DI_LINK_ClassRequest,
  USBH_DI_LINK_Process,
  USBH_DI_LINK_SOFProcess,
  NULL,
};
/**
* @}
*/


/**
  * @brief  USBH_DI_LINK_InterfaceInit
  *         The function init the DisplayLink class.
  * @param  phost: Host handle
  * @retval USBH Status
  */
static USBH_StatusTypeDef USBH_DI_LINK_InterfaceInit(USBH_HandleTypeDef *phost)
{
  USBH_StatusTypeDef status;
  uint8_t interface;
  DI_LINK_HandleTypeDef *DI_LINK_Handle;
  
  //*phost, uint8_t Class, uint8_t SubClass, uint8_t Protocol
  //Result should be Interface 0
  interface = USBH_FindInterface(phost, COMMUNICATION_INTERFACE_CLASS_CODE, 0, NO_CLASS_SPECIFIC_PROTOCOL_CODE);
  
  if ((interface == 0xFFU) || (interface >= USBH_MAX_NUM_INTERFACES)) /* No Valid Interface */
  {
    USBH_DbgLog("Cannot Find the interface");
    return USBH_FAIL;
  }
  
  status = USBH_SelectInterface(phost, interface);

  if (status != USBH_OK)
  {
    return USBH_FAIL;
  }
  
  phost->pActiveClass->pData = (DI_LINK_HandleTypeDef *)USBH_malloc(sizeof(DI_LINK_HandleTypeDef));
  DI_LINK_Handle = (DI_LINK_HandleTypeDef *) phost->pActiveClass->pData;

  if (DI_LINK_Handle == NULL)
  {
    USBH_DbgLog("Cannot allocate memory for DisplayLink Handle");
    return USBH_FAIL;
  }
  
  /* Initialize DisplayLink handler */
  USBH_memset(DI_LINK_Handle, 0, sizeof(DI_LINK_HandleTypeDef));
  
  
  for (uint8_t i = 0; i < 2; i++)//todo
  {
    if ((phost->device.CfgDesc.Itf_Desc[interface].Ep_Desc[i].bEndpointAddress & 0x80U) == 0)
    {    
      DI_LINK_Handle->DataItf.OutEp = phost->device.CfgDesc.Itf_Desc[interface].Ep_Desc[0].bEndpointAddress;
      DI_LINK_Handle->DataItf.OutEpSize  = phost->device.CfgDesc.Itf_Desc[interface].Ep_Desc[0].wMaxPacketSize;
      break;
    }
  }
  
  /*Allocate the length for host channel number out*/
  DI_LINK_Handle->DataItf.OutPipe = USBH_AllocPipe(phost, DI_LINK_Handle->DataItf.OutEp);

  /* Open channel for OUT endpoint */
  USBH_OpenPipe(phost, DI_LINK_Handle->DataItf.OutPipe, DI_LINK_Handle->DataItf.OutEp,
                phost->device.address, phost->device.speed, USB_EP_TYPE_BULK,
                DI_LINK_Handle->DataItf.OutEpSize);
  
  DI_LINK_Handle->state = DI_LINK_NEED_EDID;

  USBH_LL_SetToggle(phost, DI_LINK_Handle->DataItf.OutPipe, 0U);

  return USBH_OK;
}


/** @defgroup USBH_DI_LINK_CORE_Private_Functions
* @{
*/


/**
  * @brief  USBH_DI_LINK_InterfaceDeInit
  *         The function DeInit the Pipes used for the DI_LINK class.
  * @param  phost: Host handle
  * @retval USBH Status
  */
static USBH_StatusTypeDef USBH_DI_LINK_InterfaceDeInit(USBH_HandleTypeDef *phost)
{
  DI_LINK_HandleTypeDef *DI_LINK_Handle = (DI_LINK_HandleTypeDef *) phost->pActiveClass->pData;

  if (DI_LINK_Handle->DataItf.OutPipe)
  {
    USBH_ClosePipe(phost, DI_LINK_Handle->DataItf.OutPipe);
    USBH_FreePipe(phost, DI_LINK_Handle->DataItf.OutPipe);
    DI_LINK_Handle->DataItf.OutPipe = 0U;    /* Reset the Channel as Free */
  }

  if (phost->pActiveClass->pData)
  {
    USBH_free(phost->pActiveClass->pData);
    phost->pActiveClass->pData = 0U;
  }

  return USBH_OK;
}

/**
  * @brief  USBH_DI_LINK_ClassRequest
  *         The function is responsible for handling Standard requests
  *         for DI_LINK class.
  * @param  phost: Host handle
  * @retval USBH Status
  */
static USBH_StatusTypeDef USBH_DI_LINK_ClassRequest(USBH_HandleTypeDef *phost)
{
  //USBH_StatusTypeDef status;
  //DI_LINK_HandleTypeDef *DI_LINK_Handle = (DI_LINK_HandleTypeDef *) phost->pActiveClass->pData;
  
  return USBH_OK;
}


static USBH_StatusTypeDef USBH_DI_LINK_GetEDID(USBH_HandleTypeDef *phost)
{
  USBH_StatusTypeDef status = USBH_FAIL;
  DI_LINK_HandleTypeDef *DI_LINK_Handle = (DI_LINK_HandleTypeDef *) phost->pActiveClass->pData;

  for (uint8_t i = 0; i < 128; i++)
  {
    status = USBH_DI_LINK_ReadEDIDByte(phost, i, &(DI_LINK_Handle->EDID_Table[i]));
    if (status != USBH_OK)
      return status;
  }
  
  if (status == USBH_OK)
  {
    USBH_DbgLog("EDID received");
  }

  return status;
}

//Send special ky during device init
static USBH_StatusTypeDef USBH_DI_LINK_SendKey(USBH_HandleTypeDef *phost)
{
  USBH_StatusTypeDef status = USBH_FAIL;
  unsigned char nullkey1[16] = 
    { 0x57, 0xCD, 0xDC, 0xA7, 0x1C, 0x88, 0x5E, 0x15, 
      0x60, 0xFE, 0xC6, 0x97, 0x16, 0x3D, 0x47, 0xF2 };

  status = CommonUSBControlTransfer(phost,
		0x12, //request
                (0x02 << 5), //requesttype - todo
                0, 0, nullkey1, 16, 500);
  
  if (status == USBH_OK)
  {
    USBH_DbgLog("Send Key to DisplayLink");
  }
  
  return status;
}




static USBH_StatusTypeDef USBH_DI_LINK_ReadEDIDByte(USBH_HandleTypeDef *phost, uint8_t offset, uint8_t* result)
{
  uint16_t wLength = 2;
  uint8_t requestBuf[2];
  memset(&requestBuf, 0, sizeof(requestBuf));
  
  USBH_StatusTypeDef status;
  
  status = CommonUSBControlTransfer(
    phost,
    (0x02),  //request
    USB_D2H | USB_REQ_TYPE_VENDOR, //requesttype? 
    ((uint16_t)offset << 8),  //value
    0xA1, //index
    requestBuf, //data
    wLength, //size
    1000); //timeout
  
  if (status == USBH_OK)
  {
    *result = requestBuf[1];
  }
  
  return status;
}


/**
  * @brief  USBH_DI_LINK_Process
  *         The function is for managing state machine for DI_LINK data transfers
  * @param  phost: Host handle
  * @retval USBH Status
  */
static USBH_StatusTypeDef USBH_DI_LINK_Process(USBH_HandleTypeDef *phost)
{
  USBH_StatusTypeDef status = USBH_BUSY;
  USBH_StatusTypeDef req_status = USBH_OK;
  DI_LINK_HandleTypeDef *DI_LINK_Handle = (DI_LINK_HandleTypeDef *) phost->pActiveClass->pData;

  switch (DI_LINK_Handle->state)
  {
    case DI_LINK_NEED_EDID:
      status = USBH_DI_LINK_GetEDID(phost);
      if (status == USBH_OK)
        DI_LINK_Handle->state = DI_LINK_NEED_CONFIGURE;
      break;
      
    case DI_LINK_NEED_CONFIGURE:
      status = USBH_DI_LINK_SendKey(phost);
      if (status != USBH_OK)
        break;
      
      DI_LINK_Handle->state = DI_LINK_IDLE_STATE;
      USBH_DI_LINK_SendConfiguration(phost);
      break;
      
    case DI_LINK_IDLE_STATE:
      status = USBH_OK;
      break;

    case DI_LINK_TRANSFER_DATA:
      DI_LINK_ProcessTransmission(phost);
      break;

    case DI_LINK_ERROR_STATE:
      req_status = USBH_ClrFeature(phost, 0x00U);

      if (req_status == USBH_OK)
      {
        /*Change the state to waiting*/
        DI_LINK_Handle->state = DI_LINK_IDLE_STATE;
      }
      break;

    default:
      break;

  }

  return status;
}

/**
  * @brief  USBH_DI_LINK_SOFProcess
  *         The function is for managing SOF callback
  * @param  phost: Host handle
  * @retval USBH Status
  */
static USBH_StatusTypeDef USBH_DI_LINK_SOFProcess(USBH_HandleTypeDef *phost)
{
  /* Prevent unused argument(s) compilation warning */
  UNUSED(phost);

  return USBH_OK;
}


/**
  * @brief  USBH_DI_LINK_Stop
  *         Stop current DI_LINK Transmission
  * @param  phost: Host handle
  * @retval USBH Status
  */
USBH_StatusTypeDef  USBH_DI_LINK_Stop(USBH_HandleTypeDef *phost)
{
  DI_LINK_HandleTypeDef *DI_LINK_Handle = (DI_LINK_HandleTypeDef *) phost->pActiveClass->pData;

  if (phost->gState == HOST_CLASS)
  {
    DI_LINK_Handle->state = DI_LINK_IDLE_STATE;
    USBH_ClosePipe(phost, DI_LINK_Handle->DataItf.OutPipe);
  }
  return USBH_OK;
}


/**
  * @brief  This function prepares the state before issuing the class specific commands
  * @param  None
  * @retval None
  */
USBH_StatusTypeDef  USBH_DI_LINK_Transmit(USBH_HandleTypeDef *phost, uint8_t *pbuff, uint32_t length)
{
  USBH_StatusTypeDef Status = USBH_BUSY;
  DI_LINK_HandleTypeDef *DI_LINK_Handle = (DI_LINK_HandleTypeDef *) phost->pActiveClass->pData;

  if ((DI_LINK_Handle->state == DI_LINK_IDLE_STATE) || (DI_LINK_Handle->state == DI_LINK_TRANSFER_DATA))
  {
    DI_LINK_Handle->pTxData = pbuff;
    DI_LINK_Handle->TxDataLength = length;
    DI_LINK_Handle->state = DI_LINK_TRANSFER_DATA;
    DI_LINK_Handle->data_tx_state = DI_LINK_SEND_DATA;
    Status = USBH_OK;

#if (USBH_USE_OS == 1U)
    phost->os_msg = (uint32_t)USBH_CLASS_EVENT;
#if (osCMSIS < 0x20000U)
    (void)osMessagePut(phost->os_event, phost->os_msg, 0U);
#else
    (void)osMessageQueuePut(phost->os_event, &phost->os_msg, 0U, NULL);
#endif
#endif
  }
  return Status;
}


/**
* @brief  The function is responsible for sending data to the device
*  @param  pdev: Selected device
* @retval None
*/
static void DI_LINK_ProcessTransmission(USBH_HandleTypeDef *phost)
{
  DI_LINK_HandleTypeDef *DI_LINK_Handle = (DI_LINK_HandleTypeDef *) phost->pActiveClass->pData;
  USBH_URBStateTypeDef URB_Status = USBH_URB_IDLE;

  switch (DI_LINK_Handle->data_tx_state)
  {
    case DI_LINK_SEND_DATA:
      if (DI_LINK_Handle->TxDataLength > DI_LINK_Handle->DataItf.OutEpSize)
      {
        USBH_BulkSendData(phost,
                          DI_LINK_Handle->pTxData,
                          DI_LINK_Handle->DataItf.OutEpSize,
                          DI_LINK_Handle->DataItf.OutPipe,
                          1U);
      }
      else
      {
        USBH_BulkSendData(phost,
                          DI_LINK_Handle->pTxData,
                          (uint16_t)DI_LINK_Handle->TxDataLength,
                          DI_LINK_Handle->DataItf.OutPipe,
                          1U);
      }

      DI_LINK_Handle->data_tx_state = DI_LINK_SEND_DATA_WAIT;
      break;

    case DI_LINK_SEND_DATA_WAIT:

      URB_Status = USBH_LL_GetURBState(phost, DI_LINK_Handle->DataItf.OutPipe);

      /* Check the status done for transmission */
      if (URB_Status == USBH_URB_DONE)
      {
        if (DI_LINK_Handle->TxDataLength > DI_LINK_Handle->DataItf.OutEpSize)
        {
          DI_LINK_Handle->TxDataLength -= DI_LINK_Handle->DataItf.OutEpSize;
          DI_LINK_Handle->pTxData += DI_LINK_Handle->DataItf.OutEpSize;
        }
        else
        {
          DI_LINK_Handle->TxDataLength = 0U;
        }

        if (DI_LINK_Handle->TxDataLength > 0U)
        {
          DI_LINK_Handle->data_tx_state = DI_LINK_SEND_DATA;
        }
        else
        {
          DI_LINK_Handle->data_tx_state = DI_LINK_IDLE;
          USBH_DI_LINK_TransmitCallback(phost);
        }

#if (USBH_USE_OS == 1U)
        phost->os_msg = (uint32_t)USBH_CLASS_EVENT;
#if (osCMSIS < 0x20000U)
        (void)osMessagePut(phost->os_event, phost->os_msg, 0U);
#else
        (void)osMessageQueuePut(phost->os_event, &phost->os_msg, 0U, NULL);
#endif
#endif
      }
      else
      {
        if (URB_Status == USBH_URB_NOTREADY)
        {
          DI_LINK_Handle->data_tx_state = DI_LINK_SEND_DATA;

#if (USBH_USE_OS == 1U)
          phost->os_msg = (uint32_t)USBH_CLASS_EVENT;
#if (osCMSIS < 0x20000U)
          (void)osMessagePut(phost->os_event, phost->os_msg, 0U);
#else
          (void)osMessageQueuePut(phost->os_event, &phost->os_msg, 0U, NULL);
#endif
#endif
        }
      }
      break;

    default:
      break;
  }
}


/**
* @brief  The function informs user that data have been received
*  @param  pdev: Selected device
* @retval None
*/
__weak void USBH_DI_LINK_TransmitCallback(USBH_HandleTypeDef *phost)
{
  DI_LINK_HandleTypeDef *DI_LINK_Handle = (DI_LINK_HandleTypeDef *) phost->pActiveClass->pData;
  
  if (DI_LINK_Handle->WaitForSettings)
  {
    DI_LINK_Handle->WaitForSettings = 0;
    USBH_DbgLog("Configuration was send DisplayLink");
  }
  
  DI_LINK_Handle->state = DI_LINK_IDLE_STATE;
}



//Replacement for usb_control_msg()
USBH_StatusTypeDef CommonUSBControlTransfer(USBH_HandleTypeDef *phost,
   uint8_t bRequest, uint8_t request_type, uint16_t wValue, uint16_t wIndex,
  unsigned char* data, uint16_t wLength, unsigned int timeout) 
{
    phost->Control.setup.b.bmRequestType = request_type;
    phost->Control.setup.b.bRequest      = bRequest;
    phost->Control.setup.b.wValue.w      = wValue;
    phost->Control.setup.b.wIndex.w      = wIndex;
    phost->Control.setup.b.wLength.w     = wLength;

    USBH_StatusTypeDef status;
    
    do {
        status = USBH_CtlReq(phost, data, wLength);
    } while (status == USBH_BUSY);

    return status;
}

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


/**
* @}
*/

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
