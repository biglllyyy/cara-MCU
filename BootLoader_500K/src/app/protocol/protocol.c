/**
  ******************************************************************************
  * @file    Protocol.c
  * @author  voyager1128
  * @version V1.0.0
  * @date    2017-8-7
  * @brief   ����Э�����
  *            
  ******************************************************************************
  * @attention
      ����:
               ����֡       �������       ������Ϣ      ����֡
      ͨѶ�ӿ�-------->����------------>����---------->����------->���Ͷ˿� 
  ******************************************************************************
  */ 


#include "protocol.h"
#include "app_can.h"
#include "boot.h"
#include "srec.h"

/** @defgroup ExecutiveCommandType
  * @{
  */

typedef struct _ExecutiveCommandType
{
	unsigned int cmd;
	void (*fHandle)(PROTOCOL_COMM_INFO *info,PROTOCOL_MSG *msg);
}
ExecutiveCommandType;
/** 
  * @}
  */
/** @defgroup ��������
  * @{
  */
void ExecutiveCheckHandle(PROTOCOL_COMM_INFO *info,PROTOCOL_MSG *msg);
void ExecutiveEraseFlashHandle(PROTOCOL_COMM_INFO *info,PROTOCOL_MSG *msg);
void ExecutiveBlockWriteInfoHandle(PROTOCOL_COMM_INFO *info,PROTOCOL_MSG *msg);
void ExecutiveWriteBlockFlashHandle(PROTOCOL_COMM_INFO *info,PROTOCOL_MSG *msg);
void BOOT_ExecutiveCommandHandle(PROTOCOL_COMM_INFO *info,PROTOCOL_MSG *msg);
void ExecutiveAppHandle(PROTOCOL_COMM_INFO *info,PROTOCOL_MSG *msg);

/**
  * @}
  */

U32 data_index = 0,data_size = 0,CanStop = 1;


//!< ָ���б�
static ExecutiveCommandType commandTable[]=
{
	{OnlineCheckID,ExecutiveCheckHandle},
	{EraseFlashID,ExecutiveEraseFlashHandle},
	{BlockWriteInfoID,ExecutiveBlockWriteInfoHandle},
	{WriteBlockFlashID,ExecutiveWriteBlockFlashHandle},
	{ExcuteAppID,ExecutiveAppHandle},
	{0,NULL},
};

//!< ͨ�Žӿڶ���
PROTOCOL_COMM_INFO can_info = 
{
	IsCanEmpty,
	ReadCanData,
	SendCanData,
};
/**
  * @brief Э�����
  * @param pro [in/out] ͨѶ�˿�
  * @retval None
  */

void Protocol(PROTOCOL_COMM_INFO *pro)
{
	U32 u32temp,len;
	u32temp = pro->pIsEmpty();
	if (u32temp == FALSE)
	{
		U32 cmd;
		PROTOCOL_MSG msg;
		pro->pReadData(&msg);
		BOOT_ExecutiveCommandHandle(pro,&msg);
		
	}
	
}

/**
  * @brief ָ������
  * @param info [in/out] ͨѶ�˿�
  * @param buf[in] ���յ�������
  * @param cmd ָ��
  * @param len ���ݳ���
  * @retval None
  */
void BOOT_ExecutiveCommandHandle(PROTOCOL_COMM_INFO *info,PROTOCOL_MSG *msg)
{
	int i;
	for(i=0;;i++)
	{
		if (commandTable[i].fHandle == NULL)
		{
			break;
		}
		if (commandTable[i].cmd == msg->CMD)
		{
			commandTable[i].fHandle(info,msg);
		}
	}
}
/**
  * @brief CHECKָ��
  * @param info [in/out] ͨѶ�˿�
  * @param buf[in] ���յ�������
  * @param cmd ָ��
  * @param len ���ݳ���
  * @retval None
  */

void ExecutiveCheckHandle(PROTOCOL_COMM_INFO *info,PROTOCOL_MSG *msg)
{
	msg->CMD = OnlineCheckID;
	msg->data[0] = BL_VER_MAJOR>>8;
	msg->data[1] = BL_VER_MAJOR;
	msg->data[2] = BL_VER_REV>>8;
	msg->data[3] = BL_VER_REV;
	msg->data[4] = 0;
	msg->data[5] = 0;
	msg->data[6] = 0;
	msg->data[7] = CAN_BL_BOOT;
	msg->len = 8;
	info->pSendData(msg);
}
/**
  * @brief ����ָ��
  * @param info [in/out] ͨѶ�˿�
  * @param buf[in] ���յ�������
  * @param cmd ָ��
  * @param len ���ݳ���
  * @retval None
  */

void ExecutiveEraseFlashHandle(PROTOCOL_COMM_INFO *info,PROTOCOL_MSG *msg)
{
/**
  * @TODO:����APP������
  */
	U8 index,app_flash_result,state;

	for (index = PROG_START_PAGE; index < PROG_END_PAGE; index++) //data flash,522,SA2--SA11
	{
		app_flash_result	= mid_flash_sect_erasing(index);//!<�ú������޸��ж����μĴ���

		if (app_flash_result == FAIL)
		{
			//!<����ʧ��
			Console("\n����ʧ��!");
			break;
		}
	}
	msg->CMD = EraseFlashID;
	msg->len = 8;
	msg->data[0] = app_flash_result;
	info->pSendData(msg);
}
/**
  * @brief �����趨ָ��
  * @param info [in/out] ͨѶ�˿�
  * @param buf[in] ���յ�������
  * @param cmd ָ��
  * @param len ���ݳ���
  * @retval None
  */

void ExecutiveBlockWriteInfoHandle(PROTOCOL_COMM_INFO *info,PROTOCOL_MSG *msg)
{
	data_index = 0;
	data_size = msg->data[4];
	data_size <<=8;
	data_size =msg->data[5];
	data_size <<=8;
	data_size = msg->data[6];
	data_size <<=8;
	data_size +=msg->data[7];
	msg->CMD = BlockWriteInfoID;
	msg->len= 0;
	info->pSendData(msg);
}
/**
  * @brief ����ָ��
  * @param info [in/out] ͨѶ�˿�
  * @param buf[in] ���յ�������
  * @param cmd ָ��
  * @param len ���ݳ���
  * @retval None
  */

void ExecutiveWriteBlockFlashHandle(PROTOCOL_COMM_INFO *info,PROTOCOL_MSG *msg)
{
	U32 i;
	if(data_index<data_size)
	{
		CanStop = 0;
		for (i=0;i<msg->len;i++)
		{
			data_index++;
			if(data_index+2<=data_size){
				updata_buf.buf[(updata_buf.w++)&(BUFFER_SIZE - 1)] = msg->data[i];
			}
		}
		//Console("\n data_index=%d,data_size=%d",data_index,data_size);
	}
	if(data_index>=data_size){
		CanStop = 1;
		//Console("\n send id = %d",msg->CMD);
		info->pSendData(msg);
	}
}

void ExecutiveAppHandle(PROTOCOL_COMM_INFO *info,PROTOCOL_MSG *msg)
{
	#pragma asm
	LDI:32 #080024H, R0;  //jmp to address 80024H to run
	JMP @R0;
	#pragma endasm
}


