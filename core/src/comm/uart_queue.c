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

/* ����У���������0��������-1*/
S16 uart_en_queue(UartQueue *queue, CHAR ch)
{
    S16 result = -1;
    do
    {
		queue->queue[queue->rear] = ch;
        if(((queue->rear + 1) % (queue->queue_size)) == queue->front)//������
        {
            break;
        }

        queue->rear = (queue->rear + 1) % (queue->queue_size);
        
        result = 0;
    }
    while(0);

    return result;
}

/*�����У���������0~255��������-1*/
S16 uart_de_queue(UartQueue *queue)
{
    S16 ch = -1;

    do
    {

        /* �������Ϊ�գ��򲻳�����*/
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
        /* �������Ϊ�գ��򲻳�����*/
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



/* �������ݸ�����0��UART_QUEUE_SIZE - 1*/
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

/* ���շ���1�����򷵻�0*/
S16 uart_queue_not_empty(const UartQueue *queue)
{
    S16 result = 0;

    do
    {
        /* ���пգ�break*/
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
