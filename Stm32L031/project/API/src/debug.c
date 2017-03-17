#include "stm32l0xx_hal.h"
#include "usart.h"
#include "stdio.h"
#include "string.h"
#include <stdarg.h>  
#include "debug.h"
extern UART_HandleTypeDef huart2;
int fputc(int ch,FILE *f)
{
    return HAL_UART_Transmit_IT(&huart2, (unsigned char*)&ch, 1);
}

