#include "carTemp.h"
#include "app_can.h"
#include "app_IPconfig.h"
#include "mid_dbg.h"

static unsigned short outCarTemp;
static unsigned short inCarTemp;

unsigned short app_get_outCarTemp(){
	
	/*如果数据为0，也做无效处理*/
	if(0 == outCarTemp)
		outCarTemp = 255;

	return outCarTemp;
}

unsigned short app_get_inCarTemp(){

	/*如果数据为0，也做无效处理*/
	if(0 ==inCarTemp)
		inCarTemp = 255;
	
	return inCarTemp;
}


void app_CarTemp_init(){
	
	outCarTemp = 0xFF;
	 inCarTemp = 0xFF;
}

/*两块中控屏，如果任一ID的数据正确，则取正确的数据*/
static void app_get_outCar_temp(){/*
	if(can_id_08B_lost_timecnt < 50 || \
		can_id_485_lost_timecnt< 50)
	{
		switch(IPconfig.protocol){		
			case HAVAL_2013_2P4L_AT_2WD:
				if(can0.id_08B.outCarTemp != 0)
				{
					outCarTemp = can0.id_08B.outCarTemp;
					break;
				}
				if(can0.id_485.outCarTemp != 0)
				{
					outCarTemp = can0.id_485.outCarTemp;
					break;
				}
				outCarTemp = 0;
				break;
			case HAVAL_2013_1P5T_MT_2WD:
				if(can1.id_08B.outCarTemp != 0)
				{
					outCarTemp = can1.id_08B.outCarTemp;
					break;
				}
				if(can1.id_485.outCarTemp != 0)
				{
					outCarTemp = can1.id_485.outCarTemp;
					break;
				}
				outCarTemp = 0;
				break;
			case HAVAL_2012_1P5T_MT_2WD:
				if(can2.id_08B.outCarTemp != 0)
				{
					outCarTemp = can2.id_08B.outCarTemp;
					break;
				}
				if(can2.id_485.outCarTemp != 0)
				{
					outCarTemp = can2.id_485.outCarTemp;
					break;
				}
				outCarTemp = 0;
				break;
			case HAVAL_2015_1P5T_MT_2WD: 
			case HAVAL_2015_1P5T_6AT_2WD:
				if(can3.id_08B.outCarTemp != 0)
				{
					outCarTemp = can3.id_08B.outCarTemp;
					break;
				}
				if(can3.id_485.outCarTemp != 0)
				{
					outCarTemp = can3.id_485.outCarTemp;
					break;
				}
				outCarTemp = 0;
				break;
			case HAVAL_2011_2P0T_MT_2WD:
				if(can4.id_08B.outCarTemp != 0)
				{
					outCarTemp = can4.id_08B.outCarTemp;
					break;
				}
				if(can4.id_485.outCarTemp != 0)
				{
					outCarTemp = can4.id_485.outCarTemp;
					break;
				}
				outCarTemp = 0;
				break;
			case HAVAL_2013_1P5T_MT_4WD:
				if(can5.id_08B.outCarTemp != 0)
				{
					outCarTemp = can5.id_08B.outCarTemp;
					break;
				}
				if(can5.id_485.outCarTemp != 0)
				{
					outCarTemp = can5.id_485.outCarTemp;
					break;
				}
				outCarTemp = 0;
				break;		
			default:
				dbg_string("IPconfig.protocol ERR :outCar_temp \n");
			break;
		}
	}
	else
	{
		默认发0xff，让UI显示--.-℃
		outCarTemp = 0xFF; 
	}
*/}

static void app_get_inCar_temp(){/*
	
	if(can_id_08B_lost_timecnt < 50 || \
		can_id_485_lost_timecnt)
	{
		switch(IPconfig.protocol){		
			case HAVAL_2013_2P4L_AT_2WD:
				if(can0.id_08B.inCarTemp != 0)
				{
					inCarTemp = can0.id_08B.inCarTemp;
					break;
				}
				if(can0.id_485.inCarTemp != 0)
				{
					inCarTemp = can0.id_485.inCarTemp;
					break;
				}
				inCarTemp = 0;
				break;
			case HAVAL_2013_1P5T_MT_2WD:
				if(can1.id_08B.inCarTemp != 0)
				{
					inCarTemp = can1.id_08B.inCarTemp;
					break;
				}
				if(can1.id_485.inCarTemp != 0)
				{
					inCarTemp = can1.id_485.inCarTemp;
					break;
				}
				inCarTemp = 0;
				break;
			case HAVAL_2012_1P5T_MT_2WD:
				if(can2.id_08B.inCarTemp != 0)
				{
					inCarTemp = can2.id_08B.inCarTemp;
					break;
				}
				if(can2.id_485.inCarTemp != 0)
				{
					inCarTemp = can2.id_485.inCarTemp;
					break;
				}
				inCarTemp = 0;
				break;
			case HAVAL_2015_1P5T_MT_2WD: 
			case HAVAL_2015_1P5T_6AT_2WD:
				if(can3.id_08B.inCarTemp != 0)
				{
					inCarTemp = can3.id_08B.inCarTemp;
					break;
				}
				if(can3.id_485.inCarTemp != 0)
				{
					inCarTemp = can3.id_485.inCarTemp;
					break;
				}
				inCarTemp = 0;
				break;
			case HAVAL_2011_2P0T_MT_2WD:
				if(can4.id_08B.inCarTemp != 0)
				{
					inCarTemp = can4.id_08B.inCarTemp;
					break;
				}
				if(can4.id_485.inCarTemp != 0)
				{
					inCarTemp = can4.id_485.inCarTemp;
					break;
				}
				inCarTemp = 0;
				break;
			case HAVAL_2013_1P5T_MT_4WD:
				if(can5.id_08B.inCarTemp != 0)
				{
					inCarTemp = can5.id_08B.inCarTemp;
					break;
				}
				if(can5.id_485.inCarTemp != 0)
				{
					inCarTemp = can5.id_485.inCarTemp;
					break;
				}
				inCarTemp = 0;
				break;		
			default:
				dbg_string("IPconfig.protocol ERR :inCar_temp \n");
			break;
		}
	}
	else
	{
		默认发0xff，让UI显示--.-℃
		inCarTemp = 0xFF; 
	}
*/}

void app_get_CarTemp_5000ms(){

	app_get_outCar_temp();
	app_get_inCar_temp();
}
