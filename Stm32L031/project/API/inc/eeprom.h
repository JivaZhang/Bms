#ifndef __EEPROM_H__
#define __EEPROM_H__
#define EEPROM_FAILED  1
#define EEPROM_SUCCESS 0
typedef struct _eepromStct
{
	union
	{
		struct
		{
			unsigned char scdThresh	:3;
			unsigned char scd_delay	:2;
			unsigned char rsvd			:2;
			unsigned char rsns		    :1;
		}Protect1Bit;
		unsigned char Protect1Byte;
	}Protect1;

	union
	{
		struct
		{
			unsigned char ocdThresh 	:4;
			unsigned char ocdDelay 	:3;
			unsigned char rsvd		    :1;
		}Protect2Bit;
		unsigned char Protect2Byte;
	}Protect2;

	union
	{
		struct
		{
			unsigned char rsvd			:4;
			unsigned char ovDelay		:2;
			unsigned char uvDelay		:2;
		}Protect3Bit;
		unsigned char Protect3Byte;
	}Protect3;

	unsigned char OVTrip;
	unsigned char UVTrip;
	unsigned char crc8; 
	unsigned char reserve1; 
	unsigned char reserve2; 
}SRT_EEPROM_AFE,*PSRT_EEPROM_AFE;

/*typedef struct _eepromStct
{
   unsigned char scdDelay;
   unsigned char scdThresh;
   unsigned char ocdDelay;
   unsigned char ocdThread;
   unsigned char ovDealy;
   unsigned char uvDealy;
   unsigned short ovTrip;
   unsigned short uvTrip;
   unsigned char crc8;
   unsigned char reserve;   //for align to word
}SRT_EEPROM_AFE,*PSRT_EEPROM_AFE;*/
int API_EEpromGetAfeConfig(PSRT_EEPROM_AFE pcfg);
int API_EEpromSetAfeConfig(PSRT_EEPROM_AFE pcfg);
int API_EEpromFactoryReset(PSRT_EEPROM_AFE pcfg);
int API_EEpromSetAfeConifg(PSRT_EEPROM_AFE pcfg);
#endif //__EEPROM_H__