#ifndef __MID_FLASH_H__
#define __MID_FLASH_H__

#include "hal_flash.h"


/* Work flash����ҳ���С */
#define WF_PAGE_SZ	8192
/* FIFO�е�����������ֽ���*/
#define WF_BUF_SZ	256

void task_work_flash(void);
U8 mid_erase_flash_async(unsigned long offset);
U8 mid_write_flash_async(unsigned long offset, unsigned char *buf, unsigned short len);
U8 mid_flash_ready(void);
U8 mid_flash_sync(void);

#endif
