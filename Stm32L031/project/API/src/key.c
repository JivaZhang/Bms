#include "stm32l0xx_hal.h"
#include "key.h"
#include "main.h"
int API_keyDeal(void);
enum
{
    E_NO_KEY,            //0
	E_KEY_DOWN,          //1
	E_KEY_SHORT,         //2
	E_KEY_LONG,          //3
	E_KEY_RELEASE,       //4
};
static char s_keyState;
static int s_holdCnt;
void API_InitKeyVar(void)
{
    s_keyState = E_NO_KEY;
	s_holdCnt = 0;
}

int API_KeyScan(void)
{
    int rtKey = E_PWR_NO_PRESS;
    switch(s_keyState)
	{
	case E_NO_KEY:
		if(HAL_GPIO_ReadPin(SWITH_GPIO_Port,SWITH_Pin) == GPIO_PIN_RESET)
		{
			s_keyState = E_KEY_DOWN;
			s_holdCnt = 0;
		}
	break;	
	case E_KEY_DOWN:
		if(HAL_GPIO_ReadPin(SWITH_GPIO_Port,SWITH_Pin) == GPIO_PIN_RESET)
		{
			s_holdCnt++;
			if(s_holdCnt > 5)
			{
				s_keyState = E_KEY_SHORT;
			}
		}
		else
		{
			s_keyState = E_NO_KEY;
		}
		break;
	case E_KEY_SHORT:
		if(HAL_GPIO_ReadPin(SWITH_GPIO_Port,SWITH_Pin) == GPIO_PIN_RESET)
		{
			s_holdCnt++;
			if(s_holdCnt > 200)
			{
				s_keyState = E_KEY_LONG;
			}  
		}
		else
		{
			rtKey = E_PWR_SHORT_PRESS;
			s_keyState = E_KEY_DOWN;
		}
		break;	
	case E_KEY_LONG:	
		if(HAL_GPIO_ReadPin(SWITH_GPIO_Port,SWITH_Pin) == GPIO_PIN_RESET)
		{
			s_holdCnt++;
		}
		else
		{
			s_keyState = E_KEY_DOWN;
			rtKey = E_PWR_LONG_PRESS;
		}
		break;
	}
	return rtKey;
}

int API_keyDeal(void)
{
    
}