#include "stm32l0xx_hal.h"
#include "i2c.h"
I2C_HandleTypeDef hi2c1;
void HAL_API_I2c1Init(void);
void HAL_API_I2c1DeInit(void);
void HAL_I2C_MspDeInit(I2C_HandleTypeDef* hi2c);
void HAL_I2C_MspInit(I2C_HandleTypeDef* hi2c);
static void MX_I2C1_Init(void);
extern void Error_Handler(void);
unsigned char CRC8(unsigned char *ptr, unsigned char len,unsigned char key);
/* I2C2 init function */
void HAL_API_I2c1Init(void)
{
    MX_I2C1_Init();
}
void HAL_API_I2c1DeInit(void)
{
    HAL_I2C_MspDeInit(&hi2c1);
}

/* I2C1 init function */
static void MX_I2C1_Init(void)
{

  hi2c1.Instance = I2C1;
  hi2c1.Init.Timing = 0x00A03D53;
  hi2c1.Init.OwnAddress1 = 0;
  hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c1.Init.OwnAddress2 = 0;
  hi2c1.Init.OwnAddress2Masks = I2C_OA2_NOMASK;
  hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c1) != HAL_OK)
  {
    Error_Handler();
  }

    /**Configure Analogue filter 
    */
  if (HAL_I2CEx_ConfigAnalogFilter(&hi2c1, I2C_ANALOGFILTER_ENABLE) != HAL_OK)
  {
    Error_Handler();
  }

}
void HAL_I2C_MspInit(I2C_HandleTypeDef *hi2c)
{
  GPIO_InitTypeDef GPIO_InitStruct;
  if(hi2c->Instance==I2C1)
  {
  /* USER CODE BEGIN I2C1_MspInit 0 */

  /* USER CODE END I2C1_MspInit 0 */
  
    /**I2C1 GPIO Configuration    
    PB6     ------> I2C1_SCL
    PB7     ------> I2C1_SDA 
    */
    GPIO_InitStruct.Pin = GPIO_PIN_6|GPIO_PIN_7;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_OD;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF1_I2C1;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    /* Peripheral clock enable */
    __HAL_RCC_I2C1_CLK_ENABLE();
    /* Peripheral interrupt init */
    HAL_NVIC_SetPriority(I2C1_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(I2C1_IRQn);
  /* USER CODE BEGIN I2C1_MspInit 1 */

  /* USER CODE END I2C1_MspInit 1 */
  }
}
/*void HAL_I2C_MspInit(I2C_HandleTypeDef* hi2c
{

  GPIO_InitTypeDef GPIO_InitStruct;
  if(hi2c->Instance==I2C1)
  {
  /* USER CODE BEGIN I2C1_MspInit 0 */

  /* USER CODE END I2C1_MspInit 0 */
  
    /**I2C1 GPIO Configuration    
    PB6     ------> I2C1_SCL
    PB7     ------> I2C1_SDA 
    */
    /*GPIO_InitStruct.Pin = GPIO_PIN_6|GPIO_PIN_7;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_OD;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF1_I2C1;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    __HAL_RCC_I2C1_CLK_ENABLE();
	__HAL_RCC_I2C1_CLK_ENABLE();
	SET_BIT(RCC->APB1ENR, (RCC_APB1ENR_I2C1EN));

  }*/

//}*/

void HAL_I2C_MspDeInit(I2C_HandleTypeDef* hi2c)
{

  if(hi2c->Instance==I2C1)
  {
  /* USER CODE BEGIN I2C1_MspDeInit 0 */

  /* USER CODE END I2C1_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_I2C1_CLK_DISABLE();
  
    /**I2C1 GPIO Configuration    
    PB6     ------> I2C1_SCL
    PB7     ------> I2C1_SDA 
    */
    HAL_GPIO_DeInit(GPIOB, GPIO_PIN_6|GPIO_PIN_7);

  }
  /* USER CODE BEGIN I2C1_MspDeInit 1 */

  /* USER CODE END I2C1_MspDeInit 1 */

}

int I2C_ReadSingleReg(unsigned char i2cDeviceAddr,unsigned char reg,unsigned char * pData);
int I2C_ReadMulRegContinus(unsigned char i2cDeviceAddr,char reg, unsigned char length,unsigned char* pData);
int I2C_WriteOneByte(unsigned char i2cDeviceAddr, unsigned char Reg, unsigned char Data);
int I2C_WriteBlock(unsigned char i2cDeviceAddr, unsigned char reg, unsigned char *buffer, unsigned char length);
static int I2C_AfeDeviceTest(unsigned char i2cDeviceAddr);


int I2C_ReadSingleReg(unsigned char i2cDeviceAddr,unsigned char reg,unsigned char * pData)
{
    HAL_StatusTypeDef status;
    unsigned char r = reg;
	unsigned char timeout = 100;//timeout 
	i2cDeviceAddr = i2cDeviceAddr << 1;
	status = I2C_AfeDeviceTest(i2cDeviceAddr);
	if(status != HAL_OK)
	{
	    return I2C_ERROR;//error
	}  
	status = HAL_I2C_Master_Transmit(&hi2c1,i2cDeviceAddr,&r,1,timeout);
	if (status != HAL_OK)
	{
	    return I2C_ERROR;//error;
	}
	
	do
	{
	    status = HAL_I2C_Master_Receive(&hi2c1,i2cDeviceAddr,pData,1,timeout);
	}
	while(status != HAL_OK);
	return I2C_OK;
}
int I2C_ReadSingleRegWithCrc(unsigned char i2cDeviceAddr,unsigned char reg,unsigned char * pData)
{
    HAL_StatusTypeDef status;
    unsigned char r = reg;
	unsigned char timeout = 100;//timeout 
	unsigned char crcInput[3];
	unsigned char readData[2];
	unsigned char crc = 0;
	i2cDeviceAddr = i2cDeviceAddr << 1;
	status = I2C_AfeDeviceTest(i2cDeviceAddr);
	if(status != HAL_OK)
	{
	    return I2C_ERROR;//error
	}  
	status = HAL_I2C_Master_Transmit(&hi2c1,i2cDeviceAddr,&r,1,timeout);
	if (status != HAL_OK)
	{
	    return I2C_ERROR;//error;
	}
	
	status = HAL_I2C_Master_Receive(&hi2c1,i2cDeviceAddr,readData,2,timeout);
	if (status != HAL_OK)
	{
	    return I2C_ERROR;//error;
	}
	crcInput[0] = i2cDeviceAddr + 1;
	crcInput[1] = readData[0];
	
	crc = CRC8(crcInput,2,CRC_KEY);
	
	if(crc != readData[1])
	{
	    return I2C_ERROR;
	}
	*pData = readData[0];
	return I2C_OK;
}
int I2C_ReadMulRegContinus(unsigned char i2cDeviceAddr,char reg, unsigned char length,unsigned char* pData)
{
    unsigned char r = reg;
	unsigned char timeout = 1;//timeout 
	int status;
	
	if(!pData)
	{
	    return I2C_PARM_ERROR;//error
	}
	if (length == 0)  //if num = 0,as 1 ;
	{
	    length = 1;
	}
	i2cDeviceAddr = i2cDeviceAddr << 1;
	status = I2C_AfeDeviceTest(i2cDeviceAddr);
	if(status != HAL_OK)
	{
	    return I2C_ERROR;//error
	}  
	
	status = HAL_I2C_Master_Transmit(&hi2c1,i2cDeviceAddr,&r,1,timeout);
	if (status != HAL_OK)
	{
	    return I2C_ERROR;//error;
	}
	
	do
	{
	    status = HAL_I2C_Master_Receive(&hi2c1,i2cDeviceAddr,pData++,length,timeout);
	}
	while(status != HAL_OK);
	if (status != HAL_OK)
	{
	    return I2C_ERROR;//error;
	}
	return I2C_OK;	
}

int I2C_ReadMulRegContinusWithCrc(unsigned char i2cDeviceAddr,char reg, unsigned char length,unsigned char* pData)
{
    unsigned char r = reg;
	unsigned char timeout = 1;//timeout 
	int status;
	unsigned char *readData = NULL;
	unsigned char buffer[128];
	unsigned char *startData = NULL;
	unsigned char crcInput[2];
	unsigned char crc = 0;
	int i;
    
	startData = buffer;
	
	if (NULL == startData)
	return I2C_PARM_ERROR;
	
	readData = startData;
	i2cDeviceAddr = i2cDeviceAddr << 1;
	status = HAL_I2C_Master_Transmit(&hi2c1,i2cDeviceAddr,&r,1,timeout);
	if (status != HAL_OK)
	{
	    return I2C_ERROR;     //error;
	}
	status = HAL_I2C_Master_Receive(&hi2c1,i2cDeviceAddr,readData,2*length,timeout);
    if (status != HAL_OK)
	{
	    return I2C_ERROR;     //error;
	}
	crcInput[0] = i2cDeviceAddr + 1;
	crcInput[1] = *readData;
	crc = CRC8(crcInput,2,CRC_KEY);
	readData++;
	if (crc != *readData)
	{
		return I2C_ERROR;
	}
	else
	{
	    *pData = *(readData - 1);
	}
	
	for(i = 1; i < length; i++)
	{
		readData++;
		crc = CRC8(readData, 1, CRC_KEY);
		readData++;
		pData++;

		if (crc != *readData)
		{
			return I2C_ERROR;
		}
		else
		{
		    *pData = *(readData - 1);
		}	
	}
	startData = NULL;
	return I2C_OK;
}
int I2C_ReadRegisterWordWithCrc(unsigned char i2cDeviceAddr, unsigned char reg, unsigned short *pData)
{
	unsigned char readData[4];
	unsigned int SentByte = 0; 
	unsigned char crcInput[4];
	unsigned char r = reg;
	unsigned char timeout = 1;//timeout 
	unsigned char crc;
	int status;
	if (NULL == pData)
	{
	    return I2C_PARM_ERROR;
	}
    i2cDeviceAddr = i2cDeviceAddr << 1;
    status = HAL_I2C_Master_Transmit(&hi2c1,i2cDeviceAddr,&r,1,timeout);
    if (status != HAL_OK)
	{
	    return I2C_ERROR;     //error;
	}
	status = HAL_I2C_Master_Receive(&hi2c1,i2cDeviceAddr,readData,4,timeout);
	if (status != HAL_OK)
	{
	    return I2C_ERROR;     //error;
	}
	
	crcInput[0] = i2cDeviceAddr + 1;
	crcInput[1] = readData[0];
	crc = CRC8(crcInput,2, CRC_KEY);
	
	if (crc != readData[1])
	{
	    return I2C_ERROR;     //error;
	}
	
    crc = CRC8(readData + 2, 1, CRC_KEY);
	
	if (crc != readData[3])
	{
	    return I2C_ERROR;     //error;
	}
	

	*pData = readData[0];

	*pData = (*pData << 8) + readData[2];
	return I2C_OK;
}

static int I2C_AfeDeviceTest(unsigned char i2cDeviceAddr)
{
	HAL_StatusTypeDef status;
	unsigned char tries = 3; //try times
	unsigned char timeout = 1;//timeout 
	status = HAL_I2C_IsDeviceReady(&hi2c1,i2cDeviceAddr,tries,timeout);
	if(status == HAL_OK)
	{
	    return 0; //×´Ì¬Õý³£
	}
	return 1;//×´Ì¬´íÎó
}

int I2C_WriteRegByte(unsigned char i2cDeviceAddr, unsigned char reg, unsigned char data)
{
	HAL_StatusTypeDef status;
	unsigned char da[2];
	
	da[0] = reg;
	da[1] = data;
	unsigned char timeout = 1;//timeout 
	i2cDeviceAddr = i2cDeviceAddr << 1;
	status = I2C_AfeDeviceTest(i2cDeviceAddr);
	if(status != HAL_OK)
	{
	    return I2C_ERROR;//error
	}  
	status = HAL_I2C_Master_Transmit(&hi2c1,i2cDeviceAddr,da,2,timeout);
    if (status != HAL_OK)
	{
	    return I2C_ERROR;//error;
	}
	return I2C_OK;
}
int I2C_WriteRegByteWithCrc(unsigned char i2cDeviceAddr, unsigned char reg, unsigned char data)
{
	HAL_StatusTypeDef status;
	unsigned char da[4];
	unsigned char timeout = 1;//timeout 
	i2cDeviceAddr = i2cDeviceAddr << 1;
	
	da[0] = i2cDeviceAddr;
	da[1] = reg; 
	da[2] = data;
	da[3] = CRC8(da,3,CRC_KEY);
	
	status = HAL_I2C_Master_Transmit(&hi2c1,i2cDeviceAddr,da+1,3,timeout);
    if (status != HAL_OK)
	{
	    return I2C_ERROR;//error;
	}
	return I2C_OK;
}
int I2C_WriteBlock(unsigned char i2cDeviceAddr, unsigned char reg, unsigned char *buffer, unsigned char length)
{
	HAL_StatusTypeDef status;
	unsigned char timeout = 1;//timeout 
	
	if(!buffer)
	{
	    return I2C_PARM_ERROR;
	}
	i2cDeviceAddr = i2cDeviceAddr << 1;
	status = I2C_AfeDeviceTest(i2cDeviceAddr);
	if(status != HAL_OK)
	{
	    return I2C_ERROR;//error
	}  
	status = HAL_I2C_Master_Transmit(&hi2c1,i2cDeviceAddr,buffer,length,timeout);
    if (status != HAL_OK)
	{
	    return I2C_ERROR;//error;
	}
	return I2C_OK;
}

int I2C_WriteBlockWithCrc(unsigned char i2cDeviceAddr, unsigned char reg, unsigned char *buffer, unsigned char length)
{
	HAL_StatusTypeDef status;
	unsigned char timeout = 1;//timeout 
	unsigned char  *bufferCrc,*p;
    unsigned char buf[128];
	unsigned char i;
    if(!buffer)
	{
	    return I2C_PARM_ERROR;
	}
	bufferCrc = buf;
	//memset((unsigned char*)bufferCrc,0,2*length + 2);
	i2cDeviceAddr = i2cDeviceAddr << 1;
	p = bufferCrc;
	*p = i2cDeviceAddr;
	p++;
	*p = reg;
	p++;
	*p = *buffer;
	p++;
	*p = CRC8(bufferCrc,3,CRC_KEY);
	
	for(i = 1; i < length; i++)
	{
        p++;
        buffer++;
        *p = *buffer;
		*(p + 1) = CRC8(p, 1, CRC_KEY);
		p++;
	}
	status = HAL_I2C_Master_Transmit(&hi2c1,i2cDeviceAddr,bufferCrc + 1,2*length + 1,timeout);
    if (status != HAL_OK)
	{
	    return I2C_ERROR;//error;
	}
	return I2C_OK;
}
unsigned char CRC8(unsigned char *ptr, unsigned char len,unsigned char key)
{
	unsigned char i;
	unsigned char crc=0;
	while(len--!=0)
	{
		for(i=0x80; i!=0; i/=2)
		{
			if((crc & 0x80) != 0)
			{
				crc *= 2;
				crc ^= key;
			}
			else
				crc *= 2;

			if((*ptr & i)!=0)
				crc ^= key;
		}
		ptr++;  
	}
	return(crc);
}



