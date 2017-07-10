#include <stdlib.h>
#include "_fr.h"
#include "mid_scheduler.h"
#include "app_system.h"
#include "hal_wdg.h"
#include "hal_uart.h"
#include "mid_dbg.h"
#include "mid_trip.h"
#include "hal_reload_timer.h"
#include "hal_flash.h"
#include "app_data_store.h"
#include "ft_version.h"
#include "FLASH.h"


#define OFFSET_DEMO_CNT    4
#define OFFSET_TEST_CNT    8


extern void icr_manage(void);

static void print_tick(void)
{
	static U32 tick=0;
	U32 t;
	
	t = hal_timer_get_tick();
	if (t - tick >= 60000) //1min
	{
		tick = t;
		//dbg_string("tick:%d min\n", tick/60000);
	}
}

/* 等待按键，返回键值 */
static char read_key(void)
{
	U8 key=0;
	while (uart_read(UART_DEBUG_CHN, &key) != 0)
	{
		wdg_feed();
		print_tick();
	}
	
	return key;
}

/* 等待输入数字， ESC, CTRL-c, Enter退出 
   Enter: 返回0 
   CTRL-c, ESC: 返回-1
*/    
static int read_digit_string(char *str, int max_len)
{
	char key;
	int cnt = 0;
	
	while(1)
	{
		key = read_key();
		if ((key == 3) || (key == 27)) return -1;	/* CTRL-c, ESC */
		else if ((key >= '0') && (key <= '9'))
		{
			if (cnt >= max_len) break;
			dbg_string("%c", key);
			str[cnt] = key;
			cnt++;
		}
		else if (key == 0x0d)	/* ENTER */
		{
			dbg_string("\n");
			return 0;
		}
	}
	return -1;
}


/* 显示FLASH中的信息 */
static void show_FLASH_info(void)
{
	U32 temp_odo;
	U8  flag;
	U8  car_id;
	U16 demo_cnt;
	U16 test_cnt;
	
	/* 总里程 */
	if(read_total_trip(&temp_odo) != TRUE)
	{
		dbg_string("Not valid total trip, default 0 \n");
		temp_odo = 0;
	}				
	dbg_string("Total trip : %d\n", temp_odo);
	
	/* 激活标志 */
	if(read_activation_flag(&flag) != TRUE)
	{
		dbg_string("Not valid activation flag, default 0 \n");
		flag = 0;
	}				
	dbg_string("Activation : %d\n", flag);
	
	/* 车型代码 */
	if(read_car_num(&car_id) != TRUE)
	{
		dbg_string("Not valid Car NO. ID, default 0 \n");
		car_id = 0;
	}	
	dbg_string("Car number : %d\n", car_id);

	demo_cnt = readFLASH_data(WF_PG_8, OFFSET_DEMO_CNT);
	if(demo_cnt == 0xffff) demo_cnt = 0;
	dbg_string("Demo count : %d\n", demo_cnt);

	test_cnt = readFLASH_data(WF_PG_8, OFFSET_TEST_CNT);
	if(test_cnt == 0xffff) test_cnt = 0;
	dbg_string("Test count : %d\n", test_cnt);

}


static void print_usage(void)
{
	dbg_string("\n*************************\n");
	dbg_string("1...Show FLASH data \n");
	dbg_string("2...Set  Total trip  \n");
	dbg_string("3...Set  Active flag \n");
	dbg_string("4...Set  Car number  \n");
	dbg_string("5...Set  Demo count  \n");
	dbg_string("6...Set  Test count  \n");	
	dbg_string("7...Clear Data store\n");	
	dbg_string("8...Clear Demo&Test count \n");
//	dbg_string("a...Show ALL FLASH\n");	
	dbg_string("*************************\n");
	
}

/* 将所输入的总里程写入FLASH */
static void set_total_trip(void)
{
	char str[8]={0};
	U32 trip;
	
	dbg_string("Pls input: Odo(<999999), ESC or CTRL-c to exit\n");
	if (read_digit_string(str,8) != 0) return;
	
	trip = atoi(str);
	write_total_trip(trip);
	mid_flash_sync();
		
	dbg_string("Done...\n");
}

/* 将所输入的车型写入EEPROM */
static void set_car_number_id(void)
{
	char str[8]={0};
	U8 car_id;
	
	dbg_string("Pls input: Car number, ESC or CTRL-c to exit\n");
	if (read_digit_string(str,2) != 0) return;
	
	car_id = atoi(str);
	write_car_num(car_id);
	mid_flash_sync();
	
	dbg_string("Done...\n");
}

/* 设置激活标志 */ 
static void set_activation_flag(void)
{
	char str[8]={0};
	int flag;
	
	dbg_string("Pls input: Activation flag(0 or 1), ESC or CTRL-c to exit\n");
	if (read_digit_string(str,1) != 0) return;
	
	flag = atoi(str);
	if ((flag != 0) && (flag != 1))
	{
		dbg_string("Buddy, activation MUST be 0 or 1\n");
		return;
	}
	write_activation_flag(flag);
	mid_flash_sync();
	
	dbg_string("Done...\n");
} 

static void set_Demo_cnt(void){
	char str[8]={0};
	U32 demo_cnt;
	
	dbg_string("Pls input: demo_cnt(<65534), ESC or CTRL-c to exit\n");
	if (read_digit_string(str,8) != 0) return;
	
	demo_cnt = atoi(str);
	if(demo_cnt > 65534) 
		demo_cnt = 65534;
	writeFLASH_data(WF_PG_8, demo_cnt, OFFSET_DEMO_CNT);
		
	dbg_string("Done...\n");
}

static void set_Test_cnt(void){
	char str[8]={0};
	U32 test_cnt;
	
	dbg_string("Pls input: test_cnt(<65534), ESC or CTRL-c to exit\n");
	if (read_digit_string(str,8) != 0) return;
	
	test_cnt = atoi(str);
	if(test_cnt > 65535) 
		test_cnt = 65534;
	
	writeFLASH_data(WF_PG_8, test_cnt, OFFSET_TEST_CNT);
		
	dbg_string("Done...\n");
}

static void clear_Demo_Test_cnt()
{
	save_FW_flag();
	dbg_string("Clear Done...\n");
}



/*****************************************************************************/
/*  Main application to control the program flow                             */
/*****************************************************************************/

int main(void)
{
	char key;
	int  i; 
	
    wdg_feed();
    __set_il(0x1F); /* ILM = 31 */
    /* Enable interrupts */
    __EI();
    wdg_feed();
    app_sys_init();
    icr_manage();

	dbg_string("AutoIO FLASH diagnostics program: %d.%d.%d\n",FT_VER_MAJOR, FT_VER_MINOR, FT_VER_REV);
	dbg_string("Build date: %s, %s\n", __DATE__, __TIME__);
	
	//print_gpio();
wait_key: 
	print_usage();
	key = read_key();
	switch(key)
	{
		case '1':	/* 显示当前FLASH中所有数据 */
			show_FLASH_info();
		break;
		
		case '2':	/* 设置总里程 */
			set_total_trip();
		break;
		
		case '3':	/* 设置激活标志位 */
			set_activation_flag();
		break;
		
		case '4':	/* 设置车型 */
			set_car_number_id();
		break;
		
		case '5':	/* 设置Demo模式计数器 */
			set_Demo_cnt();
		break;
		
		case '6':	/* 设置Project模式计数器 */
			set_Test_cnt();
		break;
		
		case '7':	/* 清除FLASH中的数据 目前只清除前2页 */	
			
			for(i=1; i<3; i++){
				eraseFLASH_PG(i);
			}	
			erase_mData();
		break;

		case '8':  /* 清除 demo和Pro模式计数器*/
			clear_Demo_Test_cnt();
		break;
		
		case 'a':	/*读取WorkFlash中所有的数据 */
		//	for(i=1; i<9; i++){
		//		showFLASH_PG(i);
		//	}			
		//break;
		
		default:
			dbg_string("Wrong key\n");
	}
	goto wait_key;
}


/* dummy Int routine */
__interrupt void dummy(void)
{
	__wait_nop();

}
