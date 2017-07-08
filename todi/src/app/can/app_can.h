#ifndef    __APP_CAN_H__
#define    __APP_CAN_H__

#include  "mid_can.h"

#define    MSG_BOX_ALl				23

#define		CAN_LOST_TIME			25

typedef enum
{
	ID_100017EF, //VCU2ICU01
	ID_1801FBEF, //
	ID_1811FBEF, //
	ID_18FF08F2, //
	ID_10F8159E, //BMS STATUS 1
	ID_10F8169E, //BMS STATUS 2
	ID_18F8179E, //BMS STATUS 3
	ID_18F8189E, //BMS STATUS 4
	ID_18F8199E, //BMS STATUS 5
	ID_18F81A9E, //BMS STATUS 6
	ID_18FF12F7, //DCDC21
	ID_18FF0AF8, //OILDCAC21
	ID_18FF0BF8, //OILDCAC22
	ID_18FF0CF9, //AIRDCAC21
	ID_18FF0DF9, //AIRDCAC22
	ID_1801EFA9, //ICS01
	ID_104C1000, //电池电压 100帧报文0x104C19A4-0x104C1A07 16帧电池温度0x104C1A18-0x104C1A24
	CAN_ID_ALL
} MSG_ID_TYPE;

typedef enum
{
	//received frame
	ID_100017EF_period = 100, //VCU2ICU01
	ID_1801FBEF_period = 500, //
	ID_1811FBEF_period = 500, //
	ID_18FF08F2_period = 500, //
	ID_10F8159E_Period = 100, //BMS STATUS 1
	ID_10F8169E_Period = 100, //BMS STATUS 2
	ID_18F8179E_Period = 1000, //BMS STATUS 3
	ID_18F8189E_Period = 1000, //BMS STATUS 4
	ID_18F8199E_Period = 1000, //BMS STATUS 5
	ID_18F81A9E_Period = 1000, //BMS STATUS 6
	ID_18FF12F7_Period = 500, //DCDC21
	ID_18FF0AF8_Period = 500, //OILDCAC21
	ID_18FF0BF8_Period = 500, //OILDCAC22
	ID_18FF0CF9_Period = 500, //AIRDCAC21
	ID_18FF0DF9_Period = 500, //AIRDCAC22
	ID_1801EFA9_Period = 100, //ICS01
	ID_104C1000_Period = 1000,
	//send frame
	ID_1801EF17_Period = 100,
	ID_1802EF17_Period = 500,
	
} CAN_ID_PERIOD;

typedef struct
{
	/*can msg receive*/

	U8 ID_100017EF[8]; //VCU2ICU01
	U8 ID_1801FBEF[8]; //
	U8 ID_1811FBEF[8]; //
	U8 ID_18FF08F2[8]; //
	U8 ID_10F8159E[8]; //BMS STATUS 1
	U8 ID_10F8169E[8]; //BMS STATUS 2
	U8 ID_18F8179E[8]; //BMS STATUS 3
	U8 ID_18F8189E[8]; //BMS STATUS 4
	U8 ID_18F8199E[8]; //BMS STATUS 5
	U8 ID_18F81A9E[8]; //BMS STATUS 6
	U8 ID_18FF12F7[8]; //DCDC21
	U8 ID_18FF0AF8[8]; //OILDCAC21
	U8 ID_18FF0BF8[8]; //OILDCAC22
	U8 ID_18FF0CF9[8]; //AIRDCAC21
	U8 ID_18FF0DF9[8]; //AIRDCAC22
	U8 ID_1801EFA9[8]; //ICS01
	U8 ID_104C1000[8]; //BATTERY V and temp

} can_signal_t;

/*******************************************************************************/
typedef void (*pCanAnalyse)(can_msg_t *msg, can_pro_way_e way);

void app_init_can(void);

void app_can_sent_task(void);

void app_can_lost_time_cnt_100ms(void);

extern can_signal_t can0;

extern U16 can_id_100017EF_lost_timecnt;
extern U16 can_id_1801FBEF_lost_timecnt;
extern U16 can_id_1811FBEF_lost_timecnt;
extern U16 can_id_18FF08F2_lost_timecnt;
extern U16 can_id_10F8159E_lost_timecnt;
extern U16 can_id_10F8169E_lost_timecnt;
extern U16 can_id_18F8179E_lost_timecnt;
extern U16 can_id_18F8189E_lost_timecnt;
extern U16 can_id_18F8199E_lost_timecnt;
extern U16 can_id_18F81A9E_lost_timecnt;
extern U16 can_id_18FF12F7_lost_timecnt;
extern U16 can_id_18FF0AF8_lost_timecnt;
extern U16 can_id_18FF0BF8_lost_timecnt;
extern U16 can_id_18FF0CF9_lost_timecnt;
extern U16 can_id_18FF0DF9_lost_timecnt;
extern U16 can_id_1801EFA9_lost_timecnt;
extern U16 can_id_104C1000_lost_timecnt;


#endif
