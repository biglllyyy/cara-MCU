#include <stdlib.h>
#include "hal_ppg.h"
#include "KeyPortReg.h"
#include "hal_gpio.h"
#include "mb91520.h"
#include "string.h"
//#include "buzzer_cfg.h"

/*
#define DIM_IO				5  // from 24 to 27
#define DIM_MODE 			1
#define CarrierFreq_DIM		62500	//20k
*/

#define TWO_BIT_OPT(start_bit)			((U8)(~(0x03<<(start_bit))))
#define CLEAR_TWO_BITS(reg,start_bit)	((reg)&=TWO_BIT_OPT(start_bit))
#define SET_TWO_BITS(reg,start_bit,dat)		((reg)= (reg) | (((dat)&0x03)<<(start_bit)))

enum	//for PPG Count clock selection
{
    PPG_CKS_PCLK=0,
    PPG_CKS_Div4,
    PPG_CKS_Div16,
    PPG_CKS_Div64
};
enum	//for PPG Trigger Input edge selection
{
    PPG_EGS_NA=0,
    PPG_EGS_Rising,
    PPG_EGS_Falling,
    PPG_EGS_BOTH
};
//define ppg chn  relate io,the low 3 bit as bit0-bit7,the high 5 bit as port0-pprt19
// for MB91F522
static const U8	ppg_chn_to_io[48]={
	0x40,0x41,0x42,0x43,0x44,0x45,0x46,0x47,	//P8.0-P8.7
	0x50,0x51,0x52,0x53,0x54,0x55,0x56,0x57,	//P10.0-P10.7
	0x5a,0x5b,0x5c,0x5d,0x5e,0x5f,0x60,0x61,	//P11.2-P11.7;P12.0-P12.1
	0x14,0x15,0x16,0x17,0x18,0x19,0x1a,0x1b,	//P2.4-P2.7,P3.0-P3.3
	0x13,0x28,0x2a,0x2b,0x2c,0x2d,0x72,0x73,	//P2.3,P5.0,P5.2-P5.5,P14.2,14.3
	0x90,0x91,0x92,0x93,0x94,0x95,0x96,0x97	   //P18.0-P18.7
};
#if 0
/* for GCN1x register (common)*/
#define REG_PPG_GCN1x			(0x000001F8UL) /* offset */
#define REG_PPG_GCN1xMASK		(0xFFFFFFFFUL) /* mask */
#define REG_PPG_GCN1x_SHIFT	(0)            /* shift */

/* for GCN2x register (common)*/
#define REG_DMA_DNMIR			(0x000001F4UL) /* offset */
#define REG_DMA_DNMIR_MASK	(0x00FF0000UL) /* mask */
#define REG_DMA_DNMIR_SHIFT		(16)            /* shift */

enum	//for DMAC Request Source
{
    DMAC_RS_Software=0,
    DMAC_RS_Interrupt,
    DMAC_RS_reserved1,
    DMAC_RS_reserved2
};

enum	//for DMAC Transfer Mode
{
    DMAC_TM_BlockTx=0,
    DMAC_TM_BurstTx,
    DMAC_TM_reserved1,
    DMAC_TM_reserved2
};
#endif

enum	//for PPG Interrupt factor selection
{
    PPG_IRS_SWTrigger_TriggerInput=0,
    PPG_IRS_Borrow,
    PPG_IRS_CounterMatch,
    PPG_IRS_Borrow_CounterMatch
};



/*****************************************************************************
    [Description]
    Read register

    [Function]
    READ_REGISTER
        param : offset    [IN] offset address
    [Return value]
    value of the register
*****************************************************************************/
#ifndef WIN32
#define READ_REGISTER(BaseAddr, offset)    *((volatile unsigned long *)(BaseAddr + (offset)))
#else
#define READ_REGISTER(BaseAddr, offset)     0
#endif

/*****************************************************************************
    [Description]
    Write register

    [Function]
    WRITE_REGISTER
        param : offset    [IN] offset address
        param : data      [OUT] value
    [Return value]
    None
*****************************************************************************/
#ifndef WIN32
#define WRITE_REGISTER(BaseAddr, offset, data)   \
{    \
    volatile unsigned long *p = ((unsigned long *)(BaseAddr + (offset)));    \
    *p = (data);    \
}
#else
#define WRITE_REGISTER(BaseAddr, offset, data)   (data = data)
#endif

static void WritePPGRegister
(
		U8 a_nCh, 
		unsigned long  reg_offset, 
		unsigned long  reg_mask, 
		unsigned long  reg_shift, 
		unsigned long  reg_val)
{

    unsigned long  data;
    unsigned long ch_offset;

    if(a_nCh>3)
    {
        ch_offset=(a_nCh-4)*PPG_CHN_4_47_OFFSET;
        data = READ_REGISTER(PPG_BASE4_47, reg_offset+ch_offset);
        data = (data & (~reg_mask));
        data |= ((reg_val << reg_shift) & reg_mask);
        WRITE_REGISTER(PPG_BASE4_47, reg_offset+ch_offset, data);
    }
    else
    {
        ch_offset=a_nCh*PPG_CHN_0_3_OFFSET;
        data = READ_REGISTER(PPG_BASE0_3, reg_offset+ch_offset);
        data = (data & (~reg_mask));
        data |= ((reg_val << reg_shift) & reg_mask);
        WRITE_REGISTER(PPG_BASE0_3, reg_offset+ch_offset,data);
    }
}
/*
static void reset_pin_sts(U8 port,U8 bit,U8 sts)
{
    switch(sts)
    {
    case PPG_PIN_HZ:
        set_single_io_dir(port,bit,IO_INPUT);
        break;
    case PPG_PIN_HIGH:
        set_single_io_dir(port,bit,IO_GENERAL_OUTPUT);
        set_single_io_sts(port,bit,1);
        break;
    case PPG_PIN_LOW:
        set_single_io_dir(port,bit,IO_GENERAL_OUTPUT);
        set_single_io_sts(port,bit,0);
        break;
    default:
        break;
    }
}
*/

/*
static U8  PPGDrv_setPinState(PPGUtil* a_pPPGUtil, U8 a_nPinState)
{
    U8 port = 0;
	U8 bit = 0;

	if((a_pPPGUtil==0)||(a_pPPGUtil->nCh > 47))
        return 0;

		port = ppg_chn_to_io[a_pPPGUtil->nCh]>>3;
		bit =  ppg_chn_to_io[a_pPPGUtil->nCh]&0x07;
		reset_pin_sts(port,bit,a_nPinState);
    return 1;
}
*/


static U8 PPGDrv_Init(PPGUtil* a_pPPGUtil, U8 a_nCh, U8 a_nIORelocation)
{
    U8 port = 0;
	U8 bit = 0;
	U8 odd = 0;
	
	if(a_pPPGUtil==0)
        return 0;

    a_pPPGUtil->nCh = a_nCh;
    a_pPPGUtil->nRelocation = a_nIORelocation;

	switch(a_nIORelocation)	//change output pin,you can select PPGn_0 out put, or PPGn_1 output,or no out put
	{
		case 0:		// no output
					
					break;
		case 1:		//PPGn_0 output
		port =  ppg_chn_to_io[a_pPPGUtil->nCh]>>3;
		bit  =   ppg_chn_to_io[a_pPPGUtil->nCh]&0x07;
		set_single_io_dir(port,bit,IO_FUNC_OUTPUT);
		odd = ((a_pPPGUtil->nCh&0x01)==1)?2:0;
			switch(a_pPPGUtil->nCh)
			{
				case 0:
				case 1:
					CLEAR_TWO_BITS(EPFR[10],odd);
					SET_TWO_BITS(EPFR[10],odd,1);
					WriteKeyProtectedRegister((U32)&EPFR10,EPFR[10],BYTE_ACCESS);
					 break;
				case 2:
				case 3:
					CLEAR_TWO_BITS(EPFR[11],odd);
					SET_TWO_BITS(EPFR[11],odd,1);
					WriteKeyProtectedRegister((U32)&EPFR11,EPFR[11],BYTE_ACCESS);
					break;
				case 4:
				case 5:
					CLEAR_TWO_BITS(EPFR[12],odd);
					SET_TWO_BITS(EPFR[12],odd,1);
					WriteKeyProtectedRegister((U32)&EPFR12,EPFR[12],BYTE_ACCESS);
					break;
				case 6:
					WriteKeyProtectedRegister((U32)&EPFR13,EPFR[13]|BIT0,BYTE_ACCESS);
					break;
				case 7:
					WriteKeyProtectedRegister((U32)&EPFR13,EPFR[13]|BIT1,BYTE_ACCESS);
					break;
				case 8:
					WriteKeyProtectedRegister((U32)&EPFR14,EPFR[14]|BIT0,BYTE_ACCESS);
					break;
				case 9:
					WriteKeyProtectedRegister((U32)&EPFR14,EPFR[14]|BIT1,BYTE_ACCESS);
					break;
				case 10:
				case 11:
				case 12:
					WriteKeyProtectedRegister((U32)&EPFR15,EPFR[15]|(1<<(a_pPPGUtil->nCh -10)),BYTE_ACCESS);
					break;
				case 13:
				case 14:
				case 15:
					WriteKeyProtectedRegister((U32)&EPFR45,EPFR[45]|(1<<(a_pPPGUtil->nCh -13)),BYTE_ACCESS);
					break;
				case 16:
					CLEAR_TWO_BITS(EPFR[16],3);
					SET_TWO_BITS(EPFR[16],3,1);
					WriteKeyProtectedRegister((U32)&EPFR45,EPFR[45],BYTE_ACCESS);
					break;
				case 17:
					CLEAR_TWO_BITS(EPFR[16],5);
					SET_TWO_BITS(EPFR[16],5,1);
					WriteKeyProtectedRegister((U32)&EPFR45,EPFR[45],BYTE_ACCESS);
					break;
				case 18:
					WriteKeyProtectedRegister((U32)&EPFR71,EPFR[71]|BIT0,BYTE_ACCESS);
					break;
				case 19:
					WriteKeyProtectedRegister((U32)&EPFR71,EPFR[71]|BIT2,BYTE_ACCESS);
					break;
				case 20:
					WriteKeyProtectedRegister((U32)&EPFR71,EPFR[71]|BIT4,BYTE_ACCESS);
					break;
				case 21:
					WriteKeyProtectedRegister((U32)&EPFR71,EPFR[71]|BIT6,BYTE_ACCESS);
					break;
				case 22:
					WriteKeyProtectedRegister((U32)&EPFR72,EPFR[72]|BIT0,BYTE_ACCESS);
					break;
				case 23:
					CLEAR_TWO_BITS(EPFR[72],2);
					SET_TWO_BITS(EPFR[72],2,1);
					WriteKeyProtectedRegister((U32)&EPFR72,EPFR[72],BYTE_ACCESS);
					break;
				case 24:
					CLEAR_TWO_BITS(EPFR[72],4);
					SET_TWO_BITS(EPFR[72],4,1);
					WriteKeyProtectedRegister((U32)&EPFR72,EPFR[72],BYTE_ACCESS);
					break;
				case 25:
					CLEAR_TWO_BITS(EPFR[72],6);
					SET_TWO_BITS(EPFR[72],6,1);
					WriteKeyProtectedRegister((U32)&EPFR72,EPFR[72],BYTE_ACCESS);
					break;
				case 26:
				case 27:
				case 28:
				case 29:
					CLEAR_TWO_BITS(EPFR[73],6);
					SET_TWO_BITS(EPFR[73],6,1);
					WriteKeyProtectedRegister((U32)&EPFR73,EPFR[73],BYTE_ACCESS);
					break;
				case 30:
				case 31:
				case 32:
				case 33:
					CLEAR_TWO_BITS(EPFR[74],(a_pPPGUtil->nCh-30)<<1);
					SET_TWO_BITS(EPFR[74],(a_pPPGUtil->nCh-30)<<1,1);
					WriteKeyProtectedRegister((U32)&EPFR74,EPFR[74],BYTE_ACCESS);
					break;
				case 34:
				case 35:
				case 36:
				case 37:
					CLEAR_TWO_BITS(EPFR[75],(a_pPPGUtil->nCh-34)<<1);
					SET_TWO_BITS(EPFR[75],(a_pPPGUtil->nCh-34)<<1,1);
					WriteKeyProtectedRegister((U32)&EPFR75,EPFR[75],BYTE_ACCESS);
					break;
				case 38:
				case 39:
				case 40:
				case 41:
					CLEAR_TWO_BITS(EPFR[76],(a_pPPGUtil->nCh-38)<<1);
					SET_TWO_BITS(EPFR[76],(a_pPPGUtil->nCh-38)<<1,1);
					WriteKeyProtectedRegister((U32)&EPFR76,EPFR[76],BYTE_ACCESS);
					break;
				case 42:
					WriteKeyProtectedRegister((U32)&EPFR77,EPFR[77]|BIT0,BYTE_ACCESS);
					break;
				case 43:
					CLEAR_TWO_BITS(EPFR[77],1);
					SET_TWO_BITS(EPFR[77],1,1);
					WriteKeyProtectedRegister((U32)&EPFR77,EPFR[77],BYTE_ACCESS);
					break;
				case 44:
					CLEAR_TWO_BITS(EPFR[77],3);
					SET_TWO_BITS(EPFR[77],3,1);
					WriteKeyProtectedRegister((U32)&EPFR77,EPFR[77],BYTE_ACCESS);
					break;
				case 45:
					WriteKeyProtectedRegister((U32)&EPFR77,EPFR[77]|BIT5,BYTE_ACCESS);
					break;
				case 46:
					WriteKeyProtectedRegister((U32)&EPFR78,EPFR[78]|BIT0,BYTE_ACCESS);
					break;
				case 47:
					WriteKeyProtectedRegister((U32)&EPFR78,EPFR[78]|BIT1,BYTE_ACCESS);
					break;
				default:break;
			}
			break;
		case 2:		//PPGn_1 output
			break;
		default:break;
	}
    return 1;
}


//Channel Specified
static U8 PPGConfig_ChannelControl(PPGUtil* a_pPPGUtil, unsigned int type, U8 value)
{
    if(a_pPPGUtil==0)
        return 0;
    if(type >PPG_PCN_CNTE)
        return 0;

    switch(type)
    {
    case PPG_PCN_CNTE:	/* timer operation stopeed or not,0:stop,1:enable */
        a_pPPGUtil->nCNTE_IsActive=value;
        break;
    case PPG_PCN_STRG:
        a_pPPGUtil->nSTRG_IsSoftwareTrigger=value;
        break;
    case PPG_PCN_MDSE:
        a_pPPGUtil->nMDSE_ModeSelection=value;
        break;
    case PPG_PCN_RTRG:
        a_pPPGUtil->nRTRG_IsRestartEnable=value;
        break;
    case PPG_PCN_CKS1:
    case PPG_PCN_CKS0:
        a_pPPGUtil->nCKS_CountClockSelection=value;
        break;
    case PPG_PCN_PGMS:
        a_pPPGUtil->nPGMS_PPGOutputMask=value;
    case PPG_PCN_EGS1:
    case PPG_PCN_EGS0:
        a_pPPGUtil->nEGS_TriggerInputEdgeSel=value;
        break;
    case PPG_PCN_IREN:
        a_pPPGUtil->nIREN_InterruptRequestEnable=value;
        break;
    case PPG_PCN_IRQF:
        a_pPPGUtil->nIRQF_InterruptRequestFlag=value;
        break;
    case PPG_PCN_IRS1:
    case PPG_PCN_IRS0:
        a_pPPGUtil->nIRS_InterruptFactorSel=value;
        break;
    case PPG_PCN_OSEL:
        a_pPPGUtil->nOSEL_PolaritySelection=value;
        break;
    default:
        break;
    }

    return 1;

}

/*
static U8 PPGConfig_ChannelControl_Stop(PPGUtil* a_pPPGUtil)
{
    U16 data;
    if (a_pPPGUtil==0)
        return FALSE;

    data = (0<<PPG_PCN_CNTE)
           | (a_pPPGUtil->nSTRG_IsSoftwareTrigger 		<< PPG_PCN_STRG)
           | (a_pPPGUtil->nMDSE_ModeSelection 		<< PPG_PCN_MDSE)


           | (a_pPPGUtil->nRTRG_IsRestartEnable		<< PPG_PCN_RTRG)
           | (a_pPPGUtil->nCKS_CountClockSelection	<< PPG_PCN_CKS0)
           | (a_pPPGUtil->nPGMS_PPGOutputMask		<< PPG_PCN_PGMS)

           | (a_pPPGUtil->nEGS_TriggerInputEdgeSel	<< PPG_PCN_EGS0)
           | (a_pPPGUtil->nIREN_InterruptRequestEnable	<< PPG_PCN_IREN)
           | (a_pPPGUtil->nIRQF_InterruptRequestFlag	<< PPG_PCN_IRQF)

           | (a_pPPGUtil->nIRS_InterruptFactorSel		<< PPG_PCN_IRS0)
           | (a_pPPGUtil->nOSEL_PolaritySelection		<< PPG_PCN_OSEL);


    //write data
    WritePPGRegister(a_pPPGUtil->nCh , REG_PPG_PCN,
                     REG_PPG_PCN_MASK,
                     REG_PPG_PCN_SHIFT,
                     data);
    a_pPPGUtil->nPPG_OnOff_Sts = OFF;
    return TRUE;
}
*/

static U8 PPGConfig_ChannelControl_Start(PPGUtil* a_pPPGUtil)
{
    U16 data;
    if (a_pPPGUtil==0)
        return FALSE;

    data = 1<<PPG_PCN_CNTE	//Active the timer operation
           | a_pPPGUtil->nSTRG_IsSoftwareTrigger 		<< PPG_PCN_STRG
           | a_pPPGUtil->nMDSE_ModeSelection 		<< PPG_PCN_MDSE


           | a_pPPGUtil->nRTRG_IsRestartEnable		<< PPG_PCN_RTRG
           | a_pPPGUtil->nCKS_CountClockSelection	<< PPG_PCN_CKS0
           | a_pPPGUtil->nPGMS_PPGOutputMask		<< PPG_PCN_PGMS

           | a_pPPGUtil->nEGS_TriggerInputEdgeSel	<< PPG_PCN_EGS0
           | a_pPPGUtil->nIREN_InterruptRequestEnable	<< PPG_PCN_IREN
           | a_pPPGUtil->nIRQF_InterruptRequestFlag	<< PPG_PCN_IRQF

           | a_pPPGUtil->nIRS_InterruptFactorSel		<< PPG_PCN_IRS0
           | a_pPPGUtil->nOSEL_PolaritySelection		<< PPG_PCN_OSEL;

    data |= a_pPPGUtil->nPDUT << REG_PPG_PDUT_SHIFT;

    //write data
    WritePPGRegister(a_pPPGUtil->nCh,REG_PPG_PCN,
                     (0xFFFFFFFFUL),
                     REG_PPG_PCN_SHIFT,
                     data);
    a_pPPGUtil->nPPG_OnOff_Sts = ON;
    return TRUE;
}

static U8 PPGConfig_ChannelPCSR(PPGUtil* a_pPPGUtil, U16 a_nCycle)
{
    if (a_pPPGUtil==0)
        return FALSE;

    a_pPPGUtil->nPCSR = a_nCycle;

    //write data
    WritePPGRegister(a_pPPGUtil->nCh , REG_PPG_PCSR,
                     REG_PPG_PCSR_MASK,
                     REG_PPG_PCSR_SHIFT,
                     a_pPPGUtil->nPCSR);
    return TRUE;
}
static U8 PPGConfig_ChannelPDUT(PPGUtil* a_pPPGUtil, U16 a_pdutRes)
{
    if (a_pPPGUtil==0)
        return FALSE;

    a_pPPGUtil->nPDUT= a_pdutRes;

    //write data
    WritePPGRegister(a_pPPGUtil->nCh , REG_PPG_PDUT,
                     REG_PPG_PDUT_MASK,
                     REG_PPG_PDUT_SHIFT,
                     a_pPPGUtil->nPDUT);
    return TRUE;
}

/*
** \input
				PPGUtil *ppg_chn;保留PPG通道数据
				chn: PPG 通道选择
				freq: 频率设置
				duty: 占空比设置（0.1）
** \output
**
================================================================================
*/
void HalPwmStart(PPGUtil *ppg_chn,U8 chn,U16 frq,U8 duty)
{
    U16 cycle_value = 0;
    U8  factor_frq_div  = 0;				/* 分频系数 */

    do
    {
		if((duty == ppg_chn->nDuty)&&(ON == ppg_chn->nPPG_OnOff_Sts))
		{
			break;
		}
		ppg_chn->nDuty = duty;
		if(frq > 100000)
        {
            DBG("fre too big \n",0);
        }
        else if(frq >= 1000)
        {
            factor_frq_div = PPG_CKS_Div4;
            cycle_value = (U16)(PPG_PCLK/(4*frq)-1);
        }
        else if(frq < 1000)
        {
            factor_frq_div = PPG_CKS_Div64;		/* 64 分频 */
            cycle_value = (U16)(PPG_PCLK/(64*frq)-1);
        }
        else if(frq < 10)
        {
            DBG("req %d too samall",frq);
        }
        if(duty > 100)
        {
            duty = 100;
        }

        PPGDrv_Init(ppg_chn, chn, DIM_MODE);				//重新配置
        PPGConfig_ChannelPCSR(ppg_chn,cycle_value);			//设置PCSR（周期）
        PPGConfig_ChannelPDUT(ppg_chn,cycle_value*duty/100);//设置PDUT（占空比）
        PPGConfig_ChannelControl(ppg_chn, PPG_PCN_CNTE, 1); //count enable

        PPGConfig_ChannelControl(ppg_chn, PPG_PCN_OWFS, 0); //normal wave
        PPGConfig_ChannelControl(ppg_chn, PPG_PCN_STRG, 1); //Active by software only
        PPGConfig_ChannelControl(ppg_chn, PPG_PCN_MDSE, 0); //PWM operation
        PPGConfig_ChannelControl(ppg_chn, PPG_PCN_RTRG, 0); //restart disable
        PPGConfig_ChannelControl(ppg_chn, PPG_PCN_CKS0, factor_frq_div);	//restart disable
        //PPGConfig_ChannelControl(&l_sPPG_DIM, PPG_PCN_PGMS, 1);	//output mask and the GPIO state is assigned with the state of PPG_PCN_OSEL
        PPGConfig_ChannelControl(ppg_chn, PPG_PCN_PGMS, 0); //No output mask
        PPGConfig_ChannelControl(ppg_chn, PPG_PCN_EGS0, PPG_EGS_NA);	//No output mask
        PPGConfig_ChannelControl(ppg_chn, PPG_PCN_IREN, 0); //Interrupt request disabled
        PPGConfig_ChannelControl(ppg_chn, PPG_PCN_IRQF, 0); //Clear Interrupt request flag
        PPGConfig_ChannelControl(ppg_chn, PPG_PCN_IRS0, PPG_IRS_SWTrigger_TriggerInput);	//Don't care
        PPGConfig_ChannelControl(ppg_chn, PPG_PCN_OSEL, 0); //Normal Polarity

        PPGConfig_ChannelControl_Start(ppg_chn);	//启动PWM
    }
    while (0);
}
