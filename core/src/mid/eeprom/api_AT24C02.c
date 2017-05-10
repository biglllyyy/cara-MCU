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

	U8 byte_write_cnt_st = 0;	/* ��ʼ��Ҫд���byte��Ŀ */
	U8 page_write_cnt;			/* ��Ҫд���page��Ŀ */
	U8 last_page_write_len;		/* ���һ��page��Ҫд��ĳ��� */

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




//======================����Ĵ����ǲ���IOģ��IIC����ʵ�ֵ�
//��ʼ��IIC�ӿ�
void AT24CXX_Init(void)
{
#ifndef WIN32	
	IIC_Init();
#else
	memset(sim_e2p_ads,0xff,sizeof(sim_e2p_ads));
#endif
}

//��AT24CXXָ����ַ����һ������
//ReadAddr:��ʼ�����ĵ�ַ
//����ֵ  :����������
U8 AT24CXX_ReadOneByte(U16 ReadAddr)
{
	U8 temp=0;
#ifndef WIN32
    IIC_Start();
	if(EE_TYPE>AT24C16)
	{
		IIC_Send_Byte(0XA0);	   //����д����
		IIC_Wait_Ack();
		IIC_Send_Byte(ReadAddr>>8);//���͸ߵ�ַ
	}
	else
		IIC_Send_Byte(AT24C02_ADDRESS_WRITE);   //����������ַ0XA0,д����
	IIC_Wait_Ack();
    IIC_Send_Byte(ReadAddr%256);   //���͵͵�ַ
	IIC_Wait_Ack();
	IIC_Start();
	IIC_Send_Byte(AT24C02_ADDRESS_READ);           //�������ģʽ
	IIC_Wait_Ack();
    temp=IIC_Read_Byte(NO_ASK);
    IIC_Stop();//����һ��ֹͣ����
#else
	if(ReadAddr<255)
	{
		temp = sim_e2p_ads[ReadAddr];
	}
#endif
	return temp;
}
//��AT24CXXָ����ַд��һ������
//WriteAddr  :д�����ݵ�Ŀ�ĵ�ַ
//DataToWrite:Ҫд�������
void AT24CXX_WriteOneByte(U16 WriteAddr,U8 DataToWrite)
{
#ifndef WIN32	
	api_SetAT24C02_Writable(TRUE);
    IIC_Start();
	if (EE_TYPE > AT24C16)
	{
		IIC_Send_Byte(0XA0);	    //����д����
		IIC_Wait_Ack();
		IIC_Send_Byte(WriteAddr>>8);//���͸ߵ�ַ
	}
	else
		IIC_Send_Byte(AT24C02_ADDRESS_WRITE);   //����������ַ0XA0,д����
	IIC_Wait_Ack();
    IIC_Send_Byte(WriteAddr%256);   //���͵͵�ַ
	IIC_Wait_Ack();
	IIC_Send_Byte(DataToWrite);     //�����ֽ�
	IIC_Wait_Ack();
    IIC_Stop();//����һ��ֹͣ����
	delay_ms(6);
	api_SetAT24C02_Writable(FALSE);
#else
	if(WriteAddr<255)
		sim_e2p_ads[WriteAddr] = DataToWrite;
#endif
}
/*
//��AT24CXX�����ָ����ַ��ʼд�볤��ΪLen������
//�ú�������д��16bit����32bit������.
//WriteAddr  :��ʼд��ĵ�ַ
//DataToWrite:���������׵�ַ
//Len        :Ҫд�����ݵĳ���2,4
void AT24CXX_WriteLenByte(U16 WriteAddr,U32 DataToWrite,U8 Len)
{
	U8 t;

	for(t=0;t<Len;t++)
	{
		AT24CXX_WriteOneByte(WriteAddr+t,(DataToWrite>>(8*t))&0xff);
		wdg_feed();
	}
}

//��AT24CXX�����ָ����ַ��ʼ��������ΪLen������
//�ú������ڶ���16bit����32bit������.
//ReadAddr   :��ʼ�����ĵ�ַ
//����ֵ     :����
//Len        :Ҫ�������ݵĳ���2,4
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
//���AT24CXX�Ƿ�����
//��������24XX�����һ����ַ(255)���洢��־��.
//���������24Cϵ��,�����ַҪ�޸�
//����1:���ʧ��
//����0:���ɹ�
U8 AT24CXX_Check(void)
{
	U8 temp;
	temp=AT24CXX_ReadOneByte(255);//����ÿ�ο�����дAT24CXX
	if(temp==0X55)return 0;
	else//�ų���һ�γ�ʼ�������
	{
		AT24CXX_WriteOneByte(255,0X55);
	    temp=AT24CXX_ReadOneByte(255);
		if(temp==0X55)return 0;
	}
	return 1;
}

//��AT24CXX�����ָ����ַ��ʼ����ָ������������
//ReadAddr :��ʼ�����ĵ�ַ ��24c02Ϊ0~255
//pBuffer  :���������׵�ַ
//NumToRead:Ҫ�������ݵĸ���
void AT24CXX_Read(U16 ReadAddr,U8 *pBuffer,U16 NumToRead)
{
	while(NumToRead)
	{
		*pBuffer++=AT24CXX_ReadOneByte(ReadAddr++);
		NumToRead--;
	}
}
//��AT24CXX�����ָ����ַ��ʼд��ָ������������
//WriteAddr :��ʼд��ĵ�ַ ��24c02Ϊ0~255
//pBuffer   :���������׵�ַ
//NumToWrite:Ҫд�����ݵĸ���
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
	IIC_Send_Byte((U8)(AT24C02_ADDRESS_WRITE));   //����������ַ0XA0,д����
	IIC_Wait_Ack();
	IIC_Send_Byte(addr%256);   //���͵͵�ַ
	IIC_Wait_Ack();
	IIC_Start();
	IIC_Send_Byte((U8)(AT24C02_ADDRESS_READ));           //0xa1,�������ģʽ
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
	IIC_Stop();//����һ��ֹͣ����}
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
	IIC_Send_Byte((U8)(AT24C02_ADDRESS_WRITE));   //����������ַ0XA0,д����
	IIC_Wait_Ack();
	IIC_Send_Byte(star_addr&0xff);			//���͵͵�ַ
	IIC_Wait_Ack();
	if(EE_TYPE==AT24C02)
	{
		if(len>8)
			len=8;//an 8-byte page write
	}
	
	while(len--)
	{
		IIC_Send_Byte(buf[index]);     //�����ֽ�
		IIC_Wait_Ack();
		index++;
	}
	IIC_Stop();//����һ��ֹͣ����
#else
	while(len--)
	{
		sim_e2p_ads[star_addr] = buf[index++];
		star_addr++;
	}

#endif
}

