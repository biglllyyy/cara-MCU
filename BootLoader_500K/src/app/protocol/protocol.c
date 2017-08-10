/**
  ******************************************************************************
  * @file    Protocol.c
  * @author  voyager1128
  * @version V1.0.0
  * @date    2017-8-7
  * @brief   升级协议设计
  *            
  ******************************************************************************
  * @attention
      流程:
               数据帧       命令、数据       反馈信息      数据帧
      通讯接口-------->解析------------>处理---------->编码------->发送端口 
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
/** @defgroup 函数声明
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


//!< 指令列表
static ExecutiveCommandType commandTable[]=
{
	{OnlineCheckID,ExecutiveCheckHandle},
	{EraseFlashID,ExecutiveEraseFlashHandle},
	{BlockWriteInfoID,ExecutiveBlockWriteInfoHandle},
	{WriteBlockFlashID,ExecutiveWriteBlockFlashHandle},
	//{ExcuteAppID,ExecutiveAppHandle},
	{0,NULL},
};

//!< 通信接口定义
PROTOCOL_COMM_INFO can_info = 
{
	IsCanEmpty,
	ReadCanID,
	ReadCanData,
	SendCanData,
};
/**
  * @brief 协议解析
  * @param pro [in/out] 通讯端口
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
  * @brief 指令驱动
  * @param info [in/out] 通讯端口
  * @param buf[in] 接收到的数据
  * @param cmd 指令
  * @param len 数据长度
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
  * @brief CHECK指令
  * @param info [in/out] 通讯端口
  * @param buf[in] 接收到的数据
  * @param cmd 指令
  * @param len 数据长度
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
  * @brief 擦除指令
  * @param info [in/out] 通讯端口
  * @param buf[in] 接收到的数据
  * @param cmd 指令
  * @param len 数据长度
  * @retval None
  */

void ExecutiveEraseFlashHandle(PROTOCOL_COMM_INFO *info,U8 *buf,U32 len)
{
/**
  * @TODO:擦除APP程序区
  */
	U8 index,app_flash_result,state;
	U8 data[8];
	U32 id,len;

	for (index = PROG_START_PAGE; index < PROG_END_PAGE; index++) //data flash,522,SA2--SA11
	{
		app_flash_result	= mid_flash_sect_erasing(index);//!<该函数会修改中断屏蔽寄存器

		if (app_flash_result == FAIL)
		{
			//!<擦除失败
			Console("\n擦除失败!");
			break;
		}
	}
	id = (CAN_TX_ID) | EraseFlashID;
	len = 8;
	data[0] = app_flash_result;
	info->pSendData(id,data,len);
}
/**
  * @brief 参数设定指令
  * @param info [in/out] 通讯端口
  * @param buf[in] 接收到的数据
  * @param cmd 指令
  * @param len 数据长度
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
  * @brief 下载指令
  * @param info [in/out] 通讯端口
  * @param buf[in] 接收到的数据
  * @param cmd 指令
  * @param len 数据长度
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



