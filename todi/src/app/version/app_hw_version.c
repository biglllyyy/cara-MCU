#include "string.h"
#include "mid_adc.h"
#include "Comm_typedef.h"
#include "mid_dbg.h"
#include "hal_adc.h"

#define HW_VERSION_CHN 47 /* 硬件版本信息读取ADC通道 */
#define VERSION_DATA_SIZE 10 /* 版本信息结构体数组大小 */

#define HW_1_3V 13000 /* 1.3000V */
#define HW_1_3_5V 13500
#define HW_1_4V 14000
#define HW_1_4_5V 14500
#define HW_1_5V 15000 /* 硬件电压数值 */
#define HW_2_0V 20000
#define HW_0_7V 7000
#define HW_0_8V 8000


struct HW_Version_Data
{
	U32 ver;
	U16 Level_H;
	U16 Level_L;
};

struct HW_Version_Data Version_Data[VERSION_DATA_SIZE]=
{
	{
		0x00000402,	/* 0.4.2 */
		HW_2_0V,
		HW_1_5V
	},
	{
		0x00000403,	/* 0.4.3 */
		HW_1_4_5V,
		HW_1_3_5V
	},
	{
		0x00000500,	/* 0.5.0 */
		HW_0_8V,
		HW_0_7V
	}
};


unsigned char HW_Version_Read_flag = 0;
static U32 gHWVersion = 0;

static U16 calc_adc_value(U16 adc_value)
{
	const U16 ref = 33000;
	U16 Result_Value = (U16)((adc_value * ref) >> 12) ;
	
	return Result_Value;
}


/* 硬件版本信息更新初始化 */
void HW_Version_Read_Init(void)
{
	HW_Version_Read_flag = 0;
	mid_adc_start(HW_VERSION_CHN);
}


/* 获取硬件版本号 */
U32 get_HW_version(void)
{
	U8 i = 0;
	U16 hw_version, m;
	U32 avg_hw_version = 0;
	U16 adc_value;

	/* 如果已经读取过 */
	if (gHWVersion) return gHWVersion;
	
	/* 读几次AD，取平均值 */
	for(i = 0;i < 10;i++)
	{
		m = mid_adc_get(HW_VERSION_CHN);
	//	dbg_string("v:%d\n", m);
		avg_hw_version = avg_hw_version + m;	
	}
	mid_adc_stop(HW_VERSION_CHN);
	
	hw_version = (U16)(avg_hw_version / 10);
	adc_value = calc_adc_value(hw_version);
//	dbg_string("hw_version:%d\n", hw_version);
//	dbg_string("adc_value:%d\n", adc_value);

	for(i = 0;i < VERSION_DATA_SIZE;i++)
	{
		if((adc_value >= Version_Data[i].Level_L) && (adc_value <= Version_Data[i].Level_H))
		{
			gHWVersion = Version_Data[i].ver;
		}
	}
	return gHWVersion;
}

/* 显示硬件版本信息任务函数 */
void app_show_HW_version(void)
{
	U32 ver = 0;
	
	if(0 != HW_Version_Read_flag) return;

	ver = get_HW_version();
	if (! ver) return;
	HW_Version_Read_flag = 1;

	dbg_string("Hardware version: %d.%d.%d\n", (ver & 0xff0000)>>16, (ver & 0xff00) >> 8, (ver & 0xff));
}



