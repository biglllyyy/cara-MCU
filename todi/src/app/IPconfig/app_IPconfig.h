#ifndef _IPCONFIG_H
#define _IPCONFIG_H

#include "comm_typedef.h"

//测试模式
//#define HAVAL_H6_TEST_DOUBLE_PLINTH




/*
//根据串口协议定义
typedef enum
{
	//HAVAL_2013_2P4L_AT_2WD=0, 
	//HAVAL_2013_1P5T_MT_2WD, 
	//HAVAL_2012_1P5T_MT_2WD, 
	//HAVAL_2015_1P5T_6AT_2WD,
	//HAVAL_2011_2P0T_MT_2WD,	
	//HAVAL_2013_1P5T_MT_4WD,
	HAVAL_CAR_TYPE_NUM
}HAVAL_CAR_TYPE_T;
*/

//根据串口协议定义
typedef enum
{
	HAVAL_2011_2P0T_MT_2WD=1,
	HAVAL_2012_1P5T_MT_2WD,
	HAVAL_2013_1P5T_MT_2WD, 
	HAVAL_2013_1P5T_MT_4WD, 
	HAVAL_2013_2P4L_AT_2WD,
	HAVAL_2015_1P5T_MT_2WD,
	HAVAL_2015_1P5T_6AT_2WD,
	HAVAL_CAR_TYPE_NUM
}HAVAL_CAR_TYPE_T;





typedef struct
{
	U8 CarNum;
	HAVAL_CAR_TYPE_T   	protocol;
	U8	 				Sts;
	U32	 				Odo;
	U8 themeID;
}IPconfig_T;

//对外接口
extern IPconfig_T IPconfig;
extern void app_IPconfig_init(void);
extern void app_IPconfig_pro_1000ms(void);

#endif//#ifndef _IPCONFIG_H
