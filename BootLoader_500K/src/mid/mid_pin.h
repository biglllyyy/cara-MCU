#ifndef __MID_PIN_H_
#define __MID_PIN_H_

#include "hal_gpio.h"

#define  VALID_LOW_LEVEL	1 
#define  VALID_HIGH_LEVEL	0

typedef struct
{
	unsigned char *name;		//!<固件名称
	U8   port;					//!<端口号
	U8   bit;					//!<引脚号
	e_io_direction  dir;		//!<输入、输出方向
	U8   value; 				//!<初始值
}gpio_dev_t;

void		  mid_set_io_sts(gpio_dev_t *io_name,U8 value);
unsigned char mid_get_io_sts(gpio_dev_t *io_name);
void 	   	  mid_set_io_dir(gpio_dev_t *io_name, e_io_direction dir);
unsigned char mid_GPIO_Init(gpio_dev_t io_dev_group[],unsigned char group_size);





#endif 
