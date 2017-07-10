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

//IIC所有操作函数
extern void IIC_Init(void);                //初始化IIC的IO口
extern void IIC_Start(void);				//发送IIC开始信号
extern void IIC_Stop(void);	  			//发送IIC停止信号
extern void IIC_Send_Byte(U8 txd);			//IIC发送一个字节
//读1个字节，ack=1时，发送ACK，ack=0，发送nACK
extern U8 IIC_Read_Byte(U8 ack);//IIC读取一个字节
extern U8 IIC_Wait_Ack(void); 				//IIC等待ACK信号
extern void IIC_Ack(void);					//IIC发送ACK信号
extern void IIC_NAck(void);				//IIC不发送ACK信号

extern void IIC_Write_One_Byte(U8 daddr,U8 addr,U8 data);
extern U8 IIC_Read_One_Byte(U8 daddr,U8 addr);
extern void delay_ms(U16 a_ms);

#endif /* HAL_I2C_IOSIM_H_ */
