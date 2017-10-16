#include <stdlib.h>
#include <string.h>
#include "FLASH.h"
#include "mb91520.h"
#include "mid_dbg.h"
#include "hal_uart.h"
#include "hal_reload_timer.h"
#include "hal_wdg.h"
#include "mid_string.h"
#include "hal_flash.h"
#include "mid_flash.h"

#define WF_BUF_SIZE_BYTE 8192
#define WF_BUF_SIZE_WORD (WF_BUF_SIZE_BYTE/2)

/*一页 8K Byte*/
static unsigned char wf_buf[WF_BUF_SIZE_BYTE];


/*
	读第pg_num页的全部内容
	pg_num: 1-8
*/
unsigned char showFLASH_PG(unsigned char pg_num){

	long tgt_addr;
	unsigned short *data;
	unsigned short i;

	if(pg_num <1 || pg_num > 8) {
		dbg_string("please Enter page NO: 1-8 \n");
		return 1;
	}
	
	tgt_addr = WF_PG_ADDR(pg_num);
//	dbg_string("\n\nRead FLASH %01dst page, addr: 0X%08X\n",pg_num,tgt_addr);
	dbg_string("\n\nRead FLASH %01dst page \n",pg_num);
	
	for (i = 0; i < WF_BUF_SIZE_WORD; i++)
	{
		wdg_feed();
		if (i%16 == 0) dbg_string("%02X: ", i>>4);
		data = (unsigned short *)(tgt_addr+i*2);
		dbg_string("0X%04X,", *data);
		if (((i+1)%16 == 0) && (i > 0)) dbg_string("\n");
	}

	dbg_string("\n");
	
	return 0;
}


/* 把第pg_num页的数据备份到RAM中 */
void backupFLASH_PG(unsigned char pg_num){

	long tgt_addr;
	
	tgt_addr = WF_PG_ADDR(pg_num);
	data_copy(wf_buf,(unsigned char *)(tgt_addr), WORK_FLASH_PAGE_8K);
}

unsigned char writeFLASH_PG(unsigned char pg_num){

	long tgt_addr;
	
	tgt_addr = WF_PG_ADDR(pg_num);
	
	return hal_data_flash_sector_programing(tgt_addr, (unsigned short *)wf_buf, 0, WF_BUF_SIZE_WORD);
}

/*
pg_num : workflash第pg_num页(1~8)
data:    要写的数据
offset:  data在pg_num页的偏移量
*/
unsigned char writeFLASH_data(unsigned char pg_num, unsigned short data, unsigned short offset){

	long tgt_addr;
	int  i, tail;
	
	tgt_addr = WF_PG_ADDR(pg_num);
	
	/* 先备份 */
	backupFLASH_PG(pg_num);
	wf_buf[offset+1] = data;
	wf_buf[offset]   = data>>8;
		
	/* 再擦除 */
	hal_data_flash_sect_erase(tgt_addr); 			/* 开始擦除 */
	while(hal_check_erasing_status(tgt_addr));		/* 等待擦除完毕 */

//	writeFLASH_PG(pg_num);

	for(i=0; i<WF_BUF_SIZE_BYTE; i+=2){
		/*4字节对齐*/
		if(wf_buf[i] != 0xff)  tail = i+4;					
	}
	hal_data_flash_sector_programing(tgt_addr, (unsigned short *)wf_buf, 0, tail/2);

	return 0;
}

unsigned short readFLASH_data(unsigned char pg_num, unsigned short offset){

	long tgt_addr;
	unsigned short *ptr;

	if(pg_num <WF_PG_1 || pg_num > WF_PG_8) {
		dbg_string("please Enter page NO: 1-8 \n");
		return 1;
	}
	
	tgt_addr = WF_PG_ADDR(pg_num);
	ptr = (unsigned short *)(tgt_addr+offset);	

	return *ptr;
}



unsigned char eraseFLASH_PG(unsigned char pg_num)
{
	long tgt_addr;

	if(pg_num <WF_PG_1 || pg_num > WF_PG_8) {
		dbg_string("please Enter page NO: 1-8 \n");
		return 1;
	}
	
	dbg_string("\nerase FLASH %dst page \n",pg_num);
	
	tgt_addr = WF_PG_ADDR(pg_num);
	hal_data_flash_sect_erase(tgt_addr); 			/* 开始擦除 */
	while(hal_check_erasing_status(tgt_addr));		/* 等待擦除完毕 */
	
	dbg_string("Erase Done ...\n");

	return 0;
}

void save_FW_flag()
{
	backupFLASH_PG(WF_PG_8);
	
	hal_data_flash_sect_erase(WF_PG_ADDR(WF_PG_8)); 			/* 开始擦除 */
	while(hal_check_erasing_status(WF_PG_ADDR(WF_PG_8)));		/* 等待擦除完毕 */
	
	/*只写 FireWare 标志位*/
	hal_data_flash_sector_programing(WF_PG_ADDR(WF_PG_8), (unsigned short *)wf_buf, 0, 2);
}






