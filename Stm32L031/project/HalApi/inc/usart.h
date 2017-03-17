#ifndef __USART_H__
#define __USART_H__
extern UART_HandleTypeDef huart2;
void HAL_API_UART2Init(void);
void HAL_UART_MspDeInit(UART_HandleTypeDef* huart);
void HAL_UART_MspInit(UART_HandleTypeDef* huart);
#endif //__USART_H__