#ifndef __APP_RPM_H__
#define __APP_RPM_H__

#include "comm_typedef.h"
void app_init_rpm(void);
U16 app_get_rpm(void);
void app_set_rpm(U16 rpm);

void   app_process_rpm_task(void);

#endif
