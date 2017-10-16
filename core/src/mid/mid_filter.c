#include <stdlib.h>
#include "mid_filter.h"
#include "hal_adc.h"

#define AD_BUF_LEN 10
const U8  coe_tab[AD_BUF_LEN] = {1,2,3,4,5,6,7,8,9,10};   /*��Ȩϵ��*/
const U8  sum_coe = 1+2+3+4+5+6+7+8+9+10;


/************************************************************************/
/*                ȡ����ֵ����                                          */
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

U8 MidFilterWeight(CHAR *data,U8 num) /*��Ȩ����ƽ���˲���*/
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
** \brief		һ���ͺ��˲�
** \input		str:�˲�������Դ,num:���ݸ���
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
** \brief	   һ���ͺ��˲�
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
** \brief	   �ƶ�ƽ���˲�
** \notes		�������˲������������������仯�ϴ�ʱ�������µ�ֵ������ֵ�����ϴ�ʱ��
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
            if(p->shake_time >= 5)		/* ����ʱ����ָ�ֵ��*/
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
        if(p->keep_time >= 5)				/* ��ֵ5�����ں�ִ�� */
        {
            p->keep_time = 0;
            p->last_value = value;
        }
    }
    return p->last_value;
}


/*
	   �ƶ�ƽ���˲�
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
	�޷��˲����������ָ�����ȼ�����0xffff�����򷵻�data
	ע��: 
	��ʹ�ǲ��Ϸ���data��ҲҪ��������һ�αȽϵĻ�׼ֵ����Ϊ����˲��㷨�ı�����"�������������ȶ�����ֵʱ��Ϊ�ڶ���ֵ��Ч"
*/
U16 limit_range_filter(U16 data,U16 range)
{
	static U16 first_data_flag=1;
	static U16 data_bak=0;
	
	if(first_data_flag)
	{
		data_bak=data;//��һ������ֻ���������ݵĲο�������Ϊ����Ч����
		
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
