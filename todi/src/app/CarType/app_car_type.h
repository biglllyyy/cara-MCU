#ifndef __APP_CAR_TYPE_H__
#define __APP_CAR_TYPE_H__

#include "comm_typedef.h"
#include "mid_dbg.h"

/* ��¼���ͼ�Э���Ӧ��ϵ�Ľṹ */
typedef struct{
	U8 type;
	U8 protocol;
}T_CAR_TYPE_PROTOCOL;


U8 app_get_protocol(U8 car_type);
#endif
