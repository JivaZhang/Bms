#ifndef __CLOCK_H__
#define __CLOCK_H__

extern TIM_HandleTypeDef        htim1; 
void HAL_MspInit(void);
void HAL_API_SystemClockConfig(void);
#endif //__CLOCK_H__