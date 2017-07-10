#include <stdarg.h>
#include <stdlib.h>
#include <stdio.h>
#include "mid_dbg.h"


static char dbg_buf[256];
static hal_serialPort_dev_t *Console_dev;
/* initialize reserved UART for debugging */
void mid_Console_Init(serialPort_dev_t *device)
{
	//uart_open(UART_DEBUG_CHN,115200, 8, 1, 0);
	hal_serialPort_Init(&device->serialport);
	hal_serialPort_Open(&device->serialport);
	Console_dev = &(device->serialport);
}

void Console(const char*fmt,...)
{
	va_list vp;
	
	va_start(vp, fmt);
	vsprintf(dbg_buf, fmt, vp);
	va_end(vp);
	
	//uart_write(UART_DEBUG_CHN, dbg_buf);

	hal_serialPort_Write(Console_dev,dbg_buf);
	
	printf("%s", dbg_buf);
}
