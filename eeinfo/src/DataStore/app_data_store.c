/*******************************************
 
 数据存储、读取API
 
 *******************************************/
#include "app_data_store.h"

#include <string.h>
#include "mid_trip.h"
#include "crc16.h"
#include "aes.h"
#include "mid_string.h"
#include "g_variable.h"



/* 存储完整的待保存数据 */
static T_DATA_STORE mData;
static U8 first_time = 1;

T_DATA_STORE *get_data_store_ptr(void)
{
	if (first_time)
	{
		first_time = 0;
		if (mid_load_data((U8*)&mData, sizeof(mData)) != 0)
			return 0;
	}
		
	return &mData;
}

/* 把数据从eeprom读出来并解密，正确返回TRUE，错误返回FALSE */
U8  read_total_trip(U32 *odo)
{
	Bool ret = TRUE;
	T_DATA_STORE *sData;
	U16 crc16;
	char *pAes;
	
	sData = get_data_store_ptr();
	if (!sData) return FALSE;
	
	/* CRC校验 */
	crc16 = api_cal_crc16((U8*)sData->odo.data, sizeof(sData->odo.data));
	if (crc16 != sData->odo.crc) return FALSE;
	
	pAes = mid_aes_decrypt(sData->odo.data);	
	*odo = str_to_dec((const U8*)pAes);
	
	return ret;
}

U8  write_total_trip(U32 trip)
{
	U8	ret= TRUE;
	T_DATA_STORE *sData;
	
#ifdef __DEBUG__
	dbg_string("Write ODO:%d\n", trip);
#endif

	if(trip <= MAX_SUM_ODO)
	{
		sData = &mData;
		
		dec_to_str((U8*)sData->odo.data,trip);
		aes_encrypt((U8*)sData->odo.data,(U8*)sData->odo.data);
		/* CRC校验 */
		sData->odo.crc = api_cal_crc16((U8*)sData->odo.data, sizeof(sData->odo.data));
		
		mid_save_data((U8*)sData, sizeof(T_DATA_STORE));
	}
	return ret;
}

/* 把加密后的里程小计数据读出来，并解密，正确返回TRUE，错误返回FAIL */
U8 read_sub_trip1( U32 *odo)
{
	T_DATA_STORE *sData;
	U16 crc16;
	char *pAes;

	sData = get_data_store_ptr();
	if (!sData) return FALSE;
	
	/* CRC校验 */
	crc16 = api_cal_crc16((U8*)sData->trip1.data, sizeof(sData->trip1.data));
	if (crc16 != sData->trip1.crc) return FALSE;

	pAes = mid_aes_decrypt(sData->trip1.data);	
	*odo = str_to_dec((const U8*)pAes);
	
	return TRUE;
}

U8 write_sub_trip1(U16 trip)
{
	U8	ret=TRUE;
	T_DATA_STORE	*sData;
	
#ifdef __DEBUG__
	dbg_string("Write trip1:%d\n", trip);
#endif

	if(trip <= MAX_SUB_TRIP)
	{
		sData = &mData;
		dec_to_str((U8*)sData->trip1.data,trip);
		aes_encrypt((U8*)sData->trip1.data,(U8*)sData->trip1.data);
		/* CRC校验 */
		sData->trip1.crc = api_cal_crc16((U8*)sData->trip1.data, sizeof(sData->trip1.data));
			
		mid_save_data((U8*)sData, sizeof(T_DATA_STORE));
		
	}
	return ret;
}

U8 read_sub_trip2( U32 *odo)
{
	Bool ret = TRUE;
	T_DATA_STORE *sData;
	U16 crc16;
	char *pAes;

	sData = get_data_store_ptr();
	/* CRC校验 */
	crc16 = api_cal_crc16((U8*)sData->trip2.data, sizeof(sData->trip2.data));
	if (crc16 != sData->trip2.crc) return FALSE;
	
	pAes = mid_aes_decrypt(sData->trip2.data);	
	*odo = str_to_dec((const U8*)pAes);
	
	return ret;
}

U8 write_sub_trip2(U16 trip)
{
	U8	ret = TRUE;
	T_DATA_STORE	*sData;

	if(trip <= MAX_SUB_TRIP)
	{
		sData = &mData;
		dec_to_str((U8*)sData->trip2.data,trip);
		aes_encrypt((U8*)sData->trip2.data,(U8*)sData->trip2.data);
		/* CRC校验 */
		sData->trip2.crc = api_cal_crc16((U8*)sData->trip2.data, sizeof(sData->trip2.data));
		
		mid_save_data((U8*)sData, sizeof(T_DATA_STORE));
	}
	return ret;
}
/* 下一次保养里程 */
U8 read_service_mile(U32 *odo)
{
	Bool ret = TRUE;
	T_DATA_STORE *sData;
	U16 crc16;
	char *pAes;

	sData = get_data_store_ptr();
	if (!sData) return FALSE;
	/* CRC校验 */
	crc16 = api_cal_crc16((U8*)sData->service_mile.data, sizeof(sData->service_mile.data));
	if (crc16 != sData->service_mile.crc) return FALSE;
	
	pAes = mid_aes_decrypt(sData->service_mile.data);	
	*odo = str_to_dec((const U8*)pAes);
	
	return ret;
}

U8 write_service_mile(U32 trip)
{
	U8	ret = TRUE;
	T_DATA_STORE	*sData;

	if(trip <= MAX_SUM_ODO)
	{
		sData = &mData;
		dec_to_str((U8*)sData->service_mile.data,trip);
		aes_encrypt((U8*)sData->service_mile.data,(U8*)sData->service_mile.data);
		/* CRC校验 */
		sData->service_mile.crc = api_cal_crc16((U8*)sData->service_mile.data, 
			sizeof(sData->service_mile.data));
		
		mid_save_data((U8*)sData, sizeof(T_DATA_STORE));
	}
	return ret;
}
/* 保养计数 */
U8 read_service_num()
{
	T_DATA_STORE	*sData;
	sData = get_data_store_ptr();
	if (!sData) return FALSE;
	
	return sData->service_num;
}

U8 write_service_num(U8 num)
{
	T_DATA_STORE	*sData;
	sData = get_data_store_ptr();
	if (!sData) return FALSE;
	
	sData->service_num = num;
	mid_save_data((U8*)sData, sizeof(T_DATA_STORE));
	return TRUE;
}

/* 是否已激活标志 */
U8 read_activation_flag(U8 *flag)
{
	T_DATA_STORE	*sData;
	sData = get_data_store_ptr();
	if (!sData) return FALSE;
	
	*flag = sData->activated;
	return TRUE;	
}

U8 write_activation_flag(U8 flag)
{
	T_DATA_STORE	*sData;
	sData = get_data_store_ptr();
	if (!sData) return FALSE;

	sData->activated = flag;
	mid_save_data((U8*)sData, sizeof(T_DATA_STORE));
	return TRUE;	
}

/* 车型代码 */
U8 read_car_num(U8 *flag)
{
	T_DATA_STORE	*sData;
	sData = get_data_store_ptr();
	if (!sData) return FALSE;
	
	*flag = sData->car_num;
	return TRUE;	
}
U8 write_car_num(U8 num)
{
	T_DATA_STORE	*sData;
	sData = get_data_store_ptr();
	if (!sData) return FALSE;

	sData->car_num = num;
	mid_save_data((U8*)sData, sizeof(T_DATA_STORE));
	return TRUE;	
}

/* AFE fuel, 读取成功返回TRUE */
U8 read_afe_fuel(U32 *fuel)
{
	T_DATA_STORE	*sData;
	sData = get_data_store_ptr();
	if (!sData) return FALSE;
	
	/* 做个取补校验 */
	*fuel = sData->afe_fuel;
	if (*fuel + sData->afe_fuel_negation == 0xFFFFFFFF) 	
		return TRUE;
	else
		return FALSE;
}
U8 write_afe_fuel(U32 fuel)
{
	T_DATA_STORE	*sData;
	sData = get_data_store_ptr();
	if (!sData) return FALSE;

	sData->afe_fuel = fuel;
	sData->afe_fuel_negation = 0xFFFFFFFF -  fuel;
	mid_save_data((U8*)sData, sizeof(T_DATA_STORE));
	return TRUE;	
}

/* AFE distance, 读取成功返回TRUE */
U8 read_afe_distance(U32 *distance)
{
	T_DATA_STORE	*sData;
	sData = get_data_store_ptr();
	if (!sData) return FALSE;
	
	/* 做个取补校验 */
	*distance = sData->afe_distance;
	if (*distance + sData->afe_distance_negation == 0xFFFFFFFF) 	
		return TRUE;
	else
		return FALSE;
}
U8 write_afe_distance(U32 distance)
{
	T_DATA_STORE	*sData;
	sData = get_data_store_ptr();
	if (!sData) return FALSE;

	sData->afe_distance = distance;
	sData->afe_distance_negation = 0xFFFFFFFF -  distance;
	mid_save_data((U8*)sData, sizeof(T_DATA_STORE));
	return TRUE;	
}

/* none display info setting */
U8 read_none_display_info(U8 *info)
{
	T_DATA_STORE	*sData;
	sData = get_data_store_ptr();
	if (!sData) return FALSE;
	
	*info = sData->none_display_info;
	return TRUE;
}
U8 write_none_display_info(U8 info)
{
	T_DATA_STORE	*sData;
	sData = get_data_store_ptr();
	if (!sData) return FALSE;

	sData->none_display_info = info;
	mid_save_data((U8*)sData, sizeof(T_DATA_STORE));
	return TRUE;	
}

/* 显示模式 */
U8 read_display_mode(U8 *mode)
{
	T_DATA_STORE	*sData;
	sData = get_data_store_ptr();
	if (!sData) return FALSE;
	
	*mode = sData->display_mode;
	return TRUE;
}
U8 write_display_mode(U8 mode)
{
	T_DATA_STORE	*sData;
	sData = get_data_store_ptr();
	if (!sData) return FALSE;

	sData->display_mode = mode;
	mid_save_data((U8*)sData, sizeof(T_DATA_STORE));
	return TRUE;	
}

/* 行驶时间 */
U8 read_drive_time(U8 *hour, U8 *min)
{
	T_DATA_STORE	*sData;
	sData = get_data_store_ptr();
	if (!sData) return FALSE;

	*hour = sData->hour;
	*min = sData->min;
	
	//dbg_string("[read]hour:%d,min:%d\n", sData->hour, sData->min);
	return TRUE;
}
U8 write_drive_time(U8 hour, U8 min)
{
	T_DATA_STORE	*sData;
	sData = get_data_store_ptr();
	if (!sData) return FALSE;

	sData->hour = hour;
	sData->min = min;
	
//	dbg_string("[write]hour:%d,min:%d\n", sData->hour, sData->min);
	mid_save_data((U8*)sData, sizeof(T_DATA_STORE));
	return TRUE;	
}

/* 界面主题ID */
U8 read_theme_id(U8 *id)
{
	T_DATA_STORE	*sData;
	sData = get_data_store_ptr();
	if (!sData) return FALSE;
	
	*id = sData->themeID;
	return TRUE;
}

U8 write_theme_id(U8 id)
{
	T_DATA_STORE	*sData;
	sData = get_data_store_ptr();
	if (!sData) return FALSE;

	sData->themeID = id;
	mid_save_data((U8*)sData, sizeof(T_DATA_STORE));
	return TRUE;	
}

/* 油箱电阻值 */
U8 read_fuel_tank_resistor(U16 *res)
{
	T_DATA_STORE	*sData;
	sData = get_data_store_ptr();
	if (!sData) return FALSE;
	
	*res = sData->fuel_resistor;
	return TRUE;
}

U8 write_fuel_tank_resistor(U16 res)
{
	T_DATA_STORE	*sData;
	sData = get_data_store_ptr();
	if (!sData) return FALSE;

	sData->fuel_resistor = res;
	mid_save_data((U8*)sData, sizeof(T_DATA_STORE));
	return TRUE;	
}

U8 erase_mData()
{
	memset(&mData,0,sizeof(mData));	
	
	return TRUE;
}
