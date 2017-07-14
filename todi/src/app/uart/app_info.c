#include "app_info.h"
#include "app_moudle.h"
#include "uart_cfg.h"


static float mile = 0;
U16 single_miles = 0;
U32 total_miles = 0;
U8 key_info_value = 0;
U8 save_flag = 0;
U8 CurrentMenu = 0;
FRAMEA20TOMCU_t  g_tUart1Rec = {0};



void cal_info_task_100ms(void)
{
	//�����ۼ����
	if (pSpeed > 0) //���ٴ���0ʱ���ۼ�
	{
		mile = mile + (float) (pSpeed / 3.6 * 0.1);
		
		if (mile >= 100.0) 
		{
			mile = 0;
			save_flag = 1;
			if (single_miles >= 9999) single_miles = 0;
			else single_miles++;
			if (total_miles >= 9999999) total_miles = 0;
			else total_miles++;
		}
	}


#if 0 //С����������߼�
	if (KEY_AV && (KEY_UP == 0)) //���� 2S ���㵥��� ����AV
	{ 
		   if (lpcount >= 200) 
		   {
			   single_miles = 0;
			   save_flag = 1;
			   bkey_AV = 1; //����ʱȡ������������
			   lpcount = 100;
		   } 
		   else 
		   {
				lpcount++;

		   }
	} 
	else if (lpcount > 0) 
	{
		lpcount--;
	}
#endif	
	
			
}

void app_sub_trip_clear(void)
{
	single_miles = 0;
}


