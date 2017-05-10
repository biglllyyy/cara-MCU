#ifndef __APP_DATA_STORE_H__
#define __APP_DATA_STORE_H__

#include "mid_data_store.h"
#include "aes.h"
//#include "app_drivetime.h"

typedef struct{
	char data[AES_BLOCKSIZE];
	U16 crc;
}T_AES_DATA;

typedef struct {
	T_AES_DATA 	odo;	/* ODO总里程 */
	T_AES_DATA	trip1;			/* 小计里程 1 */
	T_AES_DATA	trip2;			/* 小计里程 2 */
	T_AES_DATA	service_mile;	/* 下一次保养的里程 */
	U8		activated;	/* 仪表是否被激活 */
	U8		service_num;	/* 保养次数计数器，首保=1， 二保=2， 三保=3 */
	U8		car_num;		/* 车型代码 */
	U8		none_display_info;	/* 行车信息中，四选三标志位，记录不显示信息标志 */
	U8		display_mode;		/* 当前显示模块标志位，行车信息、胎压和Logo三选一 */
	U16		fuel_resistor;		/* 上一次熄火时邮箱的平均阻值 */
	U32		afe_fuel;			/* 平均油耗清零后的总油耗, Average Fuel Economy   */
	U32		afe_fuel_negation;	/* afe_fuel取反 */
	U32		afe_distance;		/* 平均油耗清零后的行驶里程 */
	U32		afe_distance_negation;	/* afe_distance取反 */
	
	U8 		hour;
	U8 		min;			/* 行驶时间 */
	U8		themeID;		/* 界面主题ID */
}T_DATA_STORE;

/* 总里程 */
U8 write_total_trip(U32 trip);
U8 read_total_trip(U32 *odo);
/* 小计里程1 */
U8 read_sub_trip1(U32 *odo);
U8 write_sub_trip1(U16 trip);
/* 小计里程2 */
U8 read_sub_trip2(U32 *odo);
U8 write_sub_trip2(U16 trip);
/* 下一次保养里程 */
U8 read_service_mile(U32 *trip);
U8 write_service_mile(U32 trip);
/* 保养次数 */
U8 read_service_num();
U8 write_service_num(U8 num);
/* 系统是否已经激活 */
U8 read_activation_flag(U8 *flag);
U8 write_activation_flag(U8 flag);
/* 车型代码 */
U8 read_car_num(U8 *flag);
U8 write_car_num(U8 num);
/* AFE fuel */
U8 read_afe_fuel(U32 *fuel);
U8 write_afe_fuel(U32 fuel);
/* AFE distance */
U8 read_afe_distance(U32 *distance);
U8 write_afe_distance(U32 distance);
/* none display info */
U8 read_none_display_info(U8 *info);
U8 write_none_display_info(U8 info);
/* display mode, 显示模式 */
U8 read_display_mode(U8 *info);
U8 write_display_mode(U8 info);
/* 行驶时间 */
U8 read_drive_time(U8 *hour, U8 *min);
U8 write_drive_time(U8 hour, U8 min);

/* 瞬时油耗，平均油耗 */
U8 read_fuel_consume(U16 *inst_fule, U16 *avg_fule);
U8 write_fuel_consume(U16 inst_fule, U16 avg_fule);

/* 界面主题ID */
U8 read_theme_id(U8 *id);
U8 write_theme_id(U8 id);

/* 油箱电阻值 */
U8 read_fuel_tank_resistor(U16 *res);
U8 write_fuel_tank_resistor(U16 res);

U8 erase_mData();


#endif

