#ifndef __UART_H_
#define __UART_H_


#include "comm_typedef.h"
#include "uart_queue.h"



//Baud rate generator
#define BGR0 	MFS0_UART_BGR
#define BGR1 	MFS1_UART_BGR
//#define BGR2 LIN_UART2_BGR2_BGR


typedef enum
{
	tty0 = 0,
	tty1 = 1
}serialPort_dev_index_t;

typedef enum
{
	tty_baudrate_115200 = 115200,
	tty_baudrate_9600 = 9600
}serialPort_dev_baudrate_t;

typedef enum
{
	tty_dataBit_8bit = 8,
}serialPort_dev_dataBit_t;

typedef enum
{
	tty_stopBit_0 = 0,
	tty_stopBit_1 = 1
}serialPort_dev_stopBit_t;

typedef enum
{
	tty_NoParity = 0,
	tty_OddParity = 1,
	tty_EvenParity = 2
}serialPort_dev_Parity_t;


typedef struct
{
	U8 *tty_name; // "serialport 0";
}serialPort_dev_name_t;

typedef enum
{
	tty_Open = 1,
	tty_Close = 0
}serialPort_dev_Status_t;


typedef struct
{
	char* name;
	serialPort_dev_index_t 		index;
	serialPort_dev_baudrate_t 	baudrate;
	serialPort_dev_dataBit_t 	dataBit;
	serialPort_dev_stopBit_t 	stopBit;
	serialPort_dev_Parity_t 	parity;
	serialPort_dev_Status_t		status;
}hal_serialPort_dev_t;





unsigned char hal_serialPort_Init(hal_serialPort_dev_t *device);
unsigned char hal_serialPort_Open(hal_serialPort_dev_t *device);
unsigned char hal_serialPort_Close(hal_serialPort_dev_t *device);
unsigned char hal_serialPort_Status(hal_serialPort_dev_t *device);
unsigned char hal_serialPort_Write(hal_serialPort_dev_t *device,char *data);








enum
{
    BUF_REC_SIZE = 200+1024,
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
    char *pChar);
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

