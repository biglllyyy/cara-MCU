#include "Mid_can.h"

#include "hal_gpio.h"
#include "mid_pin.h"
/**
  ******************************************************************************
  * @file    mid_can.c
  * @author  voyager1128
  * @version V1.0.0
  * @date    2017-8-4
  * @brief   ���ļ�Ϊcan�м����������Ҫ��can�շ����������ã��Լ�����can������
  *            
  ******************************************************************************
  * @attention
    һ����ʼ������
    	1.����can��Ϣ�ڵ�
    	2.��ʼ��can����
    	3.��ʼ��can��ض˿�
    	4.��ʼ��can������
    ����ע������
    	�ڵ��� @mid_can_init ����ǰ��Ҫ�ȵ��� @hal_can_prepare
  ******************************************************************************
  */ 
//!<�ο�can TJA1043�����ֲ�
typedef enum
{

	CAN_CTR_EN,
	CAN_CTR_STB,
	CAN_CTR_WAKE,
	CAN_CTR_ERR,
	CAN_CTR_ALL,
}CAN_CTR_NAME_e;


/* ����can�������˿� */
static gpio_dev_t gpio_can0[CAN_CTR_ALL] = 
{
	{
		"CAN_CTR_EN",
		9,
		4,
		IO_GENERAL_OUTPUT,
		VALID_HIGH_LEVEL
	},
	{
		"CAN_CTR_STB",
		9,
		7,
		IO_GENERAL_OUTPUT,
		VALID_HIGH_LEVEL
	},
	{
		"CAN_CTR_WAKE",
		9,
		2,
		IO_GENERAL_OUTPUT,
		VALID_HIGH_LEVEL
	},
	{
		"CAN_CTR_ERR",
		9,
		3,
		IO_INPUT,
		VALID_HIGH_LEVEL
	},

};
/**
  * @brief  ������ģʽѡ��
  * @param  io STB����
  * @param  en en����
  * @param  mode ������ģʽ ȡֵ�ο�@can_mode_t
  * @retval None
  */

void can_chip_mode_switch(gpio_dev_t *io, gpio_dev_t *en, can_mode_t mode)
{	
	switch(mode)
	{
		case NORMAL_MODE:
			mid_set_io_sts(io,ON);
			mid_set_io_sts(en,ON);
			break;
		case STANDBY_MODE:
			mid_set_io_sts(io,OFF);
			mid_set_io_sts(en,OFF);
			break;
		case GO_TO_SLEEP_MODE:
			mid_set_io_sts(io,OFF);
			mid_set_io_sts(en,ON);
			break;
		default:break;
	}
}


/**
  * @brief  can���������ų�ʼ��
  * @param  chn �������˿�ѡ��
  * @retval None
  */

void mid_can_io_init(U8 chn)
{
	switch(chn)
	{
		case CAN_CHN:
				//!<can tx rx ���ù��ܳ�ʼ������ hal_gpio_Init()����������
				mid_GPIO_Init(gpio_can0,	CAN_CTR_ALL);
				break;
		default:break;
	}
}



/**
  * @brief  can��������ʼ��
  * @param  chn �������˿�ѡ��
  * @param  can_rx_handle ���ݽ��մ�����
  * @retval None
  */
void mid_can_init(U8 chn,pCanAnalyse can_rx_handle)
{
/**
  * @TODO: ��ʼ��can�˿�
  * @{
  */
	mid_can_io_init(chn);
	hal_can_init(chn,can_rx_handle);
	can_chip_mode_switch(&gpio_can0[CAN_CTR_STB],&gpio_can0[CAN_CTR_EN],NORMAL_MODE);//ѡ���׼ģʽ
	mid_set_io_sts(&gpio_can0[CAN_CTR_WAKE],ON);
/**
  * @}
  */
    wdg_feed();
}









