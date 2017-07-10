#ifndef __G_VARIABLE_H__
#define __G_VARIABLE_H__

#include "comm_typedef.h"


/*CAN LOST */
typedef struct
{
    U8  all_lost_flg;
    U8  id_lost_cnt;
    U16 all_lost_keep_time;
} t_CAN_LOST_CNT;
/********gpio********/
extern U8  g_u8InputIoCount;				/* 用于记录输入io口数量，防止输入IO配置遗漏 */
extern U8  g_u8OutputIoCount;				/* 用于记录输出io口配置，防止输出IO配置遗漏 */
extern const U32 BitMask32[32];
extern U8  g_u8IgnSts;
extern U32  g_u32_utcTime;

/********************app power module start*****************************************/
//extern U16 g_u16_led_check_times;			/* led check time */
extern U16 g_u16_ig_off_time;				/* ig off time */
extern U8  g_u8IgnWakeUpFlg;				/* ign wake up flg */
extern U8  g_u8CanWakeUpFlg;				/* can wake up flg */
extern U8  g_u8LinWakeUpFlg;				/* lin wake up flg */
extern U8  g_u8_A20_power_sts;				/* A20 POWER sts */
extern U8  g_u8_uart_req_bak;				/* uart req sts bak, used ad trigger */
extern U8  g_u8_power_down_req;			    /* MCU req to power down A20 power */
extern U16  g_u16_wait_A20_down_time;		/* wait A20 down time */
extern U16 g_u16_cmd_close_calculate;
extern U8  g_u8_A20_update_sts;
/********************app power module end*******************************************/
extern t_CAN_LOST_CNT  can_lost;			/* can lost time count */
/*********************************************************************************/
//extern U8 g_backlight_level_percent;					/* 0%-100% */
extern U8 g_over_speed_value;
extern U8 g_over_waring_sts;
extern U8 g_U8_core_start_work;			/* core start work */
extern void variable_init(void);
extern U8 get_g_u8IgnSts(void);
#endif

