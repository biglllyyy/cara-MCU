#ifndef __MID_PIN_H_
#define __MID_PIN_H_

#include "hal_gpio.h"

#define  VALID_LOW_LEVEL	1 
#define  VALID_HIGH_LEVEL	0

typedef struct
{
	unsigned char *name;		//!<�̼�����
	U8   port;					//!<�˿ں�
	U8   bit;					//!<���ź�
	e_io_direction  dir;		//!<���롢�������
	U8   value; 				//!<��ʼֵ
}gpio_dev_t;

void		  mid_set_io_sts(gpio_dev_t *io_name,U8 value);
unsigned char mid_get_io_sts(gpio_dev_t *io_name);
void 	   	  mid_set_io_dir(gpio_dev_t *io_name, e_io_direction dir);
unsigned char mid_GPIO_Init(gpio_dev_t io_dev_group[],unsigned char group_size);





#endif 
