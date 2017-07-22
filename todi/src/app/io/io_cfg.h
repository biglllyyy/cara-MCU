#ifndef		__IO_CFG_H__
#define		__IO_CFG_H__

#include "function_cfg.h"
#include "mid_filter.h"

/**************************************/
/*Core power */
/*********************************************/
#define  PIN_CORE_POWER				    CFG_ON
#define  PIN_IGN_IN						CFG_ON
/**********************************************/
#define  SIG_IN_HEAD_LIGHT_CONDITON     CFG_ON 
#define  SIG_IN_RIGHT_TURN				CFG_ON
#define  SIG_IN_LEFT_TURN				CFG_ON
#define  SIG_IN_DIPPLED_LIGHT			CFG_ON
#define  SIG_IN_FAR_BEAM				CFG_ON
#define  SIG_IN_FRONT_FOG				CFG_ON
#define  SIG_IN_REAR_FOG				CFG_ON
#define  SIG_IN_PARK					CFG_ON
#define  SIG_IN_POSITION_LIGHT			CFG_ON
/*********************************************/
//AD io config
/*********************************************/
/* DOOR */
#define  SIG_IN_DOORS					CFG_OFF
/* ElectronSecurity */
#define  SIG_IN_EAS						CFG_ON	
/* AIR bag */
#define  SIG_IN_MAIN_AIRBAG				CFG_ON
#define  SIG_IN_PASSENGER_AIRBAG		CFG_OFF
/* belt */
#define  SIG_IN_MAIN_BELT				CFG_ON
#define  SIG_IN_PASSENGER_BELT			CFG_OFF
/* safe */
#define  SIG_IN_BRAKE					CFG_ON
#define  SIG_IN_WASH_LIQUID_LOW			CFG_ON
#define  SIG_IN_CHARGE_INDICATE			CFG_ON
#define  SIG_IN_BATTERY_CHARGE			CFG_ON
#define  SIG_IN_AIRBAG					CFG_ON
#define  SIG_IN_KEY						CFG_ON	
#define  SIG_VIDEO_INPUT				CFG_ON
#define  SIG_BACKLIGHT_EN				CFG_ON	  /* backlight io*/
/*---------------------------------------------------------------*/
#define  SIG_OUT_LEFT_LED				CFG_OFF
#define  SIG_OUT_RIGHT_LED				CFG_OFF
#define  SIG_OUT_FAR_BEAM_LED			CFG_OFF
#define  SIG_OUT_LOW_BEAM_LED			CFG_OFF
#define  SIG_OUT_FRONT_FOG_LED			CFG_OFF
#define  SIG_OUT_REAR_FOG_LED			CFG_OFF
#define  SIG_OUT_HEAD_LIGHT_LED			CFG_OFF
#define  SIG_OUT_POSITION_LED			CFG_OFF
#define  SIG_OUT_WASH_LOW_LED			CFG_OFF
#define  SIG_OUT_EAT_LED				CFG_OFF
#define  SIG_OUT_BAT_CHARGE_LED			CFG_OFF
#define  SIG_OUT_PARK_LED				CFG_OFF
#define  SIG_OUT_ABS_LED				CFG_OFF
#define  SIG_OUT_MAIN_BELT_LED			CFG_OFF
#define  SIG_OUT_AIRBAG_LED				CFG_OFF
#define  SIG_OUT_BRAKE_LED				CFG_OFF
#define  SIG_A20_START_UP				CFG_ON

#define SIG_BUZ_CTL CFG_ON /* 蜂鸣器控制IO配置打开 */

/*-------------------------------------------------------------*/
typedef enum{
	PIN_IN_WAKEUP1 = 0,  //IGN
	PIN_IN_WAKEUP2,
	PIN_IN_WAKEUP3,
	/*PIN_IN_OIL_PRESS,
	PIN_IN_GUARD,
	PIN_IN_CHARGE,
	PIN_IN_ENGINE_GUARD,
	PIN_IN_MAIN_BELT,
	PIN_IN_ALS_ERROR,
	PIN_IN_BRAKE_WARNING,*/
	PIN_IN_KEY0,
	PIN_IN_KEY1,
	PIN_IN_KEY2,
	PIN_IN_KEY3,
	/*PIN_IN_REVERSE_SIGNAL,
	PIN_IN_REVERSE_RADAR,
	PIN_IN_VIDEO_MPOUT,
	PIN_IN_VIDEO_INTREQ,*/
	//PIN_IN_ARM_LOAD, /* CORE升级设置引脚，低有效 */
	PIN_IN_01,
	PIN_IN_02,
	PIN_IN_03,
	PIN_IN_04,
	PIN_IN_05,
	PIN_IN_06,
	PIN_IN_07,
	PIN_IN_08,
	PIN_IN_09,
	PIN_IN_10,
	PIN_IN_11,
	PIN_IN_12,
	PIN_IN_13,
	PIN_IN_14,
	PIN_IN_15,
	PIN_IN_16,
	PIN_IN_17,
	PIN_IN_18,
	PIN_IN_19,
	PIN_IN_20,
	PIN_IN_21,
	PIN_IN_22,
	PIN_IN_23,
	PIN_IN_24,
	PIN_IN_25,
	PIN_IN_26,
	PIN_IN_27,
	PIN_IN_28,
	PIN_IN_29,
	PIN_IN_30,
	PIN_IN_31,
	PIN_IN_32,
	PIN_IN_33,
	PIN_IN_34,
	PIN_IN_ALL
}PinIO_IN_e;

typedef enum{
	PIN_OUT_VIDEO_PWDN = 0,
	PIN_OUT_VIDEO_RST,
	PIN_OUT_BACKLIGHT,
	PIN_OUT_ALL
}PinIO_OUT_e;

/******************************************************************/



#endif 
