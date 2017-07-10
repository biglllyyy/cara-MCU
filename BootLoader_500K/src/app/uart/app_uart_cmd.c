#include <string.h>
#include "uart_cfg.h"
#include "uart_queue.h"

#include "app_uart_cmd.h"

#include "mid_crc16.h"
#include "mid_string.h"

#include "hal_uart.h"
#include "hal_wdg.h"

GeneralFrame	uart_general_data;
SpecialFrame	uart_special_data;
UpdateFrame   uart_update_data;

GeneralFrame  uart_general_data_test;//for test 2015/7/7 13:51:32
SpecialFrame	uart_special_data_test;//for test 2015/7/7 13:51:37
UpdateFrame   uart_update_data_test;

SettingsGeneralFrame mcu_rec_general;
SettingsSpecialFrame mcu_rec_special;
SettingsUpdateFrame mcu_rec_update;

U8		rec_queue_buf[SETTINGS_FRAME_LEN_MAX]= {0};	//uesd to process core to mcu data buf
UartQueue mcu_frame_rec = {0};
uint8_t UART_MCU_CORE_DATA_LENGTH_G=0,UART_MCU_CORE_DATA_LENGTH_S=0,CORE_MCU_DATA_BUF_LENGTH=0;


/* globle varialbe */
static void app_uart_rec_process(UartQueue *p);


//该函数就上电的时候初始化一次。
void app_uart_frame_init(void)
{
	hal_uart_init();//初始化队列
	uart_queue_init(&mcu_frame_rec,rec_queue_buf,sizeof(rec_queue_buf)/sizeof(rec_queue_buf[0]));
	uart_clear_queue(&mcu_frame_rec);
	
	memset(&uart_update_data,0,sizeof(uart_update_data));
	memset(&mcu_rec_update,0,sizeof(mcu_rec_update));
}



void app_mcu_core_update(void)
{
	U8 uart_data[UPDATE_FRAME_LEN]={0};
	U16 crc= 0;
	U32 magic = 0;
	uart_data[0]=FRAME_HEAD;
	uart_data[1]=(uint8_t)UPDATE_FRAME_LEN-9;	//len
	magic = MAGIC_UPDATE_UP;
	uart_data[4]= magic & 0xff;
	uart_data[5]= (magic & 0xff00)>>8;
	uart_data[6]= (magic & 0xff0000)>>16;
	uart_data[7]= (magic & 0xff000000)>>24;
	
	uart_data[8 ]=uart_update_data.updateInfo.cmd;
	uart_data[9 ]=uart_update_data.updateInfo.No;
	uart_data[10]=uart_update_data.updateInfo.No>>8;
	uart_data[11]=uart_update_data.updateInfo.sts;
	

	
	uart_data[12]=  FRAME_REAR;
	/* crc */
	crc = api_cal_crc16((uint8_t*)&uart_data[4],UPDATE_FRAME_LEN-5);//crc
	uart_data[2]= crc &0xff;
	uart_data[3] = (crc&0xff00)>>8;

	sent_data(UART_A20_CHN,(uint8_t *)uart_data,sizeof(uart_data));	
}


U16 timecnt_general=0;//for test 2015/7/6 17:54:57
U16 timecnt_special=0;//for test 2015/7/6 17:55:02
U16 timecnt_update=0;//for test 2015/7/6 17:54:57
U8 get_new_uart_frame=0;

void app_uart_rec_process(UartQueue *p)
{
	uint32_t  magic;
	uint16_t  check_sum;
	const    uint16_t  buf_len = SETTINGS_FRAME_LEN_MAX;
	uint16_t   i =0;
	uint8_t   data_buf[SETTINGS_FRAME_LEN_MAX] = {0};
	uint16_t   data_len;					//record the segment data len
	const U8  len_offset = 1,crc_offset = 2+1, magic_offset = 4+1,data_offset =  8+1;
	
	do 
	{
		//data move,if not move,not convenience for crc check
		for(i=0;i<buf_len;i++)
		{
			data_buf[i] = p->queue[(p->front+1+i)%(buf_len)];
		}
		data_len  = data_buf[len_offset]|data_buf[len_offset+1]<<8;
		check_sum = data_buf[crc_offset] | ((U16)data_buf[crc_offset+1]<<8);
		magic     = data_buf[magic_offset]|
			(U32)data_buf[magic_offset+1]<<8|
			(U32)data_buf[magic_offset+2]<<16|
			(U32)data_buf[magic_offset+3]<<24;

		if(magic==MAGIC_GENERAL_DOWN)
		{
			if(data_len !=  SETTINGS_GENERAL_FRAME_LEN-9 )
			{
				break;  //len err
			}
			if(check_sum != api_cal_crc16(&data_buf[magic_offset],SETTINGS_GENERAL_FRAME_LEN-5))
			{
				break;	//check sum err
			}
			timecnt_general++;//for test 2015/7/6 17:54:41
			uart_clear_queue(&mcu_frame_rec);			// clear queue      
		}
		else if(magic==MAGIC_SPECIAL_DOWN)
		{
			
			if(data_len !=  SETTINGS_SPECIAL_FRAME_LEN-9)
			{
				break;  //len err
			}
			if(check_sum != api_cal_crc16(&data_buf[magic_offset],SETTINGS_SPECIAL_FRAME_LEN-5))
			{
				break;	//check sum err
			}
			timecnt_special++;//for test 2015/7/6 17:54:47
			uart_clear_queue(&mcu_frame_rec);			// clear queue      
		}
		else if(magic==MAGIC_UPDATE_DOWN)
		{
			if(data_len !=  SETTINGS_UPDATE_FRAME_LEN-10)
			{
				break;  //len err
			}
			if(check_sum != api_cal_crc16(&data_buf[magic_offset],SETTINGS_UPDATE_FRAME_LEN-6))
			{
				check_sum=api_cal_crc16(&data_buf[magic_offset],SETTINGS_UPDATE_FRAME_LEN-6);
				break;	//check sum err
			}
			timecnt_update++;
			uart_clear_queue(&mcu_frame_rec);// clear queue 
		}
		else
		{
			break;//magic error
		}
		
		if(magic==MAGIC_GENERAL_DOWN)
		{
			mcu_rec_general.settingsInfo .first.val   =	data_buf[data_offset];
			mcu_rec_general.settingsInfo .warningSoundAndInterfaceSync=data_buf[data_offset+1];
			mcu_rec_general.settingsInfo .SoundAndInterfaceSync=data_buf[data_offset+2];
			mcu_rec_general.settingsInfo .Reserved =data_buf[data_offset+3];
			mcu_rec_general.settingsInfo .Time.u8Time[3]  = data_buf[data_offset+4]; 
			mcu_rec_general.settingsInfo .Time.u8Time[2]  = data_buf[data_offset+5]; 
			mcu_rec_general.settingsInfo .Time.u8Time[1]  = data_buf[data_offset+6]; 
			mcu_rec_general.settingsInfo .Time.u8Time[0]  = data_buf[data_offset+7]; 
			
			//get_new_uart_frame=1;
		}
		else if(magic==MAGIC_SPECIAL_DOWN)
		{
			mcu_rec_special.settingsInfo.first.val=data_buf[data_offset];
			mcu_rec_special.settingsInfo.Backlight=data_buf[data_offset+1];
			mcu_rec_special.settingsInfo.TotalTrip.u8TotalTrip[3]=data_buf[data_offset+2];
			mcu_rec_special.settingsInfo.TotalTrip.u8TotalTrip[2]=data_buf[data_offset+3];
			mcu_rec_special.settingsInfo.TotalTrip.u8TotalTrip[1]=data_buf[data_offset+4];
			mcu_rec_special.settingsInfo.TotalTrip.u8TotalTrip[0]=data_buf[data_offset+5];
			mcu_rec_special.settingsInfo.MainDisp.val=data_buf[data_offset+6];
			
			//get_new_uart_frame=1;
		}
		else if(magic==MAGIC_UPDATE_DOWN)
		{
			mcu_rec_update.settingsInfo.cmd=data_buf[data_offset];
			mcu_rec_update.settingsInfo.No=data_buf[data_offset+1];
			mcu_rec_update.settingsInfo.No+=(uint16_t)data_buf[data_offset+2]<<8;
			
			for(i=0;i<sizeof(mcu_rec_update.settingsInfo.date);i++)
			{
				mcu_rec_update.settingsInfo.date[i]=data_buf[data_offset+3+i];
			}
			
			if(get_new_uart_frame==1)
			{
				break;
			}
			else
			{
				get_new_uart_frame=1;
			}
		}
		
	} while (0);
}

void boot_app_frame_get_task(void)
{
	
	uint16_t i = BUF_REC_SIZE;
	uint8_t ch;

	while(i--)
	{
		if(uart_queue_not_empty(&uart_recv_queue))				// 如果串口有数据
		{
			ch = (uint8_t)uart_de_queue(&uart_recv_queue);		// 出队列
		}
		else
		{
			break;
		}
		if(uart_en_queue(&mcu_frame_rec,ch))			   // 入队列失败，说明当前队列已满
		{
				uart_de_queue(&mcu_frame_rec);				// out queue
				uart_en_queue(&mcu_frame_rec,ch);  
				
				if((mcu_frame_rec.queue[mcu_frame_rec.front+1] == FRAME_HEAD) 
					&&(mcu_frame_rec.queue[mcu_frame_rec.rear] == FRAME_REAR))
				{			                                                           
					app_uart_rec_process(&mcu_frame_rec);		// data parse     
				}                                                           
		}
		else
		{
			if((mcu_frame_rec.queue[mcu_frame_rec.front+1] == FRAME_HEAD)
				&&(mcu_frame_rec.queue[mcu_frame_rec.rear] == FRAME_REAR))
			{
				app_uart_rec_process(&mcu_frame_rec);		// data parse
			}
		}
	}
}



