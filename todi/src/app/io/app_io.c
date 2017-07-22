#include "app_io.h"
#include "io_cfg.h"
#include "mid_pin.h"
#include "app_led.h"
#include "mid_filter.h"
#include "mid_led.h"
#include "g_variable.h"
#include "hal_wdg.h"
#include "KeyPortReg.h"
#include "app_system.h"
#include "_fr.h"

/*****************************globale  variable *********************************/
io_type_t		pin_io_in[PIN_IN_ALL] = {0};
io_type_t		pin_io_out[PIN_OUT_ALL] = {0};
pin_filter_t    pin_filter_in[PIN_IN_ALL] = {0};

/***************************** function declare **********************************/
void app_cfg_io_in(void);
/********************************************************************************/

#if (PIN_CORE_POWER == CFG_ON)
io_type_t  VCC_4V2_EN;
io_type_t  MCU_HOLD_ON;
#endif

#if (SIG_BUZ_CTL == CFG_ON)
io_type_t BUZ_CTL_IO;
#endif

#if	   (SIG_BACKLIGHT_EN == CFG_ON)
io_type_t	BL_POWER_CTRL;
io_type_t	BL_GMA_SD; //sch0.4:add this line

#endif

/* airbag */
#if (SIG_IN_MAIN_AIRBAG == CFG_ON)
io_type_t MAIN_AIRBAG_IN;
#endif

#if (SIG_IN_PASSENGER_AIRBAG == CFG_ON)
io_type_t SUB_AIRBAG_IN;
#endif

/* belt */
#if  (SIG_IN_MAIN_BELT == CFG_ON)
io_type_t MAIN_BELT_IN;
#endif

#if  (SIG_IN_PASSENGER_BELT == CFG_ON)
io_type_t SUB_BELT_IN;
#endif

/*safe */
#if  (SIG_IN_BRAKE == CFG_ON)
io_type_t BRAKE_IN;
#endif

#if  (SIG_IN_WASH_LIQUID_LOW == CFG_ON)
io_type_t WASH_LIQUID_IN;
#endif

#if  (SIG_IN_CHARGE_INDICATE == CFG_ON)
io_type_t  CHARGE_IN;
#endif

#if (SIG_IN_BATTERY_CHARGE == CFG_ON)
io_type_t   BATTERY_CHARGE_IN;
#endif

#if (SIG_VIDEO_INPUT == CFG_ON)
io_type_t    VIDEO_SW_IN;
#endif

#if (PIN_IGN_IN == CFG_ON)			/* IGN input */
io_type_t   PIN_IGN;
#endif

#if (SIG_IN_KEY == CFG_ON)
io_type_t  KEY_IN;						/* key input */
#endif


#if (DS1302_CFG == CFG_ON)

io_type_t  IO_RTC_RST;
io_type_t  IO_RTC_IO;
io_type_t  IO_RTC_SCK;
void DS1302_io_config(void)
{
	mid_io_config(&IO_RTC_RST,11,4,IO_GENERAL_OUTPUT,0);
	mid_io_config(&IO_RTC_IO,11,5,IO_GENERAL_OUTPUT,0);
	mid_io_config(&IO_RTC_SCK,11,6,IO_GENERAL_OUTPUT,0);
}

void DS1302_io_deconfig(void)
{
	mid_set_io_dir(&IO_RTC_RST,IO_INPUT);
	mid_set_io_dir(&IO_RTC_IO,IO_INPUT);
	mid_set_io_dir(&IO_RTC_SCK,IO_INPUT);
}
#endif


/*add can chip io config*/
#if  (CAN_CFG == CFG_ON)
io_type_t   io_can_stb;	
io_type_t	io_can_en;

void can_io_config(U8 chip)
{
	switch(chip)
	{
		case TJA1042T:
			mid_io_config(&io_can_stb,9,7,IO_GENERAL_OUTPUT,0);
		break;
		case TJA1043:
			mid_io_config(&io_can_stb,9,4,IO_GENERAL_OUTPUT,OFF);
			mid_io_config(&io_can_en,9,3,IO_GENERAL_OUTPUT,OFF);
		break;
	}
		
}

#endif


#if  (SIG_IN_DOORS  == CFG_ON)
io_type_t DOOR_FL_IN;
io_type_t DOOR_FR_IN;
io_type_t DOOR_RL_IN;
io_type_t DOOR_RR_IN;
io_type_t DOOR_HOOD_IN;
io_type_t DOOR_TRUNK_IN;
void door_config_io_input(void)
{ ;
}
#endif


#if (EEPROM_CFG == ON)

io_type_t  IO_AT24C02_WP;
io_type_t  IO_IIC_SDA;
io_type_t  IO_IIC_SCL;

void EEPROM_ioconfig(void)
{
	/* io cfg */
	mid_io_config(&IO_AT24C02_WP,7,4,IO_GENERAL_OUTPUT,ON);
	mid_io_config(&IO_IIC_SDA,7,6,IO_GENERAL_OUTPUT,ON);
	mid_io_config(&IO_IIC_SCL,7,7,IO_GENERAL_OUTPUT,ON);
}
#endif


#if (A20_UPDATE == CFG_ON)

io_type_t   IO_MCU_CTL_START_UP;
io_type_t	IO_MCU_CTL_STOP_UP;

void app_update_A20_ioconfg(void)
{
	mid_io_config(&IO_MCU_CTL_START_UP,6,7,IO_GENERAL_OUTPUT,OFF);
	mid_io_config(&IO_MCU_CTL_STOP_UP,2,4,IO_INPUT,ON); //sch0.4:OFF->ON
}
#endif

void app_test_update_core_board_100MS(void)
{
	//static U8 time_cnt=0;
	//TEST1
	/*time_cnt++;
	if(time_cnt<300)
	{
		mid_io_config(&IO_MCU_CTL_START_UP,6,7,IO_GENERAL_OUTPUT,OFF);
	}
	else if(time_cnt<600)
	{
		time_cnt=0;
		mid_io_config(&IO_MCU_CTL_START_UP,6,7,IO_GENERAL_OUTPUT,ON);
	}
	else
	{
		mid_io_config(&IO_MCU_CTL_START_UP,6,7,IO_GENERAL_OUTPUT,OFF);
	}*/
	
	//TEST2
	mid_set_io_sts(&IO_MCU_CTL_START_UP,mid_get_io_sts(&IO_MCU_CTL_STOP_UP));
}

void app_cfg_io(void)
{
	/*****************************************************************/
	/*************************************************************************/
	can_io_config(CAN_CHIP);
	app_update_A20_ioconfg();
	EEPROM_ioconfig();
	
	app_cfg_io_in();
	app_cfg_io_out();
	//app_cfg_led_out_io();
}


void app_cfg_led_out_io(void)
{
	
	mid_io_config(&leds_out[LED_LEFTTURNLIGHT],1,6,IO_GENERAL_OUTPUT,1);  //led1
	mid_io_config(&leds_out[LED_REARFOGLAMP],1,5,IO_GENERAL_OUTPUT,1);    //2//2
	mid_io_config(&leds_out[LED_FRONTFOGLAMP],1,4,IO_GENERAL_OUTPUT,1);//3//3
	mid_io_config(&leds_out[LED_HIGHBEAM],1,3,IO_GENERAL_OUTPUT,1);//4//4
	mid_io_config(&leds_out[LED_PARK],1,2,IO_GENERAL_OUTPUT,1);//5
	mid_io_config(&leds_out[LED_LIGHTMAINCTL],1,1,IO_GENERAL_OUTPUT,1);//6
	mid_io_config(&leds_out[LED_POSITION],1,0,IO_GENERAL_OUTPUT,1);//7

	mid_io_config(&leds_out[LED_RIGHTTURNLIGHT],12,2,IO_GENERAL_OUTPUT,VALID_HIGH_LEVEL);//8
	mid_io_config(&leds_out[LED_RESERVELED1],12,1,IO_GENERAL_OUTPUT,VALID_HIGH_LEVEL);//9
	mid_io_config(&leds_out[LED_RESERVELED2],14,4,IO_GENERAL_OUTPUT,VALID_HIGH_LEVEL);//10
	
	mid_io_config(&leds_out[LED_SAFETYBELTSIDE],13,2,IO_GENERAL_OUTPUT,VALID_LOW_LEVEL);//11
																						//12
																						//13
																						//14
}


static void app_init_io_parameter(void)
{
	U8 index = 0;
	for(index =0;index < PIN_IN_ALL;index++)
	{
		pin_filter_in[index].filter_time = 5;
		pin_filter_in[index].valid_value = 0;
	}
	pin_filter_in[PIN_IN_WAKEUP1].valid_value = 1;
	pin_filter_in[PIN_IN_WAKEUP1].filter_time  = 1;
	pin_filter_in[PIN_IN_WAKEUP2].valid_value = 1;
	pin_filter_in[PIN_IN_WAKEUP2].filter_time  = 1;
	pin_filter_in[PIN_IN_WAKEUP3].valid_value = 1;
	pin_filter_in[PIN_IN_WAKEUP3].filter_time  = 1;
	pin_filter_in[PIN_IN_KEY0].filter_time = 1;
	pin_filter_in[PIN_IN_KEY1].filter_time = 1;
	pin_filter_in[PIN_IN_KEY2].filter_time = 1;
	pin_filter_in[PIN_IN_KEY3].filter_time = 1;
}

void app_cfg_io_in(void)
{
	mid_io_config(&pin_io_in[PIN_IN_WAKEUP1],7,5,IO_INPUT,VALID_LOW_LEVEL);   //wake up 1 危险报警
	mid_io_config(&pin_io_in[PIN_IN_WAKEUP2],7,6,IO_INPUT,VALID_LOW_LEVEL);   //wake up 2 acc
	mid_io_config(&pin_io_in[PIN_IN_WAKEUP3],7,7,IO_INPUT,VALID_LOW_LEVEL);   //wake up 3 on
//	mid_io_config(&pin_io_in[PIN_IN_OIL_PRESS],3,3,IO_INPUT,VALID_LOW_LEVEL);
//	mid_io_config(&pin_io_in[PIN_IN_GUARD],3,4,IO_INPUT,VALID_LOW_LEVEL);
//	mid_io_config(&pin_io_in[PIN_IN_CHARGE],3,6,IO_INPUT,VALID_LOW_LEVEL);
//	mid_io_config(&pin_io_in[PIN_IN_ENGINE_GUARD],3,7,IO_INPUT,VALID_LOW_LEVEL);
//	mid_io_config(&pin_io_in[PIN_IN_MAIN_BELT],5,0,IO_INPUT,VALID_LOW_LEVEL);
//	mid_io_config(&pin_io_in[PIN_IN_ALS_ERROR],5,1,IO_INPUT,VALID_LOW_LEVEL);
//	mid_io_config(&pin_io_in[PIN_IN_BRAKE_WARNING],5,3,IO_INPUT,VALID_LOW_LEVEL);
#ifdef NEWBOARD
	mid_io_config(&pin_io_in[PIN_IN_KEY0],6,0,IO_INPUT,VALID_LOW_LEVEL);
	mid_io_config(&pin_io_in[PIN_IN_KEY1],6,1,IO_INPUT,VALID_LOW_LEVEL);
	mid_io_config(&pin_io_in[PIN_IN_KEY2],6,2,IO_INPUT,VALID_LOW_LEVEL);
	mid_io_config(&pin_io_in[PIN_IN_KEY3],6,3,IO_INPUT,VALID_LOW_LEVEL);
#else
	mid_io_config(&pin_io_in[PIN_IN_KEY0],12,0,IO_INPUT,VALID_LOW_LEVEL);
	mid_io_config(&pin_io_in[PIN_IN_KEY1],3,5,IO_INPUT,VALID_LOW_LEVEL);
	mid_io_config(&pin_io_in[PIN_IN_KEY2],7,5,IO_INPUT,VALID_LOW_LEVEL);
	mid_io_config(&pin_io_in[PIN_IN_KEY3],7,6,IO_INPUT,VALID_LOW_LEVEL);
#endif
	mid_io_config(&pin_io_in[PIN_IN_01],2,0,IO_INPUT,VALID_LOW_LEVEL);
	mid_io_config(&pin_io_in[PIN_IN_02],2,1,IO_INPUT,VALID_LOW_LEVEL);
	mid_io_config(&pin_io_in[PIN_IN_03],2,2,IO_INPUT,VALID_LOW_LEVEL);
	mid_io_config(&pin_io_in[PIN_IN_04],2,3,IO_INPUT,VALID_LOW_LEVEL);
	mid_io_config(&pin_io_in[PIN_IN_05],2,5,IO_INPUT,VALID_LOW_LEVEL);
	mid_io_config(&pin_io_in[PIN_IN_06],2,6,IO_INPUT,VALID_LOW_LEVEL);
	mid_io_config(&pin_io_in[PIN_IN_07],2,7,IO_INPUT,VALID_LOW_LEVEL);
	mid_io_config(&pin_io_in[PIN_IN_08],3,0,IO_INPUT,VALID_LOW_LEVEL);
	mid_io_config(&pin_io_in[PIN_IN_09],3,1,IO_INPUT,VALID_LOW_LEVEL);
	mid_io_config(&pin_io_in[PIN_IN_10],3,2,IO_INPUT,VALID_LOW_LEVEL);
	mid_io_config(&pin_io_in[PIN_IN_11],3,3,IO_INPUT,VALID_LOW_LEVEL);
	mid_io_config(&pin_io_in[PIN_IN_12],3,4,IO_INPUT,VALID_LOW_LEVEL);
	mid_io_config(&pin_io_in[PIN_IN_13],3,5,IO_INPUT,VALID_LOW_LEVEL);
	mid_io_config(&pin_io_in[PIN_IN_14],3,6,IO_INPUT,VALID_LOW_LEVEL);
	mid_io_config(&pin_io_in[PIN_IN_15],3,7,IO_INPUT,VALID_LOW_LEVEL);
	mid_io_config(&pin_io_in[PIN_IN_16],15,0,IO_INPUT,VALID_LOW_LEVEL);
	mid_io_config(&pin_io_in[PIN_IN_17],15,1,IO_INPUT,VALID_LOW_LEVEL);
	mid_io_config(&pin_io_in[PIN_IN_18],4,0,IO_INPUT,VALID_LOW_LEVEL);
	mid_io_config(&pin_io_in[PIN_IN_19],4,1,IO_INPUT,VALID_LOW_LEVEL);
	mid_io_config(&pin_io_in[PIN_IN_20],4,3,IO_INPUT,VALID_LOW_LEVEL);
	mid_io_config(&pin_io_in[PIN_IN_21],4,4,IO_INPUT,VALID_LOW_LEVEL);
	mid_io_config(&pin_io_in[PIN_IN_22],4,5,IO_INPUT,VALID_LOW_LEVEL);
	mid_io_config(&pin_io_in[PIN_IN_23],4,6,IO_INPUT,VALID_LOW_LEVEL);
	mid_io_config(&pin_io_in[PIN_IN_24],4,7,IO_INPUT,VALID_LOW_LEVEL);
	mid_io_config(&pin_io_in[PIN_IN_25],5,0,IO_INPUT,VALID_LOW_LEVEL);
	mid_io_config(&pin_io_in[PIN_IN_26],5,1,IO_INPUT,VALID_LOW_LEVEL);
	mid_io_config(&pin_io_in[PIN_IN_27],5,2,IO_INPUT,VALID_LOW_LEVEL);
	mid_io_config(&pin_io_in[PIN_IN_28],5,3,IO_INPUT,VALID_LOW_LEVEL);
	mid_io_config(&pin_io_in[PIN_IN_29],5,4,IO_INPUT,VALID_LOW_LEVEL);
	mid_io_config(&pin_io_in[PIN_IN_30],5,5,IO_INPUT,VALID_LOW_LEVEL);
	mid_io_config(&pin_io_in[PIN_IN_31],5,6,IO_INPUT,VALID_LOW_LEVEL);
	mid_io_config(&pin_io_in[PIN_IN_32],5,7,IO_INPUT,VALID_LOW_LEVEL);
	mid_io_config(&pin_io_in[PIN_IN_33],14,2,IO_INPUT,VALID_LOW_LEVEL);
	mid_io_config(&pin_io_in[PIN_IN_34],14,3,IO_INPUT,VALID_LOW_LEVEL);

//	mid_io_config(&pin_io_in[PIN_IN_REVERSE_SIGNAL],9,0,IO_INPUT,VALID_LOW_LEVEL);
//	mid_io_config(&pin_io_in[PIN_IN_REVERSE_RADAR],9,1,IO_INPUT,VALID_LOW_LEVEL);
//	mid_io_config(&pin_io_in[PIN_IN_VIDEO_MPOUT],5,4,IO_INPUT,VALID_LOW_LEVEL);
//	mid_io_config(&pin_io_in[PIN_IN_VIDEO_INTREQ],5,2,IO_INPUT,VALID_LOW_LEVEL);
//	mid_io_config(&pin_io_in[PIN_IN_ARM_LOAD],12,5,IO_INPUT,VALID_LOW_LEVEL); /* 检测ARM是否进入mfg_tool烧录模式引脚配置 */
//
	
	app_init_io_parameter();
}

void app_cfg_io_out(void)
{
	// 启动时不关闭ARM的电源
	mid_io_config(&VCC_4V2_EN,0,2,IO_GENERAL_OUTPUT,VALID_HIGH_LEVEL);//sch0.4:_LOW_->_HIGH_
	mid_io_config(&BUZ_CTL_IO,1,7,IO_GENERAL_OUTPUT,VALID_HIGH_LEVEL);/* 蜂鸣器控制引脚初始化 */
	// Backlight enable pin
	mid_io_config(&BL_POWER_CTRL,15,3,IO_GENERAL_OUTPUT,VALID_HIGH_LEVEL);
	mid_io_config(&BL_GMA_SD,8,1,IO_GENERAL_OUTPUT,VALID_HIGH_LEVEL); //sch0.4: add this line

	mid_io_config(&MCU_HOLD_ON,6,6,IO_GENERAL_OUTPUT,VALID_LOW_LEVEL);
	
	//video
	//mid_io_config(&pin_io_out[PIN_OUT_VIDEO_PWDN],6,5,IO_GENERAL_OUTPUT,VALID_HIGH_LEVEL);
	//mid_io_config(&pin_io_out[PIN_OUT_VIDEO_RST],6,1,IO_GENERAL_OUTPUT,VALID_LOW_LEVEL);
	
	// backlight PWM pin
	mid_io_config(&pin_io_out[PIN_OUT_BACKLIGHT],8,2,IO_GENERAL_OUTPUT,VALID_LOW_LEVEL);
}

void app_io_task(void)
{
	/* add update io check */
	if(OFF == mid_get_io_sts(&IO_MCU_CTL_STOP_UP) && \
		(ON == IO_MCU_CTL_START_UP.value))//升级完成 //sch0.4:ON->OFF
	{
		mid_set_io_sts(&IO_MCU_CTL_START_UP,OFF);
		g_u8_A20_update_sts = 0;
		app_soft_reset();//复位MCU和CORE	
	}
}

