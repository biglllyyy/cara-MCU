#include "Mb91520.h"
#include "Comm_typedef.h"
#include "mid_flash.h"
#include "hal_flash.h"
#include "hal_wdg.h"
#include "mid_dbg.h"

#define FLASH_SECT_SIZE 22
#define FLASH_SECT_OFFSET 2

#define WORK_FLASH_SECT_SIZE 8

/* 程序扇区维护表 */
static const unsigned int FlashScectorsAddrs[FLASH_SECT_SIZE] = 
{
	0x74000, 	/* 扇区1 */
	0x74004,	/* 扇区2 */
	0x78000,	/* 扇区3 */
	0x78004,	/* 扇区4 */
	0x7C000,	/* 扇区5 */
	0x7C004,	/* 扇区6 */
	0x80000,	/* 扇区7 */
	0x80004,	/* 扇区8 */
	0xA0000,	/* 扇区9 */
	0xA0004,	/* 扇区10 */
	0xC0000,	/* 扇区11 */
	0xC0004,	/* 扇区12 */
	0xE0000,	/* 扇区13 */
	0xE0004,	/* 扇区14 */
	0x100000,	/* 扇区15 */
	0x100004,	/* 扇区16 */
	0x120000,	/* 扇区17 */
	0x120004,	/* 扇区18 */
	0x140000,	/* 扇区19 */
	0x140004,	/* 扇区20 */
	0x160000,	/* 扇区21 */
	0x160004	/* 扇区22 */
};


/* Work Flash 扇区维护表 */
static const unsigned int WorkFlashScectorsAddrs[WORK_FLASH_SECT_SIZE] = 
{
	0x330000,	/* 扇区1 */
	0x332000,	/* 扇区2 */
	0x334000,	/* 扇区3 */
	0x336000,	/* 扇区4 */
	0x338000,	/* 扇区5 */
	0x33A000,	/* 扇区6 */
	0x33C000,	/* 扇区7 */
	0x33E000	/* 扇区8 */
};


#pragma segment CODE=CODE_CAN_Boot, attr=CODE /* 将以下代码放到RAM区，防止放在ROM区被擦除 */
/* 写偏移位置FLASH扇区 */
unsigned char mid_data_flash_sect_programming(long addr, unsigned short* src_data, int offset, unsigned long cnt)
{
	return hal_data_flash_sector_programing(addr,src_data,offset,cnt);
}

/* 擦程序FLASH扇区 */
unsigned char mid_flash_sect_erasing(unsigned char sect_num)
{
	int index = 0;
	if((sect_num < FLASH_SECT_OFFSET) || (sect_num > (ARRAY_SIZE(FlashScectorsAddrs) + FLASH_SECT_OFFSET)))
		return FAIL;
	for(index = 0;index<ARRAY_SIZE(FlashScectorsAddrs);++index)
	{
		if(((sect_num - FLASH_SECT_OFFSET) == index) && (hal_Flash_Sect_Erasing(FlashScectorsAddrs[index]) == HAL_FALSH_SUCCESS))
			return SUCCESS;
	}
	return FAIL;
}

/* 擦workFLASH扇区 */
unsigned char mid_work_flash_sect_erasing(unsigned char sect_num)
{
	int index = 0;
	if(sect_num > ARRAY_SIZE(WorkFlashScectorsAddrs))
		return FAIL;
	for(index = 0;index < ARRAY_SIZE(WorkFlashScectorsAddrs);++index)
	{
		if((sect_num == index) && (hal_Work_Flash_Sect_Erasing(WorkFlashScectorsAddrs[index]) == HAL_FALSH_SUCCESS))
			return SUCCESS;
	}
	return FAIL;
}

/* 写程序FLASH扇区 */
unsigned char mid_Flash_Sector_Programing(long tgt_add, unsigned short* src_data, U32 cnt)
{
	return hal_Flash_Sector_Programing(tgt_add,src_data, cnt);
}


/* 写workFLASH扇区 */
unsigned char mid_Work_Flash_Sector_Programing(long tgt_add, unsigned short* src_data, U32 cnt)
{
	return hal_Work_Flash_Sector_Programing(tgt_add,src_data,cnt);
}



