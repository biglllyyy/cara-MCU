#ifndef APP_SYSTEM_H_
#define APP_SYSTEM_H_

#include "comm_typedef.h"

extern U16 fuel_task_flag;

void app_sys_init(void);
void app_igon_action(void);
void app_igoff_action(void);
void app_soft_reset();

#endif /* APP_SYSTEM_H_ */
