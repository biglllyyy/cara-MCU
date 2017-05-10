#include "Comm_typedef.h"
#include "mid_serialport.h"

unsigned char mid_Serialport_Init(serialPort_dev_t *device)
{
	if(!hal_serialPort_Init(&device->serialport))
		return FALSE;
	if(!hal_serialPort_Open(&device->serialport))
		return FALSE;
	return TRUE;
}





