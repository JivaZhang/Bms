#ifndef __BATTERY_COM_H__ 
#define __BATTERY_COM_H__ 
#define CELL_NUMBER     6
#define BATTERCOM_FAILED   1
#define BATTERCOM_OK       0
#define BATTERCOM_PRAM_ERROR 2
typedef struct srt_batteryInfo
{
    long batVoltage;
	long cellVoltage[CELL_NUMBER];
	int batTpr;
	long current;
	unsigned int batFault;
}SRT_BAT_INFO,*PSRT_BAT_INFO;

typedef struct smbus_info{
  unsigned char  smbus_cycle_count;
  unsigned char smbus_capacity;
  unsigned short smbus_volt;
  unsigned int  smbus_current;
  unsigned char smbus_id[16];
  unsigned char smbus_info_state;  
}SMBUS_INFO_MSG,*PSMBUS_INFO_MSG;
int API_GetBatteryVoltage(unsigned long *pVol);
int API_GetBatteryCellsVoltage(unsigned long *pCellVolArray);
int API_GetBatteryTmp(unsigned long *pTrh);
int API_GetBatteryCurrent(unsigned long *crt);
int API_GetBatteryInfo(PSRT_BAT_INFO pBatInfo);
int API_ConfigureAFE(PSRT_EEPROM_AFE pcfg);
unsigned char API_OvtripExchange(unsigned int ovtrip);
unsigned char API_UvtripExchange(unsigned int uvtrip);
#endif //__BATTERY_COM_H__ 
