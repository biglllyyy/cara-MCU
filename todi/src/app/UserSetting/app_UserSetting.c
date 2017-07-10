#include <string.h>
#include "app_UserSetting.h"
#include "app_data_store.h"

UserSetting_T UserSetting;

static void get_ee_user_setting(void)
{
	read_none_display_info(&UserSetting.not_disp_info_setting);
	read_display_mode(&UserSetting.disp_mode_setting);
	
	/* 如果是非法结果，就写一个进去 */
	if(UserSetting.not_disp_info_setting > NOT_DISP_INFO_SETTING_MAX) // || \
		//(UserSetting.not_disp_info_setting < NOT_DISP_INFO_SETTING_MIN)) 
	{
		UserSetting.not_disp_info_setting = NOT_DISP_INFO_SETTING_DEFAULT;
		write_none_display_info(UserSetting.not_disp_info_setting);
	}
	
	if((UserSetting.disp_mode_setting > DISP_MODE_SETTING_MAX) || (UserSetting.disp_mode_setting < DISP_MODE_SETTING_MIN))
	{
		UserSetting.disp_mode_setting = DISP_MODE_SETTING_DEFAULT;
		write_display_mode(UserSetting.disp_mode_setting);
	}
}

static void save_ee_user_setting(void)
{
	write_none_display_info(UserSetting.not_disp_info_setting);
	write_display_mode(UserSetting.disp_mode_setting);
}

void app_user_setting_init(void)
{
	memset(&UserSetting,0,sizeof(UserSetting));
	get_ee_user_setting();
}

void app_user_setting_save(void)
{
	save_ee_user_setting();
}
