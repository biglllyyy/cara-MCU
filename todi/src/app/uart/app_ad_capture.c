#include "app_ad_capture.h"
#include "mid_adc.h"
#include "app_moudle.h"


const Ad_Capture_Info ad_cap_info_arr[MCU_SER_ALL] =
{
	//{MCU_SER1,20},
	//{MCU_SER2,21},
	//{MCU_SER3,24},
	{MCU_SER4,25},
	{MCU_SER5,29},
	//{MCU_SER6,30},
	//{MCU_VBAT,12},
};


void ad_capture_info_init(void)
{
	U8 i = 0;
	for(i=0;i<MCU_SER_ALL;i++)
	{
		mid_adc_start(ad_cap_info_arr[i].ser_ad_channel);
	}

}

void ad_capture_info_get_data(void)
{
//	U8 i = 0;
//	for(i=0;i<MCU_SER_ALL;i++)
//	{
//		ADR[i] =  mid_adc_get(ad_cap_info_arr[i].ser_ad_channel);/* 先获取AD值, 0xFFF为无效 */
//		//dbg_string("ADR[%d] = %d\n",i,ADR[i]);
//		wdg_feed();
//	}
	extern U16 AD_Value[2][0x20] ;
	U32 i,sum=0;
	for (i=0;i<0x20;i++)
	{
		sum += AD_Value[0][i];
	}
	sum /= 0x20;
	ADR[3] = sum;
	sum = 0;
	for (i=0;i<0x20;i++)
	{
		sum += AD_Value[1][i];
	}
	sum /= 0x20;
	ADR[4] = sum;

}

