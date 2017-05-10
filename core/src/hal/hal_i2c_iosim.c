#include "hal_i2c_iosim.h"
#include "hal_gpio.h"
#include "comm_typedef.h"
#include "KeyPortReg.h"
#include "mid_pin.h"
#include "mid_dbg.h"


extern  io_type_t IO_IIC_SDA;

static void delay_us(U16 a_us)
{
    U16 a_u16xValue = 0, a_u16iValue = 0;

    for(a_u16xValue=0; a_u16xValue<a_us;a_u16xValue++)
    {
        for(a_u16iValue=0; a_u16iValue<8; a_u16iValue++)
        {
            ;
        }
    }

}

void delay_ms(U16 a_ms)
{
    U16 a_u16xValue = 0, a_u16iValue = 0, a_u16kValue = 0;

    for(a_u16xValue=0; a_u16xValue<a_ms;a_u16xValue++)      /*ms*/
    {
        for(a_u16iValue = 0; a_u16iValue < 1000; a_u16iValue++)    /*us*/
        {
        	for(a_u16kValue=0; a_u16kValue<8; a_u16kValue++)
        	{
        		__wait_nop();   /*32 instructions for one us */
        	}
        }
    }

}



//IO��������
static void hal_i2c_SDAIN(void)
{
	//SDA
	set_single_io_dir(IO_IIC_SDA.port,IO_IIC_SDA.bit,IO_INPUT);
}

static void hal_i2c_SDAOUT(void)
{

	set_single_io_dir(IO_IIC_SDA.port,IO_IIC_SDA.bit,IO_GENERAL_OUTPUT);
}


//��ʼ��IIC
void IIC_Init(void)
{
	
}

//����IIC��ʼ�ź�
void IIC_Start(void)
{
	hal_i2c_SDAOUT();     //sda�����
	IIC_SDA=1;
	IIC_SCL=1;
	delay_us(4);
	IIC_SDA=0;//START:when CLK is high,DATA change form high to low
	delay_us(4);
	IIC_SCL=0;//ǯסI2C���ߣ�׼�����ͻ��������
}

//����IICֹͣ�ź�
void IIC_Stop(void)
{
	hal_i2c_SDAOUT();//sda�����
	IIC_SCL=0;
	IIC_SDA=0;//STOP:when CLK is high DATA change form low to high
	delay_us(4);
	IIC_SCL=1;
	IIC_SDA=1;//����I2C���߽����ź�
	delay_us(4);
}

//�ȴ�Ӧ���źŵ���
//����ֵ��1������Ӧ��ʧ��
//        0������Ӧ��ɹ�
U8 IIC_Wait_Ack(void)
{
	U8 ucErrTime=0;
	hal_i2c_SDAIN();      //SDA����Ϊ����
	IIC_SDA=1;delay_us(1);
	IIC_SCL=1;delay_us(1);
	while(IIC_SDA)
	{
		ucErrTime++;
		delay_us(1);
		if(ucErrTime>250)
		{
			IIC_Stop();
			dbg_string("IIC_Wait_Ack Error\n");
			return 1;
		}
	}
	IIC_SCL=0;//ʱ�����0
	return 0;
}

//����ACKӦ��
void IIC_Ack(void)
{
	IIC_SCL=0;
	hal_i2c_SDAOUT();
	IIC_SDA=0;
	delay_us(2);
	IIC_SCL=1;
	delay_us(2);
	IIC_SCL=0;
}
//������ACKӦ��
void IIC_NAck(void)
{
	IIC_SCL=0;
	hal_i2c_SDAOUT();
	IIC_SDA=1;
	delay_us(2);
	IIC_SCL=1;
	delay_us(2);
	IIC_SCL=0;
}
//IIC����һ���ֽ�
//���شӻ�����Ӧ��
//1����Ӧ��
//0����Ӧ��
void IIC_Send_Byte(U8 txd)
{
    U8 t;
    IIC_SCL=0;//����ʱ�ӿ�ʼ���ݴ���
	hal_i2c_SDAOUT();
    for(t=0;t<8;t++)
    {
        IIC_SDA=(txd&0x80)>>7;
        txd<<=1;
		delay_us(2);   //��TEA5767��������ʱ���Ǳ����
		IIC_SCL=1;
		delay_us(2);
		IIC_SCL=0;
		delay_us(2);
    }
}
//��1���ֽڣ�ack=1ʱ������ACK��ack=0������nACK
U8 IIC_Read_Byte(U8 ack)
{
	U8 i,receive=0;
	hal_i2c_SDAIN();//SDA����Ϊ����
    for(i=0;i<8;i++ )
	{
        IIC_SCL=0;
        delay_us(2);
		IIC_SCL=1;
        receive<<=1;
        if(IIC_SDA)receive++;
		delay_us(1);
    }
    if (!ack)
        IIC_NAck();//����nACK
    else
        IIC_Ack(); //����ACK
    return receive;
}
