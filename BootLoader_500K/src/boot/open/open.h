#ifndef __OPEN_H__
#define __OPEN_H__
#include "mid_pin.h"		/* gpio_dev_t */

#define PROJECT "Todi"
#define OPEN_IO_GROUP_SIZE 7



gpio_dev_t *Open_GPIO_Init(void);
void Open_Upgrade_Init(void (*timeDelay)(unsigned int));
unsigned char Open_Upgrade_Condition_Judgment(void);

unsigned char Open_Upgrade_reset(unsigned char upgrade_flag);




#endif

