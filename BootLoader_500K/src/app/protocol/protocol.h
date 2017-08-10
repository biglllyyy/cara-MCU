#ifndef PROTOCOL_H__
	#define PROTOCOL_H__
#include "comm_typedef.h"

#define EraseFlashID        0x03//擦出APP储存扇区数据
#define WriteBlockFlashID   0x06//以多字节形式写数据
#define ReadBlockFlashID    0x08//以多字节形式读数据
#define BlockWriteInfoID    0x05//设置多字节写数据相关参数（写起始地址，数据量）
#define BlockReadInfoID     0x07//设置多字节数据读相关参数（读起始地址，数据量）
#define OnlineCheckID       0x01//检测节点是否在线
#define CmdSuccessID        0x0A//命令执行成功
#define CmdFaildID          0x00//命令执行失败
#define SetBaudRateID       0x04//设置节点波特率
#define ExcuteAppID         0x09//执行应用程序


#define BUFFER_SIZE		0x400

typedef struct __PROTOCOL_COMM_INFO
{
	  U32 (* pIsEmpty)(void) ;		                   //!<判断数据是否为空
	  U32 (* pGetCMD)(void) ;		                   //!<获取命令
	  U32 (* pReadData)(U8 *data) ;		               //!<获取数据，返回数据长度
	  U32 (* pSendData)(U32 cmd,U8 *data,U8 len) ;     //!<发送数据
}
PROTOCOL_COMM_INFO;




extern PROTOCOL_COMM_INFO can_info;

#endif
	
