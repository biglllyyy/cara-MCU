#ifndef _MID_FILTER_H
#define _MID_FILTER_H
#include "comm_typedef.h"

#define	 FILTER_PARAMETER	5

typedef  struct
{
    U32	last_value;	/*��ʷ�˲�ֵ*/
    U16 filter_par;	/*�˲�����*/
    U16 rest;
} filter_lag_t;

typedef  struct
{
    U16  last_value;	/*��ʱֵ*/
    U16  keep_time;		/*���ֵ����ʱ��*/
    U16	 shake_time;	/*����ʱ�� */
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
extern	U8	MidFilterWeight(CHAR *data,U8 num);/*��Ȩ����ƽ���˲���*/
extern  U16	MidFilterLag(filter_lag_t  *filter_choice,U16 new_value,U8 par);/*һ���ͺ��˲�*/
extern  U16	MidFilterAvg(CHAR  *data, U8 num);	/*��ֵ�˲�*/
extern	U16 MidPreaveShakeFilter(shake_filter_t *p,U16 value,U16 shakeAmp);
extern  U16	MidFilterLag_spd2(filter_lag_t  *filter,U16 new_value, U8 par);/* ���ٱ��˲����� */
extern void avg_data_filter(avg_filter_t *data_type,U16 data);
extern U16  get_avg_value(avg_filter_t *data_type);
extern U8  mid_pin_filter(pin_filter_t *pin_name,U8 pin_sts);
extern U16 limit_range_filter(U16 data,U16 range);

#endif
