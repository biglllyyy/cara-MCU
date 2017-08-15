
#ifndef _HAL_ADC_H__
#define _HAL_ADC_H__

#include "comm_typedef.h"

typedef enum{
	AD_GROUP1 = 0,	//0-31
	AD_GROUP2 = 1	//32-47
}ad_group_t;


#define  REG_ADTCD_BASE    (0x138C)
#define  REG_ADTCD_ADDS(n)  (REG_ADTCD_BASE + (n)<<1)

//ADTCS address
#define  REG_ADTCS_0_31_BASE	   (0x134C)
#define  REG_ADTCS_32_47_BASE		(0x14B8)
#define  REG_ADTCS_0_31_ADDS(n)  (REG_ADTCS_0_31_BASE + ((n)<<1))
#define  REG_ADTCS_32_47_ADDS(n)  (REG_ADTCS_32_47_BASE + ((n)<<1))

//ADTECS address
#define  REG_ADTECS_0_31_BASE     (0x13CC)
#define  REG_ADTECS_32_47_BASE    (0x1538)
#define  REG_ADTECS_0_31_ADDS(n)  (REG_ADTECS_0_31_BASE + ((n)<<1))
#define  REG_ADTECS_32_47_ADDS(n)  (REG_ADTECS_32_47_BASE + ((n)<<1))
//ADNCS
#define  REG_ADNCS_0_15_BASE	  (0x1448)
#define  REG_ADNCS_16_23_BASE	  (0x15B4)
#define  REG_ADNCS_0_15_ADDS(n)	  (REG_ADNCS_0_15_BASE+ (n))
#define  REG_ADNCS_16_23_ADDS(n)		  (REG_ADNCS_16_23_BASE + (n))
//ADTSE  A/D Software Activation Channel Select Register
#define  REG_ADTSE0_ADSS        (0x1308)
#define  REG_ADTSE1_ADSS        (0x1474)
//ADRCC
#define  REG_ADRCCS_0_31_BASE	(0x141C)
#define  REG_ADRCCS_32_47_BASE   (0x1588)
#define  REG_ADRCCS_32_47_ADDS(n)  (REG_ADRCCS_32_47_BASE+(n))    
#define  REG_ADRCCS_0_31_ADDS(n)   (REG_ADRCCS_0_31_BASE + (n))



#define MAX_AD_CHN  		48
#define GROUP1_MAX_CHN		31		
#define GROUP2_MAX_CHN		15

#define FUEL_CHN_A		AD_CHN44
#define FUEL_CHN_B		AD_CHN43
#define AD_KL30_CHN		1


enum
{
    SOFT_TRIGGER = 0,			//default
    EXT_FALLING_EDGE_TRIGGER = 1,
    RELOAD_TIMER_TRIGGER = 2,
    COMPARE_MATCH_TRIGGER = 3,
    PPG_RISING_TRIGGER=4,
    DISABLE_TRIGGER = 5,
};

enum
{
    BOTH_UP_DOWN = 0,
    COUNT_UP = 1,
    COUNT_DOWN = 2,
    COUNT_DISABLE = 3
};

extern U16 AD_Value[3][0x20];

void hal_adc_init(ad_group_t group, U8 single_chn);
void hal_adc_start(ad_group_t group);
void hal_adc_stop(ad_group_t  group);
U16 hal_adc_get(ad_group_t group,U8 single_chn);

#endif
