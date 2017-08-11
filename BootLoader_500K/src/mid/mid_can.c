#include "Mid_can.h"

#include "hal_gpio.h"
#include "mid_pin.h"
/**
  ******************************************************************************
  * @file    mid_can.c
  * @author  voyager1128
  * @version V1.0.0
  * @date    2017-8-4
  * @brief   该文件为can中间层驱动，主要对can收发器进行设置，以及配置can控制器
  *            
  ******************************************************************************
  * @attention
    一、初始化流程
    	1.配置can消息节点
    	2.初始化can缓存
    	3.初始化can相关端口
    	4.初始化can控制器
    二、注意事项
    	在调用 @mid_can_init 函数前需要先调用 @hal_can_prepare
  ******************************************************************************
  */ 
//!<参考can TJA1043数据手册
typedef enum
{

	CAN_CTR_EN,
	CAN_CTR_STB,
	CAN_CTR_WAKE,
	CAN_CTR_ERR,
	CAN_CTR_ALL,
}CAN_CTR_NAME_e;


/* 创建can控制器端口 */
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
  * @brief  控制器模式选择
  * @param  io STB引脚
  * @param  en en引脚
  * @param  mode 控制器模式 取值参考@can_mode_t
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
  * @brief  can控制器引脚初始化
  * @param  chn 控制器端口选择
  * @retval None
  */

void mid_can_io_init(U8 chn)
{
	switch(chn)
	{
		case CAN_CHN:
				//!<can tx rx 复用功能初始化已在 hal_gpio_Init()函数中配置
				mid_GPIO_Init(gpio_can0,	CAN_CTR_ALL);
				break;
		default:break;
	}
}



/**
  * @brief  can控制器初始化
  * @param  chn 控制器端口选择
  * @param  can_rx_handle 数据接收处理函数
  * @retval None
  */
void mid_can_init(U8 chn,pCanAnalyse can_rx_handle)
{
/**
  * @TODO: 初始化can端口
  * @{
  */
	mid_can_io_init(chn);
	hal_can_init(chn,can_rx_handle);
	can_chip_mode_switch(&gpio_can0[CAN_CTR_STB],&gpio_can0[CAN_CTR_EN],NORMAL_MODE);//选择标准模式
	mid_set_io_sts(&gpio_can0[CAN_CTR_WAKE],ON);
/**
  * @}
  */
    wdg_feed();
}









