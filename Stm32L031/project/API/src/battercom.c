#include "stm32l0xx_hal.h"
#include "afe.h"
#include "eeprom.h"
#include "battercom.h"
#include "i2c.h"
SRT_AFE_RAW afeRaw;
#define GAIN_HI_MSK 0x0C
#define GAIN_LOW_MSK 0x0E
#define BAT_STATE_MASK 0x3F
typedef struct _103AT_table
{
	unsigned long resV;
	int tp;
}_103AT_TABLE;
static unsigned short CalculateGain(char hi,char low);
int API_GetBatteryCellVoltage(unsigned long  *plCellVol,unsigned char num);
int API_GetBatteryTemperature(int *pTp);
static unsigned long CalculateAdcRawToVol(unsigned short adcRaw,unsigned short gain,char offset);
static unsigned long CalculateBatVoltage(unsigned short batVolRaw,unsigned short gain,char offset,unsigned char cellNum);
static int TempTable(unsigned long  resVal);
int API_GetBatteryVoltage(unsigned long *pbatVol)
{
    int rtv;
	unsigned char cellNum = CELL_NUMBER;
	unsigned short gain;
	unsigned char offset;
	unsigned short batVolRaw;
	if(pbatVol)
	{
		if(afeRaw.GetBatVoltage(&afeRaw) !=AFE_SUCCESS)
		{
		    return BATTERCOM_FAILED;
		}
		if(afeRaw.GetAdcGain(&afeRaw) != AFE_SUCCESS)
		{
		    return BATTERCOM_FAILED;
		}
	    if(afeRaw.GetAdcOffset(&afeRaw) != AFE_SUCCESS)
		{
		    return BATTERCOM_FAILED;
		}
		batVolRaw = afeRaw.afeReg.VBat.VBatWord;
		//batVolRaw = (((batVolRaw & 0xFF00) >> 8) |((batVolRaw & 0x00FF) << 8));
		gain = CalculateGain(afeRaw.afeReg.ADCGain1.ADCGain1Byte,afeRaw.afeReg.ADCGain2.ADCGain2Byte);
		offset = afeRaw.afeReg.ADCOffset;
		*pbatVol = CalculateBatVoltage(batVolRaw,gain,offset,cellNum);
		rtv = BATTERCOM_OK;
	}
	else
	{
	    rtv = BATTERCOM_PRAM_ERROR;
	}
	return rtv;
}
int API_GetBatteryCellVoltage(unsigned long  *plCellVol,unsigned char num)
{
  	unsigned short gain;
	unsigned short offset;
	unsigned short rawVol;
    unsigned short *pCell;
	pCell = (&(afeRaw.afeReg.VCell11.VCell11Word) + sizeof(short)*num);
	
	if(afeRaw.GetCellVoltage(&afeRaw,num) !=AFE_SUCCESS)
	{
	    return BATTERCOM_FAILED;
	}
	if(afeRaw.GetAdcGain(&afeRaw) != AFE_SUCCESS)
	{
	    return BATTERCOM_FAILED;
	}
	    if(afeRaw.GetAdcOffset(&afeRaw) != AFE_SUCCESS)
	{
	    return BATTERCOM_FAILED;
	}
	
	offset = afeRaw.afeReg.ADCOffset;
	gain = CalculateGain(afeRaw.afeReg.ADCGain1.ADCGain1Byte,afeRaw.afeReg.ADCGain2.ADCGain2Byte);
	
	rawVol = *pCell;
	rawVol = (((rawVol & 0xFF00) >> 8) |((rawVol & 0x00FF) << 8));
	*plCellVol = CalculateAdcRawToVol(rawVol,gain,offset); 
	return  BATTERCOM_OK;
}
int API_GetBatteryCellsVoltage(unsigned long  *plCellVolArray)
{
  	unsigned short gain;
	unsigned short offset;
	unsigned short rawVol;
	unsigned char index;
	unsigned char i;
	unsigned short *pCell;
	unsigned char cellNum[6] = {0,1,4,5,6,9};
	pCell = &(afeRaw.afeReg.VCell1.VCell1Word);

	if(afeRaw.GetAdcGain(&afeRaw) != AFE_SUCCESS)
	{
	    return BATTERCOM_FAILED;
	}
	if(afeRaw.GetAdcOffset(&afeRaw) != AFE_SUCCESS)
	{
	    return BATTERCOM_FAILED;
	}
	
	offset = afeRaw.afeReg.ADCOffset;
	gain = CalculateGain(afeRaw.afeReg.ADCGain1.ADCGain1Byte,afeRaw.afeReg.ADCGain2.ADCGain2Byte);
	
	for(i = 0,index = 0; i < CELL_NUMBER; i++)
	{
	    if(afeRaw.GetCellVoltage(&afeRaw,cellNum[index]) !=AFE_SUCCESS)
		{
			return BATTERCOM_FAILED;
		}
		rawVol = *(pCell +cellNum[index]);
		index++;
		//rawVol = (((rawVol & 0xFF00) >> 8) |((rawVol & 0x00FF) << 8));	
		*plCellVolArray++ = CalculateAdcRawToVol(rawVol,gain,offset); 
	}
	return  BATTERCOM_OK;    
}
int API_GetBatteryCurrent(unsigned long *crt)
{
    *crt = 2.222;
    return 1;
}
//int API_GetBatteryState(PSRT_BAT_INFO pBatInfo)
int API_GetBatteryTemperature(int *pTp)
{
	unsigned short gain;
	unsigned short offset;
	unsigned short tpVolRaw;
	float vtsx = 0,resV;
	if(afeRaw.GetTs1(&afeRaw) !=AFE_SUCCESS)
	{
		return BATTERCOM_FAILED;
	}
	if(afeRaw.GetAdcGain(&afeRaw) != AFE_SUCCESS)
	{
		return BATTERCOM_FAILED;
	}
	if(afeRaw.GetAdcOffset(&afeRaw) != AFE_SUCCESS)
	{
		return BATTERCOM_FAILED;
	}	
	
	tpVolRaw = afeRaw.afeReg.TS1.TS1Word;
	tpVolRaw = (((tpVolRaw & 0xFF00) >> 8) |((tpVolRaw & 0x00FF) << 8));
	gain = CalculateGain(afeRaw.afeReg.ADCGain1.ADCGain1Byte,afeRaw.afeReg.ADCGain2.ADCGain2Byte);
	offset = afeRaw.afeReg.ADCOffset;
	vtsx = (float)CalculateAdcRawToVol(tpVolRaw,gain,offset);
	if((3.3 - vtsx)!= 0)
	{
	    resV = ((10000*vtsx)/(3.3 - vtsx));
	}
	else
	{
	    return AFE_FAILED;
	}
	*pTp = TempTable((unsigned long)resV);
	return AFE_SUCCESS;
}

static int TempTable(unsigned long  resVal)
{   
    _103AT_TABLE    tb[] = 
	{
	  {526,100},
	  {596,95},
	  {677,90},
	  {772,85},
	  {883,100},
	  {1014,95},
	  {1168,90},
	  {1924,85},
	  {2228,80},
	  {2588,75},
	  {3020,70},
	  {3536,65},
	  {4160,60},
	  {4911,55},
	  {5827,50},
	  {6940,45},
	  {8313,40},
	  {10000,100},
	  {12090,95},
	  {14690,90},
	  {17960,85},
	  {22050,100},
	  {27280,95},
	  {33900,90},
	  {42470,85},
	  {53410,80},
	  {67770,75},
	  {111300,70},
	  {144100,65},
	  {188600,60},
	  {247000,55},
	  {329500,50},
	  {0,0},
	};
	
	unsigned char i = 0;
	while(tb[i].resV != 0)
	{
	    if(tb[i].resV >= resVal)
		{
		    return tb[i].tp;
		}
		i++;
	}
	return 0;
}
int API_GetBatteryInfo(PSRT_BAT_INFO pBatInfo)
{
	char fault;
	/*采集电池信息*/
	unsigned long volBat;
	/*status = afeRaw.GetWholeRegVal((void*)(&afeRaw));
	
	if(status != HAL_OK)
	{
	    return BATTERCOM_FAILED;
	}*/
	if(API_GetBatteryVoltage(&volBat) == BATTERCOM_OK)
	{
	    pBatInfo->batVoltage = volBat;
	}
	else
	{
	    return BATTERCOM_FAILED;
	}
	
	API_GetBatteryCellsVoltage(&pBatInfo->cellVoltage[0]);
    API_GetBatteryTemperature(&pBatInfo->batTpr);	
	
	//fault = afeRaw.regArray[SYSTEM_STATUS_ADDR].regVal & BAT_STATE_MASK;
	pBatInfo->batFault = (unsigned int) fault;
	return BATTERCOM_OK;
}
static unsigned long CalculateAdcRawToVol(unsigned short adcRaw,unsigned short gain,char offset)
{
    unsigned long batVol;
	batVol = ((unsigned long)gain)*adcRaw/1000 + offset;    
	return batVol;
}

static unsigned long CalculateBatVoltage(unsigned short batVolRaw,unsigned short gain,char offset,unsigned char cellNum)
{
    unsigned long batVol = 0;
	batVol = ((4*gain*batVolRaw) + 500)/1000 + offset*cellNum;
	return batVol;
}


static unsigned short CalculateGain(char hi,char low)
{
    unsigned short gain = 0;
	hi &= GAIN_HI_MSK;    
	low &= GAIN_LOW_MSK;
    hi <<= 1;
	low>>= 5;
	gain |= hi;
	gain  |= low;
	gain += 365;
	return gain;
}



int API_ConfigureAFE(PSRT_EEPROM_AFE pcfg)
{
	int result = 0;
	unsigned char protectionConfig[5];
	result = I2C_WriteBlockWithCrc(AFE_ADDRESS, PROTECT1, (unsigned char*)(pcfg), 5);
	result = I2C_ReadMulRegContinusWithCrc(AFE_ADDRESS, PROTECT1,5,protectionConfig);
	if(protectionConfig[0] != pcfg->Protect1.Protect1Byte
			|| protectionConfig[1] != pcfg->Protect2.Protect2Byte
			|| protectionConfig[2] != pcfg->Protect3.Protect3Byte
			|| protectionConfig[3] != pcfg->OVTrip
			|| protectionConfig[4] != pcfg->UVTrip)
	{
		result = -1;
	}
	return result;
}
unsigned char API_OvtripExchange(unsigned int ovtrip)
{
	unsigned char offset = afeRaw.afeReg.ADCOffset;
	float gain;
    unsigned char rgOvtrip;
	gain = (365 + ((afeRaw.afeReg.ADCGain1.ADCGain1Byte & 0x0C) << 1) + ((afeRaw.afeReg.ADCGain2.ADCGain2Byte & 0xE0)>> 5)) / 1000.0;
    rgOvtrip = (unsigned char)((((unsigned short)((ovtrip - offset)/gain + 0.5) - OV_THRESH_BASE) >> 4) & 0xFF);
	return rgOvtrip;
}
unsigned char API_UvtripExchange(unsigned int uvtrip)
{
	unsigned char offset = afeRaw.afeReg.ADCOffset;
	float gain;
    unsigned char rgUvtrip;
	gain = (365 + ((afeRaw.afeReg.ADCGain1.ADCGain1Byte & 0x0C) << 1) + ((afeRaw.afeReg.ADCGain2.ADCGain2Byte & 0xE0)>> 5)) / 1000.0;
    rgUvtrip = (unsigned char)((((unsigned short)((uvtrip - offset)/gain + 0.5) - UV_THRESH_BASE) >> 4) & 0xFF);
	return rgUvtrip;
}