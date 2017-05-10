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
	T_AES_DATA 	odo;	/* ODO����� */
	T_AES_DATA	trip1;			/* С����� 1 */
	T_AES_DATA	trip2;			/* С����� 2 */
	T_AES_DATA	service_mile;	/* ��һ�α�������� */
	U8		activated;	/* �Ǳ��Ƿ񱻼��� */
	U8		service_num;	/* �����������������ױ�=1�� ����=2�� ����=3 */
	U8		car_num;		/* ���ʹ��� */
	U8		none_display_info;	/* �г���Ϣ�У���ѡ����־λ����¼����ʾ��Ϣ��־ */
	U8		display_mode;		/* ��ǰ��ʾģ���־λ���г���Ϣ��̥ѹ��Logo��ѡһ */
	U16		fuel_resistor;		/* ��һ��Ϩ��ʱ�����ƽ����ֵ */
	U32		afe_fuel;			/* ƽ���ͺ����������ͺ�, Average Fuel Economy   */
	U32		afe_fuel_negation;	/* afe_fuelȡ�� */
	U32		afe_distance;		/* ƽ���ͺ���������ʻ��� */
	U32		afe_distance_negation;	/* afe_distanceȡ�� */
	
	U8 		hour;
	U8 		min;			/* ��ʻʱ�� */
	U8		themeID;		/* ��������ID */
}T_DATA_STORE;

/* ����� */
U8 write_total_trip(U32 trip);
U8 read_total_trip(U32 *odo);
/* С�����1 */
U8 read_sub_trip1(U32 *odo);
U8 write_sub_trip1(U16 trip);
/* С�����2 */
U8 read_sub_trip2(U32 *odo);
U8 write_sub_trip2(U16 trip);
/* ��һ�α������ */
U8 read_service_mile(U32 *trip);
U8 write_service_mile(U32 trip);
/* �������� */
U8 read_service_num();
U8 write_service_num(U8 num);
/* ϵͳ�Ƿ��Ѿ����� */
U8 read_activation_flag(U8 *flag);
U8 write_activation_flag(U8 flag);
/* ���ʹ��� */
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
/* display mode, ��ʾģʽ */
U8 read_display_mode(U8 *info);
U8 write_display_mode(U8 info);
/* ��ʻʱ�� */
U8 read_drive_time(U8 *hour, U8 *min);
U8 write_drive_time(U8 hour, U8 min);

/* ˲ʱ�ͺģ�ƽ���ͺ� */
U8 read_fuel_consume(U16 *inst_fule, U16 *avg_fule);
U8 write_fuel_consume(U16 inst_fule, U16 avg_fule);

/* ��������ID */
U8 read_theme_id(U8 *id);
U8 write_theme_id(U8 id);

/* �������ֵ */
U8 read_fuel_tank_resistor(U16 *res);
U8 write_fuel_tank_resistor(U16 res);

U8 erase_mData();


#endif

