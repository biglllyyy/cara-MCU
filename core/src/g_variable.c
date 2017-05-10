#include "g_variable.h"
#include "comm_typedef.h"

/****************comm  variable ***********************************************/
U8  g_u8InputIoCount = 0;				/* 用于记录输入io口数量，防止输入IO配置遗漏 */
U8  g_u8OutputIoCount = 0;				/* 用于记录输出io口配置，防止输出IO配置遗漏 */
U8  g_u8IgnSts = OFF;					/* IGN 状态 */
//------------------------------------------------------------------------------------
const U32 BitMask32[32] =
{
    0x1u,		0x3u,		0x7u,		0xFu,
    0x1Fu,		0x3Fu,		0x7Fu,		0xFFu,
    0x1FFu,		0x3FFu,		0x7FFu,		0xFFFu,
    0x1FFFu,	0x3FFFu,	0x7FFFu,	0xFFFFu,
    0x1FFFFu,	0x3FFFFu,	0x7FFFFu,	0xFFFFFu,
    0x1FFFFFu,	0x3FFFFFu,	0x7FFFFFu,	0xFFFFFu,
    0x1FFFFFFu,	0x3FFFFFFu,	0x7FFFFFFu,	0xFFFFFFu,
    0x1FFFFFFFu,0x3FFFFFFFu,0x7FFFFFFFu,0xFFFFFFFFu,
};
/*********************************************************************************/

/********************module start**************************************************/
U8  g_turn_beep_flg = 0;				/* 转向灯报警设置 */
U8  g_u8_led_double_blink_sts = 0;		/* double blink sts */
/********************module end*****************************************************/

/********************app power module start*****************************************/
//U16 g_u16_led_check_times = 0;			/* led check time */
U16 g_u16_ig_off_time = 0;				/* ig off time */
U8  g_u8IgnWakeUpFlg = 0;				/* ign wake up flg */
U8  g_u8CanWakeUpFlg = 0;				/* can wake up flg */
U8  g_u8LinWakeUpFlg = 0;				/* lin wake up flg */
U8  g_u8_A20_power_sts = 0;
U8  g_u8_uart_req_bak = 0;				/* uart req sts bak, used ad trigger */
U8  g_u8_power_down_req = 0;			/* MCU req to power down A20 power */
U16  g_u16_wait_A20_down_time = 0;		/* wait A20 down time */
U32  g_u32_utcTime = 0;					/* UTC time */
U8   g_u8_A20_update_sts = 0;			/* a20 update flg */
U8   g_U8_core_start_work = 0;			/* core start work */

//U16 g_u16_cmd_close_calculate = 0;
/********************app power module end*******************************************/

t_CAN_LOST_CNT  can_lost= {0};

void variable_init(void)
{
//	g_turn_beep_flg = 0;				/* 转向灯报警设置 */
//	g_u8_led_double_blink_sts = 0;		/* double blink sts */
//  g_u16_led_check_times = 0;			/* led check time */
    g_u16_ig_off_time = 0;				/* ig off time */
    g_u8IgnWakeUpFlg = 0;				/* ign wake up flg */
    g_u8CanWakeUpFlg = 0;				/* can wake up flg */
    g_u8LinWakeUpFlg = 0;				/* Lin wake up flg */
    g_u8_A20_power_sts = 0;
    can_lost.all_lost_flg = 0;
    can_lost.id_lost_cnt = 0;
    can_lost.all_lost_keep_time = 0;
    g_u8_power_down_req = 0;
	g_u8_A20_update_sts = 0;
//	g_u16_cmd_close_calculate = 0;
	g_U8_core_start_work = 0;
}

//U8 g_backlight_level_percent;					/* 0%-100% */
U8 g_over_speed_value = 60;
U8 g_over_waring_sts = 0;	//no active

U8 get_g_u8IgnSts(void)
{
	return g_u8IgnSts;
}
