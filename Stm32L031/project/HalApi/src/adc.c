#include "stm32l0xx_hal.h"
//#include "stm32fl0xx.h"
//#include "stm32flxx_it.h"
#include "main.h"
#include "adc.h"
ADC_HandleTypeDef hadc;
static void MX_ADC_Init(void);
void HAL_ADC_MspInit(ADC_HandleTypeDef* hadc);
void HAL_ADC_MspDeInit(ADC_HandleTypeDef* hadc);
void HAL_API_AdcInit(void)
{
    MX_ADC_Init();
}

/* ADC1 init function */
static void MX_ADC_Init(void)
{

  ADC_ChannelConfTypeDef sConfig;
  HAL_StatusTypeDef  status;
  ADC_AnalogWDGConfTypeDef AnalogWDGConfig;
    /**Configure the global features of the ADC (Clock, Resolution, Data Alignment and number of conversion) 
    */
  hadc.Instance = ADC1;
  hadc.Init.OversamplingMode = DISABLE;
  hadc.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV1;
  hadc.Init.Resolution = ADC_RESOLUTION_12B;
  hadc.Init.SamplingTime = ADC_SAMPLETIME_1CYCLE_5;
  hadc.Init.ScanConvMode = ADC_SCAN_DIRECTION_FORWARD;
  hadc.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  hadc.Init.ContinuousConvMode = DISABLE;
  hadc.Init.DiscontinuousConvMode = ENABLE;
  hadc.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
  hadc.Init.ExternalTrigConv = ADC_SOFTWARE_START;
  hadc.Init.DMAContinuousRequests = DISABLE;
  hadc.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
  hadc.Init.Overrun = ADC_OVR_DATA_PRESERVED;
  hadc.Init.LowPowerAutoWait = DISABLE;
  hadc.Init.LowPowerFrequencyMode = DISABLE;
  hadc.Init.LowPowerAutoPowerOff = DISABLE;
  if (HAL_ADC_Init(&hadc) != HAL_OK)
  {
    Error_Handler();
  }

    /**Configure for the selected ADC regular channel to be converted. 
    */
  /*sConfig.Channel = ADC_CHANNEL_3;
  sConfig.Rank = ADC_RANK_CHANNEL_NUMBER;
  if (HAL_ADC_ConfigChannel(&hadc, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }*/

	/**Configure for the selected ADC regular channel to be converted. 
	*/
  sConfig.Channel = ADC_CHANNEL_7;
  if (HAL_ADC_ConfigChannel(&hadc, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
  
  AnalogWDGConfig.WatchdogMode = ADC_ANALOGWATCHDOG_SINGLE_REG;
  AnalogWDGConfig.Channel = ADC_CHANNEL_3;
  AnalogWDGConfig.ITMode = ENABLE;
  AnalogWDGConfig.HighThreshold = 4095;
  AnalogWDGConfig.LowThreshold = 0;
  if (HAL_ADC_AnalogWDGConfig(&hadc, &AnalogWDGConfig) != HAL_OK)
  {
    Error_Handler();
  }
  status = HAL_ADCEx_Calibration_Start(&hadc,ADC_SINGLE_ENDED);
  if(status != HAL_OK)
  {
	  HAL_ADC_Start(&hadc);
	  status = HAL_ADCEx_Calibration_Start(&hadc,ADC_SINGLE_ENDED);
	  if(status != HAL_OK)
	  {
	      while(1);
	  }
  }
  
  
  HAL_ADC_Start_IT(&hadc);  
  //HAL_ADC_Start();
}

void HAL_ADC_MspInit(ADC_HandleTypeDef* hadc)
{

  GPIO_InitTypeDef GPIO_InitStruct;
  if(hadc->Instance==ADC1)
  {
  /* USER CODE BEGIN ADC1_MspInit 0 */

  /* USER CODE END ADC1_MspInit 0 */
    /* Peripheral clock enable */
    __HAL_RCC_ADC1_CLK_ENABLE();
  
    /**ADC GPIO Configuration    
    PC0-CK_IN     ------> ADC_IN0
    PA7     ------> ADC_IN7 
    */
    //GPIO_InitStruct.Pin = HFET_DIAG_Pin|PACK_VOL_Pin;
	GPIO_InitStruct.Pin = PACK_VOL_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(PACK_VOL_GPIO_Port, &GPIO_InitStruct);
    
  /* Peripheral interrupt init */
    HAL_NVIC_SetPriority(ADC1_COMP_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(ADC1_COMP_IRQn);
  /* USER CODE BEGIN ADC1_MspInit 1 */
    
  /* USER CODE END ADC1_MspInit 1 */
  }

}

void HAL_ADC_MspDeInit(ADC_HandleTypeDef* hadc)
{

  if(hadc->Instance==ADC1)
  {
  /* USER CODE BEGIN ADC1_MspDeInit 0 */

  /* USER CODE END ADC1_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_ADC1_CLK_DISABLE();
  
    /**ADC GPIO Configuration    
    PA0-CK_IN     ------> ADC_IN0
    PA7     ------> ADC_IN7 
    */
    HAL_GPIO_DeInit(GPIOA, HFET_DIAG_Pin|PACK_VOL_Pin);
  }
  /* USER CODE BEGIN ADC1_MspDeInit 1 */

  /* USER CODE END ADC1_MspDeInit 1 */
}

void ADC1_COMP_IRQHandler(void)
{
  /* USER CODE BEGIN ADC1_COMP_IRQn 0 */

  /* USER CODE END ADC1_COMP_IRQn 0 */
  HAL_ADC_IRQHandler(&hadc);
  /* USER CODE BEGIN ADC1_COMP_IRQn 1 */

  /* USER CODE END ADC1_COMP_IRQn 1 */
}

void HAL_ADC_LevelOutOfWindowCallback(ADC_HandleTypeDef* hadc)
{
  /* Prevent unused argument(s) compilation warning */
  HAL_ADC_GetValue(hadc);

  /* NOTE : This function should not be modified. When the callback is needed,
            function HAL_ADC_LevelOoutOfWindowCallback must be implemented in the user file.
   */
}
void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc)
{
  /* Prevent unused argument(s) compilation warning */
  HAL_ADC_GetValue(hadc);
  
  //HAL_ADC_Stop_IT(hadc);
  /* NOTE : This function should not be modified. When the callback is needed,
            function HAL_ADC_ConvCpltCallback must be implemented in the user file.
   */
}