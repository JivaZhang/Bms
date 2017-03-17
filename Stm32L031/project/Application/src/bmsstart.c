#include "bmsstart.h"
#include "main.h"
#include "stm32l0xx_hal.h"
#include "gpio.h"
#include "clock.h"
#include "i2c.h"
#include "adc.h"
#include "usart.h"
#include "timer.h"
#include "afe.h"
#include "eeprom.h"
#include "chargedischarge.h"
#include "battercom.h"
#include "./gauge.h"
enum
{
    E_MCU_HAL_INITIAL,
	E_AFE_INITIAL,
	E_FAUL_GAUGE_INITIAL,
	E_DSG_CHG_INITIAL,
};
void BS_McuHalInitial(void);
void BS_AfeInitial(void);
void BS_FaulGaugeInitial(void);
void BS_DsgChgInitial(void);
void BS_ChgDsgDisable(void);
FunctionTable const BmsStartTable[] = 
{
    BS_McuHalInitial,
	BS_AfeInitial,
	BS_FaulGaugeInitial,
	BS_DsgChgInitial,
};
static unsigned char s_bmsStartState = E_MCU_HAL_INITIAL;
extern unsigned char g_bmsState;
extern SRT_AFE_RAW afeRaw;
void BS_BmsStart(void)
{
    (*BmsStartTable[s_bmsStartState])();
}

void BS_McuHalInitial(void)
{
    HAL_Init();
	HAL_API_SystemClockConfig();
	HAL_API_GpioInit();
	//HAL_API_UART2Init();
	API_ChgDsgDisable();//第一时间把冲放电关闭。
	HAL_API_I2c1Init();
	HAL_API_AdcInit();
	HAL_API_Timer2Init();
	//printf("System initial done!");
	s_bmsStartState = E_AFE_INITIAL;
}

void BS_AfeInitial(void)
{
    //拉高AFE的控制引脚，启动AFE芯片。
    //与AFE通信读取相关的参数。
    //如果参数设置不正确，设置相关参数。
    HAL_StatusTypeDef sts;
	SRT_EEPROM_AFE tcfg = {0}; 
    AFE_InitEnv(&afeRaw);
	int status;
    //I2C_WriteRegisterByte(((unsigned char)AFE_ADDRESS), ((unsigned char)SYSTEM_CONTROL2), data);
	//I2C_WriteBlock(((unsigned char)AFE_ADDRESS), ((unsigned char)SYSTEM_CONTROL1), buff,2);
	//status = afeRaw.GetRegisterByte(OV_TRIP,(void*)(&afeRaw));
	//status = afeRaw.GetRegisterByte(0x0c,(void*)(&afeRaw));
	//status = afeRaw.GetRegisterByte(0x0d,(void*)(&afeRaw));*/
	status = afeRaw.GetWholeRegVal((void*)(&afeRaw));
	//status = afeRaw.GetSystemStatus((void*)(&afeRaw));
	if(status != HAL_OK)
	{
	   //while(1);
	}
    if (EEPROM_SUCCESS  != API_EEpromGetAfeConfig(&tcfg)) 
	{
	    API_EEpromFactoryReset(&tcfg);
		API_ConfigureAFE(&tcfg);
	}
	
    s_bmsStartState = E_FAUL_GAUGE_INITIAL;   
}

void BS_FaulGaugeInitial(void)
{
    //gauge();
    s_bmsStartState = E_DSG_CHG_INITIAL;
}

void BS_DsgChgInitial(void)
{
    //s_bmsStartState = E_DSG_CHG_INITIAL;
	g_bmsState = E_BMS_NORMAL;
}