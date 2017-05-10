#ifndef __MID_PIN_H_
#define __MID_PIN_H_

#include "hal_gpio.h"

#define  VALID_LOW_LEVEL	1 
#define  VALID_HIGH_LEVEL	0

typedef struct
{
	unsigned char *name;
	U8   port;
	U8   bit;
	e_io_direction  dir;
	U8   value; 
}gpio_dev_t;

void		  mid_set_io_sts(gpio_dev_t *io_name,U8 value);
unsigned char mid_get_io_sts(gpio_dev_t *io_name);
void 	   	  mid_set_io_dir(gpio_dev_t *io_name, e_io_direction dir);
unsigned char mid_GPIO_Init(gpio_dev_t io_dev_group[],unsigned char group_size);





#endif 
