#ifndef HAL_DS1302_H_
#define HAL_DS1302_H_

#include "comm_typedef.h"

#define DS1302_REGADDR_SEC   		(uint8_t)0x80
#define DS1302_REGADDR_MIN   		(uint8_t)0x82
#define DS1302_REGADDR_HR    		(uint8_t)0x84
#define DS1302_REGADDR_DATE  		(uint8_t)0x86
#define DS1302_REGADDR_MON   		(uint8_t)0x88
#define DS1302_REGADDR_WDAY   		(uint8_t)0x8A
#define DS1302_REGADDR_YEAR  		(uint8_t)0x8C
#define DS1302_REGADDR_WP  			(uint8_t)0x8E/*write-protect*/
#define DS1302_REGADDR_TRICHARGER   (uint8_t)0x90
#define DS1302_REGADDR_CLKBURST     (uint8_t)0xBE/*burst mode*/
#define DS1302_READ_MASK            (uint8_t)0x01
#define DS1302_WRITE_MASK			(uint8_t)0x00
void writeDS1302(uint8_t u8addr, uint8_t u8data);
uint8_t readDS1302(uint8_t u8addr);
void hal_start_DS1302(void);
void hal_stop_DS1302(void);
void readDS1302_burst(uint8_t*buf);

void writeDS1302_burst(const uint8_t*buf);

void DS1302WriteProtect(Bool WriteEnable);

#endif
