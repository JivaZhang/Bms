/*
 * 	i2c.h
 *
 *	Created on: Jun 25, 2013
 *		Author: Denis aka caat
 */

#ifndef _I2C_H_
#define _I2C_H_
//#include "stm32f1xx_hal_gpio.h"
#include <stdint.h>
#define I2C_WITH_CRC    1
#define I2C_OK          0
#define I2C_ERROR       1
#define I2C_PARM_ERROR  2
#define CRC_KEY 7   
void HAL_API_I2c1Init(void);
void HAL_API_I2c1DeInit(void);
void HAL_I2C_MspDeInit(I2C_HandleTypeDef* hi2c);
void HAL_I2C_MspInit(I2C_HandleTypeDef* hi2c);
int I2C_ReadSingleReg(unsigned char i2cDeviceAddr,unsigned char reg,unsigned char * pData);
int I2C_ReadMulRegContinus(unsigned char i2cDeviceAddr,char startReg, unsigned char length,unsigned char* pData);
int I2C_WriteRegByte(unsigned char i2cDeviceAddr, unsigned char reg, unsigned char data);
int I2C_WriteBlock(unsigned char i2cDeviceAddr, unsigned char startReg, unsigned char *buffer, unsigned char length);
int I2C_WriteBlockWithCrc(unsigned char i2cDeviceAddr, unsigned char reg, unsigned char *buffer, unsigned char length);
int I2C_ReadSingleRegWithCrc(unsigned char i2cDeviceAddr,unsigned char reg,unsigned char * pData);
int I2C_ReadRegisterWordWithCrc(unsigned char i2cDeviceAddr, unsigned char reg, unsigned short *pData);
int I2C_ReadMulRegContinusWithCrc(unsigned char i2cDeviceAddr,char reg, unsigned char length,unsigned char* pData);
unsigned char CRC8(unsigned char *ptr, unsigned char len,unsigned char key);
int I2C_WriteRegByteWithCrc(unsigned char i2cDeviceAddr, unsigned char reg, unsigned char data);
#endif /* _I2C_H_*/
