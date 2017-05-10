#include <stdlib.h>
#include "hal_ds1302.h"
#include "_fr.h"
#include "KeyPortReg.h"
#include "hal_gpio.h"
#include "mid_pin.h"


/* IO Port  Register */
#define  DS1302_SCLK 		PDR11_P6
#define  DS1302_SDA  		PDR11_P5
#define  DS1302_RST  		PDR11_P4
#define	 DS1302_SDA_PORT	(11)
#define  DS1302_SDA_BIT		(5)


static void delay_ns(uint16_t t)
{
    uint16_t i;

    for (i=0; i<t; i++)
    {
        i = i;
    }
}

static void delay_us(uint16_t t)
{
    uint16_t i = 0, j = 0;

    for(i=0; i<t; i++)
    {
        for(j=0; j<8; j++)
        {
            ;
        }
    }

}

/*发送一个数据到总线*/
static void DS1302Writebyte(uint8_t u8WriteData)
{
    uint8_t u8LoopIndex = 0;

   	set_single_io_dir(DS1302_SDA_PORT,DS1302_SDA_BIT,IO_GENERAL_OUTPUT);  //RTC IO
    DS1302_SCLK = 0;

    delay_us(10);

    for (u8LoopIndex=0; u8LoopIndex < 8; u8LoopIndex++)
    {
        //DS1302_SCLK = 0;
        delay_ns(2);
        if(u8WriteData & 0x1)
            DS1302_SDA = 1;
        else
            DS1302_SDA = 0;
        delay_ns(5);
        DS1302_SCLK = 1;
        delay_ns(5);
        DS1302_SCLK = 0;
        u8WriteData >>= 1;
    }

    //DS1302_RST = 0;
}

/*从总线接收一个数据*/
static uint8_t DS1302Readbyte(void)
{
    uint8_t u8Receive_data = 0, u8LoopIndex = 0, u8Mask = 0;

    set_single_io_dir(DS1302_SDA_PORT,DS1302_SDA_BIT,IO_INPUT);        //RTC_IO
    DS1302_SCLK = 0;
    for (u8LoopIndex=0; u8LoopIndex<8; u8LoopIndex++)
    {
        if (DS1302_SDA == 1)
        {
            u8Mask = (0x1<<u8LoopIndex);
            u8Receive_data = (u8Receive_data + u8Mask) ;
        }
        else
        {
            u8Mask = ~(0x1<<u8LoopIndex);
            u8Receive_data = (u8Receive_data&u8Mask);
        }
        delay_ns(2);
        DS1302_SCLK = 1;
        delay_ns(2);
        DS1302_SCLK = 0;
    }
    //DS1302_SCLK = 0;
    //DS1302_RST = 0;
    return(u8Receive_data);
}
/*DS1302 write enable/disable */
void DS1302WriteProtect(Bool WriteEnable)
{
    uint8_t u8Value = 0;

    if(WriteEnable)
    {
        u8Value = 0;
    }
    else
    {
        u8Value = 1;
    }

	DS1302_RST = 0;
	delay_us(50);
	DS1302_RST = 1;
	DS1302Writebyte(DS1302_REGADDR_WP);   /* 写入寄存器地址 */
	DS1302Writebyte(u8Value);          /* 写入寄存器值   */

    DS1302_RST = 0;
}

/*DS1302写操作*/
void writeDS1302(uint8_t u8addr, uint8_t u8data)
{
    DS1302_RST = 0;
    delay_us(50);
    DS1302_RST = 1;
    DS1302Writebyte(u8addr);  
    DS1302Writebyte(u8data);  

    DS1302_RST = 0;
    //DS1302_SCLK = 1;
}

/*DS1302读操作*/
uint8_t readDS1302(uint8_t u8addr)
{
    uint8_t u8RetData = 0;

    DS1302_RST = 0;
    DS1302_SCLK = 0;
    //delay_us(50);
    DS1302_RST = 1;
    DS1302Writebyte(u8addr);
    u8RetData = DS1302Readbyte();
    DS1302_SCLK = 1;
    //delay_us(50);
    DS1302_RST = 0;
    return(u8RetData);

}

void readDS1302_burst(uint8_t*buf)
{
    uint8_t u8ByteIndex = 0, u8BitIndex = 0, u8Mask = 0, u8Receive_data = 0;

    if(buf == NULL)
    {
        return;
    }

    DS1302_RST = 0;
    DS1302_SCLK = 0;
    DS1302_RST = 1;
    DS1302Writebyte(DS1302_REGADDR_CLKBURST | DS1302_READ_MASK);

    set_single_io_dir(DS1302_SDA_PORT,DS1302_SDA_BIT,IO_INPUT);
    DS1302_SCLK = 0;

    for(u8ByteIndex=0; u8ByteIndex<8; u8ByteIndex++)
    {
        u8Receive_data = 0;
        for (u8BitIndex=0; u8BitIndex<8; u8BitIndex++)
        {
            if (DS1302_SDA == 1)
            {
                u8Mask = (0x1<<u8BitIndex);
                u8Receive_data = (u8Receive_data + u8Mask) ;
            }
            else
            {
                u8Mask = ~(0x1<<u8BitIndex);
                u8Receive_data = (u8Receive_data&u8Mask);
            }
            delay_ns(2);
            DS1302_SCLK = 1;
            delay_ns(2);
            DS1302_SCLK = 0;
        }
        *buf++ = u8Receive_data;
    }
    DS1302_SCLK = 1;
    DS1302_RST = 0;
}

void writeDS1302_burst(const uint8_t *buf)
{
    uint8_t u8ByteLoopIndex = 0, a_u8WriteData = 0;

    if(buf == NULL)
    {
        return;
    }

    DS1302_RST = 0;
    DS1302_SCLK = 0;
    DS1302_RST = 1;
    DS1302Writebyte(DS1302_REGADDR_CLKBURST);

    for(u8ByteLoopIndex=0; u8ByteLoopIndex<8; u8ByteLoopIndex++)
    {

        a_u8WriteData = *buf++;
        DS1302Writebyte(a_u8WriteData);
    }

    DS1302_RST = 0;
}


void hal_start_DS1302(void)
{
	uint8_t temp=0;
	temp=readDS1302(DS1302_REGADDR_SEC|DS1302_READ_MASK);
	DS1302WriteProtect(FALSE);
	writeDS1302(DS1302_REGADDR_SEC,temp&MASK7);					/* write  bit7 0, clock start */
	DS1302WriteProtect(TRUE);
}

void hal_stop_DS1302(void)
{
	uint8_t temp=0;
	temp=readDS1302(DS1302_REGADDR_SEC|DS1302_READ_MASK);
	DS1302WriteProtect(FALSE);
	writeDS1302(DS1302_REGADDR_SEC,temp|BIT7);					/*write bit7 1,clock stop*/
	DS1302WriteProtect(TRUE);
}
