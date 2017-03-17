#ifndef __ADC_H__
#define __ADC_H__

//ADC_HandleTypeDef hadc1;
extern ADC_HandleTypeDef hadc;
void HAL_API_AdcInit(void);
void HAL_ADC_MspInit(ADC_HandleTypeDef* hadc);
void HAL_ADC_MspDeInit(ADC_HandleTypeDef* hadc);
void ADC1_COMP_IRQHandler(void);
#endif //__ADC_H__