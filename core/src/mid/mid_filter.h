#ifndef _MID_FILTER_H
#define _MID_FILTER_H
#include "comm_typedef.h"

#define	 FILTER_PARAMETER	5

typedef  struct
{
    U32	last_value;	/*历史滤波值*/
    U16 filter_par;	/*滤波参数*/
    U16 rest;
} filter_lag_t;

typedef  struct
{
    U16  last_value;	/*历时值*/
    U16  keep_time;		/*误差值持续时间*/
    U16	 shake_time;	/*抖动时间 */
} shake_filter_t;


typedef struct
{
    U32 sum_value;
    U16 avg_value;
    U16 buf[10];
    U16 point;
    U16 count;
} avg_filter_t;

typedef struct  
{	
	U8   result;		//filter result
	S8	 count;			//count 
	S8	 filter_time;	//the resut filter time
	U8   valid_value;	//valid value
}pin_filter_t;

extern  U32	Absolute(S32 x);
extern	U8	MidFilterWeight(CHAR *data,U8 num);/*加权递推平均滤波法*/
extern  U16	MidFilterLag(filter_lag_t  *filter_choice,U16 new_value,U8 par);/*一阶滞后滤波*/
extern  U16	MidFilterAvg(CHAR  *data, U8 num);	/*均值滤波*/
extern	U16 MidPreaveShakeFilter(shake_filter_t *p,U16 value,U16 shakeAmp);
extern  U16	MidFilterLag_spd2(filter_lag_t  *filter,U16 new_value, U8 par);/* 车速表滤波函数 */
extern void avg_data_filter(avg_filter_t *data_type,U16 data);
extern U16  get_avg_value(avg_filter_t *data_type);
extern U8  mid_pin_filter(pin_filter_t *pin_name,U8 pin_sts);
extern U16 limit_range_filter(U16 data,U16 range);

#endif
