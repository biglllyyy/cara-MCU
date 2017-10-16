#ifndef  __HAL_IO_H__
#define  __HAL_IO_H__
#include "comm_typedef.h"

typedef enum
{
    IO_INPUT = 0,
    IO_GENERAL_OUTPUT = 1,			/* 作为普通IO口输出 */
    IO_FUNC_OUTPUT = 2				/*  选择第三方复用功能,注意还需要设置EPFR寄存器，由于EPFR寄存器较多，未有采用统一接口，后续可以继续优化 */
} e_io_direction;


extern U8  EPFR[88]; //存储EPFR的值
extern void   hal_gpio_Init(void);
extern  void  set_single_io_sts(U8 port,U8 bit,Bool value);
extern  void  set_group_io_sts(U8 port,U8 value);
extern  void  set_single_io_dir(U8 port,U8 bit,e_io_direction dir);
extern  void  set_group_io_dir(U8 port, e_io_direction dir);
extern  Bool  get_single_io_sts(U8 port,U8 bit);
extern  U8	  get_group_io_sts(U8 port);

#endif
