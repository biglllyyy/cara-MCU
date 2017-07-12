#include <stdlib.h>
#include  "uart_queue.h"

//static UartQueue uart1_recv_queue = {0};

S16 drop_first_cmd(UartQueue *queue);

void uart_clear_queue(UartQueue *queue)
{
    if(queue != 0)
    {
        queue->rear = 0;
        queue->front = queue->rear;
    }
}

/* 入队列，正常返回0，出错返回-1*/
S16 uart_en_queue(UartQueue *queue, CHAR ch)
{
    S16 result = -1;
    do
    {
		queue->queue[queue->rear] = ch;
        if(((queue->rear + 1) % (queue->queue_size)) == queue->front)//队列满
        {
            break;
        }

        queue->rear = (queue->rear + 1) % (queue->queue_size);
        
        result = 0;
    }
    while(0);

    return result;
}

/*出队列，正常返回0~255，出错返回-1*/
S16 uart_de_queue(UartQueue *queue)
{
    S16 ch = -1;

    do
    {

        /* 如果队列为空，则不出队列*/
        if(queue->front == queue->rear)
        {
            break;
        }
		ch = queue->queue[queue->front];
        queue->front = (queue->front + 1) % (queue->queue_size);
		
        

    }
    while(0);

    return ch;
}

S16 drop_first_cmd(UartQueue *queue)
{
    S16 ch = -1;

    do
    {
        /* 如果队列为空，则不出队列*/
        if(queue->front == queue->rear)
        {
            break;
        }

        queue->front = (queue->front + 1) % (queue->queue_size);

        ch = queue->queue[queue->front];

    }
    while(ch != '\r');

    return 0;
}



/* 返回数据个数，0～UART_QUEUE_SIZE - 1*/
S16 uart_queue_size(const UartQueue *queue)
{
    S16 count = 0;

    do
    {

        count = (queue->rear - queue->front + (queue->queue_size)) % (queue->queue_size);

    }
    while (0);

    return count;
}

/* 不空返回1，否则返回0*/
S16 uart_queue_not_empty(const UartQueue *queue)
{
    S16 result = 0;

    do
    {
        /* 队列空，break*/
        if(queue->front == queue->rear)
        {
            break;
        }

        result = 1;

    }
    while (0);

    return result;
}

S16 uart_queue_init(UartQueue *queue,U8* buf_address,U8 buf_len)
{
    S16  ret = 0;
    if((queue != NULL) && (buf_address != NULL))
    {
        queue->queue = buf_address;
        queue->queue_size = buf_len;
        uart_clear_queue(queue);
    }
    else
    {
        ret = 1;
    }
    return ret;
}
