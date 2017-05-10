/* 开放可修改文件,请遵循函数格式修改,禁止hal层 */
/* 如需添加bootloader相关mid及hal层，请编写完成验证后，提交06-software仓库 */

#include "boot.h" 			/* 用于获取FLASH_UPDATE_STS_ADDRESS */
#include "Comm_typedef.h" 	/* 用于调用变量类型 */
#include "open.h"	 		/* 用于声明函数信息 */
#include "mid_lcd.h"		/* 用于设置对LCD驱动的PWM频率 */
#include "mid_dbg.h" 		/* 用于输出打印信息 */


/* 设置全局私有,必须添加static关键字 */

/* GPIO组设备的index */
typedef enum 
{
	Key0 = 0,
	Key1 = 1,
	Key2 = 2,
	BL_POWER_CTRL = 3,
	BL_GMA_SD = 4,
	IO_MCU_CTL_START_UP = 5,
	IO_MCU_CTL_STOP_UP = 6
}OPEN_GPIO_GROUP_INDEX; // 用于找出相应的GPIO设备

/* 创建GPIO组设备，OPEN_IO_GROUP_SIZE 必须与open_io_group的Size相同，
	不同会导致崩溃或初始化不完全 */
static gpio_dev_t open_io_group[OPEN_IO_GROUP_SIZE] = 
{
	{
		"Key0",
		12,
		0,
		IO_INPUT,
		VALID_LOW_LEVEL
	},
	{
		"Key1",
		3,
		5,
		IO_INPUT,
		VALID_LOW_LEVEL
	},
	{
		"Key2",
		7,
		5,
		IO_INPUT,
		VALID_LOW_LEVEL
	},
	{
		"BL_POWER_CTRL",
		15,
		3,
		IO_GENERAL_OUTPUT,
		VALID_HIGH_LEVEL
	},
	{
		"BL_GMA_SD",
		8,
		1,
		IO_GENERAL_OUTPUT,
		VALID_HIGH_LEVEL
	},
	{
		"IO_MCU_CTL_START_UP", 	//IO名称
		6,					  	//IO Port 
		7,						//IO Bit
		IO_GENERAL_OUTPUT,		//IO类型
		OFF						//IO初始化状态
	},
	{
		"IO_MCU_CTL_STOP_UP",
		2,
		4,
		IO_INPUT,
		OFF
	}
};




#pragma segment CODE=CODE_CAN_Boot, attr=CODE /* 将以下代码放到RAM区，防止放在ROM区被擦除 */


/* 	这里配置进行升级条件准备的初始化，其中
 *	VCC_4V2引脚,IGN 引脚，MCU_HOLD_ON引脚，
 *	由Bootloader进行初始化，其余由open_io_group指定，
 *	Open_GPIO_Init 返回Open_GPIO_Init的地址
 */ 
gpio_dev_t * Open_GPIO_Init(void)
{	//比如Todi这里进行按键或背光的初始化,返回GPIO设备组的地址
	return open_io_group;
}



/* 判断是否进入升级
 * 返回值: 进入升级返回   1.
 *		   不进入升级返回 0.
 */
unsigned char Open_Upgrade_Condition_Judgment(void)
{
	if(0x55AA != *(unsigned short*)FLASH_UPDATE_STS_ADDRESS)
	{
		Console("Not a valid firmware, force to upgrade\n");
		return 1;
	}				
	if(mid_get_io_sts(&open_io_group[Key0]) == OFF)
	{
		Console("Upgrade key pressed\n");
		return 1;
	}	
	return 0;
}


/* 	函数功能，获取升级IO标志，返回升级状态
 * 	参数upgrade_flag，是否升级进行中，1为处于升级状态，0是未处于升级状态
 * 	返回值 要进入update_states 6(正确升级或失败)并重启返回1，升级未开始就需要重启返回0
 *	未知状态返回2
 */ 
unsigned char Open_Upgrade_reset(unsigned char upgrade_flag)
{
	unsigned char result = 2; //此处默认状态必须为 不为0或1的数
		// 升级完成,CORE PIN置高 
	if(upgrade_flag == 1)
	{
		if (ON == mid_get_io_sts(&open_io_group[IO_MCU_CTL_STOP_UP]) && (ON == open_io_group[IO_MCU_CTL_STOP_UP].value))
		{
			mid_set_io_sts(&open_io_group[IO_MCU_CTL_START_UP], OFF);
			result = 1;
		}
	}
	else
	{
		if (ON == mid_get_io_sts(&open_io_group[IO_MCU_CTL_STOP_UP]))
			result = 0;
	}
	return result;
}


/* 进行升级前的初始化 
   入口参数是设置延时的函数
 */
void Open_Upgrade_Init(void (*timeDelay)(unsigned int))
{
	/* 此处进行打开背光延时前的操作 */
	mid_set_io_sts(&open_io_group[IO_MCU_CTL_START_UP], ON);
	mid_set_io_sts(&open_io_group[BL_GMA_SD], ON); 			

	/* 此处设置打开背光的延时 */
	timeDelay(LIGHT_LCD_DELAY_TIME);
				
	/* 此处进行打开背光延时后的操作 */
	mid_set_io_sts(&open_io_group[BL_POWER_CTRL], ON); 

	/* 此处设置背光的PWM频率 */
	mid_set_lcd_frq(LCD_BL_PWM_CHN,LCD_BACKLIGHT_FRQ,80 * 0xff / 100);
}





