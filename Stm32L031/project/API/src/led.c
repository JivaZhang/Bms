#include "stm32l0xx_hal.h"
#include "led.h"
#define LED1_MASK 0x01
#define LED2_MASK 0x03
#define LED3_MASK 0x07
#define LED4_MASK 0x0F
void API_PowerLedOn(void)
{
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_3, GPIO_PIN_SET);
}
void API_PowerLedOff(void)
{
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_3, GPIO_PIN_RESET);
}

void API_FuelGaugeLedExchange(unsigned char percent,char* pLedState)
{
    *pLedState = 0;
	if(percent > 0 && percent <=25)
	{
	    *pLedState |= LED1_MASK;
	}
    else if(percent > 26 && percent <=50)
	{
	    *pLedState |= LED2_MASK;
	}
	else if(percent > 26 && percent <=50)
	{
	    *pLedState |= LED3_MASK;
	}
    else if(percent > 51 && percent <=100)
	{
	    *pLedState |= LED4_MASK;
	}
}
void API_FuelGaugeLedDisplay(char ledState)
{
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_3, ledState&0x01);
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_3, ledState&0x02);
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_3, ledState&0x04);
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_3, ledState&0x08);
}