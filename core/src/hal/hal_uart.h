#ifndef __UART_H_
#define __UART_H_


#include "comm_typedef.h"
#include "uart_queue.h"

//Baud rate generator
#define BGR0 	MFS0_UART_BGR
#define BGR1 	MFS1_UART_BGR
//#define BGR2 LIN_UART2_BGR2_BGR

enum
{
    BUF_REC_SIZE = 200,
    BUF_SENT_SIZE = 200
};


#define  UART_A20_CHN	 		1
#define  UART_DEBUG_CHN			0

extern int uart_open(
    U8  nPortNum,
    U32 nBaudRate,
    U8  nDataBits,
    U8  nNumOfStopBit,
    U8  nParity);
extern int uart_changeBaudRate(U8 nPortNum, U32 nBaudRate);
extern int uart_write(
    U8 nPortNum,
    U8 *pChar);
extern void  uart_sent_task10ms(void);
extern U8    uart_read(U8 nPortNum, U8 * r_pChar);
extern U8    hal_uart_put_char(U8 chn, U8 data);
extern UartQueue uart_recv_queue;
extern UartQueue uart_sent_queue;
extern void hal_enable_uart_tx_interrupt(U8  enable);
#ifdef WIN32
extern void uart0_rx_interrupt();
extern void uart1_tx_interrupt();
#endif

extern U8   forbid_output;
extern void hal_uart_init(void);

#endif

