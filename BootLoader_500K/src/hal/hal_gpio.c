#include <string.h>
#include "hal_gpio.h"
#include "KeyPortReg.h"
#include "_fr.h"



/*
mb91520 io operation
端口寄存器：		PDR00~PDR19 		单个IO操作：PDR00_P0
方向寄存器：		DDR00~DDR19			单个IO操作：DDR00_P0
特殊功能寄存器：	PFR00~PFR19：		0:IO_INPUT, 1:other function		单个IO操作：DDR00_P0:
输入数据直接寄存器：PDDR00~PDDR19：		单个IO操作：PDDR00_P0
上拉使能寄存器：	PPER00~PPER19		单个IO操作：PPER00_P0
外部扩展功能寄存器：EPFR00~EPFR88：配合特殊功能寄存器使用	EPFR00_XXX

DDR, PFR, EPFR, PPER, ADER, and DACR need key code 
*/
void IO_GENERAL_OUTPUT_io_high_set(void);

U8  DDR[20]= {0};	//存储DDR方向寄存器的值。
U8  PFR[20]= {0};	//存储特殊功能寄存器的值
U8  EPFR[88] = {0};	//存储EPFR的值
U8  PDR[20] = {0};  //输出值寄存器


U32 DDR_ADS[20]= {(U32)&DDR00,(U32)&DDR01,(U32)&DDR02,(U32)&DDR03,(U32)&DDR04,(U32)&DDR05,(U32)&DDR06,(U32)&DDR07,(U32)&DDR08,(U32)&DDR09,
                  (U32)&DDR10,(U32)&DDR11,(U32)&DDR12,(U32)&DDR13,(U32)&DDR14,(U32)&DDR15,(U32)&DDR16,(U32)&DDR17,(U32)&DDR18,(U32)&DDR19
                 };

U32 PFR_ADS[20]= {(U32)&PFR00,(U32)&PFR01,(U32)&PFR02,(U32)&PFR03,(U32)&PFR04,(U32)&PFR05,(U32)&PFR06,(U32)&PFR07,(U32)&PFR08,(U32)&PFR09,
                  (U32)&PFR10,(U32)&PFR11,(U32)&PFR12,(U32)&PFR13,(U32)&PFR14,(U32)&PFR15,(U32)&PFR16,(U32)&PFR17,(U32)&PFR18,(U32)&PFR19
                 };
U32 PDR_ADS[20] = {	(U32)&PDR00,(U32)&PDR01,(U32)&PDR02,(U32)&PDR03,(U32)&PDR04,(U32)&PDR05,(U32)&PDR06,(U32)&PDR07,(U32)&PDR08,(U32)&PDR09,
                    (U32)&PDR10,(U32)&PDR11,(U32)&PDR12,(U32)&PDR13,(U32)&PDR14,(U32)&PDR15,(U32)&PDR16,(U32)&PDR17,(U32)&PDR18,(U32)&PDR19
                  };
U32 PDDR_ADS[20] = {(U32)&PDDR00,(U32)&PDDR01,(U32)&PDDR02,(U32)&PDDR03,(U32)&PDDR04,(U32)&PDDR05,(U32)&PDDR06,(U32)&PDDR07,(U32)&PDDR08,(U32)&PDDR09,
                    (U32)&PDDR10,(U32)&PDDR11,(U32)&PDDR12,(U32)&PDDR13,(U32)&PDDR14,(U32)&PDR15,(U32)&PDDR16,(U32)&PDDR17,(U32)&PDDR18,(U32)&PDDR19
                   };

/* set Px,y valule */
void set_single_io_sts(U8 port,U8 bit,Bool value)
{
    volatile U8 *ads = NULL;

    if((port < 20)&&(bit<8))
    {
        if(0 == value)
        {
            PDR[port] &= ~(1<<bit);
        }
        else
        {
            PDR[port] |= 1<<bit;
        }
        ads =  (U8*)PDR_ADS[port];
        *ads = PDR[port];
    }

}

void set_group_io_sts(U8 port,U8 value)
{
    volatile  U8 *ads = NULL;

    if(port < 20)
    {
        ads =  (U8*)PDR_ADS[port];
        *ads = value;
    }
}


Bool get_single_io_sts(U8 port,U8 bit)
{
    Bool ret = 0;
    volatile U8*  ads = NULL;

    if((port < 20)&&(bit <= 8))
    {
        ads = (U8*)PDDR_ADS[port];
        ret =	((*ads) >> bit)&(0x01);
    }

    return (ret);
}

U8 get_group_io_sts(U8 port)
{
    volatile U8   *ads = NULL;
    U8	ret = 0;

    ads = (U8*)PDDR_ADS[port];
    ret = *ads;

    return(ret);
}

/* set Pport.x 为输入或是输出 */
void set_single_io_dir(U8 port,U8 bit,e_io_direction dir)
{
    if((port <=19)&& (bit <=7))
    {
        if(IO_INPUT == dir)
        {
            PFR[port]&=(U8)(~(1<<bit));
            WriteKeyProtectedRegister(PFR_ADS[port],PFR[port] , BYTE_ACCESS);
            DDR[port]&=((U8)(~(1<<bit)));
            WriteKeyProtectedRegister(DDR_ADS[port], DDR[port], BYTE_ACCESS);
        }
        else
        {

            if(IO_GENERAL_OUTPUT == dir)			// PFR = 0
            {
                PFR[port] &= (U8)(~(1<<bit));
                WriteKeyProtectedRegister(PFR_ADS[port], PFR[port], BYTE_ACCESS);
            }
            else if(IO_FUNC_OUTPUT == dir)			//PFR = 1
            {
                PFR[port] |= (U8)(1<<bit);
                WriteKeyProtectedRegister(PFR_ADS[port], PFR[port], BYTE_ACCESS);
            }
            else
            {
                ;
            }
            DDR[port] |= (U8)(1<<bit);				// DDR IO_GENERAL_OUTPUT
            WriteKeyProtectedRegister(DDR_ADS[port], DDR[port], BYTE_ACCESS);
        }
    }
}

void set_group_io_dir(U8 port, e_io_direction dir)
{
    do
    {
        if(port >= 20)
            break;
        if(IO_INPUT == dir)
        {
            PFR[port] = 0;
            WriteKeyProtectedRegister(PFR_ADS[port],PFR[port],BYTE_ACCESS);
            DDR[port] = 0;
            WriteKeyProtectedRegister(DDR_ADS[port],DDR[port], BYTE_ACCESS);
        }
        else
        {
            if(IO_GENERAL_OUTPUT == dir)			// PFR = 0
            {
                PFR[port] = 0;
                WriteKeyProtectedRegister(PFR_ADS[port], PFR[port], BYTE_ACCESS);
            }
            else if(IO_FUNC_OUTPUT == dir)			//PFR = 1
            {
                PFR[port] = 0xff;
                WriteKeyProtectedRegister(PFR_ADS[port], PFR[port], BYTE_ACCESS);
            }
            else
            {
                ;
            }
            DDR[port] = 0xff;
            WriteKeyProtectedRegister(DDR_ADS[port], DDR[port], BYTE_ACCESS);

        }
    }
    while (0);
}


void hal_gpio_Init(void)
{
    memset(&DDR[0],0,sizeof(DDR)/sizeof(DDR[0])); //寄存器全部清空
    memset(&PFR[0],0,sizeof(PFR)/sizeof(PFR[0]));
	
    WriteKeyProtectedRegister( (unsigned int)&ADERL0, 0x0000, HALFWORD_ACCESS ); //关闭AD功能
    WriteKeyProtectedRegister( (unsigned int)&ADERH0, 0x0000, HALFWORD_ACCESS );
    WriteKeyProtectedRegister( (unsigned int)&ADERL1, 0, HALFWORD_ACCESS );
    WriteKeyProtectedRegister( (unsigned int)&PORTEN, 0xFD, BYTE_ACCESS);	//IO输入功能使能

	/* hal can init */
#if 1//
    set_single_io_dir(9,6,IO_INPUT);	//RX0,IO_INPUT
    PFR[9] |=BIT5;						//pin 9.5 special fun select
    DDR[9] |=BIT5;						//pin 9.5 IO_GENERAL_OUTPUT
    EPFR[86]|=BIT0|BIT5|BIT6|BIT7;		//bit5,6,7 default1
    EPFR[86]&=MASK1;					//01:CAN channel TX0(128) IO_GENERAL_OUTPUT enabled
    WriteKeyProtectedRegister(PFR_ADS[9],PFR[9],BYTE_ACCESS);		// write PFR
    WriteKeyProtectedRegister((unsigned int )&EPFR86,EPFR[86],BYTE_ACCESS);	// write EPFR
    WriteKeyProtectedRegister(DDR_ADS[9],DDR[9],BYTE_ACCESS);		// write DDR
#endif
}




