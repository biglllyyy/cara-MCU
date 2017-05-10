#ifndef   __APP_FUEL_H__
#define   __APP_FUEL_H__

#include "comm_typedef.h"

#define  MAX_AVG_OIL_CONSUMP_L_100KM_X10     (300) 

#define  MAX_FUEL_CONSUMPTION			   999	
#define  U16_INVALID_VALUE			  (0xFFFF)

/* AD值缓冲大小 */
#define FUEL_AD_BUF_1_NUM 			50
#define FUEL_AD_BUF_2_NUM			60

/* 判断是否加过油的阈值 */
#define REFUEL_THRESHOLD		12000

extern U32	AFE_fuel_used;
extern U32	AFE_distance;
extern U32	AFE_Fuel_Accum;
extern U32	AFE_Distance_Accum;
extern U16  AFE;
extern U16  FuelSigTn_bak;
extern U16  FuelSigTn;
extern U32 s_u32_instance_accumulation;
extern U16 g_u16_instance_consump;


void app_init_fuel(void);
U8 app_get_fuel_percent(void);

U16 app_get_fuel_volum_ml(void);
void app_pro_fuel_task(void);
U16  app_get_avg_fuel_consump(void);
U16  app_get_ins_fuel_consump(void);
void fuel_ins_consumption_cal(void);
void app_cal_avg_fuel_consump(void);
void app_clear_avg_fuel_consump(void);
void save_ee_AFE_fuel_used(void);
void save_ee_AFE_distance(void);
void get_ee_AFE_info(void);

/* IGN ON，立即计算燃油量 */
U8 app_read_fuel_ign_on(void);
U8 get_g_u8IgnSts_bak(void);
void set_g_u8IgnSts_bak(U8 value);

//void save_fuel_ad(void);
#endif
