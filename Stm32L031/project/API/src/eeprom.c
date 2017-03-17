#include "stm32l0xx_hal.h"
#include "eeprom.h"
#include "battercom.h"
#include "afe.h"
#include "i2c.h"
#define AFE_ADDR (DATA_EEPROM_BASE + 0)
#define EEPROM_PAGE_SIZE 128
const unsigned int OVPThreshold = 4300;
const unsigned int UVPThreshold = 2500;
const unsigned char SCDDelay = SCD_DELAY_100us;
const unsigned char SCDThresh = SCD_THRESH_89mV_44mV;
const unsigned char OCDDelay = OCD_DELAY_320ms;
const unsigned char OCDThresh = OCD_THRESH_22mV_11mV;
const unsigned char OVDelay = OV_DELAY_2s;
const unsigned char UVDelay = UV_DELAY_8s;
extern SRT_AFE_RAW afeRaw;
int API_EEpromGetAfeConfig(PSRT_EEPROM_AFE pcfg)
{
    if(!pcfg)
	{
	    return EEPROM_FAILED;
	}
	unsigned int  address = DATA_EEPROM_BASE;
	unsigned char crc8;
	pcfg->Protect1.Protect1Byte = *(__IO unsigned char*)address++;
	pcfg->Protect2.Protect2Byte = *(__IO unsigned char*)address++;
	pcfg->Protect3.Protect3Byte = *(__IO unsigned char*)address++;
	pcfg->OVTrip = *(__IO unsigned char*)address++;
	pcfg->UVTrip = *(__IO unsigned char*)address++;
	pcfg->crc8 = *(__IO unsigned char*)address;
	crc8 = CRC8((unsigned char *)(pcfg),(sizeof(SRT_EEPROM_AFE) - 3),CRC_KEY);
	if(crc8 != pcfg->crc8)
	{
	    return EEPROM_FAILED;
	}
    return EEPROM_SUCCESS;
};
int API_EEpromSetAfeConifg(PSRT_EEPROM_AFE pcfg)
{
  	uint32_t address = DATA_EEPROM_BASE;
	unsigned char crc;
    if(!pcfg)
	{
	    return EEPROM_FAILED;
	}
	crc = CRC8((unsigned char *)(pcfg),(sizeof(SRT_EEPROM_AFE) - 3),CRC_KEY);
	if(crc != pcfg->crc8)
	{
	    return EEPROM_FAILED;
	}
    HAL_FLASHEx_DATAEEPROM_Unlock();
	HAL_FLASHEx_DATAEEPROM_Program(FLASH_TYPEPROGRAMDATA_BYTE, address++, pcfg->Protect1.Protect1Byte);
	HAL_FLASHEx_DATAEEPROM_Program(FLASH_TYPEPROGRAMDATA_BYTE, address++, pcfg->Protect2.Protect2Byte);
	HAL_FLASHEx_DATAEEPROM_Program(FLASH_TYPEPROGRAMDATA_BYTE, address++, pcfg->Protect3.Protect3Byte);
	HAL_FLASHEx_DATAEEPROM_Program(FLASH_TYPEPROGRAMDATA_BYTE, address++, pcfg->OVTrip);
	HAL_FLASHEx_DATAEEPROM_Program(FLASH_TYPEPROGRAMDATA_BYTE, address++, pcfg->UVTrip);
	//pcfg->crc8 = CRC8((unsigned char *)(pcfg),(sizeof(SRT_EEPROM_AFE) - 2),CRC_KEY);
    HAL_FLASHEx_DATAEEPROM_Program(FLASH_TYPEPROGRAMDATA_BYTE, address, pcfg->crc8);
	HAL_FLASHEx_DATAEEPROM_Lock();
	return EEPROM_SUCCESS;
}

int API_EEpromFactoryReset(PSRT_EEPROM_AFE pcfg)
{
    SRT_EEPROM_AFE cfg;
	if(!pcfg)
	{
	    return EEPROM_FAILED;
	}
	cfg.Protect1.Protect1Bit.scd_delay = SCDDelay;
	cfg.Protect1.Protect1Bit.scdThresh = SCDThresh;
	cfg.Protect2.Protect2Bit.ocdDelay = OCDDelay;
	cfg.Protect2.Protect2Bit.ocdThresh = OCDThresh;
	cfg.Protect3.Protect3Bit.ovDelay = OVDelay;
	cfg.Protect3.Protect3Bit.uvDelay = UVDelay;
	cfg.OVTrip = API_OvtripExchange(OVPThreshold);
	cfg.UVTrip =  API_UvtripExchange(UVPThreshold);
	cfg.crc8 = CRC8((unsigned char *)(&cfg),(sizeof(SRT_EEPROM_AFE) - 3),CRC_KEY);
    API_EEpromSetAfeConifg(&cfg);
	pcfg->Protect1.Protect1Bit.scd_delay = cfg.Protect1.Protect1Bit.scd_delay;
	pcfg->Protect1.Protect1Bit.scdThresh = cfg.Protect1.Protect1Bit.scdThresh;
	pcfg->Protect2.Protect2Bit.ocdDelay = cfg.Protect2.Protect2Bit.ocdDelay;
	pcfg->Protect2.Protect2Bit.ocdThresh = cfg.Protect2.Protect2Bit.ocdThresh;
	pcfg->Protect3.Protect3Bit.ovDelay = cfg.Protect3.Protect3Bit.ovDelay;
	pcfg->Protect3.Protect3Bit.uvDelay = cfg.Protect3.Protect3Bit.uvDelay;
	pcfg->OVTrip = cfg.OVTrip;
	pcfg->UVTrip =  cfg.UVTrip;
	pcfg->crc8 = cfg.crc8;
	return EEPROM_SUCCESS;
}


/*int ConfigureBqMaximo(PSRT_EEPROM_AFE pcfg)
{
	int result = 0;
	unsigned char protectionConfig[5];

	result = I2CWriteBlockWithCRC(BQMAXIMO, PROTECT1, &(Registers.Protect1.Protect1Byte), 5);
	result = I2CReadBlockWithCRC(BQMAXIMO, PROTECT1, protectionConfig, 5);
	if(protectionConfig[0] != Registers.Protect1.Protect1Byte
			|| protectionConfig[1] != Registers.Protect2.Protect2Byte
			|| protectionConfig[2] != Registers.Protect3.Protect3Byte
			|| protectionConfig[3] != Registers.OVTrip
			|| protectionConfig[4] != Registers.UVTrip)
	{
		result = -1;
	}
	return result;
}

int InitialisebqMaximo()
{
	int result = 0;

	Registers.Protect1.Protect1Bit.SCD_DELAY = SCDDelay;
	Registers.Protect1.Protect1Bit.SCD_THRESH = SCDThresh;
	Registers.Protect2.Protect2Bit.OCD_DELAY = OCDDelay;
	Registers.Protect2.Protect2Bit.OCD_THRESH = OCDThresh;
	Registers.Protect3.Protect3Bit.OV_DELAY = OVDelay;
	Registers.Protect3.Protect3Bit.UV_DELAY = UVDelay;

	result = GetADCGainOffset();

	Gain = (365 + ((Registers.ADCGain1.ADCGain1Byte & 0x0C) << 1) + ((Registers.ADCGain2.ADCGain2Byte & 0xE0)>> 5)) / 1000.0;
	iGain = 365 + ((Registers.ADCGain1.ADCGain1Byte & 0x0C) << 1) + ((Registers.ADCGain2.ADCGain2Byte & 0xE0)>> 5);

    Registers.OVTrip = (unsigned char)((((unsigned short)((OVPThreshold - Registers.ADCOffset)/Gain + 0.5) - OV_THRESH_BASE) >> 4) & 0xFF);
    Registers.UVTrip = (unsigned char)((((unsigned short)((UVPThreshold - Registers.ADCOffset)/Gain + 0.5) - UV_THRESH_BASE) >> 4) & 0xFF);

    result = ConfigureBqMaximo();
    return result;
}*/