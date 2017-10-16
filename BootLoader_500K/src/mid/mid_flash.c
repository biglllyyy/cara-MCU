#include "Mb91520.h"
#include "Comm_typedef.h"
#include "mid_flash.h"
#include "hal_flash.h"
#include "hal_wdg.h"
#include "mid_dbg.h"

#define FLASH_SECT_SIZE 22
#define FLASH_SECT_OFFSET 2

#define WORK_FLASH_SECT_SIZE 8

/* ��������ά���� */
static const unsigned int FlashScectorsAddrs[FLASH_SECT_SIZE] = 
{
	0x74000, 	/* ����1 */
	0x74004,	/* ����2 */
	0x78000,	/* ����3 */
	0x78004,	/* ����4 */
	0x7C000,	/* ����5 */
	0x7C004,	/* ����6 */
	0x80000,	/* ����7 */
	0x80004,	/* ����8 */
	0xA0000,	/* ����9 */
	0xA0004,	/* ����10 */
	0xC0000,	/* ����11 */
	0xC0004,	/* ����12 */
	0xE0000,	/* ����13 */
	0xE0004,	/* ����14 */
	0x100000,	/* ����15 */
	0x100004,	/* ����16 */
	0x120000,	/* ����17 */
	0x120004,	/* ����18 */
	0x140000,	/* ����19 */
	0x140004,	/* ����20 */
	0x160000,	/* ����21 */
	0x160004	/* ����22 */
};


/* Work Flash ����ά���� */
static const unsigned int WorkFlashScectorsAddrs[WORK_FLASH_SECT_SIZE] = 
{
	0x330000,	/* ����1 */
	0x332000,	/* ����2 */
	0x334000,	/* ����3 */
	0x336000,	/* ����4 */
	0x338000,	/* ����5 */
	0x33A000,	/* ����6 */
	0x33C000,	/* ����7 */
	0x33E000	/* ����8 */
};


#pragma segment CODE=CODE_CAN_Boot, attr=CODE /* �����´���ŵ�RAM������ֹ����ROM�������� */
/* дƫ��λ��FLASH���� */
unsigned char mid_data_flash_sect_programming(long addr, unsigned short* src_data, int offset, unsigned long cnt)
{
	return hal_data_flash_sector_programing(addr,src_data,offset,cnt);
}

/* ������FLASH���� */
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

/* ��workFLASH���� */
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

/* д����FLASH���� */
unsigned char mid_Flash_Sector_Programing(long tgt_add, unsigned short* src_data, U32 cnt)
{
	return hal_Flash_Sector_Programing(tgt_add,src_data, cnt);
}


/* дworkFLASH���� */
unsigned char mid_Work_Flash_Sector_Programing(long tgt_add, unsigned short* src_data, U32 cnt)
{
	return hal_Work_Flash_Sector_Programing(tgt_add,src_data,cnt);
}



