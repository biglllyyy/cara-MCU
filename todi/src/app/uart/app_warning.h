#ifndef APP_WARNING_H
#define APP_WARNING_H
#include "comm_typedef.h"

typedef struct
{
    uint8_t  ID_180689f4_byte7_bit0:1;   //ģ���ѹ����
    uint8_t  ID_180689f4_byte7_bit1:1;   //ģ���ѹ����
    uint8_t  ID_180689f4_byte7_bit2:1;   //SOC����
    uint8_t  ID_180689f4_byte7_bit3:1;   //SOC����
    uint8_t  ID_180689f4_byte7_bit4:1;   //������
    uint8_t  ID_180689f4_byte7_bit5:1;   //�¶ȹ���
    uint8_t  ID_180689f4_byte7_bit6:1;   //SOC̫��ͣ��
    uint8_t  ID_180689f4_byte7_bit7:1;   //�¶ȹ���
    uint8_t  ID_180689f4_byte7;
	
    uint8_t  ID_181B80f4_byte5_bit01:2;  //�����ѹ���� 				    00���ޱ���. 01��һ������. 10����������
    uint8_t  ID_181B80f4_byte5_bit23:2;  //�����ѹ���� �����ߵ�ѹ��ѹ������ 00���ޱ���. 01��һ������. 10����������
    uint8_t  ID_181B80f4_byte5_bit45:2;  //����Ƿѹ����  				    00���ޱ���. 01��һ������. 10����������
    uint8_t  ID_181B80f4_byte5_bit67:2;  //����Ƿѹ���� �����ߵ�ѹǷѹ������ 00���ޱ���. 01��һ������. 10����������
    uint8_t  ID_181B80f4_byte5;

	uint8_t  ID_181B80f4_byte6_bit01:2;  //��ص��±��� 				    00���ޱ���. 01��һ������. 10����������
    uint8_t  ID_181B80f4_byte6_bit23:2;  //��ظ��±���                   00���ޱ���. 01��һ������. 10����������
    uint8_t  ID_181B80f4_byte6_bit45:2;  //����ѹ���  				    00���ޱ���. 01��һ������. 10����������
    uint8_t  ID_181B80f4_byte6_bit67:2;  //ģ���²��                   00���ޱ���. 01��һ������. 10����������
    uint8_t  ID_181B80f4_byte6;

	uint8_t  ID_181B80f4_byte7_bit01:2;  //��طŵ�������󱨾�      	    00���ޱ���. 01��һ������. 10����������
    uint8_t  ID_181B80f4_byte7_bit23:2;  //��س��������󱨾�            00���ޱ���. 01��һ������. 10����������
    uint8_t  ID_181B80f4_byte7_bit45:2;  //��Ե��ⱨ��  				    00���ޱ���. 01��һ������. 10����������
    uint8_t  ID_181B80f4_byte7_bit67:2;  //��� SOC �ͱ���                00���ޱ���. 01��һ������. 10����������
    uint8_t  ID_181B80f4_byte7;

	uint8_t  ID_181B80f4_byte8_bit01:2;  //����ͨѶ���� 				    00���ޱ���. 01��һ������. 10����������
    uint8_t  ID_181B80f4_byte8_bit23:2;  //����ͨѶ���� �����ߵ�ѹ��ѹ������  00���ޱ���. 01��һ������. 10����������
    uint8_t  ID_181B80f4_byte8_bit45:2;  //Ԥ��
    uint8_t  ID_181B80f4_byte8_bit67:2;  //Ԥ��
    uint8_t  ID_181B80f4_byte8;

	uint8_t  ID_0C068980_byte8_bit0:1;  //����DC/DC ״̬������.����
    uint8_t  ID_0C068980_byte8_bit1:1;  //����DC/DC ״̬������.����
    uint8_t  ID_0C068980_byte8_bit2:1;  //����DC/DC ״̬������.Ƿѹ
    uint8_t  ID_0C068980_byte8_bit3:1;  //����DC/DC ״̬������.��ѹ
    uint8_t  ID_0C068980_byte8_bit4:1;  //����DC/DC ״̬������.����
    uint8_t  ID_0C068980_byte8_bit5:1;  //����DC/DC ״̬������.ȱ��
    uint8_t  ID_0C068980_byte8_bit6:1;  //����DC/DC ״̬������.��ѹ�ź�
    uint8_t  ID_0C068980_byte8_bit7:1;  //����DC/DC ״̬������.1 ���� 0 ͣ��
    uint8_t  ID_0C068980_byte8;         //����DC/DC ״̬������

	uint8_t  ID_0C058980_byte8_bit0:1;  //�ͱ�DC/DC ״̬������.����
    uint8_t  ID_0C058980_byte8_bit1:1;  //�ͱ�DC/DC ״̬������.����
    uint8_t  ID_0C058980_byte8_bit2:1;  //�ͱ�DC/DC ״̬������.Ƿѹ
    uint8_t  ID_0C058980_byte8_bit3:1;  //�ͱ�DC/DC ״̬������.��ѹ
    uint8_t  ID_0C058980_byte8_bit4:1;  //�ͱ�DC/DC ״̬������.����
    uint8_t  ID_0C058980_byte8_bit5:1;  //�ͱ�DC/DC ״̬������.ȱ��
    uint8_t  ID_0C058980_byte8_bit6:1;  //�ͱ�DC/DC ״̬������.Ԥ��
    uint8_t  ID_0C058980_byte8_bit7:1;  //�ͱ�DC/DC ״̬������.1 ���� 0 ͣ��
    uint8_t  ID_0C058980_byte8;         //�ͱ�DC/DC ״̬������

	uint8_t  ID_0C048980_byte8_bit0:1;  //DC/DC ״̬������.����
    uint8_t  ID_0C048980_byte8_bit1:1;  //DC/DC ״̬������.����
    uint8_t  ID_0C048980_byte8_bit2:1;  //DC/DC ״̬������.Ƿѹ
    uint8_t  ID_0C048980_byte8_bit3:1;  //DC/DC ״̬������.��ѹ
    uint8_t  ID_0C048980_byte8_bit4:1;  //DC/DC ״̬������.Ԥ��
    uint8_t  ID_0C048980_byte8_bit5:1;  //DC/DC ״̬������.Ԥ��
    uint8_t  ID_0C048980_byte8_bit6:1;  //DC/DC ״̬������.Ԥ��
    uint8_t  ID_0C048980_byte8_bit7:1;  //DC/DC ״̬������.1 ���� 0 ͣ��
    uint8_t  ID_0C048980_byte8;         //DC/DC ״̬������
} WarningDataStruct;




extern WarningDataStruct warning_data;
void warning_get_data(void);
void warning_send_data(void);
#endif
