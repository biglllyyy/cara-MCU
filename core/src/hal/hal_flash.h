#ifndef __HAL_FLASH_H__
#define __HAL_FLASH_H__

#include <string.h>
#include <stdlib.h>
#include "hal_uart.h"
#include "mid_dbg.h"
#include "hal_reload_timer.h"
#include "hal_wdg.h"

#define   WORK_FLASH_START_ADDRESS  0x330000
#define   WORK_FLASH_END_ADDRESS    0x33FFFF

int hal_check_erasing_status(unsigned long addr);
unsigned char hal_data_flash_sect_erase(unsigned long tgt_add);
unsigned char hal_data_flash_sector_programing(long tgt_add, unsigned short* src_data, int offset, unsigned long cnt);
void hal_data_flash_start_programming(void);
void hal_data_flash_stop_programming(void);

#endif
