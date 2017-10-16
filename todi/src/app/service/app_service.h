#ifndef SERVICE_H
#define SERVICE_H
#include "comm_typedef.h"
#include "app_data_store.h"

#define FIRST_SERVICE_NUM		1
#define SECOND_SERVICE_NUM 	2
#define THIRD_SERVICE_NUM  	3


typedef struct{
	U8 	service_num;
	U8 	service_flag;
	U32	service_mile;
	U32	current_mile;
	U32 rest_service_mile;
}CarService_T;

extern CarService_T CarService;
//extern U8 CarServiceFlag;
void app_service_init(void);
void app_get_rest_service_task_1000ms(void);

void app_service_clean_service();


#endif
