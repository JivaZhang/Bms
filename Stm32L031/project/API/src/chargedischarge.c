#include "stm32l0xx_hal.h"
#include "chargedischarge.h"
#include "main.h"
void API_ChgDsgDisable(void)
{
    HAL_GPIO_WritePin(GPIOA, CHG_EN_Pin, RESET);
}
int API_EnableChg(void)
{
    HAL_GPIO_WritePin(GPIOA, CHG_EN_Pin, SET);
}

int API_DisableChg(void)
{
     HAL_GPIO_WritePin(GPIOA, CHG_EN_Pin, RESET);
}

int API_EnableDischarge(void)
{
    HAL_GPIO_WritePin(GPIOA, DSG_EN_Pin, SET);
    HAL_GPIO_WritePin(GPIOA, CP_EN_Pin, SET);
	HAL_GPIO_WritePin(POWER_LED_Port, POWER_LED_PIN, GPIO_PIN_RESET);
}

int API_DisableDischarge(void)
{
    HAL_GPIO_WritePin(GPIOA, DSG_EN_Pin, RESET);
	HAL_GPIO_WritePin(GPIOA, CP_EN_Pin, RESET);
	HAL_GPIO_WritePin(POWER_LED_Port, POWER_LED_PIN, GPIO_PIN_SET);
}

