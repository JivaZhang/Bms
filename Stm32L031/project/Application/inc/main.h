/**
  ******************************************************************************
  * File Name          : main.h
  * Description        : This file contains the common defines of the application
  ******************************************************************************
  *
  * COPYRIGHT(c) 2016 STMicroelectronics
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H
  /* Includes ------------------------------------------------------------------*/
#define HFET_DIAG_Pin         GPIO_PIN_0
#define HFET_DIAG_GPIO_Port   GPIOA
#define HFET_CTL_Pin          GPIO_PIN_1
#define HFET_CTL_GPIO_Port    GPIOA
#define CHG_EN_Pin            GPIO_PIN_2
#define CHG_EN_GPIO_Port      GPIOA
#define CP_EN_Pin             GPIO_PIN_3
#define CP_EN_GPIO_Port       GPIOA
#define DSG_EN_Pin            GPIO_PIN_4
#define DSG_EN_GPIO_Port      GPIOA
#define PMON_EN_Pin           GPIO_PIN_5
#define PMON_EN_GPIO_Port     GPIOA
#define PCH_EN_Pin            GPIO_PIN_6
#define PCH_EN_GPIO_Port      GPIOA
#define PACK_VOL_Pin          GPIO_PIN_8
#define PACK_VOL_GPIO_Port    GPIOA
#define LED1_Pin              GPIO_PIN_10
#define LED1_GPIO_Port        GPIOB
#define LED2_Pin              GPIO_PIN_1
#define LED2_GPIO_Port        GPIOB
#define LED3_Pin              GPIO_PIN_11
#define LED3_GPIO_Port        GPIOA
#define POWER_LED_PIN              GPIO_PIN_12
#define POWER_LED_Port             GPIOA
#define SWITH_Pin             GPIO_PIN_13
#define SWITH_GPIO_Port       GPIOC
#define __ORDER_BIG_ENDIAN__  1
#define __ORDER_LITTLE_ENDIAN__  0
#define __BYTE_ORDER__    __ORDER_LITTLE_ENDIAN__
typedef void (*FunctionTable)(void);
enum
{
    E_BMS_START = 0,
	E_BMS_NORMAL = 1,
	E_BMS_SHIP = 2,
};

extern void Error_Handler(void);

/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private define ------------------------------------------------------------*/

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

/**
  * @}
  */ 

/**
  * @}
*/ 

#endif /* __MAIN_H */
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
