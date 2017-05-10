/* ���ſ��޸��ļ�,����ѭ������ʽ�޸�,��ֹhal�� */
/* �������bootloader���mid��hal�㣬���д�����֤���ύ06-software�ֿ� */

#include "boot.h" 			/* ���ڻ�ȡFLASH_UPDATE_STS_ADDRESS */
#include "Comm_typedef.h" 	/* ���ڵ��ñ������� */
#include "open.h"	 		/* ��������������Ϣ */
#include "mid_lcd.h"		/* �������ö�LCD������PWMƵ�� */
#include "mid_dbg.h" 		/* ���������ӡ��Ϣ */


/* ����ȫ��˽��,�������static�ؼ��� */

/* GPIO���豸��index */
typedef enum 
{
	Key0 = 0,
	Key1 = 1,
	Key2 = 2,
	BL_POWER_CTRL = 3,
	BL_GMA_SD = 4,
	IO_MCU_CTL_START_UP = 5,
	IO_MCU_CTL_STOP_UP = 6
}OPEN_GPIO_GROUP_INDEX; // �����ҳ���Ӧ��GPIO�豸

/* ����GPIO���豸��OPEN_IO_GROUP_SIZE ������open_io_group��Size��ͬ��
	��ͬ�ᵼ�±������ʼ������ȫ */
static gpio_dev_t open_io_group[OPEN_IO_GROUP_SIZE] = 
{
	{
		"Key0",
		12,
		0,
		IO_INPUT,
		VALID_LOW_LEVEL
	},
	{
		"Key1",
		3,
		5,
		IO_INPUT,
		VALID_LOW_LEVEL
	},
	{
		"Key2",
		7,
		5,
		IO_INPUT,
		VALID_LOW_LEVEL
	},
	{
		"BL_POWER_CTRL",
		15,
		3,
		IO_GENERAL_OUTPUT,
		VALID_HIGH_LEVEL
	},
	{
		"BL_GMA_SD",
		8,
		1,
		IO_GENERAL_OUTPUT,
		VALID_HIGH_LEVEL
	},
	{
		"IO_MCU_CTL_START_UP", 	//IO����
		6,					  	//IO Port 
		7,						//IO Bit
		IO_GENERAL_OUTPUT,		//IO����
		OFF						//IO��ʼ��״̬
	},
	{
		"IO_MCU_CTL_STOP_UP",
		2,
		4,
		IO_INPUT,
		OFF
	}
};




#pragma segment CODE=CODE_CAN_Boot, attr=CODE /* �����´���ŵ�RAM������ֹ����ROM�������� */


/* 	�������ý�����������׼���ĳ�ʼ��������
 *	VCC_4V2����,IGN ���ţ�MCU_HOLD_ON���ţ�
 *	��Bootloader���г�ʼ����������open_io_groupָ����
 *	Open_GPIO_Init ����Open_GPIO_Init�ĵ�ַ
 */ 
gpio_dev_t * Open_GPIO_Init(void)
{	//����Todi������а����򱳹�ĳ�ʼ��,����GPIO�豸��ĵ�ַ
	return open_io_group;
}



/* �ж��Ƿ��������
 * ����ֵ: ������������   1.
 *		   �������������� 0.
 */
unsigned char Open_Upgrade_Condition_Judgment(void)
{
	if(0x55AA != *(unsigned short*)FLASH_UPDATE_STS_ADDRESS)
	{
		Console("Not a valid firmware, force to upgrade\n");
		return 1;
	}				
	if(mid_get_io_sts(&open_io_group[Key0]) == OFF)
	{
		Console("Upgrade key pressed\n");
		return 1;
	}	
	return 0;
}


/* 	�������ܣ���ȡ����IO��־����������״̬
 * 	����upgrade_flag���Ƿ����������У�1Ϊ��������״̬��0��δ��������״̬
 * 	����ֵ Ҫ����update_states 6(��ȷ������ʧ��)����������1������δ��ʼ����Ҫ��������0
 *	δ֪״̬����2
 */ 
unsigned char Open_Upgrade_reset(unsigned char upgrade_flag)
{
	unsigned char result = 2; //�˴�Ĭ��״̬����Ϊ ��Ϊ0��1����
		// �������,CORE PIN�ø� 
	if(upgrade_flag == 1)
	{
		if (ON == mid_get_io_sts(&open_io_group[IO_MCU_CTL_STOP_UP]) && (ON == open_io_group[IO_MCU_CTL_STOP_UP].value))
		{
			mid_set_io_sts(&open_io_group[IO_MCU_CTL_START_UP], OFF);
			result = 1;
		}
	}
	else
	{
		if (ON == mid_get_io_sts(&open_io_group[IO_MCU_CTL_STOP_UP]))
			result = 0;
	}
	return result;
}


/* ��������ǰ�ĳ�ʼ�� 
   ��ڲ�����������ʱ�ĺ���
 */
void Open_Upgrade_Init(void (*timeDelay)(unsigned int))
{
	/* �˴����д򿪱�����ʱǰ�Ĳ��� */
	mid_set_io_sts(&open_io_group[IO_MCU_CTL_START_UP], ON);
	mid_set_io_sts(&open_io_group[BL_GMA_SD], ON); 			

	/* �˴����ô򿪱������ʱ */
	timeDelay(LIGHT_LCD_DELAY_TIME);
				
	/* �˴����д򿪱�����ʱ��Ĳ��� */
	mid_set_io_sts(&open_io_group[BL_POWER_CTRL], ON); 

	/* �˴����ñ����PWMƵ�� */
	mid_set_lcd_frq(LCD_BL_PWM_CHN,LCD_BACKLIGHT_FRQ,80 * 0xff / 100);
}





