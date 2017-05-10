#ifndef API_AT24C02_H_
#define API_AT24C02_H_

#include "comm_typedef.h"

#define AT24C01		(127)
#define AT24C02		(255)
#define AT24C04		(511)
#define AT24C08		(1023)
#define AT24C16		(2047)
#define AT24C32		(4095)
#define AT24C64	    (8191)
#define AT24C128	(16383)
#define AT24C256	(32767)
#define EE_TYPE 	AT24C02
#define	EE_SIZE		AT24C02//unit bytes


#define DEVICE_ADDRESS (0)
#define AT24C02_ADDRESS_READ	(0xA0|(DEVICE_ADDRESS<<1)|1)
#define AT24C02_ADDRESS_WRITE	(0xA0|(DEVICE_ADDRESS<<1)|0)
#define AT24C02_ADDRESS_7BITS	(0x50)

#define IIC_CHANNEL	(3)


extern void api_AT24C02_WriteByte(U8 u8EEPROMaddr, U8 u8data);

extern void api_AT24C02_WriteBuffer(U8 u8EEPROMStartAddr, U8 *pBuffer, U8 u8len);

extern U8 api_AT24C02_ReadByte(U8 u8EEPROMaddr);

extern void api_AT24C02_WriteBuffer(U8 u8EEPROMStartAddr, U8 *pBuffer, U8 u8len);


extern U8	AT24CXX_ReadOneByte(U16 ReadAddr);							/*ָ����ַ��ȡһ���ֽ�*/
extern void AT24CXX_WriteOneByte(U16 WriteAddr,U8 DataToWrite);		/*ָ����ַд��һ���ֽ�*/
extern void AT24CXX_WriteLenByte(U16 WriteAddr,U32 DataToWrite,U8 Len);/*ָ����ַ��ʼд��ָ�����ȵ�����*/
extern U32	AT24CXX_ReadLenByte(U16 ReadAddr,U8 Len);					/*ָ����ַ��ʼ��ȡָ����������*/
extern void AT24CXX_Write(U16 WriteAddr,U8 *pBuffer,U16 NumToWrite);	/*��ָ����ַ��ʼд��ָ�����ȵ�����*/
extern void AT24CXX_Read(U16 ReadAddr,U8 *pBuffer,U16 NumToRead);   	/*��ָ����ַ��ʼ����ָ�����ȵ�����*/

extern U8   AT24CXX_Check(void);  /* ������� */
extern void AT24CXX_Init(void); /* ��ʼ��IIC */

#endif /* API_AT24C02_H_ */
