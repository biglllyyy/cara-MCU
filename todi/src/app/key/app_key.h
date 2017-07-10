#ifndef _KEY_H
#define _KEY_H

#include "comm_typedef.h"

#define KEY_NUM_MAX 3
#define CONST_TIME_200MS 2//单位:100ms
#define CONST_TIME_10S 100//单位:100ms
#define CONST_SHORT_PRESSED_TIME	3//单位:100ms
#define CONST_LONG_PRESSED_TIME		100//单位:100ms
#define CONST_TIME_3S		 30//单位：100ms
#define CONST_TIME_5S		 50//单位：100ms
#define LEFT  			0	
#define RIGHT				1
#define CONFIRM			2

typedef enum
{
	NOT_PRESSED=0,
	PRESSED=1
}key_status_e;

typedef enum
{
	NO_PRESSED=0,
	SHORT_PRESSED,
	LONG_PRESSED
}key_handle_status_e;

typedef struct
{
	key_status_e status_in_from_pin;
	key_status_e status_out_to_core;
	key_handle_status_e status_handle;
	U8 time_cnt;
}key_t;

//对外的接口
extern key_t key[KEY_NUM_MAX];
extern void app_key_init(void);
extern void app_key_scan_task_100ms(void);


#endif//#ifndef _KEY_H
