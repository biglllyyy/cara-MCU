#ifndef _USERSETTING_H
#define _USERSETTING_H

#include "comm_typedef.h"

//���ݴ���Э��������
#define NOT_DISP_INFO_SETTING_MIN 0
#define NOT_DISP_INFO_SETTING_MAX	3
#define NOT_DISP_INFO_SETTING_DEFAULT 3

#define DISP_MODE_SETTING_MIN			1
#define DISP_MODE_SETTING_MAX			3
#define DISP_MODE_SETTING_DEFAULT 3


typedef struct
{
	U8 disp_mode_setting;
	U8 not_disp_info_setting;
}UserSetting_T;

//����ӿ�
extern UserSetting_T UserSetting;
extern void app_user_setting_init(void);
extern void app_user_setting_save(void);

#endif
