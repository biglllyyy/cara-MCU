#ifndef __MID_DBG_H__
#define __MID_DBG_H__

#include "mid_serialport.h"
#include "hal_uart.h"



void mid_Console_Init(serialPort_dev_t *device);
void Console(const char*fmt,...);

//void dbg_string(const char*fmt,...);

#endif
