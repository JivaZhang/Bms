#include "afe.h"
#include "stm32l0xx_hal.h"
#include "i2c.h"
#include "stdio.h"
#include "string.h"
#define AFE_GetBitValue(data,bit) do {data >>= bit;}while(0)
int AFE_InitEnv(PSRT_AFE_RAW praw);
static int AFE_DeviceAddressCheck(unsigned char registerAddress);
static int AFE_SetRegBit(unsigned char regAddr,char bit,char bitValue,unsigned char *pByte);
static int AFE_SetRegByte(unsigned char regAddr,unsigned char data,unsigned char *pByte);
static int AFE_SetRegNBytes(unsigned char regAddr,unsigned char data,unsigned char length,void* this);
static int AFE_GetWholeRegVal(void* this);
static int AFE_GetRegBit(unsigned char regAddr,char bit,unsigned char *pByte);
static int AFE_GetRegByte(unsigned char regAddr,unsigned char *pByte);
static int AFE_GetRegWord(unsigned char regAddr,unsigned short *pWord);
static int AFE_GetMulReg(unsigned char regAddr,unsigned char lenth,unsigned char  *pByte);


static int AFE_GetSystemStatus(void* this);
static int AFE_GetOcdStatus(void* this);
static int AFE_GetScdStatus(void* this);
static int AFE_GetOvStatus(void* this);
static int AFE_GetUvStatus(void* this);
static int AFE_GetDevxdStatus(void* this);
static int AFE_GetCcrdyStatus(void* this);
static int AFE_EnterShipMode(void *this);
static int AFE_GetTempsStaus(void *this);
static int AFE_GetAdcStaus(void *this);
static int AFE_SetAdcStatus(void *this,unsigned char status);
static int AFE_GetChgOnStatus(void *this);
static int AFE_GetDsgOnStatus(void *this);
static int AFE_SetChgStatus(void *this,unsigned status);
static int AFE_SetDsgStatus(void *this,unsigned status);
static int AFE_GetCcEnStatus(void *this);
static int AFE_SetCcEnStatus(void *this,unsigned char status);
static int AFE_GetDlyDisStatus(void *this);
static int AFE_SetDlyDisStatus(void *this,unsigned status);
static int AFE_GetCellBalanceStatus(void *this,unsigned char cellNum);
static int AFE_SetCellBalanceStatus(void *this,unsigned char cellNum);
static int AFE_GetProtection1(void *this);
static int AFE_SetProtection1(void* this,unsigned char value);
static int AFE_GetProtection2(void *this);
static int AFE_SetProtection2(void* this,unsigned char value);
static int AFE_GetProtection3(void *this);
static int AFE_SetProtection3(void* this,unsigned char value);
static int AFE_GetUv(void *this);
static int AFE_SetUv(void *this,unsigned char value);
static int AFE_GetOv(void *this);
static int AFE_SetOv(void *this,unsigned char value);
static int AFE_GetCellVoltage(void *this,unsigned char num);
static int AFE_GetCellsVoltage(void *this,unsigned char startNum,unsigned char num);
static int AFE_GetBatVoltage(void *this);
static int AFE_GetAdcGain(void *this);
static int AFE_GetAdcOffset(void *this);
static int AFE_GetTs1(void *this);

/**
  * @brief  initial afe register struct
  * @note   
  * @param  PSRT_AFE_RAW praw
* @retval int type return value:AFE_FAILED(1) ---praw is null,AFE_SUCCESS(0) --- succesfull
  */

int AFE_InitEnv(PSRT_AFE_RAW praw)
{
    //unsigned char i;
    if(!praw)
	{
	    return AFE_FAILED;//
	}
	praw->GetWholeRegVal = AFE_GetWholeRegVal;
	praw->GetSystemStatus = AFE_GetSystemStatus;
	praw->GetOcdStatus = AFE_GetOcdStatus;
	praw->GetScdStatus = AFE_GetScdStatus;
	praw->GetOvStatus = AFE_GetOvStatus;
	praw->GetUvStatus = AFE_GetUvStatus;
    praw->GetDevxdStatus = AFE_GetDevxdStatus;
    praw->GetCcrdyStatus = AFE_GetCcrdyStatus;
    praw->EnterShipMode = AFE_EnterShipMode;
    praw->GetTempsStaus = AFE_GetTempsStaus;
    praw->GetAdcStaus = AFE_GetAdcStaus;
    praw->SetAdcStatus = AFE_SetAdcStatus;
    praw->GetChgOnStatus = AFE_GetChgOnStatus;
    praw->GetDsgOnStatus = AFE_GetDsgOnStatus;
    praw->SetChgStatus = AFE_SetChgStatus;
    praw->SetDsgStatus = AFE_SetDsgStatus;
    praw->GetCcEnStatus = AFE_GetCcEnStatus;
    praw->SetCcEnStatus = AFE_SetCcEnStatus;
    praw->GetDlyDisStatus = AFE_GetDlyDisStatus;
    praw->SetDlyDisStatus = AFE_SetDlyDisStatus;
    praw->GetCellBalanceStatus = AFE_GetCellBalanceStatus;
    praw->SetCellBalanceStatus = AFE_SetCellBalanceStatus;
    praw->GetProtection1 = AFE_GetProtection1;
    praw->SetProtection1 = AFE_SetProtection1;
    praw->GetProtection2 = AFE_GetProtection2;
    praw->SetProtection2 = AFE_SetProtection2;
    praw->GetProtection3 = AFE_GetProtection3;
    praw->SetProtection3 = AFE_SetProtection3;
    praw->GetUv = AFE_GetUv;
    praw->SetUv = AFE_SetUv;
    praw->GetOv = AFE_GetOv;
    praw->SetOv = AFE_SetOv;
	praw->GetTs1 = AFE_GetTs1;
    praw->GetCellVoltage = AFE_GetCellVoltage;
	praw->GetCellsVoltage = AFE_GetCellsVoltage;
    praw->GetBatVoltage  = AFE_GetBatVoltage;
    praw->GetAdcGain = AFE_GetAdcGain;
    praw->GetAdcOffset = AFE_GetAdcOffset;
	return AFE_SUCCESS;
}
static int AFE_DeviceAddressCheck(unsigned char registerAddress)
{
    if (registerAddress < CONTINUS_REGISTER_NUM || registerAddress == ADC_GAIN1 || registerAddress == ADC_GAIN2 || registerAddress == ADC_OFFSET)
	{
	    return AFE_SUCCESS;
	}
	else
	{
	    return AFE_FAILED;
	}
}
static int AFE_GetRegBit(unsigned char regAddr,char bit,unsigned char *pByte)
{
    int rtv = AFE_SUCCESS;
	unsigned char byteValue;
#ifdef I2C_WITH_CRC
	rtv = I2C_ReadSingleReg((unsigned char)AFE_ADDRESS,(unsigned char)regAddr,&byteValue);
#else
	rtv = I2C_ReadSingleRegWithCrc((unsigned char)AFE_ADDRESS,(unsigned char)regAddr,&byteValue);
#endif
		if(rtv == I2C_ERROR)
    	{
		  rtv = AFE_FAILED;
		}	    
	AFE_GetBitValue((byteValue),bit);
	*pByte = byteValue&0x01;
	return rtv;
}

static int AFE_GetRegByte(unsigned char regAddr,unsigned char *pByte)
{
    int rtv = AFE_SUCCESS;
	
	if(AFE_DeviceAddressCheck(regAddr) == AFE_SUCCESS)
	{
#ifdef I2C_WITH_CRC
	    rtv = I2C_ReadSingleRegWithCrc((unsigned char)AFE_ADDRESS,(unsigned char)regAddr,pByte);
#else
	    rtv = I2C_ReadSingleReg((unsigned char)AFE_ADDRESS,(unsigned char)regAddr,pByte);
#endif
		if(rtv == I2C_ERROR)
    	{
		  rtv = AFE_FAILED;
		}
	}
	else
	{
	    rtv = AFE_FAILED;
	}
	return rtv;
}

/*static int AFE_GetWholeRegVal(void* this)
{
    unsigned char da1[CONTINUS_REGISTER_NUM]; //3 =  ADC_GIAN1+ADC_OFFSET + ADC_GIAN2
	unsigned char da2[3];//ADC_GIAN1&ADC_OFFSET &ADC_GIAN2
	int rtv;
	unsigned char i = 0;
	PSRT_AFE_RAW pthis = (PSRT_AFE_RAW)this;
    rtv = I2C_ReadMulRegContinus((unsigned char)AFE_ADDRESS,(char)SYSTEM_STATUS_ADDR, CONTINUS_REGISTER_NUM,da1);
	if(rtv == I2C_OK)
	{
	   rtv = I2C_ReadMulRegContinus((unsigned char)AFE_ADDRESS,(char)ADC_GAIN1, 2,da2);//3 =  ADC_GIAN1+ADC_OFFSET + ADC_GIAN2
	   if(rtv == I2C_OK) 
	   {
		   rtv = I2C_ReadSingleReg((unsigned char)AFE_ADDRESS,(char)ADC_GAIN2,&da2[2]);
		   if(rtv != I2C_OK) 
	       {
	           return AFE_FAILED;
	       }
	   }
	}
	for(i = 0; i < CONTINUS_REGISTER_NUM; i++)
	{
	     pthis->regArray[i].regVal = da1[i];
		 
	}
	pthis->regArray[ADC_GAIN1].regVal = da2[0];
	pthis->regArray[ADC_OFFSET].regVal = da2[1];
	pthis->regArray[ADC_GAIN2].regVal = da2[2];
    return AFE_SUCCESS;
}*/



static int AFE_SetRegBit(unsigned char regAddr,char bit,char bitValue,unsigned char* pByte)
{
   // int I2C_WriteRegByte(unsigned char i2cSlaveAddr, unsigned char reg, unsigned char data)
    unsigned char cv;
	if(AFE_DeviceAddressCheck(regAddr) == AFE_FAILED)
	{
	    return AFE_FAILED;
	}
    if (I2C_ReadSingleReg((unsigned char)AFE_ADDRESS,regAddr,&cv) == I2C_OK)
	{
	    if(0 == bitValue)
		{
		    cv &= ~(1 << bit);
		}
		else
		{
		    cv |= 1 << bit;
		}
	}
	else
	{
	    return AFE_FAILED;
	}
	
    if (I2C_WriteRegByte((unsigned char)AFE_ADDRESS,regAddr,cv) == I2C_OK)
	{
	    *pByte = cv;
	}
    return AFE_SUCCESS;
}

static int AFE_SetRegByte(unsigned char regAddr,unsigned char data,unsigned char *pByte)
{
	//int status;
  	if(AFE_DeviceAddressCheck(regAddr) == AFE_FAILED)
	{
	    return AFE_FAILED;
	}
    if (I2C_WriteRegByte((unsigned char)AFE_ADDRESS,regAddr,data) == I2C_OK)
	{
	    *pByte = data;
	}
	return AFE_SUCCESS;
}

static int AFE_SetRegNBytes(unsigned char regAddr,unsigned char data,unsigned char length,void* this)
{
    
}
static int AFE_GetRegWord(unsigned char regAddr,unsigned short *pWord)
{
	int rtv;
#ifdef I2C_WITH_CRC	
    rtv = I2C_ReadRegisterWordWithCrc((unsigned char)AFE_ADDRESS,(unsigned char)regAddr, pWord);
#else
    rtv = I2C_ReadMulRegContinus((unsigned char)AFE_ADDRESS,(unsigned char)regAddr, 2,(unsigned char*)pWord);
#endif 
	if(rtv != I2C_OK)
	{
	   return AFE_FAILED;
	}
    return AFE_SUCCESS;
}

static int AFE_GetMulReg(unsigned char regAddr,unsigned char lenth,unsigned char *pByte)
{
	int rtv;
#ifdef I2C_WITH_CRC
    rtv = I2C_ReadMulRegContinusWithCrc((unsigned char)AFE_ADDRESS,(char)regAddr,lenth,pByte);
#else
    rtv = I2C_ReadMulRegContinusWith((unsigned char)AFE_ADDRESS,(char)regAddr,lenth,pByte);
#endif
	if(rtv != I2C_OK)
	{
	    return AFE_FAILED;
	}	
    return AFE_SUCCESS;
}

int AFE_GetWholeRegVal(void  *this)
{
	unsigned char *p = (unsigned char*)(&(((PSRT_AFE_RAW)this)->afeReg));
	int rtv;
#ifdef I2C_WITH_CRC
    rtv = I2C_ReadMulRegContinusWithCrc((unsigned char)AFE_ADDRESS,(char)SYSTEM_STATUS_ADDR, CONTINUS_REGISTER_NUM,p);
	I2C_ReadMulRegContinusWithCrc((unsigned char)AFE_ADDRESS,(char)ADC_GAIN1, 2,p);//3 =  ADC_GIAN1+ADC_OFFSET + ADC_GIAN2
	p += 2;
	I2C_ReadSingleRegWithCrc((unsigned char)AFE_ADDRESS,(char)ADC_GAIN2,p);
	p += CONTINUS_REGISTER_NUM;
	if(rtv == I2C_OK)
	{
	   rtv = I2C_ReadMulRegContinusWithCrc((unsigned char)AFE_ADDRESS,(char)ADC_GAIN1, 2,p);//3 =  ADC_GIAN1+ADC_OFFSET + ADC_GIAN2
	   if(rtv == I2C_OK) 
	   {
		   p += 2;
		   rtv = I2C_ReadSingleRegWithCrc((unsigned char)AFE_ADDRESS,(char)ADC_GAIN2,p);
		   if(rtv != I2C_OK) 
	       {
	           return AFE_FAILED;
	       }
	   }
	}
#else
    rtv = I2C_ReadMulRegContinus((unsigned char)AFE_ADDRESS,(char)SYSTEM_STATUS_ADDR, CONTINUS_REGISTER_NUM,p);
	p +=CONTINUS_REGISTER_NUM;
	if(rtv == I2C_OK)
	{
	   rtv = I2C_ReadMulRegContinus((unsigned char)AFE_ADDRESS,(char)ADC_GAIN1, 2,p);//3 =  ADC_GIAN1+ADC_OFFSET + ADC_GIAN2
	   if(rtv == I2C_OK) 
	   {
		   p += 2;
		   rtv = I2C_ReadSingleReg((unsigned char)AFE_ADDRESS,(char)ADC_GAIN2,p);
		   if(rtv != I2C_OK) 
	       {
	           return AFE_FAILED;
	       }
	   }
	}	
#endif
    //return AFE_SUCCESS;
}
static int AFE_GetSystemStatus(void* this)
{
    return AFE_GetRegByte(SYS_STAT,&(((PSRT_AFE_RAW)this)->afeReg.SysStatus.StatusByte));
}
static int AFE_GetOcdStatus(void* this)
{
    unsigned char byte;
	if(AFE_GetRegBit(SYS_STAT,OCD,&byte) == I2C_OK)
	{
	    ((PSRT_AFE_RAW)this)->afeReg.SysStatus.StatusBit.ocd = byte;
	    return AFE_SUCCESS;
	}
    return AFE_FAILED;  
}
static int AFE_GetScdStatus(void* this)
{
    unsigned char byte;
	if(AFE_GetRegBit(SYS_STAT,SCD,&byte) == I2C_OK)
	{
	    ((PSRT_AFE_RAW)this)->afeReg.SysStatus.StatusBit.scd = byte;
	    return AFE_SUCCESS;
	}
    return AFE_FAILED;  
}
static int AFE_GetOvStatus(void* this)
{
    unsigned char byte;
	if(AFE_GetRegBit(SYS_STAT,OV,&byte) == I2C_OK)
	{
	    ((PSRT_AFE_RAW)this)->afeReg.SysStatus.StatusBit.ov = byte;
	    return AFE_SUCCESS;
	}
    return AFE_FAILED;
}
static int AFE_GetUvStatus(void* this)
{
    unsigned char byte;
	if(AFE_GetRegBit(SYS_STAT,UV,&byte) == I2C_OK)
	{
	    ((PSRT_AFE_RAW)this)->afeReg.SysStatus.StatusBit.uv = byte;
	    return AFE_SUCCESS;
	}
    return AFE_FAILED;
}
static int AFE_GetOvrdalStatus(void* this)
{
    unsigned char byte;
	if(AFE_GetRegBit(SYS_STAT,OVRD_ALERT,&byte) == I2C_OK)
	{
	    ((PSRT_AFE_RAW)this)->afeReg.SysStatus.StatusBit.ovrdAlert = byte;
	    return AFE_SUCCESS;
	}
    return AFE_FAILED;
}
static int AFE_GetDevxdStatus(void* this)
{
    unsigned char byte;
	if(AFE_GetRegBit(SYS_STAT,DEVICE_XREADY,&byte) == AFE_SUCCESS)
	{
	    ((PSRT_AFE_RAW)this)->afeReg.SysStatus.StatusBit.deviceXredy = byte;
	    return AFE_SUCCESS;
	}
    return AFE_FAILED;
}
static int AFE_GetCcrdyStatus(void* this)
{
    unsigned char byte;
	if(AFE_GetRegBit(SYS_STAT,CC_READY,&byte) == AFE_SUCCESS)
	{
	    ((PSRT_AFE_RAW)this)->afeReg.SysStatus.StatusBit.ccReady = byte;
	    return AFE_SUCCESS;
	}
    return AFE_FAILED;
}

static int AFE_EnterShipMode(void *this)
{
    
}
static int AFE_GetTempsStaus(void *this)
{
    unsigned char byte;
	if(AFE_GetRegBit(SYS_CTRL1,TEMP_SEL,&byte) == AFE_SUCCESS)
	{
	    ((PSRT_AFE_RAW)this)->afeReg.SysCtrl1.SysCtrl1Bit.tempSel = byte;
	    return AFE_SUCCESS;
	}
    return AFE_FAILED;
}

static int AFE_GetAdcStaus(void *this)
{
    unsigned char byte;
	if(AFE_GetRegBit(SYS_CTRL1,ADC_EN,&byte) == AFE_SUCCESS)
	{
	    ((PSRT_AFE_RAW)this)->afeReg.SysCtrl1.SysCtrl1Bit.adcEn = byte;
	    return AFE_SUCCESS;
	}
    return AFE_FAILED;
}
static int AFE_SetAdcStatus(void *this,unsigned char status)
{
    
}
static int AFE_GetChgOnStatus(void *this)
{
    unsigned char byte;
	if(AFE_GetRegBit(SYS_CTRL2,CHG_ON,&byte) == AFE_SUCCESS)
	{
	    ((PSRT_AFE_RAW)this)->afeReg.SysCtrl2.SysCtrl2Bit.chgOn = byte;
	    return AFE_SUCCESS;
	}
    return AFE_FAILED;
}
static int AFE_GetDsgOnStatus(void *this)
{
    unsigned char byte;
	if(AFE_GetRegBit(SYS_CTRL2,DSG_ON,&byte) == AFE_SUCCESS)
	{
	    ((PSRT_AFE_RAW)this)->afeReg.SysCtrl2.SysCtrl2Bit.dsgOn = byte;
	    return AFE_SUCCESS;
	}
    return AFE_FAILED;

}
static int AFE_SetChgStatus(void *this,unsigned status)
{
    unsigned char byte;
	if(AFE_GetRegBit(SYS_CTRL2,DSG_ON,&byte) == AFE_SUCCESS)
	{
	    ((PSRT_AFE_RAW)this)->afeReg.SysCtrl2.SysCtrl2Bit.dsgOn = byte;
	    return AFE_SUCCESS;
	}
    return AFE_FAILED;
}
static int AFE_SetDsgStatus(void *this,unsigned status)
{
    
}
static int AFE_GetCcEnStatus(void *this)
{
    unsigned char byte;
	if(AFE_GetRegBit(SYS_CTRL2,CC_EN,&byte) == AFE_SUCCESS)
	{
	    ((PSRT_AFE_RAW)this)->afeReg.SysCtrl2.SysCtrl2Bit.ccEn = byte;
	    return AFE_SUCCESS;
	}
    return AFE_FAILED;
}
static int AFE_SetCcEnStatus(void *this,unsigned char status)
{
    
}
static int AFE_GetDlyDisStatus(void *this)
{
    unsigned char byte;
	if(AFE_GetRegBit(SYS_CTRL2,DELAY_DIS,&byte) == AFE_SUCCESS)
	{
	    ((PSRT_AFE_RAW)this)->afeReg.SysCtrl2.SysCtrl2Bit.delayDis = byte;
	    return AFE_SUCCESS;
	}
    return AFE_FAILED;
}
static int AFE_SetDlyDisStatus(void *this,unsigned status)
{
    
}
static int AFE_GetCellBalanceStatus(void *this,unsigned char cellNum)
{
    //return AFE_GetRegByte(SYS_CTRL1,DELAY_DIS,((PAFE_REG)&afeReg)->SysStatus.StatusBit.delayDis);
}
static int AFE_SetCellBalanceStatus(void *this,unsigned char cellNum)
{
    //return AFE_GetRegByte(SYS_CTRL1,DELAY_DIS,((PAFE_REG)&afeReg)->SysStatus.StatusBit.cb1);
}

static int AFE_GetProtection1(void *this)
{
    return AFE_GetRegByte(PROTECT1,&(((PSRT_AFE_RAW)this)->afeReg.Protect1.Protect1Byte));
}
static int AFE_SetProtection1(void* this,unsigned char value)
{
    
}
static int AFE_GetProtection2(void *this)
{
    return AFE_GetRegByte(PROTECT2,&(((PSRT_AFE_RAW)this)->afeReg.Protect2.Protect2Byte));
}
static int AFE_SetProtection2(void* this,unsigned char value)
{
    
}
static int AFE_GetProtection3(void *this)
{
    return AFE_GetRegByte(PROTECT3,&(((PSRT_AFE_RAW)this)->afeReg.Protect3.Protect3Byte));
}
static int AFE_SetProtection3(void* this,unsigned char value)
{
    //return AFE_GetRegByte(PROTECT2,((PAFE_REG)&afeReg).Protect2.Protect2Byte);
}
static int AFE_GetUv(void *this)
{
    return AFE_GetRegByte(OV_TRIP,&(((PSRT_AFE_RAW)this)->afeReg.OVTrip));
}
static int AFE_SetUv(void *this,unsigned char value)
{
    
}
static int AFE_GetOv(void *this)
{
    return AFE_GetRegByte(UV_TRIP,&(((PSRT_AFE_RAW)this)->afeReg.UVTrip));
}
static int AFE_SetOv(void *this,unsigned char value)
{
    
}
static int AFE_GetCellVoltage(void *this,unsigned char num)
{
    unsigned short *pCellVol;
	pCellVol = (unsigned short*)(&(((PSRT_AFE_RAW)this)->afeReg.VCell1));
	pCellVol += num;
    return AFE_GetRegWord(VC1_HI_BYTE + sizeof(short)*num,pCellVol);
}
static int AFE_GetCellsVoltage(void *this,unsigned char startNum,unsigned char num)
{
    unsigned short *pCellVol;
	pCellVol = (unsigned short*)(&(((PSRT_AFE_RAW)this)->afeReg.VCell1));
	pCellVol += startNum;
    return AFE_GetRegWord(VC1_HI_BYTE + sizeof(short)*num,pCellVol);
}
static int AFE_GetBatVoltage(void *this)
{
    return  AFE_GetRegWord(BAT_HI,&(((PSRT_AFE_RAW)this)->afeReg.VBat.VBatWord));
}
static int AFE_GetTs1(void *this)
{
    return  AFE_GetRegWord(TS1_HI,&(((PSRT_AFE_RAW)this)->afeReg.TS1.TS1Word));
}
static int AFE_GetAdcGain(void *this)
{
    if(AFE_GetRegByte(ADCGAIN1,&(((PSRT_AFE_RAW)this)->afeReg.ADCGain1.ADCGain1Byte)) == AFE_SUCCESS)
	{
	    return  AFE_GetRegByte(ADCGAIN2,&(((PSRT_AFE_RAW)this)->afeReg.ADCGain2.ADCGain2Byte));
	}
	return AFE_FAILED;
}
static int AFE_GetAdcOffset(void *this)
{
    return AFE_GetRegByte(ADCOFFSET,&(((PSRT_AFE_RAW)this)->afeReg.ADCOffset));
}