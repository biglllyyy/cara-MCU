#ifndef __MID_BOOTFLASH_H_
#define __MID_BOOTFLASH_H_




/* public部分函数 */



/* 读取启动标志，计数器和写入启动标志，计数器功能函数 */
unsigned char mid_data_flash_sect_programming(long addr, unsigned short* src_data, int offset, unsigned long cnt);


/* 扇区擦除 */
unsigned char mid_flash_sect_erasing(unsigned char sect_num);
unsigned char mid_work_flash_sect_erasing(unsigned char sect_num);

/* 扇区烧录 */
unsigned char mid_Flash_Sector_Programing(long tgt_add, unsigned short* src_data, U32 cnt);
unsigned char mid_Work_Flash_Sector_Programing(long tgt_add, unsigned short* src_data, U32 cnt);

#endif

