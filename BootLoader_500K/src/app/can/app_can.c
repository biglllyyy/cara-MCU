/**
  ******************************************************************************
  * @file    app_can.c
  * @author  voyager1128
  * @version V1.0.0
  * @date    2017-8-4
  * @brief   can Ӧ�ò����
  *            
  ******************************************************************************
  * @attention
      can���ò�ѯ��ʽ���������ж�
  ******************************************************************************
  */ 


#include "app_can.h"
#include "Mid_can.h"
#include "hal_can.h"
#include "protocol.h"




can_msg_t can_msg[ID_PROCESS_ALL]={0};

static can_msg_t *rx_msg = &can_msg[0];
static can_msg_t *tx_msg_1 = &can_msg[1];
static can_msg_t *tx_msg_2 = &can_msg[2];
static can_msg_t *tx_msg_3 = &can_msg[3];
static can_msg_t *tx_msg_4 = &can_msg[4];
static can_msg_t *tx_msg_5 = &can_msg[5];



/**
  * @brief  can �ڵ��ʼ��
  * @param  id �ڵ�id
  * @param  id_mask ����
  * @param  dlc ���ݳ���
  * @param  format ֡����
  * @param  msg_buf �����ַ
  * @param  period ��ʱʱ��
  * @retval None
  */
static void app_bound_id(U32 id,U32 id_mask,U8 dlc,CAN_ID_FORMAT_e  format,U8 msg_buf,U16 period)
{
	static U8 msg_buf_num = 0;
	if((msg_buf_num <= ID_PROCESS_ALL)&&(msg_buf >= 1))
	{
		can_msg[msg_buf_num].id =  id;
		can_msg[msg_buf_num].dlc = dlc;
		can_msg[msg_buf_num].format = format;
		can_msg[msg_buf_num].buffer_num = msg_buf;
		can_msg[msg_buf_num].id_mask = id_mask;
		if(period >= PERIOD_MS(1000))
		{
			can_msg[msg_buf_num].period_ms= period>>1;  
		}
		else
		{
			can_msg[msg_buf_num].period_ms= period;  
		}
		msg_buf_num ++;
	}
}
/**
  * @brief  can ��ʼ���������������can�ӿڽ��г�ʼ������
  * @param None
  * @retval None
  */

void app_init_can(void)
{
	//!<��ʼ���ڵ�
	hal_can_prepare(ID_RECV_NUM_ALL,ID_SENT_NUM_ALL,can_msg);
	app_bound_id(BOOT_ADDR,0x1FFFFFF0,8,EXTERN_ID,1,PERIOD_MS(100));

	
	app_bound_id(CHECK_ID,0x1FFFFFF0,8,EXTERN_ID,2,PERIOD_MS(100));
	app_bound_id(Erase_ID,0x1FFFFFF0,8,EXTERN_ID,3,PERIOD_MS(100));
	app_bound_id(Info_ID,0x1FFFFFF0,8,EXTERN_ID,4,PERIOD_MS(100));
	app_bound_id(Write_ID,0x1FFFFFF0,8,EXTERN_ID,5,PERIOD_MS(100));
//	app_bound_id(BOOT_ADDR+5,0x1FFFFFF0,8,EXTERN_ID,6,PERIOD_MS(100));
	//!<can������ ��ʼ��
	mid_can_init(CAN_CHN);
	
}
/**
  * @brief �ж��Ƿ�������
  * @param msg �жϽڵ�
  * @retval:1Ϊû���µ����ݡ�
  */
U32 IsCanEmpty(void)
{
	return (rx_msg->new_frame == 0)?TRUE:FALSE;
}

/**
  * @brief ��ȡ����;
  * @param data[out] ���ݴ��
  * @retval ���ݳ���
  * @attention
  * 1.���ݳ���Ϊ8���ֽڣ���ָֹ�����
  * 2.ִ�иú�����ı�ڵ�״̬
  */

U32 ReadCanData(PROTOCOL_MSG *msg)
{
	U32 id;
	memset(msg->data,0,MSG_BUF_SIZE);//!<data���ݳ��ȱ���Ϊ8�������8
	if (rx_msg->new_frame == 0)
	{
		return FAIL;
	}
	switch(rx_msg->dlc)
	{
		case 8:msg->data[7] = rx_msg->data[7];
		case 7:msg->data[6] = rx_msg->data[6];
		case 6:msg->data[5] = rx_msg->data[5];
		case 5:msg->data[4] = rx_msg->data[4];
		case 4:msg->data[3] = rx_msg->data[3];
		case 3:msg->data[2] = rx_msg->data[2];
		case 2:msg->data[1] = rx_msg->data[1];
		case 1:msg->data[0] = rx_msg->data[0];
		case 0:break;
		default: break;
	}
	msg->CMD = (rx_msg->id)&0x0F;
	msg->len = rx_msg->dlc;
	rx_msg->new_frame = 0;
	return SUCCESS;
}
/**
  * @brief ����can����
  * @param ID ����ID
  * @param data[IN] ���ݴ��
  * @param len ���ݳ���
  */

U32 SendCanData(PROTOCOL_MSG *msg)
{
	can_msg_t *tx_msg;

	switch(msg->CMD + BOOT_ADDR)
	{
		case CHECK_ID:
			tx_msg = tx_msg_1;
			break;
		case Erase_ID:
			tx_msg = tx_msg_2;
			break;
		case Info_ID:
			tx_msg = tx_msg_3;
			break;
		case Write_ID:
			tx_msg = tx_msg_4;
			break;
		default :
			return;
	}
	switch(tx_msg->dlc)
	{
		case 8:tx_msg->data[7] = msg->data[7];
		case 7:tx_msg->data[6] = msg->data[6];
		case 6:tx_msg->data[5] = msg->data[5];
		case 5:tx_msg->data[4] = msg->data[4];
		case 4:tx_msg->data[3] = msg->data[3];
		case 3:tx_msg->data[2] = msg->data[2];
		case 2:tx_msg->data[1] = msg->data[1];
		case 1:tx_msg->data[0] = msg->data[0];
		case 0:break;
		default:break;
	}	
	hal_can_sent(0,tx_msg);
	return SUCCESS;
}



/* signal lost task */
void app_can_get_task(void)
{
    U8		index = 0;
    U32		nMob = 0;
	static  U16  count = 0;
	static  U16     max_count = 0;
	
	can_msg_t can_msg_temp ={0};

	if(hal_canerror_statecheck() == CAN_BOFF)   
	{
		return;
	}
	nMob = HalChkMob();
	if(0 == nMob)
	{
		return ;
	}

	if(TRUE == hal_can_get(rx_msg))					/* ���������ݷ���tCanMsg�ṹ����,buffer start form 1*/
	{
		rx_msg->new_frame = 1;
		rx_msg->lost = 0;
		rx_msg->count = 0;
	}
}

