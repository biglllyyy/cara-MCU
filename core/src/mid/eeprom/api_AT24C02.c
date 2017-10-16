#include <stdlib.h>
#include "hal_gpio.h"
#include "hal_i2c.h"
#include "hal_i2c_iosim.h"
#include "api_AT24C02.h"
#include <string.h>
#include "hal_wdg.h"


#ifdef WIN32

U8   sim_e2p_ads[255] = {0xff};

#endif


static void api_SetAT24C02_Writable(Bool IsWritable)
{
	//dbg_string("PDR07=0x%02x, DDR07=0x%02x, PFR07=0x%02x\n", PDR07, DDR07, PFR07);
	if (IsWritable)
	{
		AT24C02_WP = 0;    /* write able */
	}
	else
	{
		AT24C02_WP = 1;    /* Read only */
	}
}
#if 0
void api_AT24C02_WriteByte(U8 u8EEPROMaddr, U8 u8data)
{
	U8 u8WriteAddr = u8EEPROMaddr, u8WriteData = u8data;

	api_SetAT24C02_Writable(TRUE);       /* Write enable */
	hal_i2c_WriteData(IIC_CHANNEL, AT24C02_ADDRESS_7BITS, 1, (U8*)&u8WriteAddr);
	hal_i2c_WriteData(IIC_CHANNEL, AT24C02_ADDRESS_7BITS, 2,  (U8*)&u8WriteData);
	api_SetAT24C02_Writable(FALSE);       /* Write diable */
}

U8 api_AT24C02_ReadByte(U8 u8EEPROMaddr)
{
	U8 u8WriteAddr = u8EEPROMaddr, u8RetByte = 0;

	hal_i2c_WriteData(IIC_CHANNEL, AT24C02_ADDRESS_7BITS, 1, (U8*)&u8WriteAddr);
	hal_i2c_ReadData(IIC_CHANNEL, AT24C02_ADDRESS_7BITS, 1, (U8*)&u8RetByte);

	return (u8RetByte);
}

static U8 api_AT24C02_WritePage(U8 u8EEPROMaddr,  U8 *pbuffer, U8 u8len)
{
	U8 reval = 0, u8Addr = u8EEPROMaddr;

	if (pbuffer == NULL)
		reval = 0;

	api_SetAT24C02_Writable(TRUE);       /* Write enable */
	if (u8len <= 8 && (0 ==  u8EEPROMaddr & 0x07))
	{
		hal_i2c_WriteData(IIC_CHANNEL, AT24C02_ADDRESS_7BITS, 1, (U8*)&u8Addr);
		hal_i2c_WriteData(IIC_CHANNEL, AT24C02_ADDRESS_7BITS, u8len, pbuffer);

		reval = 1;
	}
	else
	{
		reval = 0;
	}
	api_SetAT24C02_Writable(FALSE);       /* Write diable */

	return reval;
}


void api_AT24C02_WriteBuffer(U8 u8EEPROMStartAddr, U8 *pBuffer, U8 u8len)
{
	U8 i = 0, j = 0;

	U8 byte_write_cnt_st = 0;	/* 开始需要写入的byte数目 */
	U8 page_write_cnt;			/* 需要写入的page数目 */
	U8 last_page_write_len;		/* 最后一个page需要写入的长度 */

	if (0 != (u8EEPROMStartAddr & 0x07))
	{
		byte_write_cnt_st = 8 - (u8EEPROMStartAddr & 0x07);
	}
	else
	{
		byte_write_cnt_st = 0;
	}

	if (u8len <= byte_write_cnt_st)
	{
		byte_write_cnt_st = u8len;
		page_write_cnt = 0;
	}
	else
	{
		page_write_cnt = (u8len - byte_write_cnt_st) >> 3;
		last_page_write_len = u8len - byte_write_cnt_st - (page_write_cnt << 3);
		page_write_cnt ++;
		if (last_page_write_len == 0)
		{
			last_page_write_len = 8;
		}

	}

	for(i = 0; i < byte_write_cnt_st; i++)
	{
		api_AT24C02_WriteByte(u8EEPROMStartAddr + i, pBuffer[i]);
	}

	for (j = 0; j < page_write_cnt - 1; j++)
	{
		api_AT24C02_WritePage(u8EEPROMStartAddr + i + (j << 3), pBuffer + i + (j << 3), 8);
	}
	if (page_write_cnt > 0)
	{
		api_AT24C02_WritePage(u8EEPROMStartAddr + i + (j << 3), pBuffer + i + (j << 3), last_page_write_len);
	}
}

void api_AT24C02_ReadBuffer(U8 u8EEPROMStartAddr, U8 *pBuffer, U8 u8len)
{
	U8 u8Addr = u8EEPROMStartAddr;

	hal_i2c_WriteData(IIC_CHANNEL, AT24C02_ADDRESS_7BITS, 1, (U8*)&u8Addr);
	hal_i2c_ReadData(IIC_CHANNEL, AT24C02_ADDRESS_7BITS, u8len, pBuffer);
}
#endif




//======================下面的代码是采用IO模拟IIC驱动实现的
//初始化IIC接口
void AT24CXX_Init(void)
{
#ifndef WIN32	
	IIC_Init();
#else
	memset(sim_e2p_ads,0xff,sizeof(sim_e2p_ads));
#endif
}

//在AT24CXX指定地址读出一个数据
//ReadAddr:开始读数的地址
//返回值  :读到的数据
U8 AT24CXX_ReadOneByte(U16 ReadAddr)
{
	U8 temp=0;
#ifndef WIN32
    IIC_Start();
	if(EE_TYPE>AT24C16)
	{
		IIC_Send_Byte(0XA0);	   //发送写命令
		IIC_Wait_Ack();
		IIC_Send_Byte(ReadAddr>>8);//发送高地址
	}
	else
		IIC_Send_Byte(AT24C02_ADDRESS_WRITE);   //发送器件地址0XA0,写数据
	IIC_Wait_Ack();
    IIC_Send_Byte(ReadAddr%256);   //发送低地址
	IIC_Wait_Ack();
	IIC_Start();
	IIC_Send_Byte(AT24C02_ADDRESS_READ);           //进入接收模式
	IIC_Wait_Ack();
    temp=IIC_Read_Byte(NO_ASK);
    IIC_Stop();//产生一个停止条件
#else
	if(ReadAddr<255)
	{
		temp = sim_e2p_ads[ReadAddr];
	}
#endif
	return temp;
}
//在AT24CXX指定地址写入一个数据
//WriteAddr  :写入数据的目的地址
//DataToWrite:要写入的数据
void AT24CXX_WriteOneByte(U16 WriteAddr,U8 DataToWrite)
{
#ifndef WIN32	
	api_SetAT24C02_Writable(TRUE);
    IIC_Start();
	if (EE_TYPE > AT24C16)
	{
		IIC_Send_Byte(0XA0);	    //发送写命令
		IIC_Wait_Ack();
		IIC_Send_Byte(WriteAddr>>8);//发送高地址
	}
	else
		IIC_Send_Byte(AT24C02_ADDRESS_WRITE);   //发送器件地址0XA0,写数据
	IIC_Wait_Ack();
    IIC_Send_Byte(WriteAddr%256);   //发送低地址
	IIC_Wait_Ack();
	IIC_Send_Byte(DataToWrite);     //发送字节
	IIC_Wait_Ack();
    IIC_Stop();//产生一个停止条件
	delay_ms(6);
	api_SetAT24C02_Writable(FALSE);
#else
	if(WriteAddr<255)
		sim_e2p_ads[WriteAddr] = DataToWrite;
#endif
}
/*
//在AT24CXX里面的指定地址开始写入长度为Len的数据
//该函数用于写入16bit或者32bit的数据.
//WriteAddr  :开始写入的地址
//DataToWrite:数据数组首地址
//Len        :要写入数据的长度2,4
void AT24CXX_WriteLenByte(U16 WriteAddr,U32 DataToWrite,U8 Len)
{
	U8 t;

	for(t=0;t<Len;t++)
	{
		AT24CXX_WriteOneByte(WriteAddr+t,(DataToWrite>>(8*t))&0xff);
		wdg_feed();
	}
}

//在AT24CXX里面的指定地址开始读出长度为Len的数据
//该函数用于读出16bit或者32bit的数据.
//ReadAddr   :开始读出的地址
//返回值     :数据
//Len        :要读出数据的长度2,4
U32 AT24CXX_ReadLenByte(U16 ReadAddr,U8 Len)
{
	U8 t;
	U32 temp=0;
	for(t=0;t<Len;t++)
	{
		temp<<=8;
		temp+=AT24CXX_ReadOneByte(ReadAddr+Len-t-1);
	}
	return temp;
}*/
//检查AT24CXX是否正常
//这里用了24XX的最后一个地址(255)来存储标志字.
//如果用其他24C系列,这个地址要修改
//返回1:检测失败
//返回0:检测成功
U8 AT24CXX_Check(void)
{
	U8 temp;
	temp=AT24CXX_ReadOneByte(255);//避免每次开机都写AT24CXX
	if(temp==0X55)return 0;
	else//排除第一次初始化的情况
	{
		AT24CXX_WriteOneByte(255,0X55);
	    temp=AT24CXX_ReadOneByte(255);
		if(temp==0X55)return 0;
	}
	return 1;
}

//在AT24CXX里面的指定地址开始读出指定个数的数据
//ReadAddr :开始读出的地址 对24c02为0~255
//pBuffer  :数据数组首地址
//NumToRead:要读出数据的个数
void AT24CXX_Read(U16 ReadAddr,U8 *pBuffer,U16 NumToRead)
{
	while(NumToRead)
	{
		*pBuffer++=AT24CXX_ReadOneByte(ReadAddr++);
		NumToRead--;
	}
}
//在AT24CXX里面的指定地址开始写入指定个数的数据
//WriteAddr :开始写入的地址 对24c02为0~255
//pBuffer   :数据数组首地址
//NumToWrite:要写入数据的个数
void AT24CXX_Write(U16 WriteAddr,U8 *pBuffer,U16 NumToWrite)
{

	while(NumToWrite--)
	{	
		wdg_feed();//for test 2015/7/21 16:24:42
		AT24CXX_WriteOneByte(WriteAddr,*pBuffer);
		WriteAddr++;
		pBuffer++;
	}
}

static void Sequential_Read(U16 addr, U8* const buf, U16 len)
{
	U16 data_len = len;
	U8  index = 0;
#ifndef WIN32
	if(data_len>EE_SIZE)
		data_len=EE_SIZE;
	IIC_Start();
	IIC_Send_Byte((U8)(AT24C02_ADDRESS_WRITE));   //发送器件地址0XA0,写数据
	IIC_Wait_Ack();
	IIC_Send_Byte(addr%256);   //发送低地址
	IIC_Wait_Ack();
	IIC_Start();
	IIC_Send_Byte((U8)(AT24C02_ADDRESS_READ));           //0xa1,进入接收模式
	IIC_Wait_Ack();
	while(data_len)
	{
		if(1 == data_len)
		{
			buf[index] = IIC_Read_Byte(NO_ASK);
		}
		else
		{
			buf[index] = IIC_Read_Byte(HAS_ASK);
		}
		index++;
		data_len--;
	}
	IIC_Stop();//产生一个停止条件}
#else
	while(len--)
	{
		buf[index] = sim_e2p_ads[addr];
		addr++;
		index++;
	}
#endif
}


static void Page_Write(U16 star_addr,U8* const buf,U16 len)
{
	U8 index = 0;
#ifndef WIN32
	IIC_Start();
	IIC_Send_Byte((U8)(AT24C02_ADDRESS_WRITE));   //发送器件地址0XA0,写数据
	IIC_Wait_Ack();
	IIC_Send_Byte(star_addr&0xff);			//发送低地址
	IIC_Wait_Ack();
	if(EE_TYPE==AT24C02)
	{
		if(len>8)
			len=8;//an 8-byte page write
	}
	
	while(len--)
	{
		IIC_Send_Byte(buf[index]);     //发送字节
		IIC_Wait_Ack();
		index++;
	}
	IIC_Stop();//产生一个停止条件
#else
	while(len--)
	{
		sim_e2p_ads[star_addr] = buf[index++];
		star_addr++;
	}

#endif
}

