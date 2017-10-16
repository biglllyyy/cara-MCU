#ifndef PROTOCOL_H__
	#define PROTOCOL_H__
#include "comm_typedef.h"

#define EraseFlashID        0x03//����APP������������
#define WriteBlockFlashID   0x06//�Զ��ֽ���ʽд����
#define ReadBlockFlashID    0x08//�Զ��ֽ���ʽ������
#define BlockWriteInfoID    0x05//���ö��ֽ�д������ز�����д��ʼ��ַ����������
#define BlockReadInfoID     0x07//���ö��ֽ����ݶ���ز���������ʼ��ַ����������
#define OnlineCheckID       0x01//���ڵ��Ƿ�����
#define CmdSuccessID        0x0A//����ִ�гɹ�
#define CmdFaildID          0x00//����ִ��ʧ��
#define SetBaudRateID       0x04//���ýڵ㲨����
#define ExcuteAppID         0x09//ִ��Ӧ�ó���




/** @defgroup ��Ϣ����
  * @{
  */
#define MSG_BUF_SIZE	(0x08)
typedef struct __PROTOCOL_MSG
{
	U32 CMD;					//!<����
	U8 data[MSG_BUF_SIZE];      //!<����
	U32 len;					//!<���ݳ���
	U8  pro;					//!<�˿ڲ����ظ�
}
PROTOCOL_MSG;
/**
 * @}
 */

typedef struct __PROTOCOL_COMM_INFO
{
	  U32 (* pIsEmpty)(void) ;		                   //!<�ж������Ƿ�Ϊ��
	  U32 (* pReadData)(PROTOCOL_MSG *msg) ;		   //!<��ȡ���ݣ����ش���״̬
	  U32 (* pSendData)(PROTOCOL_MSG *msg) ;           //!<��������
}
PROTOCOL_COMM_INFO;
extern U32 data_index,data_size,CanStop;




extern PROTOCOL_COMM_INFO can_info;

#endif
	
