#include "stm32l0xx_hal.h"
#include "bmsnormal.h"
#include "main.h"
#include "chargedischarge.h"
#include "eeprom.h"
#include "battercom.h"
#include "key.h"
#include "string.h"
#include "led.h"
#define MSG_SM2FMU_CMD  (0x01)
#define MSG_FMU2SM_CMD  (0x11)
#define MSG_FMU2SM_SET_CURR_LEN (5)
//static char s_bmsNormalDebugSwith = 0;
//#define PRINTF(s_bmsNormalDebugSwitch,)
/*************bms normal declare start*********************************************/
extern ADC_HandleTypeDef hadc;
enum
{
    E_BMS_STANDBY,           //0
	E_BMS_DSG,               //1
	E_BMS_CHG,               //2
	E_BMS_FIRMWARE_UPDATE,   //3
};
FunctionTable const BmsNormalTable[] = 
{
    BN_BmsStandby,
	BN_BmsDsg,
	BN_BmsChg,
	BN_BmsFirwareUpdate,
};

/*************bms normal declare end**********************************************/

/***********standby  declare start*****/
enum
{
    E_STANDBY_INIT,                  //0
	E_STANDBY_STATE_MNG,             //1
	E_STANDBY_BAT_INFO,              //2
	E_STANDBY_COM_TO_FLC,            //3
	E_STANDBY_KEY_DEAL,              //4
	E_STANDBY_SWITCH_TO_CHG,         //5
	E_STANDBY_SWITCH_TO_DSG,         //6
	E_STANDBY_FAULT_DEAL,            //7
	E_STANDBY_TIMER_EVENT,           //8
	E_STANDBY_FEED_WATCHDOG,         //9
	E_STANDBY_FUEL_GAUGE_DISPLAY,    //10
	E_STANDBY_CHARGE_DETECT,         //11
};
void StandbyInit(void);
void StandbyStateMng(void);
void StandbyBatInfo(void);
void StandbyComToFlc(void);
void StandbyKeyDeal(void);
void StandbySwitchToChg(void);
void StandbySwitchToDsg(void);
void StandbyFaultDeal(void);
void StandbyFeedWatchDog(void);
void StandbyTimerEvent(void);
void StandbyFuelGaugeDisplay(void);
void StandbyChargeDetect(void);
int  ExchangeBatInfo2Txinfo(const PSRT_BAT_INFO pBatInfo,PSMBUS_INFO_MSG pMsg);
void StandbyUsartSendBatInfo(const PSMBUS_INFO_MSG pMsg);
/***********standby  declare end*****/

/***********standby  varable define start*****/
FunctionTable const StandbyTable[] = 
{
    StandbyInit,
	StandbyStateMng,
	StandbyBatInfo,
	StandbyComToFlc,
    StandbyKeyDeal,
    StandbySwitchToChg,
	StandbySwitchToDsg,
	StandbyFaultDeal,
	StandbyTimerEvent,
	StandbyFeedWatchDog,
	StandbyFuelGaugeDisplay,
	StandbyChargeDetect,
};

unsigned char g_standbyEventTick;
unsigned int s_standbyBatInfoUpdateCnt;
unsigned char s_standbyBatInfoUpdateFlag;
unsigned int s_standbyChargeStateDetectCnt;
unsigned char s_standbyChargeStateDetectFlag;
unsigned int s_standbyCom2FlcCnt;
unsigned char s_standbyCom2FlcFlag;
unsigned char s_standbyBatteryFaultFlag;
unsigned char s_standbyChargeOnFlag;
unsigned char s_standbyPowerKeyPressedFlag;
/***********standby  varable define end*****/

/***********discharge  declare start*****/

enum
{
    E_DISCHARGE_INIT,                  //0
	E_DISCHARGE_STATE_MNG,             //1
	E_DISCHARGE_BAT_INFO,              //2
	E_DISCHARGE_COM_TO_FLC,            //3
	E_DISCHARGE_KEY_DEAL,              //4
	E_DISCHARGE_SWITCH_TO_STANDBY,     //5
	E_DISCHARGE_SWITCH_TO_DSG,         //6
	E_DISCHARGE_FAULT_DEAL,            //7
	E_DISCHARGE_TIMER_EVENT,           //8
	E_DISCHARGE_FEED_WATCHDOG,         //9
	E_DISCHARGE_FUEL_GAUGE_DISPLAY,    //10
	E_DISCHARGE_CHARGE_DETECT,         //11
	//E_DISCHARGE_FIRMWARE_UPDATE,       //12
};
void DischargeInit(void);
void DischargeStateMng(void);
void DischargeBatInfo(void);
void DischargeComToFlc(void);
void DischargeKeyDeal(void);
void DischargeSwitchToChg(void);
void DischargeSwitchToStandby(void);
void DischargeFaultDeal(void);
void DischargeFeedWatchDog(void);
void DischargeTimerEvent(void);
void DischargeFuelGaugeDisplay(void);
void DischargeChargeDetect(void);
int  ExchangeBatInfo2Txinfo(const PSRT_BAT_INFO pBatInfo,PSMBUS_INFO_MSG pMsg);
void DischargeUsartSendBatInfo(const PSMBUS_INFO_MSG pMsg);
/*******************discharge  declare end*************************************/


/*******************discharge  varable declare start***************************/
FunctionTable const DischargeTable[] = 
{
    DischargeInit,
	DischargeStateMng,
	DischargeBatInfo,
	DischargeComToFlc,
    DischargeKeyDeal,
    DischargeSwitchToChg,
	DischargeSwitchToStandby,
	DischargeFaultDeal,
	DischargeTimerEvent,
	DischargeFeedWatchDog,
	DischargeFuelGaugeDisplay,
	DischargeChargeDetect,
};
unsigned char g_dischargeEventTick;
unsigned int s_dischargeBatInfoUpdateCnt;
unsigned char s_dischargeBatInfoUpdateFlag;
unsigned int s_dischargeChargeStateDetectCnt;
unsigned char s_dischargeChargeStateDetectFlag;
unsigned int s_dischargeCom2FlcCnt;
unsigned char s_dischargeCom2FlcFlag;
unsigned char s_dischargeBatteryFaultFlag;
unsigned char s_dischargeChargeOnFlag;
unsigned char s_dischargePowerKeyPressedFlag;
unsigned char s_ledTwinkleCnt;
unsigned char s_ledTwinkleFlag;

/********************discharge  varable end*****************************************/

/*********************standby  varable define end***********************************/

/*********************charge  declare start*****************************************/

enum
{
    E_CHARGE_INIT,                  //0
	E_CHARGE_STATE_MNG,             //1
	E_CHARGE_BAT_INFO,              //2
	E_CHARGE_COM_TO_FLC,            //3
	E_CHARGE_KEY_DEAL,              //4
	E_CHARGE_SWITCH_TO_STANDBY,     //5
	E_CHARGE_SWITCH_TO_DSG,         //6
	E_CHARGE_FAULT_DEAL,            //7
	E_CHARGE_TIMER_EVENT,           //8
	E_CHARGE_FEED_WATCHDOG,         //9
	E_CHARGE_FUEL_GAUGE_DISPLAY,    //10
	E_CHARGE_CHARGE_DETECT,         //11
};
void ChargeInit(void);
void ChargeStateMng(void);
void ChargeBatInfo(void);
void ChargeComToFlc(void);
void ChargeKeyDeal(void);
void ChargeSwitchToChg(void);
void ChargeSwitchToStandby(void);
void ChargeFaultDeal(void);
void ChargeFeedWatchDog(void);
void ChargeTimerEvent(void);
void ChargeFuelGaugeDisplay(void);
void ChargeChargeDetect(void);
int  ExchangeBatInfo2Txinfo(const PSRT_BAT_INFO pBatInfo,PSMBUS_INFO_MSG pMsg);
void ChargeUsartSendBatInfo(const PSMBUS_INFO_MSG pMsg);
/**************************charge  declare end*************************************/


/**************************charge  varable declare start***************************/
FunctionTable const ChargeTable[] = 
{
    ChargeInit,
	ChargeStateMng,
	ChargeBatInfo,
	ChargeComToFlc,
    ChargeKeyDeal,
    ChargeSwitchToChg,
	ChargeSwitchToStandby,
	ChargeFaultDeal,
	ChargeTimerEvent,
	ChargeFeedWatchDog,
	ChargeFuelGaugeDisplay,
	ChargeChargeDetect,
};
unsigned char g_chargeEventTick;
unsigned int s_chargeBatInfoUpdateCnt;
unsigned char s_chargeBatInfoUpdateFlag;
unsigned int s_chargeCom2FlcCnt;
unsigned char s_chargeCom2FlcFlag;
unsigned char s_chargeBatteryFaultFlag;
/********************charge  varable end******************************************/
static unsigned char s_bmsNormalState = E_BMS_STANDBY;
static unsigned char s_bmsStandbyState = E_STANDBY_INIT; 
static unsigned char s_bmsDischargeState = E_DISCHARGE_INIT; 
static unsigned char s_bmsChargeState = E_CHARGE_INIT; 
extern unsigned char g_bmsState;
extern UART_HandleTypeDef huart2;
unsigned char g_keyTick;

unsigned char s_batteryFaultFlag;
unsigned char s_batteryInfoUpdateOk;
unsigned char s_displayFaulGauge;
static SRT_BAT_INFO batInfo;
static PSMBUS_INFO_MSG smbus_info_msg;

/**********************bms normal function define start*******************************/
void BS_BmsNormal(void)
{
    do
	{
	    (*BmsNormalTable[s_bmsNormalState])();             
	}
	while(g_bmsState == E_BMS_NORMAL);
}
void BN_BmsStandby(void)
{
	(*StandbyTable[s_bmsStandbyState])();                    
}
void BN_BmsDsg(void)
{ 
    (*DischargeTable[s_bmsDischargeState])();                    
}
void BN_BmsChg(void)
{
                                 
}
void BN_BmsFirwareUpdate(void)
{
        
}

/*****************************bms normal function define end*******************************/



/*****************************standby function define start*******************************/
void StandbyInit(void)
{
    s_bmsStandbyState = E_STANDBY_BAT_INFO;
	API_DisableDischarge();
	//API_EnableChg();
	API_EnableDischarge();
	API_InitKeyVar();
	API_PowerLedOff();
}

void StandbyStateMng(void)
{	
	if(s_batteryFaultFlag)
	{
	    //系统错误处理
	}
	else
	{
		if(0)
		{
			s_bmsNormalState = E_BMS_CHG;
			s_bmsStandbyState = E_STANDBY_BAT_INFO;
		}
		if(1 == s_standbyPowerKeyPressedFlag)
		{
			s_bmsStandbyState = E_STANDBY_SWITCH_TO_DSG;
			s_standbyPowerKeyPressedFlag = 0;
		}
		if(1 == s_standbyChargeOnFlag)
		{
			s_bmsNormalState = E_BMS_CHG;
			s_bmsStandbyState = E_STANDBY_SWITCH_TO_CHG;
		}	    
	}
	s_bmsStandbyState = E_STANDBY_BAT_INFO;
}
void StandbyBatInfo(void)
{
    //unsigned char aa[] = "123";
    if(1 == s_standbyBatInfoUpdateFlag)
	{
	    s_standbyBatInfoUpdateFlag = 0;
	    if (API_GetBatteryInfo(&batInfo) == BATTERCOM_OK)
		{
		    s_batteryInfoUpdateOk = 1;
			//HAL_UART_Transmit_IT(&huart2, aa, sizeof(aa));
		}
		//电池状态检查，如果错误进入错误处理
	}
	s_bmsStandbyState = E_STANDBY_COM_TO_FLC;
}	
void StandbyComToFlc(void)
{
	if(1 == s_standbyCom2FlcFlag)
	{ 
		if(0 == s_batteryInfoUpdateOk)
		{
		    s_standbyCom2FlcFlag = 0;
		    SMBUS_INFO_MSG msg;
		    ExchangeBatInfo2Txinfo(&batInfo,&msg);	
	        StandbyUsartSendBatInfo(&msg);		  
		}
		//TransferBatInfoToFlc();
	}   
	s_bmsStandbyState = E_STANDBY_KEY_DEAL;
}
void StandbyKeyDeal(void)
{
    int key;
    if(g_keyTick)
	{
	    //扫描按键
	    g_keyTick = 0;
        key = API_KeyScan();
		if(key == E_PWR_SHORT_PRESS)
		{
		    s_displayFaulGauge = 1;
		}
		else if(key == E_PWR_LONG_PRESS)
		{
		    s_standbyPowerKeyPressedFlag = 1;
			s_bmsStandbyState = E_STANDBY_SWITCH_TO_DSG;
			return ;
		}
	}
	s_bmsStandbyState = E_STANDBY_CHARGE_DETECT;
}

void StandbyChargeDetect(void)
{
    unsigned char adcV = 0;
    HAL_ADC_Start(&hadc);
    s_bmsStandbyState = E_STANDBY_FAULT_DEAL;
}

void StandbyFaultDeal(void)
{
    s_batteryFaultFlag = 0;
    s_bmsStandbyState = E_STANDBY_FEED_WATCHDOG;
}

void StandbyFeedWatchDog(void)
{
    s_bmsStandbyState = E_STANDBY_FUEL_GAUGE_DISPLAY;
}

void StandbyFuelGaugeDisplay(void)
{
    char ledState;
	unsigned char percent = 100;
    if(1 == s_displayFaulGauge)
	{
		API_FuelGaugeLedExchange(percent,&ledState);
		API_FuelGaugeLedDisplay(ledState);
	}
    s_bmsStandbyState = E_STANDBY_TIMER_EVENT;
}

void StandbyTimerEvent(void)
{
    if(g_standbyEventTick == 1)
	{
	    g_standbyEventTick = 0;
		if(s_standbyBatInfoUpdateCnt++ >=200)
		{
		    s_standbyBatInfoUpdateCnt = 0;
			s_standbyBatInfoUpdateFlag = 1;
		}
		if(s_standbyChargeStateDetectCnt++ > 1)
		{
		    s_standbyChargeStateDetectCnt = 0;
		    s_standbyChargeStateDetectFlag = 1;
		}
	    if(s_standbyCom2FlcCnt++ > 500)
		{
		    s_standbyCom2FlcCnt = 0;
		    s_standbyCom2FlcFlag = 1;
		}
	}
	s_bmsStandbyState = E_STANDBY_STATE_MNG;
}

void StandbySwitchToChg(void)
{
	s_bmsNormalState = E_BMS_CHG;
	s_bmsStandbyState = E_STANDBY_BAT_INFO;
}

void StandbySwitchToDsg(void)
{
	s_bmsNormalState = E_BMS_DSG;
	s_bmsStandbyState = E_STANDBY_INIT;
	s_bmsDischargeState = E_DISCHARGE_INIT;
}
int  ExchangeBatInfo2Txinfo(const PSRT_BAT_INFO pBatInfo,PSMBUS_INFO_MSG pMsg)
{
    
    if(pBatInfo && pMsg)
	{
	    //memset(pMsg, 0, sizeof(SMBUS_INFO_MSG));//all clear, every bit 1 point some info valid,0 invalid
		pMsg->smbus_cycle_count = 80;
		pMsg->smbus_info_state |= 0x01;
		pMsg->smbus_capacity = 60;  //%
		pMsg->smbus_info_state |= 0x02;
		pMsg->smbus_volt = (unsigned short)pBatInfo->batVoltage;   //mV
		pMsg->smbus_info_state |= 0x04;
		pMsg->smbus_current = (unsigned int)pBatInfo->current; //mA
		pMsg->smbus_info_state |= 0x08;
		memcpy(pMsg->smbus_id, "BRSM0012", 8);
		pMsg->smbus_info_state |= 0x10;
	}
}
void StandbyUsartSendBatInfo(const PSMBUS_INFO_MSG pMsg)
{
  unsigned char send_buf[32] = {0};
  unsigned char crc = 0;
  unsigned char i = 0;
  struct smbus_info *smbus = pMsg;
  
  send_buf[0] = send_buf[1] = 0xff;  //info head
  send_buf[2] = 17;   //fix value
  crc = send_buf[0] + send_buf[1] + send_buf[2];
  
  send_buf[3] = MSG_SM2FMU_CMD; 
  send_buf[4] = smbus->smbus_cycle_count;  
  send_buf[5] = smbus->smbus_capacity;   //X%  
  send_buf[6] = (uint8_t)(smbus->smbus_volt & 0xFF);  //volt low 8 bits
  send_buf[7] = (uint8_t)((smbus->smbus_volt >> 8)&0xFF);  //volt high 8 bits
  send_buf[8] = (uint8_t)(smbus->smbus_current & 0xFF);  //current low 8 bits
  send_buf[9] = (uint8_t)((smbus->smbus_current >> 8)&0xFF);  //current mid 8 bits
  send_buf[10] = (uint8_t)((smbus->smbus_current >> 16)&0xFF);  //current high 8 bits
  send_buf[11] = 0;                                             //current highest 8 bit = 0
  memcpy(&send_buf[12], smbus->smbus_id, 7);    //buf[11---17]
  send_buf[19] = smbus->smbus_info_state;
  for(i = 3; i < 20; i++)
    crc += send_buf[i];
  send_buf[20] = crc;
    
  HAL_UART_Transmit_IT(&huart2, send_buf, 21);
}
/*****************************standby function define end***************************************/


/*****************************bms discharge function define start*******************************/
void DischargeInit(void)
{    
    s_bmsDischargeState = E_DISCHARGE_BAT_INFO;
	API_EnableDischarge();
	API_InitKeyVar();
	API_PowerLedOn();
	s_batteryFaultFlag = 0;
	g_chargeEventTick = 0;
}

void DischargeStateMng(void)
{
  	if(s_batteryFaultFlag)
	{
	    //系统错误处理
	}
	else
	{
		/*if(1)
		{
			s_bmsNormalState = E_BMS_CHG;
			s_bmsStandbyState = E_STANDBY_BAT_INFO;
		}*/
		if(1 == s_dischargePowerKeyPressedFlag)
		{
		    s_dischargePowerKeyPressedFlag = 0;
			s_bmsNormalState = E_BMS_STANDBY;
			s_bmsStandbyState = E_STANDBY_INIT;
		}
		/*if(1)//低电流状态持续30分钟
		{
			s_bmsNormalState = E_BMS_STANDBY;
			s_bmsStandbyState = E_STANDBY_INIT;
		} */
	}
    s_bmsDischargeState = E_DISCHARGE_BAT_INFO;
}

void DischargeBatInfo(void)
{
    if(1 == s_standbyBatInfoUpdateFlag)
	{
	    s_standbyBatInfoUpdateFlag = 0;
	    if (API_GetBatteryInfo(&batInfo) == BATTERCOM_OK)
		{
		    s_batteryInfoUpdateOk = 1;
		}
		//电池状态检查，如果错误进入错误处理
	}
    s_bmsDischargeState = E_DISCHARGE_COM_TO_FLC;
}

void DischargeComToFlc(void)
{
  	if(1 == s_standbyCom2FlcFlag)
	{  
	    s_standbyCom2FlcFlag = 0;
		if(1 == s_batteryInfoUpdateOk)
		{
		    SMBUS_INFO_MSG msg;
		    ExchangeBatInfo2Txinfo(&batInfo,&msg);	
	        StandbyUsartSendBatInfo(&msg);		  
		}
		//TransferBatInfoToFlc();
	}   
    s_bmsDischargeState = E_DISCHARGE_KEY_DEAL;
}

void DischargeKeyDeal(void)
{
    int key;
    if(g_keyTick)
	{
	    //扫描按键
	    g_keyTick = 0;
        key = API_KeyScan();
		if(key == E_PWR_SHORT_PRESS)
		{
		    s_displayFaulGauge = 1;
		}
		else if(key == E_PWR_LONG_PRESS)
		{
		    s_dischargePowerKeyPressedFlag = 1;
		}
	}
	//s_bmsStandbyState = E_STANDBY_CHARGE_DETECT;
    s_bmsDischargeState = E_DISCHARGE_FAULT_DEAL;
}

void DischargeSwitchToChg(void)
{
    s_bmsDischargeState = E_DISCHARGE_BAT_INFO;
}

void DischargeSwitchToStandby(void)
{
    s_bmsDischargeState = E_DISCHARGE_BAT_INFO;
}

void DischargeFaultDeal(void)
{
    s_batteryFaultFlag = 0;
    s_bmsDischargeState = E_DISCHARGE_TIMER_EVENT;
}

void DischargeFeedWatchDog(void)
{
    s_bmsDischargeState = E_DISCHARGE_FUEL_GAUGE_DISPLAY;
}

void DischargeTimerEvent(void)
{
    if(g_dischargeEventTick == 1)
	{
	    g_dischargeEventTick = 0;
		if(s_dischargeBatInfoUpdateCnt++ >=200)
		{
		    s_dischargeBatInfoUpdateCnt = 0;
			s_dischargeBatInfoUpdateFlag = 1;
		}
		if(s_dischargeChargeStateDetectCnt++ > 1)
		{
		    s_dischargeChargeStateDetectCnt = 0;
		    s_dischargeChargeStateDetectFlag = 1;
		}
	    if(s_dischargeCom2FlcCnt++ > 500)
		{
		    s_dischargeCom2FlcCnt = 0;
		    s_dischargeCom2FlcFlag = 1;
		}
		if(s_ledTwinkleCnt++ >200)
		{
		    s_ledTwinkleCnt = 0;
			s_ledTwinkleFlag = 1;
		}
	}
    s_bmsDischargeState = E_DISCHARGE_FEED_WATCHDOG;
}

void DischargeFuelGaugeDisplay(void)
{
    char ledState;
	unsigned char percent = 100;
	if(1 == s_displayFaulGauge)
	{
	    API_FuelGaugeLedExchange(percent,&ledState);
        API_FuelGaugeLedDisplay(ledState);  
	}
    s_bmsDischargeState = E_DISCHARGE_CHARGE_DETECT;
}

void DischargeChargeDetect(void)
{
    unsigned char adcV = 0;
    HAL_ADC_Start(&hadc);
    s_bmsDischargeState = E_DISCHARGE_STATE_MNG;
}

/*int  ExchangeBatInfo2Txinfo(const PSRT_BAT_INFO pBatInfo,PSMBUS_INFO_MSG pMsg)
{
    s_bmsDischargeState = E_DISCHARGE_BAT_INFO;
}*/
void DischargeUsartSendBatInfo(const PSMBUS_INFO_MSG pMsg)
{
    s_bmsDischargeState = E_DISCHARGE_BAT_INFO;
}
/************************bms discharge function define end*******************************/


/***********************bms charge function define start*******************************/
void ChargeInit(void)
{
    
}

void ChargeStateMng(void)
{
    //预充电
    //正常充电
    //充电结束
    //监测充电器拔出
    //切换到待机状态
}

void ChargeBatInfo(void)
{
    
}

void ChargeComToFlc(void)
{
    
}

void ChargeKeyDeal(void)
{
    
}

void ChargeSwitchToChg(void)
{
    
}

void ChargeSwitchToStandby(void)
{
    
}

void ChargeFaultDeal(void)
{
     
}

void ChargeFeedWatchDog(void)
{
    
}

void ChargeTimerEvent(void)
{
    
}

void ChargeFuelGaugeDisplay(void)
{
    
}

void ChargeChargeDetect(void)
{
     
}

/*int  ExchangeBatInfo2Txinfo(const PSRT_BAT_INFO pBatInfo,PSMBUS_INFO_MSG pMsg)
{
    
}*/

void ChargeUsartSendBatInfo(const PSMBUS_INFO_MSG pMsg)
{
    
}
/**********************bms charge function define end*******************************/