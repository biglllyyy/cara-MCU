#ifndef _MID_DS1302_H_
#define _MID_DS1302_H_

#include "comm_typedef.h"

#define REF_YEAR  (2000)/*参考起始年份*/

struct tm getCurrentTime_DS1302(void);
ERROR_CODE setCurrentTime_DS1302(struct tm time);
void mid_DS1302_close(void);
void mid_DS1302_open(void (*ptr)(void));

#endif
