#ifndef __MID_SERIALPORT_H__
#define __MID_SERIALPORT_H__

#include "hal_uart.h"

typedef enum
{
	tty_Data = 1,
	tty_Console = 0
}serialPort_dev_type_t;

typedef struct
{
	unsigned char serialportType;
	hal_serialPort_dev_t serialport;
}serialPort_dev_t;


unsigned char mid_Serialport_Init(serialPort_dev_t *device);

#endif


