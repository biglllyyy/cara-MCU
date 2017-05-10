#ifndef __HAL_BOOTFLASH_H_
#define __HAL_BOOTFLASH_H_

#define HAL_FALSH_SUCCESS 0
#define HAL_FALSH_FAIL 1

unsigned char hal_data_flash_sector_programing(long addr, unsigned short* src_data, int offset, unsigned long cnt);

unsigned char hal_Flash_Sect_Erasing(unsigned long tgt_add);
unsigned char hal_Work_Flash_Sect_Erasing(U32 tgt_add);

unsigned char hal_Flash_Sector_Programing(long tgt_add, unsigned short* src_data, U32 cnt);
unsigned char hal_Work_Flash_Sector_Programing(long tgt_add, unsigned short* src_data, U32 cnt);


#endif

