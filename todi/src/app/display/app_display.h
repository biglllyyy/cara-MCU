#ifndef _APP_DISPLAY_H_
#define _APP_DISPLAY_H_

typedef enum
{
	MODE_NORMAL = 0,
	MODE_DISPLAY,
	MODE_PROJECT,
	MODE_ALL
}E_SCREEN_MODE;

E_SCREEN_MODE get_screen_mode();
void judge_which_screen_mode();

void app_display_rand_data_task(void);
void app_mcu_core_general_demo(void);
void app_mcu_core_special_demo(void);

#endif
