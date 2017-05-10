#ifndef _PPG_H_
#define _PPG_H_

#include "comm_typedef.h"
#include "hal_gpio.h"
#include "hal_clock.h"

typedef enum	//for pin state
{
    PPG_PIN_HZ=0,			/* 高阻态 */
    PPG_PIN_HIGH,
    PPG_PIN_LOW,
    PPG_PIN_PWM
} PPG_PIN_STS_e;


#define PPG_BASE0_3			 (0x1A40)
#define PPG_BASE4_47	     (0x1AB0)
#define PPG_BASE0			 (0x1A40)
/* 定义0-3个通道的起始地址 */
#define PPG_BASE0			(0x1A40)
#define PPG_BASE1			(0x1A5C)
#define PPG_BASE2			(0x1A78)
#define PPG_BASE3 			(0x1A94)
/* 从4 到47 通道地址 */
#define PPG_BASE(n)   (0x1AB0 + (n-4)*0x10)

//definition
#ifndef TRUE
#define TRUE	(1==1)
#define FALSE	(1==0)
#endif

#define PPG_PCLK   PCLK 	

//channel offset
#define PPG_CHN_4_47_OFFSET		0x10
#define PPG_CHN_0_3_OFFSET		0x1C

//define PCN shift bit
#define PPG_PCN_CNTE	15	//Timer operation enable
#define PPG_PCN_STRG	14	//Software Trigger
#define PPG_PCN_MDSE	13	//Mode Selection
#define PPG_PCN_RTRG	12	//Restart enable
#define PPG_PCN_CKS1	11	//Count clock selection
#define PPG_PCN_CKS0	10	//Count clock selection
#define PPG_PCN_PGMS	9	//PPG output mask selection
#define PPG_PCN_OWFS    8   //normal and center aligned wave seltct
#define PPG_PCN_EGS1	7	//Trigger input edge seleciton
#define PPG_PCN_EGS0	6	//Trigger input edge seleciton
#define PPG_PCN_IREN	5	//Interrupt request enable
#define PPG_PCN_IRQF	4	//Interrupt request flag
#define PPG_PCN_IRS1	3	//Interrupt factor selection
#define PPG_PCN_IRS0	2	//Interrupt factor selection
#define PPG_PCN_OSEL	0	//Polarity selection

/* for PCN register */
#define REG_PPG_PCN				(0x00000000UL) /* offset */
#define REG_PPG_PCN_MASK		(0xFFFF0000UL) /* mask */
#define REG_PPG_PCN_SHIFT		(16)           /* shift */

/* for PCSR register */
#define REG_PPG_PCSR			(0x00000000UL) /* offset */
#define REG_PPG_PCSR_MASK		(0x0000FFFFUL) /* mask */
#define REG_PPG_PCSR_SHIFT		(00)            /* shift */


/* for PDUT register */
#define REG_PPG_PDUT			(0x00000004UL) /* offset */
#define REG_PPG_PDUT_MASK		(0xFFFF0000UL) /* mask */
#define REG_PPG_PDUT_SHIFT		(16)            /* shift */

/* for PTMR register */
#define REG_PPG_PTMR			(0x00000004UL) /* offset */
#define REG_PPG_PTMR_MASK		(0x0000FFFFUL) /* mask */
#define REG_PPG_PTMR_SHIFT		(00)            /* shift */



typedef struct __PPGUtil
{
    U8 nCh;
    U8 nRelocation;						  //重新配置引脚状态
	U8 nPPG_OnOff_Sts;					  //ppg sts
	U8 nDuty;							  //duty store
	U32 nFrq;

    U8 nCNTE_IsActive;					  //Active
    U8 nSTRG_IsSoftwareTrigger;			  //Software Trigger
    U8 nMDSE_ModeSelection;				  //Mode Selection
    U8 nRTRG_IsRestartEnable;				//Restart Enable

    U8 nCKS_CountClockSelection;			//Count Clock Selection
    U8 nPGMS_PPGOutputMask;				   //PPG Output Mask Selection

    U8 nEGS_TriggerInputEdgeSel;			//Trigger input edge selection
    U8 nIREN_InterruptRequestEnable;		//Interrupt Request Enable
    U8 nIRQF_InterruptRequestFlag;			//Interrupt Request Flag

    U8 nIRS_InterruptFactorSel;				//Interrupt Factor Selection
    U8 nOSEL_PolaritySelection;				//Output Polarity Selection

    U16  nPCSR;								//Cycle Setting
    U16  nPDUT;								//Duty Setting
    
} PPGUtil;



#define CountRatio 	1
#define DIM_MODE 	1

#define CarrierFreq_DIM		(50000)	//62.5k

#define PCSR_VALUE			(PPG_PCLK/(CountRatio*CarrierFreq_DIM))

#define PDUT_1_PERCENT	(PCSR_VALUE/100)


extern void HalPwmStart(PPGUtil *ppg_chn,U8 chn,U32 frq,U8 duty);
extern void HalPwmStop(PPGUtil *ppg_chn,PPG_PIN_STS_e sts);
void HalPwmFineStart(PPGUtil *ppg_chn,U8 chn,U32 frq,U16 duty);
extern void HalPwmInit(PPGUtil *ppg_chn,U8 chn); //inint globle variable
extern void Init_PPG_Test(void);
extern U8 PPGConfig_ChannelPCSR(PPGUtil* a_pPPGUtil, U16 a_nCycle);
extern U8 PPGConfig_ChannelPDUT(PPGUtil* a_pPPGUtil, U16 a_pdutRes);
extern U8 PPGConfig_ChannelControl_Start(PPGUtil* a_pPPGUtil);
extern U8 PPGConfig_ChannelControl_Stop(PPGUtil* a_pPPGUtil);

#endif



