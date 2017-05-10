#ifndef __MID_TRIP_H
#define __MID_TRIP_H

#include "Comm_typedef.h"
#include "crc16.h"
#include "aes.h"
#include "mid_string.h"
#include "g_variable.h"
#include "mid_dbg.h"
#include "mid_filter.h"
#include "hal_wdg.h"

#define   ONE_KILOMETER				(1000)
#define   TWO_KILOMETER				(ONE_KILOMETER*2)
#define   ONE_HUNDRED				(100)

#define	  MAX_SHIFT_ENLARGE         16
#define   MAX_SUB_TRIP							9999
#define   MAX_SUM_ODO								999999
#define   MAX_REST_MILE							999
#define   CAL_PERIOD_MS             100
#define   ODO_CLEAR_TIMES_CNT        6


typedef struct  
{
	U8   odo_aes[AES_BLOCKSIZE];
}eeprom_t;

typedef struct{
	eeprom_t  data; 
	U16		  crc;
}eeprom_data_t;

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


#endif
