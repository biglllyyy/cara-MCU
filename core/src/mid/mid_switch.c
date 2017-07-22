
#include	"mid_switch.h"
#include	"mid_pin.h"
#include	"mid_filter.h"

static U8 pin_in_all=0;
void mid_switch_init(U8 pin_in_num)
{
 	hal_gpio_Init();
	pin_in_all=pin_in_num;
	
}

U8 mid_get_io_filtered(pin_filter_t *pin_filter_in, U8 name)
{
    U8 ret = 0;

    if(name < pin_in_all)
    {
        ret = pin_filter_in[name].result;
    }

    return ret;
}

/* 得到滤波后的输入值 */
void mid_switch_task10(io_type_t *pin_io_in, pin_filter_t*pin_filter_in)
{
	U8  index = 0;   
	U8  temp = 0;

	for(index =0; index< pin_in_all; index++)			//input pin filter
	{	
		temp = mid_get_io_sts(&pin_io_in[index]);//直接从寄存器读取IO的输入值
		mid_pin_filter(&pin_filter_in[index],temp);//根据配置的滤波系数进行滤波，得到滤波后的输入值
		wdg_feed();
	}
}


void mid_set_all_pin_highz(void)
{
    int i =0;
    for(i=0; i<=15; i++)
    {
        set_group_io_dir(i,IO_INPUT);
    }
}
