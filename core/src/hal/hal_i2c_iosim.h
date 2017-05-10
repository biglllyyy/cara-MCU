#ifndef HAL_I2C_IOSIM_H_
#define HAL_I2C_IOSIM_H_

#include "_fr.h"
#include "comm_typedef.h"

#define	NO_ASK 0
#define	HAS_ASK 1
#define AT24C02_WP		(PDR07_P4)
#define IIC_SDA		    (PDR07_P6)
#define IIC_SCL		    (PDR07_P7)
#define	IIC_SDA_PORT	7
#define IIC_SDA_BIT		6	

//IIC���в�������
extern void IIC_Init(void);                //��ʼ��IIC��IO��
extern void IIC_Start(void);				//����IIC��ʼ�ź�
extern void IIC_Stop(void);	  			//����IICֹͣ�ź�
extern void IIC_Send_Byte(U8 txd);			//IIC����һ���ֽ�
//��1���ֽڣ�ack=1ʱ������ACK��ack=0������nACK
extern U8 IIC_Read_Byte(U8 ack);//IIC��ȡһ���ֽ�
extern U8 IIC_Wait_Ack(void); 				//IIC�ȴ�ACK�ź�
extern void IIC_Ack(void);					//IIC����ACK�ź�
extern void IIC_NAck(void);				//IIC������ACK�ź�

extern void IIC_Write_One_Byte(U8 daddr,U8 addr,U8 data);
extern U8 IIC_Read_One_Byte(U8 daddr,U8 addr);
extern void delay_ms(U16 a_ms);

#endif /* HAL_I2C_IOSIM_H_ */
