#ifndef __MID_PIN_H_
#define __MID_PIN_H_

#include "hal_gpio.h"

#define  VALID_LOW_LEVEL	1 
#define  VALID_HIGH_LEVEL	0


typedef struct
{
	U8   port;				
	U8   bit;				
	e_io_direction  dir;	//!<·½Ïò
	U8   value; 			
}io_type_t;

void 	mid_set_io_sts(io_type_t *io_name,U8 value);
U8  	mid_get_io_sts(io_type_t *io_name);
void	mid_io_config(io_type_t *io_name, U8 port, U8 bit,e_io_direction dir,U8 value);
void	mid_set_io_dir(io_type_t *io_name, e_io_direction dir);
#endif 
