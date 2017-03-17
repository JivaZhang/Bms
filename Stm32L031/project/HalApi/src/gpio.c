#include "stm32l0xx_hal.h"
//#include "stm32f1xx.h"
//#include "stm32f1xx_it.h"
#include "main.h"
#include "gpio.h"
static void MX_GPIO_Init(void);

void HAL_API_GpioInit(void)
{
    MX_GPIO_Init();
}

void SysTick_Handler(void)
{
  /* USER CODE BEGIN SysTick_IRQn 0 */

  /* USER CODE END SysTick_IRQn 0 */
  HAL_IncTick();
  HAL_SYSTICK_IRQHandler();
  /* USER CODE BEGIN SysTick_IRQn 1 */

  /* USER CODE END SysTick_IRQn 1 */
}

static void MX_GPIO_Init(void)
{

  GPIO_InitTypeDef GPIO_InitStruct;

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();
  
  /*Configure GPIO pins : HFET_CTL_Pin CHG_EN_Pin CP_EN_Pin DSG_EN_Pin 
                           PMON_EN_Pin PCH_EN_Pin LED3_Pin LED4_Pin */
  GPIO_InitStruct.Pin = DSG_EN_Pin|CHG_EN_Pin|PCH_EN_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
  HAL_GPIO_WritePin(GPIOA, DSG_EN_Pin, GPIO_PIN_RESET);
  
  GPIO_InitStruct.Pin = POWER_LED_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(POWER_LED_Port, &GPIO_InitStruct);
  HAL_GPIO_WritePin(POWER_LED_Port, POWER_LED_PIN, GPIO_PIN_SET);
  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, HFET_CTL_Pin|CHG_EN_Pin|CP_EN_Pin|DSG_EN_Pin 
                          |PMON_EN_Pin|PCH_EN_Pin|LED3_Pin, GPIO_PIN_RESET);
  

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, LED1_Pin|LED2_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pins : HFET_CTL_Pin CHG_EN_Pin CP_EN_Pin DSG_EN_Pin 
                           PMON_EN_Pin PCH_EN_Pin LED3_Pin LED4_Pin 
  GPIO_InitStruct.Pin = HFET_CTL_Pin|CHG_EN_Pin|CP_EN_Pin|DSG_EN_Pin 
                          |PMON_EN_Pin|PCH_EN_Pin|LED3_Pin|LED4_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);*/

  /*Configure GPIO pins : LED1_Pin LED2_Pin */
  GPIO_InitStruct.Pin = LED1_Pin|LED2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
  HAL_GPIO_WritePin(GPIOB, LED1_Pin, GPIO_PIN_RESET);
  /*Configure GPIO pin : SWITH_Pin */
  GPIO_InitStruct.Pin = SWITH_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(SWITH_GPIO_Port, &GPIO_InitStruct);
  HAL_GPIO_WritePin(SWITH_GPIO_Port, SWITH_Pin, GPIO_PIN_SET);
  
  /*Configure GPIO pin : PB3 */
  GPIO_InitStruct.Pin = GPIO_PIN_3;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pins : PB4 PB5 */
  GPIO_InitStruct.Pin = GPIO_PIN_4|GPIO_PIN_5;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  GPIO_InitStruct.Pin = PMON_EN_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
  
  GPIO_InitStruct.Pin = CP_EN_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(CP_EN_GPIO_Port, &GPIO_InitStruct);  
  HAL_GPIO_WritePin(GPIOA, CHG_EN_Pin, GPIO_PIN_SET);
  //HAL_GPIO_WritePin(PMON_EN_GPIO_Port, PMON_EN_Pin, GPIO_PIN_SET);
  HAL_GPIO_WritePin(GPIOA, CHG_EN_Pin, RESET);
}