#ifndef __TIMER_H__
//#include "stm32f1xx_hal_tim.h"
void HAL_TIM_Base_MspInit(TIM_HandleTypeDef* htim_base);
void HAL_TIM_Base_MspDeInit(TIM_HandleTypeDef* htim_base);
extern TIM_HandleTypeDef htim2;
void HAL_API_Timer2Init(void);
void TIM2_IRQHandler(void);
#endif //__TIMER_H__