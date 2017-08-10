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


#define BUFFER_SIZE		0x400

typedef struct __PROTOCOL_COMM_INFO
{
	  U32 (* pIsEmpty)(void) ;		                   //!<�ж������Ƿ�Ϊ��
	  U32 (* pGetCMD)(void) ;		                   //!<��ȡ����
	  U32 (* pReadData)(U8 *data) ;		               //!<��ȡ���ݣ��������ݳ���
	  U32 (* pSendData)(U32 cmd,U8 *data,U8 len) ;     //!<��������
}
PROTOCOL_COMM_INFO;




extern PROTOCOL_COMM_INFO can_info;

#endif
	
