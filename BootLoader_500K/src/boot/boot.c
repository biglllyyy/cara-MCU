#include <string.h>
#include "_fr.h"
#include "comm_typedef.h"
#include "boot.h"

#include "open.h"


#include "app_uart_cmd.h"
#include "app_can.h"
#include "protocol.h"

#include "mid_pin.h"
#include "mid_flash.h"
#include "mid_clock.h"
#include "mid_string.h"
#include "mid_dbg.h"
#include "mid_crc16.h"
#include "mid_tick.h"
#include "mid_serialport.h"
#include "mid_interrupt.h"
#include "mid_wdg.h"
#include "mid_can.h"


#define BOOT_IO_GROUP_SIZE 3


U64 size = 0;


/* 创建2个串口设备 */
static serialPort_dev_t serialPort_A20 =
{
	tty_Data, 						//串口类型
	{
		"serialport<->CoreBoard",	//串口名称
		tty1,						//串口编号
		tty_baudrate_115200,		//串口波特率
		tty_dataBit_8bit,			//串口数据位
		tty_stopBit_1,				//串口停止位
		tty_NoParity,				//串口奇偶校验
		tty_Close					//串口状态
	}

};
static serialPort_dev_t serialPort_Debug = 
{
	tty_Console,
	{
		"serialport<->PC",
		tty0,
		tty_baudrate_115200,
		tty_dataBit_8bit,
		tty_stopBit_1,
		tty_NoParity,
		tty_Close

	}
};


typedef enum 
{
	IO_IGN = 0,
	VCC_4V2_EN = 1,
	MCU_HOLD_ON = 2
}BOOT_GPIO_GROUP_INDEX; // 用于找出相应的GPIO设备


/* 创建5个GPIO设备 */
static gpio_dev_t gpio_not_open[BOOT_IO_GROUP_SIZE] = 
{

	{
		"IO_IGN",
		6,
		0,
		IO_INPUT,
		VALID_LOW_LEVEL
	},
	{
		"VCC_4V2_EN",
		0,
		2,
		IO_GENERAL_OUTPUT,
		VALID_HIGH_LEVEL
	},
	{
		"MCU_HOLD_ON",
		6,
		6,
		IO_GENERAL_OUTPUT,
		VALID_HIGH_LEVEL
	}
};



/* Mian do function */
void CAN_Boot_Process(void);






/* 升级功能函数 */
static U8 Serial_Port_Upgrade_Condition_Judgment(void);
static void Serial_Port_Upgrade_Init(void);
static U8 Serial_Port_Upgrade(U64 * p_offset, U32 * p_len_update_data);
static U8 Earse_Flash_Firmware(U32 * p_len_update_data);
static U8 Program_New_Firmware(U64 * p_offset);
static U8 Checkout_New_Firmware(U64 * p_offset, U32 * p_len_update_data);

/*
 * Description : CAN boot loader ready routine,be loaded from startup.asm
 * parameter   : None
 * retern value: None
 * Other	   : None
 */
#pragma segment CODE=CODE_CAN_Boot_Ready, attr=CODE


void CAN_Boot_Loader(void)
{
	extern U32 ROM_CODE_CAN_Boot;
	extern U32 RAM_CODE_CAN_Boot;

	U32 size_index = 0;

	size = 0;
	size_index = 0;


	mid_Feed_WatchDog();
	mid_Interrupt_Disable();


#pragma asm
	LDI:32 #SIZEOF(CODE_CAN_Boot), R1
	LDI:32 #_size, R2
	ST R1, @R2
#pragma endasm //copy code from ROM to RAM 
	for (size_index = 0; size_index < size; size_index++)
	{
		* ((char *) (((U32) &RAM_CODE_CAN_Boot) +size_index)) = * ((char *) (((U32) &ROM_CODE_CAN_Boot) +size_index));
		mid_Feed_WatchDog();								/* H/W WDT Clear */
	}

	CAN_Boot_Process();
	RSTCR_SRST = 1;

	if (RSTCR_SRST) //mcu_reset request
	{
		__wait_nop();
	}
}


/*
 * 
 * Description : boot loader process routine, main entry of BootLoader project.
 * Parameter   : None
 * Retern value: None
 * Other	   : None
 * Attention   : need to add CAN update
 */
#pragma segment CODE=CODE_CAN_Boot, attr=CODE
void CAN_Boot_Process(void)
{
	U32 idx;
	U64 offset			= 0;
	U32 len_update_data = 0;
	U8 boot_update_flag = 0;
	static U8 boot_step = 0;
	U8 update_status	= 0;

	U16 demo_read[6];
	U16 boot_flag_read	  = * (U16 *) 0x33E000;
	U16 demo_flag_read	  = * (U16 *) 0x33E004;
	U16 display_flag_read = * (U16 *) 0x33E008;

	#if 0  // just for first flash app into no core board
	U8 i = 0;
	U8 index = 0;
	U8 erase_flag		= 0;

	U16 flash_up_success_flg = 0;
	#endif


	mid_Clock_Init();
	mid_System_Tick_Init();
	mid_Feed_WatchDog();	
	mid_Interrupt_Enable();
	mid_GPIO_Init(Open_GPIO_Init(), OPEN_IO_GROUP_SIZE);
	mid_GPIO_Init(gpio_not_open,	BOOT_IO_GROUP_SIZE);
	mid_Interrupt_Init();
	mid_Serialport_Init(&serialPort_A20);
	mid_Console_Init(&serialPort_Debug);
	app_init_can();
	app_uart_frame_init();


	Console("Saber Bootloader in %s version %d.%d\n",PROJECT ,BL_VER_MAJOR, BL_VER_REV);
	while(1)
	{
		U32 temp;
		app_can_get_task();
		Protocol(&can_info);
		temp = ProgramFlash();
		if (temp != 0)
		{
			Console("\写入错误 错误码:%d",temp); 
		}
		wdg_feed(); 
	}

#if 0   // just for first flash app into no core board

	for (index = WF_START_PAGE; index < WF_END_PAGE; index++) // work flash
	{
		erase_flag = mid_work_flash_sect_erasing(index);

		if (erase_flag == FAIL)
		{
			//uart_update_data.updateInfo.sts = 10;
			break;
		}
		else 
		{
			//uart_update_data.updateInfo.sts = 0;
		}
	}




	flash_up_success_flg = 0x55AA;

	for (i = 0; i < 3; i++)
	{
		if (mid_Work_Flash_Sector_Programing(FLASH_UPDATE_STS_ADDRESS, &flash_up_success_flg, 1) == 0)
		{
			//uart_update_data.updateInfo.sts = 0; /* CRC校验通过，则升级成功 */
			break;
		}
		else 
		{
			//uart_update_data.updateInfo.sts = 1;
		}
	}


#endif

#if 0  //just for test io output
	while(1)
	{
		wdg_feed();
		/*enable VCC_4V2_EN*/
#if 1
		set_single_io_dir(0,2,IO_GENERAL_OUTPUT);
		set_single_io_sts(0,2,1);

#endif

		/*enable LED1*/
#if 1
		set_single_io_dir(1,0,IO_GENERAL_OUTPUT);
		set_single_io_sts(1,0,1);

#endif

		/*enable LED2*/
#if 1
		set_single_io_dir(1,1,IO_GENERAL_OUTPUT);
		set_single_io_sts(1,1,1);

#endif

		/*enable LED3*/
#if 1
		set_single_io_dir(1,2,IO_GENERAL_OUTPUT);
		set_single_io_sts(1,2,1);

#endif

		/*enable LED4*/
#if 1
		set_single_io_dir(1,3,IO_GENERAL_OUTPUT);
		set_single_io_sts(1,3,1);

#endif

		/*enable LED5*/
#if 1
		set_single_io_dir(1,4,IO_GENERAL_OUTPUT);
		set_single_io_sts(1,4,1);

#endif

		/*enable LED6*/
#if 1
		set_single_io_dir(1,5,IO_GENERAL_OUTPUT);
		set_single_io_sts(1,5,1);

#endif

		/*enable LED7*/
#if 1
		set_single_io_dir(1,6,IO_GENERAL_OUTPUT);
		set_single_io_sts(1,6,1);

#endif

		/*enable LED8*/
#if 1
		set_single_io_dir(1,7,IO_GENERAL_OUTPUT);
		set_single_io_sts(1,7,1);

#endif
	}
#endif

#if 0
	while (1)
	{
		wdg_feed();

		switch (boot_step)
		{
			case 0: /*判断*/
					boot_update_flag = 0; //Serial_Port_Upgrade_Condition_Judgment(); //Determine whether to upgrade

					if (boot_update_flag)
					{
						Console("Enter upgrade mode.\n");
						boot_step = 1;
					}
					else 
					{
						boot_step = 4;
					}
					break;

			case 1: /*初始化*/
					Serial_Port_Upgrade_Init(); /* 初始化串口升级 */
					Console("Updating...\n");
					boot_step = 2;
					break;

			case 2: /*串口升级*/
					update_status = Serial_Port_Upgrade(&offset, &len_update_data);

					if (4 == update_status)
					{
						Console("Not a valid file for update,going to rebot.\n");
						boot_step = 3;		/*没有检测到升级文件，或者升级完成，进Firmware*/
					}
					else if (6 == update_status)
					{
						Console("Upgrade complete,going to rebot.\n");
						boot_step = 3;
					}
					else if (5 == update_status)
					{
						Console("Upgrade Failed,going to rebot.\n");

						boot_step = 3;		/*UI 升级失败，清标志位并重启*/
						/*清除标志0x55AA*/
						mid_work_flash_sect_erasing(WF_START_PAGE);
					}
					break;

			case 3: /*??*/
					boot_flag_read = * (U16 *) 0x33E000;
					demo_read[0] = boot_flag_read; /* 读取启动标志，工程模式，演示模式计数器 */
					demo_read[1] = 0xFFFF;
					demo_read[2] = demo_flag_read;
					demo_read[3] = 0xFFFF;
					demo_read[4] = display_flag_read;
					demo_read[5] = 0xFFFF;
					mid_data_flash_sect_programming(0x33E000, demo_read, 0, sizeof(demo_read) / sizeof(U16)); /* 写入启动 */
					hal_soft_reset();
					break;

			case 4: /*进入FireWare*/
					Console("to app.\n");
#pragma asm
	LDI:32 #080024H, R0;  //jmp to address 80024H to run
	JMP @R0;
#pragma endasm
					break;

				default:
					break;
		}
	}
#else

#endif
}




/*
 * Description : Determine whether to upgrade.
 * Parameter   : None
 * Retern value: On update,return 1.
 *				 On no update return 0.
 * Other	   : None
 * Attention   : None
 */
static U8 Serial_Port_Upgrade_Condition_Judgment(void)
{
	return Open_Upgrade_Condition_Judgment();
}



static void Serial_Port_Upgrade_TimeDelay(unsigned int time_ms)
{
	U32 t1; 			
	t1 = mid_System_Tick_Now();
	while ((mid_System_Tick_Passed(t1) < time_ms))
		wdg_feed(); 
	return;	
}

/*
 * Description : Initialize all upgrade conditions.
 * Parameter   : None
 * Retern value: None
 * Other	   : None
 * Attention   : Must be power-up to core board first.
 */
static void Serial_Port_Upgrade_Init(void)
{
	mid_set_io_sts(&gpio_not_open[VCC_4V2_EN], ON);				//core-board power ON 


	Open_Upgrade_Init(Serial_Port_Upgrade_TimeDelay);
}


/*
 * Description : Major upgrade function.
 * Parameter   : *p_offset,be used in Program_New_Firmware() and  Checkout_New_Firmware(),
 *				 each times need be change in that function,
 *				 for Positioning the data frames.
 *				 *p_len_update_data,be used in Program_New_Firmware() and Checkout_New_Firmware(),
 *				 get the MCU.bin's length.
 * Retern value: each funtion's runtime result.
 * Other	   : None
 * Attention   : None
 */
static U8 Serial_Port_Upgrade(U64 * p_offset, U32 * p_len_update_data)
{
	U8 Update_status	= 0;
	static U8 flag_updating = 0;

	if(flag_updating == 1)
	{
		if(Open_Upgrade_reset(flag_updating) == 1)
			Update_status = 6;
	}
	else
	{
		if(Open_Upgrade_reset(flag_updating) == 0)
			hal_soft_reset();
	}
	

	boot_app_frame_get_task();						//recived the data form core-board 

	if (get_new_uart_frame)
	{
		get_new_uart_frame	= 0;
		flag_updating = 1;

		uart_update_data.updateInfo.cmd = mcu_rec_update.settingsInfo.cmd;
		uart_update_data.updateInfo.No = mcu_rec_update.settingsInfo.No;

		switch (uart_update_data.updateInfo.cmd)
		{
			case 0:
					Earse_Flash_Firmware(p_len_update_data); //erase FLASH
					Update_status = 1;
					break;

			case 1:
					Program_New_Firmware(p_offset); // programming
					Update_status = 2;
					break;

			case 2:
					Checkout_New_Firmware(p_offset, p_len_update_data); //checkout
					break;

			case 3:
					//uart_update_data.updateInfo.sts=4;	//no update file
					Update_status = 4;
					break;

			case 4:
					uart_update_data.updateInfo.sts = 0; //UI update failed
					Update_status = 5;
					break;

			case 5:
					uart_update_data.updateInfo.sts = 0; //OK
					Update_status = 6;
					break;

			case 255:
					uart_update_data.updateInfo.sts = 0xff; //unvalid
					Update_status = 7;
					break;

			default:
					uart_update_data.updateInfo.sts = 0xff;
					Update_status = 8;
					break;
		}

		app_mcu_core_update();
	}
	return Update_status;
}


/*
 * Description : Erase firmware space at 0x80000-0xA0004 and work flash
 * 擦除firmware扇区和workflash扇区
 * Parameter   : *p_len_update_data from Serial_Port_Upgrade(),get length of MCU.bin
 * Retern value: On seccess,return 0.
 *				 On failed,return 10.
 * Other	   : None
 * Attention   : None
 */
static U8 Earse_Flash_Firmware(U32 * p_len_update_data)
{
	U32 len_update_data = 0;
	U32 index			= 0;
	U8 erase_flag		= 0;
	U8 app_flash_result = 0;

	//when earse time,get the length of MCU.bin
 	len_update_data = (mcu_rec_update.settingsInfo.date[3] << 24) | (mcu_rec_update.settingsInfo.date[2] << 16) | \
 	(mcu_rec_update.settingsInfo.date[1] << 8) | mcu_rec_update.settingsInfo.date[0];

	*p_len_update_data	= len_update_data;

	for (index = PROG_START_PAGE; index < PROG_END_PAGE; index++) //data flash,522,SA2--SA11
	{
		app_flash_result	= mid_flash_sect_erasing(index);

		if (app_flash_result == FAIL)
		{
			uart_update_data.updateInfo.sts = 10;
			break;
		}
		else 
		{
			uart_update_data.updateInfo.sts = 0;
		}
	}

	for (index = WF_START_PAGE; index < WF_END_PAGE; index++) // work flash
	{
		erase_flag = mid_work_flash_sect_erasing(index);

		if (erase_flag == FAIL)
		{
			uart_update_data.updateInfo.sts = 10;
			break;
		}
		else 
		{
			uart_update_data.updateInfo.sts = 0;
		}
	}

	return uart_update_data.updateInfo.sts;
}


/*
 * Description : Program Firmware space at 0x80000-0xA0004.
 * Parameter   : *p_offset,each times need be change in this function,
 *				 for Positioning the data frames.
 * Retern value: On seccess,return 0.
				 On failed,return 1,write failed.
				 On failed,return 3,the code number is not continuous.
 * Other	   : None
 * Attention   : None
 */
static U8 Program_New_Firmware(U64 * p_offset)
{
	U8 i				= 0;
	static U16 last_No	= 0;

	if (uart_update_data.updateInfo.No == last_No + 1) // The data frame number is continuous
	{
		if ((FLASH_START_ADDRESS + (*p_offset) + (sizeof(mcu_rec_update.settingsInfo.date))) > (0x0BFFFF + 1)) // free FLASH space
		{
			uart_update_data.updateInfo.sts = 9;
			return uart_update_data.updateInfo.sts;
		}

		for (i = 0; i < 3; i++) // writethrough 3 times
		{
			if (0 == mid_Flash_Sector_Programing(FLASH_START_ADDRESS + (*p_offset), 
				(unsigned short *) (mcu_rec_update.settingsInfo.date), 
				(sizeof(mcu_rec_update.settingsInfo.date) / sizeof(unsigned short))))
			{
				(*p_offset) 		+= (sizeof(mcu_rec_update.settingsInfo.date)); // point to the next address
				last_No++;
				uart_update_data.updateInfo.sts = 0; //write seccessfully
				break;
			}
			else 
			{
				uart_update_data.updateInfo.sts = 1; //write failed
			}
		}
	}
	else if (uart_update_data.updateInfo.No == last_No) //the last data frame
	{
		uart_update_data.updateInfo.sts = 0;		//write seccessfully
		return uart_update_data.updateInfo.sts;
	}
	else 
	{
		uart_update_data.updateInfo.sts = 3;		// the code number is not continuous
	}

	return uart_update_data.updateInfo.sts;
}


/*
 * Description : checkout Firmware space
 * Parameter   : *p_offset and *p_len_update_data, be compared in this funtion.
 * Retern value: the failed reason consult the file named chinese(升级错误勘误).ts
 * Other	   : None
 * Attention   : None
 */
static U8 Checkout_New_Firmware(U64 * p_offset, U32 * p_len_update_data)
{
	U16 flash_up_success_flg = 0;
	U8 i				= 0;

	if ((*p_offset) == (*p_len_update_data))
	{
		//dbg_string("MCU_CRC= %d \n",api_cal_crc16((unsigned char *)FLASH_START_ADDRESS,(*p_offset)));
		//dbg_string("Qt_CRC= %d \n",(mcu_rec_update.settingsInfo.date[0]|(U16)(mcu_rec_update.settingsInfo.date[1]<<8)));
		if (api_cal_crc16((unsigned char *) FLASH_START_ADDRESS, (*p_offset)) ==
			 (mcu_rec_update.settingsInfo.date[0] | (U16) (mcu_rec_update.settingsInfo.date[1] << 8)))
		{
			flash_up_success_flg = 0x55AA;

			for (i = 0; i < 3; i++)
			{
				if (mid_Work_Flash_Sector_Programing(FLASH_UPDATE_STS_ADDRESS, &flash_up_success_flg, 1) == 0)
				{
					uart_update_data.updateInfo.sts = 0; /* CRC校验通过，则升级成功 */
					break;
				}
				else 
				{
					uart_update_data.updateInfo.sts = 1;
				}
			}
		}
		else 
		{
			uart_update_data.updateInfo.sts = 5;
		}
	}
	else 
	{
		uart_update_data.updateInfo.sts = 11;
	}

	return uart_update_data.updateInfo.sts;
}









