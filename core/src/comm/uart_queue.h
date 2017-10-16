#ifndef __UART_QUEUE_H__
#define __UART_QUEUE_H__

#include "comm_typedef.h"


typedef struct
{
    /*volatile unsigned S16 read_mutex;*/
    volatile U16 front;
    volatile U16 rear;
    volatile U8* queue;	//the queue buf address
    volatile U8 queue_size;
} UartQueue;

void uart_clear_queue(UartQueue *queue);
S16  uart_en_queue(UartQueue *queue, CHAR ch); 					//in queue
S16  uart_de_queue(UartQueue *queue);							//out queue
S16  drop_first_cmd(UartQueue *queue);							//give out first cmd
S16  uart_queue_size(const UartQueue *queue);					//return queue data numbers
S16  uart_queue_not_empty(const UartQueue *queue);				//empty return 0, else reruen 1
S16  uart_queue_init(UartQueue *queue,U8* buf_address,U8 buf_len); // need point buf address  and buf lenth

#endif
