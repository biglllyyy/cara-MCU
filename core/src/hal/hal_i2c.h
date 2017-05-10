#ifndef HAL_I2C_H_
#define HAL_I2C_H_

#include "hal_i2c.h"
#include "comm_typedef.h"
#include "KeyPortReg.h"

#define I2C_SupplyClk (40000000)	//40M
#define I2C_SPEED_100K	(100000)
#define I2C_SPEED_400K	(400000)
#define I2C_NUM_CHANNEL	2

//Baud rate generator
#define I2C_BGR3	MFS3_I2C_BGR
#define I2C_BGR8	MFS8_I2C_BGR
#define I2C_BGR10	MFS10_I2C_BGR

#define I2C_SlaveAddress 0xA0
#define I2C_SlaveAddressMask 0x7F


#define I2C_Error_Prefix 0x8000
enum	//enum for error source
{
	i2c_Error_NoError					=0,
	i2c_Error_ProcessingTX				=I2C_Error_Prefix+(1<<0),
	i2c_Error_ProcessingRx				=I2C_Error_Prefix+(1<<1),
	i2c_Error_InvalidParameter			=I2C_Error_Prefix+(1<<2),
	i2c_Error_BusBusy_ArbitrationLost	=I2C_Error_Prefix+(1<<3),
	i2c_Error_SlaveNoACK				=I2C_Error_Prefix+(1<<4),
	i2c_Error_NoSetting					=I2C_Error_Prefix+(1<<5),
	i2c_Error_DeviceBusy				=I2C_Error_Prefix+(1<<6),
	i2c_Error_RxOverRun				=I2C_Error_Prefix+(1<<7),
	i2c_Error_InvalidSettingStep			=I2C_Error_Prefix+(1<<8),
	i2c_Error_TXSuccess					=I2C_Error_Prefix+(1<<9),
	i2c_Error_OverRetryTXThreshold		=I2C_Error_Prefix+(1<<10),
	i2c_Error_OverRetryRXThreshold		=I2C_Error_Prefix+(1<<11),
	i2c_Error_RXSuccess					=I2C_Error_Prefix+(1<<12),

};

enum	//enum for Read/Write
{
	i2c_Operation_Write=0,
	i2c_Operation_Read,
};

enum	//enum for error source
{
	i2c_State_Ready=0,
	i2c_State_Busy,
};


extern S32 hal_i2c_open(U8 nPortNum, U32 nBaudRate);

extern S32 hal_i2c_WriteData(U8 nPortNum, U8 a_ucSlaveAddress_7Bit, U8 a_ucNumOfData, U8 *a_pDataArray);

extern S32 hal_i2c_ReadData(U8 nPortNum, U8 a_ucSlaveAddress_7Bit, U8 a_ucNumOfData, U8 *a_pDataArray);







#endif /* HAL_I2C_H_ */
