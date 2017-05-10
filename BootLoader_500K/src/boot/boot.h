#ifndef  __BOOT_H__
#define  __BOOT_H__

/* Saber Bootloader Version*/
#define BL_VER_MAJOR 1
#define BL_VER_MINOR 0
#define BL_VER_REV	 8


#define LIGHT_LCD_DELAY_TIME   20



#define FLASH_START_ADDRESS      0x080000
#define FLASH_END_ADDRESS        0x0BFFFF


#define WORK_FLASH_START_ADDRESS  0x330000
#define WORK_FLASH_END_ADDRESS    0x33FFFF


#define FLASH_UPDATE_STS_ADDRESS  0x33E000 //0x33E000

#define PROG_START_PAGE	8
#define PROG_END_PAGE	12

#define WF_START_PAGE 7
#define WF_END_PAGE 8

#endif
