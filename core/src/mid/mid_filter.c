#include <stdlib.h>
#include "mid_filter.h"
#include "hal_adc.h"

#define AD_BUF_LEN 10
const U8  coe_tab[AD_BUF_LEN] = {1,2,3,4,5,6,7,8,9,10};   /*加权系数*/
const U8  sum_coe = 1+2+3+4+5+6+7+8+9+10;


/************************************************************************/
/*                取绝对值函数                                          */
/************************************************************************/
U32  Absolute(S32 x)
{
    U32 ret = 0;
    if(x < 0)
    {
        ret = -x;
    }
    else
    {
        ret = x;
    }
    return ret;
}

U8 MidFilterWeight(CHAR *data,U8 num) /*加权递推平均滤波法*/
{
    U32  total_value = 0;
    U8	 i =0;
    do
    {
        {
            if( data == NULL)
                break;
        }
        for(i =0; i<num; i++)
        {
            total_value += (data[i] * coe_tab[i]);
        }
        total_value/= sum_coe;
    }
    while (0);
    return (U8)total_value;
}
/*
** \name		MidFilterWeight
** \brief		一阶滞后滤波
** \input		str:滤波数据来源,num:数据个数
*/
U16  MidFilterAvg(CHAR  *data, U8 num)
{
    U16 i=0;
    U32 temp =0;
    do
    {
        if(data == NULL)
        {
            break;
        }
        for(i =0; i<num; i++)
        {
            temp  += data[i];
        }
        temp/=num;
    }
    while(0);
    return temp;
}
/*
** \brief	   一阶滞后滤波
** \input
** \output
*/
U16  MidFilterLag(filter_lag_t  *filter,U16 new_value,U8 par)
{
    U16 ret_value =0;

	if(0 == filter->last_value)
	{
		filter->last_value = new_value;
	}
	else
	{
		filter->filter_par = par;
		filter->last_value = (filter->last_value*(filter->filter_par -1)
							  + (U32)new_value*filter->filter_par
							  + filter->rest )/filter->filter_par;

		filter->rest  = filter->last_value%filter->filter_par;

		ret_value = filter->last_value/filter->filter_par;
	}
    return ret_value;
}
/*
** \brief	   移动平均滤波
** \notes		防抖动滤波，当两个数据连续变化较大时，忽略新的值，当该值持续较大时，
*/
U16  MidPreaveShakeFilter(shake_filter_t *p,U16 value,U16 shakeAmp)
{
    if(Absolute(p->last_value -value) < shakeAmp)
    {
        if(Absolute(p->last_value -value) > 1)
        {
            p->last_value = value;
            p->shake_time = 0;
        }
        else
        {
            p->shake_time ++;
            if(p->shake_time >= 5)		/* 若长时间出现该值，*/
            {
                p->shake_time = 0;
                p->last_value = value;
            }
        }
        p->keep_time = 0;
    }
    else
    {
        p->keep_time ++;
        p->shake_time = 0;
        if(p->keep_time >= 5)				/* 新值5个周期后执行 */
        {
            p->keep_time = 0;
            p->last_value = value;
        }
    }
    return p->last_value;
}


/*
	   移动平均滤波
*/
void avg_data_filter(avg_filter_t *data_type,U16 data)
{
    if(data_type->count < 10)
    {
        data_type->sum_value += data;
    }
    else
    {
        data_type->sum_value += data;
        data_type->sum_value -= data_type->buf[(data_type->point)%10];
    }
    data_type->buf[data_type->point] = data;
    data_type->point = (data_type->point+1)%10;
    data_type->count++;
    if(data_type->count >= 10)
    {
        data_type->count = 10;
    }
}

U16  get_avg_value(avg_filter_t *data_type)
{
    if(data_type->count != 0)
    {
        data_type->avg_value = data_type->sum_value/data_type->count;
    }
    else
    {
        data_type->avg_value  = 0;
    }
    return (data_type->avg_value);
}

U8  mid_pin_filter(pin_filter_t *pin_name,U8 pin_sts)
{
	U8  temp;
	temp = (pin_sts == pin_name->valid_value) ? 1:0;
	if(temp > 0)
	{
		if(pin_name->count <  pin_name->filter_time)
		{
			pin_name->count++;
		}
	}
	else
	{
		if(pin_name->count  > - pin_name->filter_time)
		{
			pin_name->count--;
		}
	}
	if(pin_name->count >= pin_name->filter_time)
	{
		pin_name->result = 1;
	}
	else if(pin_name->count <= -pin_name->filter_time)
	{
		pin_name->result = 0;
	}
	
	return temp;
}

/* 
	限幅滤波，如果超过指定幅度即返回0xffff，否则返回data
	注意: 
	即使是不合法的data，也要用来做下一次比较的基准值，因为这个滤波算法的本意是"当有连续两个稳定的数值时认为第二个值有效"
*/
U16 limit_range_filter(U16 data,U16 range)
{
	static U16 first_data_flag=1;
	static U16 data_bak=0;
	
	if(first_data_flag)
	{
		data_bak=data;//第一个数据只做后面数据的参考，不认为是有效数据
		
		first_data_flag=0;
	}
	else
	{
		if(Absolute(data-data_bak)<=range)
		{
			data_bak=data;
			
			return data;
		}
		else
		{
			data_bak=data;
		}
	}
	
	return 0xffff;//invalid data
}
