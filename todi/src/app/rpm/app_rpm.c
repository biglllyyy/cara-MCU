#include "app_rpm.h"
#include "rpm_cfg.h"
#include "app_can.h"
#include <math.h>

U16             g_rpm   = 0;
static U16		rpm_src = 0;
extern  U32	Absolute(S32 x);

void app_init_rpm(void)
{
	rpm_src = 0;
	g_rpm = 0;
}
void app_set_rpm(U16 rpm)
{
	rpm_src = rpm;
}

U16 app_get_rpm(void)
{
	return (g_rpm);
}
void app_process_rpm_task(void)
{
	U16 temp_value = 0;
	static U16 temp_value_bak=0;
	
	if(can_id_111_lost_timecnt>CAN_LOST_TIME)/*CAN��ʧʱ�����2.5s��ת��Ϊ0*/
	{
		g_rpm=rpm_src=temp_value_bak=0;
		return;
	}
	
	//�����ʵ��ת��
	temp_value = rpm_src;
	temp_value>>=3;//����ϵ��0.125
	
	if(temp_value>=RPM_DIS_MAX)//8000ת���ϣ�ֱ����ʾ8000ת
	{
		g_rpm=temp_value_bak=RPM_DIS_MAX;
	}
	else//8000ת���£����˲�����
	{
		//��ֹת����ʾ�Ĳ���
		if(Absolute(temp_value-temp_value_bak)>=50)//ǰ������ת�ٲ�����Χ���ڵ��ڡ�50ʱ��������ʾת��
		{
			g_rpm=temp_value;
			temp_value_bak=temp_value;
		}
		else
		{
			
		}
	}
}
