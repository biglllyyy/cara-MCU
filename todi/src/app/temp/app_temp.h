#ifndef   __APP_TEMP_H__
#define   __APP_TEMP_H__

#include "comm_typedef.h"

void app_set_temp(U8 temp);

void app_init_temp(void);
U8 app_get_temp_percent(void);

U8 app_get_temp_value(void);
void app_pro_temp_task(void);

#endif
