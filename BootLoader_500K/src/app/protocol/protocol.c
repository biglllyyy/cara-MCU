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
	void (*fHandle)(PROTOCOL_COMM_INFO *info,U8 *data,U32 len);
}
ExecutiveCommandType;
/** 
  * @}
  */
/** @defgroup ��������
  * @{
  */
void ExecutiveCheckHandle(PROTOCOL_COMM_INFO *info,U8 *data,U32 len);
void ExecutiveEraseFlashHandle(PROTOCOL_COMM_INFO *info,U8 *buf,U32 len);
void ExecutiveBlockWriteInfoHandle(PROTOCOL_COMM_INFO *info,U8 *buf,U32 len);
void ExecutiveWriteBlockFlashHandle(PROTOCOL_COMM_INFO *info,U8 *buf,U32 len);
void BOOT_ExecutiveCommandHandle(PROTOCOL_COMM_INFO *info,U8 *buf,U32 len);
/**
  * @}
  */

static U32 data_index = 0,data_size = 0,CanStop = 0;


//!< ָ���б�
static ExecutiveCommandType commandTable[]=
{
	{OnlineCheckID,ExecutiveCheckHandle},
	{EraseFlashID,ExecutiveEraseFlashHandle},
	{BlockWriteInfoID,ExecutiveBlockWriteInfoHandle},
	{WriteBlockFlashID,ExecutiveWriteBlockFlashHandle},
	//{ExcuteAppID,ExecutiveAppHandle},
	{0,NULL},
};

//!< ͨ�Žӿڶ���
PROTOCOL_COMM_INFO can_info = 
{
	IsCanEmpty,
	ReadCanID,
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
		U8 buf[64];
		cmd = pro->pGetCMD();
		cmd &=0x0F;
		len = pro->pReadData(buf);
		BOOT_ExecutiveCommandHandle(pro,buf,cmd,len);
		
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
void BOOT_ExecutiveCommandHandle(PROTOCOL_COMM_INFO *info,U8 *buf,U8 cmd,U32 len)
{
	int i;
	for(i=0;;i++)
	{
		if (commandTable[i].fHandle == NULL)
		{
			break;
		}
		if (commandTable[i].cmd == cmd)
		{
			commandTable[i].fHandle(info,buf);
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

void ExecutiveCheckHandle(PROTOCOL_COMM_INFO *info,U8 *buf,U32 len)
{
	U8 data[8];
	U32 id,len;
	id = (CAN_TX_ID) | OnlineCheckID;
	data[0] = BL_VER_MAJOR>>8;
	data[1] = BL_VER_MAJOR;
	data[2] = BL_VER_REV>>8;
	data[3] = BL_VER_REV;
	data[4] = 0;
	data[5] = 0;
	data[6] = 0;
	data[7] = CAN_BL_BOOT;
	len = 8;
	info->pSendData(id,data,len);
}
/**
  * @brief ����ָ��
  * @param info [in/out] ͨѶ�˿�
  * @param buf[in] ���յ�������
  * @param cmd ָ��
  * @param len ���ݳ���
  * @retval None
  */

void ExecutiveEraseFlashHandle(PROTOCOL_COMM_INFO *info,U8 *buf,U32 len)
{
/**
  * @TODO:����APP������
  */
	U8 index,app_flash_result,state;
	U8 data[8];
	U32 id,len;

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
	id = (CAN_TX_ID) | EraseFlashID;
	len = 8;
	data[0] = app_flash_result;
	info->pSendData(id,data,len);
}
/**
  * @brief �����趨ָ��
  * @param info [in/out] ͨѶ�˿�
  * @param buf[in] ���յ�������
  * @param cmd ָ��
  * @param len ���ݳ���
  * @retval None
  */

void ExecutiveBlockWriteInfoHandle(PROTOCOL_COMM_INFO *info,U8 *buf,U32 len)
{
	U32 id,len;
	data_index = 0;
	data_size = buf[4];
	data_size <<=8;
	data_size =buf[5];
	data_size <<=8;
	data_size = buf[6];
	data_size <<=8;
	data_size +=buf[7];
	id = (CAN_TX_ID) | BlockWriteInfoID;
	len = 0;
	info->pSendData(id,buf,len);
}
/**
  * @brief ����ָ��
  * @param info [in/out] ͨѶ�˿�
  * @param buf[in] ���յ�������
  * @param cmd ָ��
  * @param len ���ݳ���
  * @retval None
  */

void ExecutiveWriteBlockFlashHandle(PROTOCOL_COMM_INFO *info,U8 *buf,U32 len)
{
	int i;
	if(data_index<data_size)
	{
		for (i=0;i<len;i++)
		{
			data_index++;
			if(data_index+2<=data_size){
				updata_buf.buf[(updata_buf.w++)&(BUFFER_SIZE - 1)] = buf[i];
			}
		}
	}
	if(data_index>=data_size){
		CanStop = 1;
	}
}



