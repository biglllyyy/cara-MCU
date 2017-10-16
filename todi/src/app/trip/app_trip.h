#ifndef  __APP_TRIP_H__
#define  __APP_TRIP_H__

#include "comm_typedef.h"
#include "aes.h"

#define   ONE_KILOMETER				(1000)
#define   TWO_KILOMETER				(ONE_KILOMETER*2)
#define   ONE_HUNDRED				(100)

#define	  MAX_SHIFT_ENLARGE         16
#define   MAX_SUB_TRIP							9999
#define   MAX_SUM_ODO								999999
#define   MAX_REST_MILE							999
#define   CAL_PERIOD_MS             100
#define   ODO_CLEAR_TIMES_CNT        6



typedef  struct  
{
	U32  sub_trip_temp;			/* trip1 unit worth0.1km */
	U32  total_trip_temp;		/* unit 0.1km */
	U8   trip_last_bit;			/* last bit of trip valule is 0.1km */
	U32  Odo;					/* odo value */	
	U8   allow_odo_clear_cnt;	/* allow odo clear times cnt */
	U16  Trip1;					/* trip1 value */
	U16	 Trip2;					/* trip2 value */
	U8   Odo_fault;				/* odo invalid */
	U32  old_time;				/* record last read timer time*/
}INFO_TYPE;

extern INFO_TYPE      info;

//trip 对外的接口函数
void app_trip_init(void);
U32 app_get_total_trip(void);
U8  app_odo_clear(void);
U8  app_odo_clear_no_limit(void);
U8 save_ee_total_trip(U32 trip);
U16 app_get_sub_trip1(void);
U8 app_sub_trip1_clear(void);
U8 save_ee_sub_trip1(U16 trip);
U16 app_get_sub_trip2(void);
U8 app_sub_trip2_clear(void);
U8 save_ee_sub_trip2(U16 trip);


void app_info_task100(void);
void app_rest_mile_cal_task(void);
U16  app_rest_mile_get(void);
void check_ee_data(void);
#endif
