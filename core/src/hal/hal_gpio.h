#ifndef  __HAL_IO_H__
#define  __HAL_IO_H__
#include "comm_typedef.h"

typedef enum
{
    IO_INPUT = 0,
    IO_GENERAL_OUTPUT = 1,			/* ��Ϊ��ͨIO����� */
    IO_FUNC_OUTPUT = 2				/*  ѡ����������ù���,ע�⻹��Ҫ����EPFR�Ĵ���������EPFR�Ĵ����϶࣬δ�в���ͳһ�ӿڣ��������Լ����Ż� */
} e_io_direction;


extern U8  EPFR[88]; //�洢EPFR��ֵ
extern void   hal_gpio_Init(void);
extern  void  set_single_io_sts(U8 port,U8 bit,Bool value);
extern  void  set_group_io_sts(U8 port,U8 value);
extern  void  set_single_io_dir(U8 port,U8 bit,e_io_direction dir);
extern  void  set_group_io_dir(U8 port, e_io_direction dir);
extern  Bool  get_single_io_sts(U8 port,U8 bit);
extern  U8	  get_group_io_sts(U8 port);

#endif
