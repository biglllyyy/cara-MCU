#ifndef _APP_CAR_KEY_H
#define _APP_CAR_KEY_H

#include "comm_typedef.h"

typedef enum
{
	IGN_OFF=0,
	IGN_ON
}Ign_sts_e;

typedef enum
{
	KEY_OFF=0,
	KEY_ACC=1,
	KEY_ON=2,
	KEY_START=3
}Key_sts_e;

typedef enum
{
	KEY_OUT=0,
	KEY_IN
}Key_pos_e;

//extern Ign_sts_e app_get_ign_sts(void);
extern Key_sts_e  app_get_car_key_sts(void);
extern Key_pos_e app_get_car_key_pos(void);

#endif
