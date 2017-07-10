#include <string.h>
#include "app_uds.h"
#include "app_service.h"



#define DiagnosticSessionControl 0x10
#define ECUReset 0x11
#define SecurityAccess 0x27
#define TesterPresent 0x3E
#define ReadDataByIdentifier 0x22
#define WriteDataByIdentifier 0x2E
#define ClearDiagnosticInfo 0x14
#define ReadDTCInfo 0x19
#define InputOutputControlById 0x2F
#define CommControl 0x28
#define ControlDTCSetting 0x85


T_UDS uds_recv;
T_UDS uds_sent;
				  					   
void app_uds_task()
{
	U8 len = uds_recv.len;
	U8 srv_id = uds_recv.id;
	static U8 flag = 0;

	//UDS不发送 则不进行处理
	if(00 ==uds_recv.id) return; 

	uds_sent.id = uds_recv.id + 0x40;
		
	switch(srv_id)
	{
		case DiagnosticSessionControl:
			if(0x02 == len)
			{			
				if(0x01 == uds_recv.data[0])
				{
					uds_sent.data[0] = uds_recv.data[0];
					uds_sent.data[1] = 0x00;
					uds_sent.data[2] = 0x32;
					uds_sent.data[3] = 0x13;
					uds_sent.data[4] = 0x88;
					uds_sent.data[5] = 0x00;
					uds_sent.len = 6;
					flag  = flag | 0x01;
				}
				if(0x03 == uds_recv.data[0])
				{
					uds_sent.data[0] = uds_recv.data[0];
					uds_sent.data[1] = 0x00;
					uds_sent.data[2] = 0x32;
					uds_sent.data[3] = 0x13;
					uds_sent.data[4] = 0x88;
					uds_sent.data[5] = 0x00;
					uds_sent.len = 6;
					flag  = flag | 0x02;
				}
			}		
		break;
		case SecurityAccess:
			if(0x02 == len)
			{
				if(0x01 == uds_recv.data[0])
				{
					uds_sent.data[0] = uds_recv.data[0];
					uds_sent.data[1] = 0xAA;
					uds_sent.data[2] = 0x55;
					uds_sent.data[3] = 0x00;
					uds_sent.data[4] = 0x00;
					uds_sent.data[5] = 0x00;
					uds_sent.len = 4;
					flag  = flag | 0x04;
				}
			}
			if(0x04 == len)
			{
				if(0x02 == uds_recv.data[0])
				{
					//if(0x55==uds_recv.data[1]&&0xAA==uds_recv.data[2])
					//{
						uds_sent.data[0] = uds_recv.data[0];
						uds_sent.data[1] = 0x00;
						uds_sent.data[2] = 0x00;
						uds_sent.data[3] = 0x00;
						uds_sent.data[4] = 0x00;
						uds_sent.data[5] = 0x00;
						uds_sent.len = 2;
						flag  = flag | 0x08;
					//}
				}
			}
			
		break;

		case TesterPresent:
			if(0x02 == len)
			{
				if(0x00 == uds_recv.data[0])
				{
					uds_sent.data[0] = uds_recv.data[0];
					uds_sent.data[1] = 0x00;
					uds_sent.data[2] = 0x00;
					uds_sent.data[3] = 0x00;
					uds_sent.data[4] = 0x00;
					uds_sent.data[5] = 0x00;
					uds_sent.len = 2;
				}
			}
		break;
		case WriteDataByIdentifier:
			if(0x05 == len)
			{
				if(0xFE == uds_recv.data[0])
				{
					uds_sent.data[0] = uds_recv.data[0];
					uds_sent.data[1] = 0x00;
					uds_sent.data[2] = 0x00;
					uds_sent.data[3] = 0x00;
					uds_sent.data[4] = 0x00;
					uds_sent.data[5] = 0x00;
					uds_sent.len = 3;
					if(flag == 0x0F)
					{
						app_service_clean_service(); //保养里程清零标志
						dbg_string("UDS service clean\n");
						flag = 0;
					}
				}
			}
		break;
		
		default :
	//	dbg_string("UDS service id not used \n");
		break;
	}
	memset(&uds_recv,0,sizeof(uds_recv));
	
}




