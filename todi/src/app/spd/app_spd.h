#ifndef   __APP_SPD_H__
#define   __APP_SPD_H__
#include "comm_typedef.h"

typedef enum
{
	NO_ERR = 1,
	OVER_MAX = 2,	//over max value 
}SPD_ERR_CODE_e;

typedef struct
{
	U32 spd_total;
	U32 cnt_total;
}avg_spd_info_t;
void app_init_spd(void);
U8  app_get_fact_spd(void);
U8  app_get_dis_spd(void);
U8  app_get_avg_spd(void);
void app_clear_avg_spd(void);
void app_set_spd(U16 spd);
void app_process_spd_task(void);

#endif
