#include <stdlib.h>
#include "hal_adc.h"
#include "KeyPortReg.h"
#include "mb91520.h"
#include "hal_gpio.h"

#define   AD0_DATA_BASE_ADDS   (0x138C)
#define   AD1_DATA_BASE_ADDS   (0x14F8)
#define   AD0_DATA_ADDS_0_31(chn)	   (AD0_DATA_BASE_ADDS+2*(chn))
#define   AD1_DATA_ADDS_32_47(chn)	   (AD1_DATA_BASE_ADDS+2*(chn))

#define   ADTCS_BASE_0_31_ADDS		   (0x134C)
#define   ADTCS_ADDS_0_31(chn)		   (ADTCS_BASE_0_31_ADDS + 2*(chn))
#define   ADTCS_BASE_32_47_ADDS        (0x14B8)
#define   ADTCS_ADDS_32_47(chn)		   (ADTCS_BASE_32_47_ADDS + 2*(chn))


static U16  REG_ADERLO = 0;
static U16  REG_ADERH0 = 0;
static U16  REG_ADERL1 = 0;
U16 AD_Value[MAX_AD_CHN] = {0xffff};
U8          current_chn = 0;

/*
ad chn to io table,hight 5 bits as port,low 3 bits as bit
*/
static const U8	ad_chn_to_io[48]={
	0x41,0x42,0x43,0x44,0x48,0x49,0x4A,0x4B,//AN0-AN7:P8.1_P8.4,P9.0-P9.2,P9.3
	0x4C,0x4D,0x4E,0x4F,0x50,0x51,0x52,0x53,//AN8-AN15:P9.4-P9.7,P10.0-P10.3
	0x54,0x55,0x56,0x57,0x7C,0x7D,0x58,0x59,//AN16-AN23:P10.4_P10.7,P15.4,P15.5,P11.0,P11.1
	0x5a,0x5b,0x5c,0x5d,0x5e,0x5f,0x60,0x62,//AN24-AN31:P11.2-P11.7,P12.0,P12.2
	0x7b,0x3b,0x3a,0x39,0x57,0x55,0x54,0x53,//AN32-AN39:P15.3,P7.3,P7.2,P7.1,P6.7,P6.6,P6.4,P6.3
	0x32,0x31,0x2F,0x2d,0x2b,0x27,0x25,0x23 //AN40-AN47:P6.2,P6.1,P5.7,P5.5,P5.3,P4.7,P4.5,P4.3
};

void hal_adc_init(ad_group_t group, U8 single_chn)
{
#ifndef WIN32	
	U8 Triger_Mode = SOFT_TRIGGER;
	U8 Count_Mode = COUNT_DISABLE;
	U16 *ptr_reg = NULL;
	U16 temp;
	U8  port,bit = 0;
	U32 bits_chn = 0;
	
	bits_chn = 1<<single_chn; 
	if(single_chn < 32)
	{
		if(single_chn < 16)
		{
			REG_ADERLO |= bits_chn;	 //Analog input enabled
		}
		else
		{
			REG_ADERH0 |= (bits_chn>>16);	 //Analog input enabled
		}	
	}
	else
	{
		REG_ADERL1 |= 1<<(single_chn-32);	//Analog input enabled
	}
	WriteKeyProtectedRegister((unsigned int)&ADERL0,REG_ADERLO, HALFWORD_ACCESS);		//Analog input enabled
	WriteKeyProtectedRegister((unsigned int)&ADERH0,REG_ADERH0, HALFWORD_ACCESS);//Analog input enabled
	WriteKeyProtectedRegister((unsigned int)&ADERL1,REG_ADERL1, HALFWORD_ACCESS); //Analog input enabled

	port = ad_chn_to_io[single_chn]>>3;
	bit  = ad_chn_to_io[single_chn]&0x07;
	set_single_io_dir(port,bit,IO_INPUT);	//set io direction 

    if(AD_GROUP1 == group)	//chn 0-31
    {    
		ADTSS0_START = 0;	// Stop AD converter
        ADTSE0  = bits_chn ;		// ad active
        ADMD0  = 0x0F;		   // Sampling time setting of all channel commonness //Compare time 28 Peripheral clock // Sampling time 12 Peripheral clock
		Count_Mode &= 0x03;
        Triger_Mode &= 0x07;
		ptr_reg = (U16 *)((U32)REG_ADTECS_0_31_ADDS(single_chn));
		temp = *ptr_reg;
        if((Triger_Mode & 0x04) == 1)/* set ADTECn_STS2 = 1*/
		{
			temp |= BIT8;
		}
		else/* set ADTECn_STS2 = 0*/
		{
			temp &= MASK8;
		}
		temp &= 0xFE00;
		temp |=  single_chn;			// : Analog channel select bit,5bits
		*ptr_reg = temp;
		Triger_Mode &= 0x03;
		ptr_reg = (U16 *)((U32)REG_ADTCS_0_31_ADDS(single_chn));
        *ptr_reg =  (0 <<15)			// Busy flag
                  |(0 <<14)			// Interrupt flag
                  |(1 <<13)			// Interrupt enable
                  |(Triger_Mode<<11)// Triger mode
                  |(0 <<10)			// Single or repeat conversion
                  |(0 << 9)			// Protect enable
                  |(0 << 8)			// A/D data register protection clear select bit
                  |(Count_Mode<<6)	// Counting direction select bits
                  |(1 << 5)			// Compare register buffer function control bit   // disable
                  |(0 << 4)			// Compare register buffer transfer control bit
                  |(0 << 0)			// bit 0-3 are reserved
                  |(0 << 0)
                  |(0 << 0);
		/*ptr_reg = (U16 *)((U32)REG_ADNCS_0_15_ADDS(single_chn/2));
		if(1 == (single_chn & 0x01))	// i is odd
		{
			*ptr_reg |= BIT7;
		}
		else  //i is even
		{
			*ptr_reg |= BIT3;
		}*/
		//ptr_reg32=(U32*)((U32)REG_ADTSE0_ADSS);
		//*ptr_reg32|=(1<<(single_chn));
		//ADSCANS0 = BIT6;				//bit 7: SCINT
											//bit 6:SCIE 0:can conversion completion interrupt disabled
											//			 1:can conversion completion interrupt enabled
											//bit5: SCMD: 0:Continuous scan conversion mode
											//			  1:Stop scan conversion mode		
        //ICR32 = 20;
		current_chn = single_chn;
    }
	else//chn 32-47
    {
		ADTSS1_START = 0;	// Stop AD converter
        ADTSE1 = 1<<(single_chn-32); //Software activation enabled;
        ADMD1  = 0x0F; //增加了采样时间和比较时间
        //ICR41 = 20;
        Count_Mode &= 0x03;
        Triger_Mode &= 0x07;
		ptr_reg = (U16 *)((U32)REG_ADTECS_32_47_ADDS(single_chn-32)); //A/D activation trigger extend control register
        temp = *ptr_reg;
        if((Triger_Mode & 0x04) == 1)
		{
			temp |= BIT8;// Triger mode
		}
		else
		{
			 temp &= MASK8;
		}
		 temp &= 0xFF00;
		 temp |= single_chn-32;			// select the analog channel
		 *ptr_reg = temp;
         Triger_Mode &= 0x03;
		 ptr_reg = (U16 *)((U32)REG_ADTCS_32_47_ADDS(single_chn-32));
         *ptr_reg =   (0 <<15)			// Busy flag
                       |(0 <<14)			// Interrupt flag
                       |(1 <<13)			// Interrupt enable
                       |(Triger_Mode<<11)	// Triger mode,soft trigger
                       |(0 <<10)			// Single or repeat conversion
                       |(0 << 9)			// Protect enable
                       |(0 << 8)			// A/D data register protection clear select bit
                       |(Count_Mode<<6)	// Counting direction select bits
                       |(1 << 5)			// Compare register buffer function control bit   // disable
                       |(0 << 4)			// Compare register buffer transfer control bit
                       |(0 << 0)			// bit 0-3 are reserved
                       |(0 << 0)
                       |(0 << 0);
		/*ptr_reg = (U16 *)((U32)REG_ADNCS_16_23_ADDS((single_chn-32)/2));
		if(1 == (single_chn&0x01))	// i is odd
		{
			*ptr_reg |= BIT7;
		}
		else  //i is even
		{
			*ptr_reg |= BIT3;
		}*/
		//ptr_reg32=(U32*)((U32)REG_ADTSE1_ADSS);
		//*ptr_reg32|=1<<(single_chn-32);
		//	ADSCANS1 = BIT6;				//bit 7: SCINT
			//bit 6:SCIE 0:can conversion completion interrupt disabled
			//			 1:can conversion completion interrupt enabled
			//bit5: SCMD: 0:Continuous scan conversion mode
			//			  1:Stop scan conversion mode
		current_chn = single_chn;
    }
#endif
}

void hal_adc_start(ad_group_t group)
{
	if( AD_GROUP1 == group)
	{
		ADTSS0_START = 1;
	}
	else 
	{
		ADTSS1_START = 1;
	}
}

void hal_adc_stop(ad_group_t group)
{
    if( AD_GROUP1 == group)
    {
		ADTSS0_START = 0;
    }
    else 
    {
		ADTSS1_START = 0;
    }
}


__interrupt void  AD_Converter_interrupt_0_31(void)
{
    U16 temp_value = 0;
    U16 *ptr = 0;
	
	ptr = (U16 *)((U32)REG_ADTCS_0_31_ADDS(current_chn));	//get adtcs value
	temp_value = *ptr;
	temp_value &= MASK14;
	*ptr = temp_value;	//set  bit INT  as 0;

	ptr =  (U16 *)((U32)AD0_DATA_ADDS_0_31(current_chn));
	temp_value = *ptr;
	if(0 == (temp_value & 0x8000)) //最高位为1表示错误
	{
		AD_Value[current_chn] = temp_value & 0x0FFF;
	}
	else
	{
		AD_Value[current_chn] = 0xffff;
	}
	ADTSS0_START = 0;
}

__interrupt void  AD_Converter_interrupt_32_47(void)
{
    U16 temp_value = 0;
    U16 *ptr = 0;
	
	ptr = (U16 *)((U32)REG_ADTCS_32_47_ADDS(current_chn-32));	//get adtcs value
	temp_value = *ptr;
	temp_value &= MASK14;
	*ptr = temp_value;	//set  bit INT  as 0;
	ptr = (U16*)(U32)AD1_DATA_ADDS_32_47(current_chn-32);
	temp_value = *ptr;
	if(0 == (temp_value & 0x8000)) //最高位为1表示错误
	{
		AD_Value[current_chn] = temp_value & 0x0FFF;
	}
	 else
	{
    	AD_Value[current_chn] = 0Xffff;
 	}
 	ADTSS1_START = 0;
}


/* get ad value */
/*
return : 0 convert finish and ok
return : 1: conver err or no finish
*/
U16 hal_adc_get(ad_group_t group,U8 single_chn)
{
    U16 ret = 1;

	if(single_chn < MAX_AD_CHN)
	{
		ret = AD_Value[single_chn];
	}
	else
	{
		ret = 0xFFFF;
	}
	return ret;
}
/*
	this functon changed ad follows:
	input--> ouput
	0x01 -->0
	0x02 -->1
	0x04 -->2
	0x08 -->3
	... ---> ...
	0x80000000-->31
*/
#if 0
static U8  group_chn_to_signal_chn(U32 chn)
{
	U8 cnt = 0;
	
	while(!(chn&0x01))
	{
		chn>>=1;
		cnt++;
	}
	return(cnt+1);
}
#endif
