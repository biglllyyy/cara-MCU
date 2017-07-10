#ifndef __APP_POWER_H__
#define __APP_POWER_H__
#include "hal_gpio_cfg.h"
#include "mid_pin.h"
#include "io_cfg.h"

#define A20_POWER_ON()		mid_set_io_sts(&VCC_4V2_EN,ON);
#define A20_POWER_OFF()		mid_set_io_sts(&VCC_4V2_EN,OFF);

extern U8   g_u8_pre_sleep_flag;


typedef enum
{
	ENTER,
	KEEP,
	LEAVE,
} STATE_ACTION;

typedef enum
{
	PWR_NML = 0,            /* 正常状态 */
	PWR_CHECK = 1,			/* 自检完成  */
	PWR_PRE_SLEEP = 2,      /* 与睡眠状态，此状态下，核心板断电,IG OFF */
	PWR_OFF = 3,            /* 电源关闭状态 */
	PWR_SLEEP = 4,          /* 睡眠状态 */	
	PWR_ALL =5
} SYS_POWER_STATE;

enum
{
    VECHILE_MODE_DRVING = 1,
    VECHILE_MODE_CHARGING = 2
};

typedef void (*state_handle)(STATE_ACTION action);

void keep_state(void);
void first_state(SYS_POWER_STATE state);
SYS_POWER_STATE app_get_power_sts(void);
void app_A20_power_off(void);
void app_A20_power_on(void);
void app_power_manager_task10(void);
void reboot_ARM(void);




#endif
