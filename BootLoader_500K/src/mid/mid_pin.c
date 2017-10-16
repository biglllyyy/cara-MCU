#include "Comm_typedef.h"
#include "hal_gpio.h"
#include "mid_pin.h"

static unsigned char GPIO_Init_Flag = FALSE;

void mid_set_io_sts(gpio_dev_t *io_name,U8 value)
{
	if(io_name != NULL)
	{
		io_name->value = (value >0) ? 1:0;
		if(IO_GENERAL_OUTPUT == io_name->dir)
		{
			set_single_io_sts(io_name->port,io_name->bit,io_name->value);
		}
		else
		{
			io_name->dir = IO_GENERAL_OUTPUT;
			set_single_io_dir(io_name->port,io_name->bit,IO_GENERAL_OUTPUT);
			set_single_io_sts(io_name->port,io_name->bit,io_name->value);	
		}	
	}
}

U8  mid_get_io_sts(gpio_dev_t *io_name)
{
	if(IO_INPUT == io_name->dir)
	{
		io_name->value = get_single_io_sts(io_name->port,io_name->bit);
	}
	else
	{
		io_name->dir = IO_INPUT;
		set_single_io_dir(io_name->port,io_name->bit,IO_INPUT);
		io_name->value = get_single_io_sts(io_name->port,io_name->bit);
	}
	
	return (io_name->value); 
}



void mid_set_io_dir(gpio_dev_t *io_name,e_io_direction dir)
{
	if(io_name != NULL)
	{
		io_name->dir = dir;
		set_single_io_dir(io_name->port,io_name->bit,dir);
	}

}

static void mid_io_config(gpio_dev_t *io_name, U8 port, U8 bit,e_io_direction dir,U8 value)
{
	if((io_name != NULL)&& (port < 20) && (bit < 8))
	{
		io_name->port = port;
		io_name->bit = bit;
		io_name->dir = dir;
		io_name->value = value;
		set_single_io_dir(port,bit,dir);
		if(dir == IO_GENERAL_OUTPUT)
		{
			mid_set_io_sts(io_name,value);
		}
	}
}


unsigned char mid_GPIO_Init(gpio_dev_t io_dev_group[],unsigned char group_size)
{
	int index = 0;

	if(!GPIO_Init_Flag)
	{
		GPIO_Init_Flag = TRUE;
		hal_gpio_Init(); //关闭AD,使能IO输入功能
	}
		
	for(index = 0;index < group_size; ++index) //初始化GPIO组
	{
		mid_io_config(	&io_dev_group[index],
						io_dev_group[index].port,
						io_dev_group[index].bit,
						io_dev_group[index].dir,
						io_dev_group[index].value);
	}

	return 1;
}








