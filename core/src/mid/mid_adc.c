#include "mid_adc.h"
#include "hal_adc.h"
#include "comm_typedef.h"
#define  MAX_CHN   10

typedef struct  
{
	U8  chn[MAX_CHN];	//!<adc 通道号
	U8  count;			//!<当前位置
}chn_manger;

chn_manger  ad_chn_mgr;

void mid_adc_init(void)
{
	ad_chn_mgr.count = 0;
}

void mid_adc_start(U8  signal_chn)//增加一个通道作为AD输入
{
	U8  index = 0;
	U8  find_the_same_chn = 0;

    if(signal_chn < 48)
	{
		for(index = 0;index < ad_chn_mgr.count;index++)
		{
			if(ad_chn_mgr.chn[index] == signal_chn)
			{
				find_the_same_chn = 1;
			}
		}
		if(0 == find_the_same_chn)
		{
			if(ad_chn_mgr.count < MAX_CHN)
			{
				ad_chn_mgr.chn[ad_chn_mgr.count] = signal_chn;
				ad_chn_mgr.count++;
			}
		}
	}
}


void mid_adc_stop(U8  signal_chn)//删除一个作为AD输入的通道
{
	U8  index = 0;
	U8  top = 0;
	if(signal_chn < 48)
	{
		for(index = 0;index < ad_chn_mgr.count;index++)
		{
			if(ad_chn_mgr.chn[index] == signal_chn)
			{
				top = --ad_chn_mgr.count;
				if(index != top)
				{
					ad_chn_mgr.chn[index] = ad_chn_mgr.chn[top];
				}
			}
		}
	}
}

/* mid adc task manager */
void mid_adc_manager_task(void)//该任务的作用就是把所有配置了的AD输入使能了
{
	U8     index = ad_chn_mgr.count;
	extern U8          current_chn;
	static U8  cur_chn_cnt = 0;
	if(index != 0)
	{
		
		cur_chn_cnt = (cur_chn_cnt+1)%index;//!<限幅,防止数组角标越界
		dbg_printf("+current_chn = %d\n",ad_chn_mgr.chn[cur_chn_cnt]);
		if(ad_chn_mgr.chn[cur_chn_cnt] < 32)
		{
			hal_adc_init(AD_GROUP1,ad_chn_mgr.chn[cur_chn_cnt]);
			hal_adc_start(AD_GROUP1);
		}
		else if(ad_chn_mgr.chn[cur_chn_cnt] < 48)
		{
			hal_adc_init(AD_GROUP2,ad_chn_mgr.chn[cur_chn_cnt]);
			hal_adc_start(AD_GROUP2);
		}
		dbg_printf("-current_chn = %d\n",ad_chn_mgr.chn[cur_chn_cnt]);
	}
}



U16  mid_adc_get(U8 single_chn)
{
	U16  ret = 0;

	//ret = AD_Value[single_chn];
	
	return ret;
}
