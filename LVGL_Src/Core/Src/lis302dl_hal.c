//Code for LIS302dl accelerometer

#include "main.h"
#include "lis302dl_hal.h"

extern SPI_HandleTypeDef hspi1;

#define ACC_SPI_NAME            hspi1
#define ACC_SPI_CS_LOW          HAL_GPIO_WritePin(CS_I2C_SPI_GPIO_Port, CS_I2C_SPI_Pin, GPIO_PIN_RESET)
#define ACC_SPI_CS_HIGH         HAL_GPIO_WritePin(CS_I2C_SPI_GPIO_Port, CS_I2C_SPI_Pin, GPIO_PIN_SET)

#define LIS302DL_LIS3DSH_REG_WHO_I_AM		0x0F
#define LIS302DL_ID		                0x3B
#define LIS302DL_CTRL_REG2_ADDR                 0x21
#define LIS302DL_CTRL_REG1_ADDR                 0x20
#define LIS302DL_OUT_X_ADDR                     0x29
#define LIS302DL_OUT_Y_ADDR                     0x2B
#define LIS302DL_OUT_Z_ADDR                     0x2D

#define LIS302DL_BOOT_REBOOTMEMORY              ((uint8_t)0x40)
#define LIS302DL_DATARATE_100                   ((uint8_t)0x00)
#define LIS302DL_DATARATE_400                   ((uint8_t)0x80)
#define LIS302DL_SENSITIVITY_2_3G               18  /* 18 mg/digit*/
#define LIS302DL_SENSITIVITY_9_2G               72  /* 72 mg/digit*/
#define LIS302DL_LOWPOWERMODE_ACTIVE            ((uint8_t)0x40)
#define LIS302DL_SELFTEST_NORMAL                ((uint8_t)0x00)
#define LIS302DL_XYZ_ENABLE                     ((uint8_t)0x07)
#define LIS302DL_FULLSCALE_2_3                  ((uint8_t)0x00)
#define LIS302DL_FULLSCALE_9_2                  ((uint8_t)0x20)
#define LIS302DL_FILTEREDDATASELECTION_OUTPUTREGISTER     ((uint8_t)0x20)
#define LIS302DL_HIGHPASSFILTER_LEVEL_0         ((uint8_t)0x00)
#define LIS302DL_HIGHPASSFILTER_LEVEL_1         ((uint8_t)0x01)
#define LIS302DL_HIGHPASSFILTER_LEVEL_2         ((uint8_t)0x02)
#define LIS302DL_HIGHPASSFILTER_LEVEL_3         ((uint8_t)0x03)
#define LIS302DL_HIGHPASSFILTERINTERRUPT_OFF    ((uint8_t)0x00)
#define LIS302DL_HIGHPASSFILTERINTERRUPT_1      ((uint8_t)0x04)
#define LIS302DL_HIGHPASSFILTERINTERRUPT_2      ((uint8_t)0x08)
#define LIS302DL_HIGHPASSFILTERINTERRUPT_1_2    ((uint8_t)0x0C)


void AccelerometerReadSPI(uint8_t* data, uint8_t addr, uint8_t count);
void AccelerometerWriteSPI(uint8_t* data, uint8_t addr, uint8_t count);
uint8_t AccelerometerDetect(void);
void AccelerometerConfigure(AccSensitivity_t Sensitivity, AccFilter_t Filter);

uint8_t AccelerometerFound = 0;

//*****************************************************************************

void AccelerometerInit(void) 
{
  //used here
  //hspi1.Init.CLKPolarity = SPI_POLARITY_LOW;
  //hspi1.Init.CLKPhase = SPI_PHASE_1EDGE;
  /* Detect proper device and init it */
  if (AccelerometerDetect() != 0) 
  {
    AccelerometerConfigure(AccSensitivity_2_3G, AccFilter_500mHz);
    AccelerometerFound = 1;
  }
}

void AccelerometerConfigure(AccSensitivity_t Sensitivity, AccFilter_t Filter) 
{
  uint16_t ctrl;
  
  // Reboot 
  AccelerometerReadSPI((uint8_t *)&ctrl, LIS302DL_CTRL_REG2_ADDR, 1);
  ctrl |= LIS302DL_BOOT_REBOOTMEMORY;
  AccelerometerWriteSPI((uint8_t *)&ctrl, LIS302DL_CTRL_REG2_ADDR, 1);
  
  // Init settings
  ctrl = (uint16_t) (LIS302DL_DATARATE_100 | LIS302DL_LOWPOWERMODE_ACTIVE | LIS302DL_SELFTEST_NORMAL | LIS302DL_XYZ_ENABLE);
  
  if (Sensitivity == AccSensitivity_2_3G) 
  {
    ctrl |= (uint16_t) LIS302DL_FULLSCALE_2_3;
  } 
  else if (Sensitivity == AccSensitivity_9_2G) 
  {
    ctrl |= (uint16_t) LIS302DL_FULLSCALE_9_2;
  } else 
  {
    return;
  }

  AccelerometerWriteSPI((uint8_t *)&ctrl, LIS302DL_CTRL_REG1_ADDR, 1);
  
  // Read filter
  AccelerometerWriteSPI((uint8_t *)&ctrl, LIS302DL_CTRL_REG2_ADDR, 1);
  ctrl &= (uint8_t) ~(LIS302DL_FILTEREDDATASELECTION_OUTPUTREGISTER | LIS302DL_HIGHPASSFILTER_LEVEL_3 | LIS302DL_HIGHPASSFILTERINTERRUPT_1_2);

  ctrl |= (uint8_t) (LIS302DL_HIGHPASSFILTERINTERRUPT_OFF | LIS302DL_FILTEREDDATASELECTION_OUTPUTREGISTER);

  if (Filter == AccFilter_2Hz) {
    ctrl |= (uint8_t) LIS302DL_HIGHPASSFILTER_LEVEL_0;
  } else if (Filter == AccFilter_1Hz) {
    ctrl |= (uint8_t) LIS302DL_HIGHPASSFILTER_LEVEL_1;
  } else if (Filter == AccFilter_500mHz) {
    ctrl |= (uint8_t) LIS302DL_HIGHPASSFILTER_LEVEL_2;
  } else if (Filter == AccFilter_250mHz) {
    ctrl |= (uint8_t) LIS302DL_HIGHPASSFILTER_LEVEL_3;
  } else {
    return;
  }

  AccelerometerWriteSPI((uint8_t *)&ctrl, LIS302DL_CTRL_REG2_ADDR, 1);
}

uint8_t AccelerometerDetect(void)
{
  uint8_t id;

  /* Get ID */
  AccelerometerReadSPI(&id, LIS302DL_LIS3DSH_REG_WHO_I_AM, 1);

  if (id == LIS302DL_ID) 
    return 1;

  return 0;
}


void AccelerometerReadAxes(AccelerometerData_t* Axes_Data) 
{
  if (AccelerometerFound == 0)
    return;
  
  int8_t buffer[3];
  AccelerometerReadSPI((uint8_t*)&buffer[0], LIS302DL_OUT_X_ADDR, 1);
  AccelerometerReadSPI((uint8_t*)&buffer[1], LIS302DL_OUT_Y_ADDR, 1);
  AccelerometerReadSPI((uint8_t*)&buffer[2], LIS302DL_OUT_Z_ADDR, 1);
  
  /* Set axes */
  Axes_Data->X = (int16_t) (buffer[0]);
  Axes_Data->Y = (int16_t) (buffer[1]);
  Axes_Data->Z = (int16_t) (buffer[2]);
}


void AccelerometerReadSPI(uint8_t* data, uint8_t addr, uint8_t count) 
{
  ACC_SPI_CS_LOW;
  
  /* Add read bit */
  addr |= 0x80;
  
  if (count > 1) 
  {
    addr |= 0x40; //Add autoincrement bit
  }
  
  HAL_SPI_Transmit(&ACC_SPI_NAME, &addr, 1, 10); //send addr
  HAL_SPI_Receive(&ACC_SPI_NAME, data, count, 10);//read
  ACC_SPI_CS_HIGH;
}

void AccelerometerWriteSPI(uint8_t* data, uint8_t addr, uint8_t count) 
{
  ACC_SPI_CS_LOW;
  
  if (count > 1) 
  {
    /* Add autoincrement bit */
    /* Only LIS302DL device */
    addr |= 0x40;
  }
  
  HAL_SPI_Transmit(&ACC_SPI_NAME, &addr, 1, 10); //send addr
  HAL_SPI_Transmit(&ACC_SPI_NAME, data, count, 10); //send addr
  ACC_SPI_CS_HIGH;
}