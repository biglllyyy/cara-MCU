#ifndef _FLASH_H_
#define _FLASH_H_


#define   WORK_FLASH_START_ADDRESS  0x330000
#define   WORK_FLASH_END_ADDRESS    0x33FFFF
#define   WORK_FLASH_PAGE_8K        0x2000
#define   WF_PG_ADDR(pg_num)        (WORK_FLASH_START_ADDRESS + WORK_FLASH_PAGE_8K*(pg_num-1))

#define   WF_PG_1  1
#define   WF_PG_2  2
#define   WF_PG_3  3
#define   WF_PG_4  4
#define   WF_PG_5  5
#define   WF_PG_6  6
#define   WF_PG_7  7
#define   WF_PG_8  8


unsigned char showFLASH_PG(unsigned char pg_num);
unsigned char eraseFLASH_PG(unsigned char pg_num);

void backupFLASH_PG(unsigned char pg_num);
unsigned char writeFLASH_PG(unsigned char pg_num);
unsigned char writeFLASH_data(unsigned char pg_num, unsigned short data, unsigned short offset);
unsigned short readFLASH_data(unsigned char pg_num, unsigned short offset);
void save_FW_flag();




#endif
