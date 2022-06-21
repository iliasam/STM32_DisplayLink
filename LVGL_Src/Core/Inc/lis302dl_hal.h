
#ifndef LIS320DL_HAL_H
#define LIS320DL_HAL_H

typedef enum 
{
  /* LIS302DL */
  AccSensitivity_2_3G,
  AccSensitivity_9_2G
} AccSensitivity_t;

typedef enum {
  /* LIS302DL */
  AccFilter_2Hz,
  AccFilter_1Hz,
  AccFilter_500mHz,
  AccFilter_250mHz
} AccFilter_t;

typedef struct {
  int16_t X;
  int16_t Y;
  int16_t Z;
} AccelerometerData_t;

void AccelerometerInit(void);
void AccelerometerReadAxes(AccelerometerData_t* Axes_Data);

#endif 
