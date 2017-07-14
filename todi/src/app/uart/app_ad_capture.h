#ifndef APP_AD_CAPTURE
#define APP_AD_CAPTURE

#include "comm_typedef.h"

typedef enum
{
	MCU_SER1,
	MCU_SER2,
	MCU_SER3,
	MCU_SER4,
	MCU_SER5,
	MCU_SER6,
	MCU_SER_ALL
}MCU_SER_ENUM;



typedef struct
{
	U8 mcu_ser_index;
	U8 ser_ad_channel;
}Ad_Capture_Info;

const Ad_Capture_Info ad_cap_info_arr[MCU_SER_ALL];
void ad_capture_info_init(void);
void ad_capture_info_get_data(void);


#endif

