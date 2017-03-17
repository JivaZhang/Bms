#ifndef __AFE_H__ 
#define __AFE_H__

#define AFE_FAILED           1
#define AFE_SUCCESS          0
#define BIT_SET              1
#define BIT_RESET            0
#define AFE_ADDRESS          0x18//0x30 // << 1 
	

#define LOWBYTE(Data) (unsigned char)(0x00ff & Data)
#define HIGHBYTE(Data) (unsigned char)((0xff00 & Data)>>8)
//#define DISABLE_INT asm(" BIC #8,SR")

#define DELAY_LIMIT 0xffff

#define SYS_STAT 0
#define CELLBAL1 1
#define CELLBAL2 2
#define CELLBAL3 3
#define SYS_CTRL1 4
#define SYS_CTRL2 5
#define PROTECT1 0x06
#define PROTECT2 0x07
#define PROTECT3 0x08
#define OV_TRIP 0x09
#define UV_TRIP 0x0A
#define VC1_HI_BYTE 0x0C

#define ADCGAIN1 0x50
#define ADCOFFSET 0x51
#define ADCGAIN2 0x59

#define SCD_DELAY_50us		0x0
#define SCD_DELAY_100us		0x1
#define SCD_DEALY_200us		0x2
#define SCD_DELAY_400us		0x3

#define SCD_THRESH_44mV_22mV	0
#define SCD_THRESH_67mV_33mV	1
#define SCD_THRESH_89mV_44mV	2
#define SCD_THRESH_111mV_56mV	3
#define SCD_THRESH_133mV_67mV	4
#define SCD_TRHESH_155mV_68mV	5
#define SCD_THRESH_178mV_89mV	6
#define SCD_THRESH_200mV_100mV	7

#define OCD_DEALY_10ms		0x0
#define OCD_DELAY_20ms		0x1
#define OCD_DELAY_40ms		0x2
#define OCD_DELAY_80ms		0x3
#define OCD_DELAY_160ms		0x4
#define OCD_DELAY_320ms		0x5
#define OCD_DELAY_640ms		0x6
#define OCD_DELAY_1280ms	0x7

#define OCD_THRESH_17mV_8mV		0
#define OCD_THRESH_22mV_11mV	1
#define OCD_THRESH_28mV_14mV	2
#define OCD_THRESH_33mV_17mV	3
#define OCD_THRESH_39mV_19mV	4
#define OCD_THRESH_44mV_22mV	5
#define OCD_THRESH_50mV_25mV	6
#define OCD_THRESH_56mV_28MV	7
#define OCD_THRESH_61mV_31mV	8
#define OCD_THRESH_67mV_33mV	9
#define OCD_THRESH_72mV_36mV	0xA
#define OCD_THRESH_78mV_39mV	0xB
#define OCD_THRESH_83mV_42mV	0xC
#define OCD_THRESH_89mV_44mV	0xD
#define OCD_THRESH_94mV_47mV	0xE
#define OCD_THRESH_100mV_50mV	0xF

#define UV_DELAY_1s				0
#define UV_DELAY_4s				1
#define UV_DELAY_8s				2
#define UV_DELAY_16s			3

#define OV_DELAY_1s				0
#define OV_DELAY_2s				1
#define OV_DELAY_4s				2
#define OV_DELAY_8s				3

#define OV_THRESH_BASE			0x2008
#define UV_THRESH_BASE			0x1000
#define OV_STEP					0x10
#define UV_STEP					0x10

#define ADCGAIN_BASE			365

#define LOW_BYTE(Data)			(unsigned char)(0xff & Data)
#define HIGH_BYTE(Data)			(unsigned char)(0xff & (Data >> 8))
#define SYSTEM_STATUS_ADDR   0x00
#define CELL_BALANCE1_ADDR   0x01
#define CELL_BALANCE2_ADDR   0x02
#define CELL_BALANCE3_ADDR   0x03
#define SYSTEM_CONTROL1_ADDR 0x04
#define SYSTEM_CONTROL2_ADDR 0x05
#define PROTECTION1_ADDR     0x06
#define PROTECTION2_ADDR     0x07
#define PROTECTION3_ADDR     0x08
#define OV_TRIP_ADDR         0x09
#define UV_TRIP_ADDR         0x0A
#define CC_CFG_ADDR          0x0B
#define VC1_HI               0x0C
#define VC1_LOW              0x0D
#define CELL_VOL_BASE        VC1_HI
/*volitage from 0x0c ~~~0x29*/
#define BAT_HI               0x2A
#define BAT_LO               0x2B
#define TS1_HI               0x2C
#define TS1_LO               0x2D
#define TS2_HI               0x2E
#define TS2_LO               0x2F
#define TS3_HI               0x30
#define TS3_LO               0x31
#define CC_HI                0x32
#define CC_LO                0x33
#define CONTINUS_REGISTER_NUM         (CC_LO + 1)
#define ADC_GAIN1            0x50
#define ADC_OFFSET           0x51
#define ADC_GAIN2            0x59
#define REGISTER_NUM         (ADC_GAIN2 + 1)

#define GET_BIT(byte,BIT)    (byte >> BIT)
#define BIT_7                7
#define BIT_6                6
#define BIT_5                5
#define BIT_4                4
#define BIT_3                3
#define BIT_2                2
#define BIT_1                1
#define BIT_0                0
#define BIT_7_MASK           (0x80)
#define BIT_6_MASK           (0x40)
#define BIT_5_MASK           (0x20)
#define BIT_4_MASK           (0x10)
#define BIT_3_MASK           (0x08)
#define BIT_2_MASK           (0x04)
#define BIT_1_MASK           (0x02)
#define BIT_0_MASK           (0x01)

/*#define OCD                  BIT_0
#define SCD                  BIT_1
#define OV                   BIT_2
#define UV                   BIT_3
#define OVRDAL               BIT_4
#define DEV_XD               BIT_5
#define CCRDY                BIT_7
#define CHG_ON               BIT_0
#define DSG_ON               BIT_1
#define CC_ONE               BIT_5
#define CC_EN                BIT_6
#define DELAY_DIS            BIT_7
#define CB15			BIT_7
#define CB14			BIT_6
#define CB13			BIT_5
#define CB12			BIT_4
#define CB11			BIT_3*/
#define  OCD			BIT_0
#define  SCD			BIT_1
#define  OV			    BIT_2
#define  UV			    BIT_3
#define  OVRD_ALERT	    BIT_4
#define  DEVICE_XREADY	BIT_5
#define  WAKE			BIT_6
#define  CC_READY		BIT_7

#define  CB5			BIT_4
#define  CB4			BIT_3
#define  CB3			BIT_2
#define  CB2			BIT_1
#define  CB1			BIT_0
				
#define  CB10			BIT_4
#define  CB9			BIT_3
#define  CB8			BIT_2
#define  CB7			BIT_1
#define  CB6			BIT_0
						
#define  CB15			BIT_4
#define  CB14			BIT_3
#define  CB13			BIT_2
#define  CB12			BIT_1
#define  CB11			BIT_0
			
#define  SHUT_B		    BIT_0
#define  SHUT_A		    BIT_1
#define  TEMP_SEL		BIT_3
#define  ADC_EN		    BIT_4
#define  LOAD_PRESENT	BIT_7
			
			
#define  CHG_ON		    BIT_0
#define  DSG_ON		    BIT_1
#define  WAKE_T		    BIT_2
#define  WAKE_EN		BIT_3
#define  CC_ONESHOT	    BIT_5
#define  CC_EN			BIT_6
#define  DELAY_DIS		BIT_7
			
#define  SCD_THRESH	
#define  SCD_DELAY		
#define  RSVD			
			
			
#define  RSNS			
#define  OCD_THRESH	
#define  OCD_DELAY		
#define  RSVD			
			
#define  RSVD			
#define  OV_DELAY		
#define  UV_DELAY		
			
#define  RSVD1			
#define  ADCGAIN_4_3	
#define  RSVD2			
			
#define  RSVD			
#define  ADCGAIN_2_0	
typedef struct _Register_Group
{
	union
	{
		struct
		{
			unsigned char ocd			:1;
			unsigned char scd			:1;
			unsigned char ov			:1;
			unsigned char uv			:1;
			unsigned char ovrdAlert	    :1;
			unsigned char deviceXredy   :1;
			unsigned char wake			:1;
			unsigned char ccReady		:1;
		}StatusBit;
		unsigned char StatusByte;
	}SysStatus;

	union
	{
		struct
		{
			unsigned char cb1			:1;
			unsigned char cb2			:1;
			unsigned char cb3			:1;
			unsigned char cb4			:1;
			unsigned char cb5			:1;
			unsigned char rsvd			:3;
		}CellBal1Bit;
		unsigned char CellBal1Byte;
	}CellBal1;

	union
	{
		struct
		{
			unsigned char rsvd			:3;
			unsigned char cb6		    :1;
			unsigned char cb7			:1;
			unsigned char cb8			:1;
			unsigned char cb9			:1;
			unsigned char cb10			:1;	
		}CellBal2Bit;
		unsigned char CellBal2Byte;
	}CellBal2;

	union
	{
		struct
		{
			unsigned char rsvd			:3;
			unsigned char cb11			:1;
			unsigned char cb12			:1;
			unsigned char cb13  		:1;
			unsigned char cb14	    	:1;
			unsigned char cb15			:1;	
		}CellBal3Bit;
		unsigned char CellBal3Byte;
	}CellBal3;

	union
	{
		struct
		{
			unsigned char shutB		:1;
			unsigned char shutA		:1;
			unsigned char rsvd1		:1;
			unsigned char tempSel		:1;
			unsigned char adcEn		:1;
			unsigned char rsvd2		:2;
			unsigned char loadPresent	:1;
		}SysCtrl1Bit;
		unsigned char SysCtrl1Byte;
	}SysCtrl1;

	union
	{
		struct
		{
			unsigned char chgOn		:1;
			unsigned char dsgOn		:1;
			unsigned char wakeT		:2;
			unsigned char wakeEn		:1;
			unsigned char ccOneShort	:1;
			unsigned char ccEn			:1;
			unsigned char delayDis 	:1;
		}SysCtrl2Bit;
		unsigned char SysCtrl2Byte;
	}SysCtrl2;

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
	unsigned char CCCfg;			//must be 0x19

	union
	{
		struct
		{
			unsigned char vc1Hi;
			unsigned char vc1Lo;
		}VCell1Byte;
		unsigned short VCell1Word;
	}VCell1;

	union
	{
		struct
		{
			unsigned char vc2Hi;
			unsigned char vc2Lo;
		}VCell2Byte;
		unsigned short VCell2Word;
	}VCell2;

	union
	{
		struct
		{
			unsigned char vc3Hi;
			unsigned char vc3Lo;
		}VCell3Byte;
		unsigned short VCell3Word;
	}VCell3;

	union
	{
		struct
		{
			unsigned char vc4Hi;
			unsigned char vc4Lo;
		}VCell4Byte;
		unsigned short VCell4Word;
	}VCell4;

	union
	{
		struct
		{
			unsigned char vc5Hi;
			unsigned char vc5Lo;
		}VCell5Byte;
		unsigned short VCell5Word;
	}VCell5;

	union
	{
		struct
		{
			unsigned char vc6Hi;
			unsigned char vc6Lo;
		}VCell6Byte;
		unsigned short VCell6Word;
	}VCell6;

	union
	{
		struct
		{
			unsigned char vc6Hi;
			unsigned char vc6Lo;
		}VCell7Byte;
		unsigned short VCell7Word;
	}VCell7;

	union
	{
		struct
		{
			unsigned char vc8Hi;
			unsigned char vc8Lo;
		}VCell8Byte;
		unsigned short VCell8Word;
	}VCell8;

	union
	{
		struct
		{
			unsigned char vc9Hi;
			unsigned char vc9Lo;
		}VCell9Byte;
		unsigned short VCell9Word;
	}VCell9;

	union
	{
		struct
		{
			unsigned char vc10Hi;
			unsigned char vc10Lo;
		}VCell10Byte;
		unsigned short VCell10Word;
	}VCell10;

	union
	{
		struct
		{
			unsigned char vc11Hi;
			unsigned char vc11Lo;
		}VCell11Byte;
		unsigned short VCell11Word;
	}VCell11;

	union
	{
		struct
		{
			unsigned char vc12Hi;
			unsigned char vc12Lo;
		}VCell12Byte;
		unsigned short VCell12Word;
	}VCell12;

	union
	{
		struct
		{
			unsigned char vc13Hi;
			unsigned char vc13Lo;
		}VCell13Byte;
		unsigned short VCell13Word;
	}VCell13;

	union
	{
		struct
		{
			unsigned char vc14Hi;
			unsigned char vc14Lo;
		}VCell14Byte;
		unsigned short VCell14Word;
	}VCell14;

	union
	{
		struct
		{
			unsigned char vc15Hi;
			unsigned char vc15Lo;
		}VCell15Byte;
		unsigned short VCell15Word;
	}VCell15;

	union
	{
		struct
		{
			unsigned char batHi;
			unsigned char batLo;
		}VBatByte;
		unsigned short VBatWord;
	}VBat;

	union
	{
		struct
		{
			unsigned char ts1Hi;
			unsigned char ts1Lo;
		}TS1Byte;
		unsigned short TS1Word;
	}TS1;

	union
	{
		struct
		{
			unsigned char ts2Hi;
			unsigned char ts2Lo;
		}TS2Byte;
		unsigned short TS2Word;
	}TS2;

	union
	{
		struct
		{
			unsigned char ts3Hi;
			unsigned char ts3Lo;
		}TS3Byte;
		unsigned short TS3Word;
	}TS3;

	union
	{
		struct
		{
			unsigned char ccHi;
			unsigned char ccLo;
		}CCByte;
		unsigned short CCWord;
	}CC;

	union
	{
		struct
		{
			unsigned char rsvd1			:2;
			unsigned char adcGain_4_3   	:2;
			unsigned char rsvd2			:4;
		}ADCGain1Bit;
		unsigned char ADCGain1Byte;
	}ADCGain1;

	unsigned char ADCOffset;

	union
	{
		struct
		{
			unsigned char rsvd			:5;
			unsigned char adcGain_2_0	:3;
		}ADCGain2Bit;
		unsigned char ADCGain2Byte;
	}ADCGain2;

}AFE_REG,*PAFE_REG;
typedef struct srt_afe_raw 
{
    AFE_REG afeReg;
	int (*GetWholeRegVal)(void * this);
	int (*GetSystemStatus)(void* this);
	int (*GetOcdStatus)(void* this);
	int (*GetScdStatus)(void* this);
	int (*GetOvStatus)(void* this);
	int (*GetUvStatus)(void* this);
	int (*GetDevxdStatus)(void * this);
	int (*GetCcrdyStatus)(void * this);
	int (*EnterShipMode)(void *this);
	int (*GetTempsStaus)(void *this);
	int (*GetAdcStaus)(void *this);
	int (*SetAdcStatus)(void *this,unsigned char status);
	int (*GetChgOnStatus)(void *this);
	int (*GetDsgOnStatus)(void *this);
	int (*SetChgStatus)(void *this,unsigned status);
	int (*SetDsgStatus)(void *this,unsigned status);
	int (*GetCcEnStatus)(void *this);
	int (*SetCcEnStatus)(void *this,unsigned char status);
	int (*GetDlyDisStatus)(void *this);
	int (*SetDlyDisStatus)(void *this,unsigned status);
	int (*GetCellBalanceStatus)(void *this,unsigned char cellNum);
	int (*SetCellBalanceStatus)(void *this,unsigned char cellNum);
	int (*GetProtection1)(void *this);
	int (*SetProtection1)(void * this,unsigned char value);
	int (*GetProtection2)(void *this);
	int (*SetProtection2)(void * this,unsigned char value);
	int (*GetProtection3)(void *this);
	int (*SetProtection3)(void * this,unsigned char value);
	int (*GetUv)(void *this);
	int (*SetUv)(void *this,unsigned char value);
	int (*GetOv)(void *this);
	int (*SetOv)(void *this,unsigned char value);
	int (*GetCellVoltage)(void *this,unsigned char num);
    int (*GetCellsVoltage)(void *this,unsigned char startNum,unsigned char num);
	int (*GetBatVoltage)(void *this);
	int (*GetAdcGain)(void *this);
	int (*GetAdcOffset)(void *this);
	int (*GetTs1)(void *this);
}SRT_AFE_RAW,*PSRT_AFE_RAW;
extern int AFE_InitEnv(PSRT_AFE_RAW praw);

#endif //__AFE_H__