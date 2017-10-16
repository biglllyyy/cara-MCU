#ifndef _APP_UDS_H
#define _APP_UDS_H
#include "comm_typedef.h"

typedef struct {
U8 len;
U8 id;
U8 data[6];

}T_UDS;

extern T_UDS uds_recv;
extern T_UDS uds_sent;

void app_uds_task();



#endif
