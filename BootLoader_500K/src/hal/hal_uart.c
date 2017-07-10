#include "hal_uart.h"
#include "comm_typedef.h"
#include "KeyPortReg.h"
#include "mb91520.h"
#include "hal_gpio.h"
#include "uart_queue.h"
#include "hal_clock.h"
#include "uart_cfg.h"
#ifdef WIN32
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#endif

#define SERIALPORT_FUNCTION_MAX 6

UartQueue	uart_recv_queue = {0};
static U8	recv_buf[BUF_REC_SIZE] = {0};
UartQueue	uart_sent_queue = {0};
static U8	sent_buf[BUF_SENT_SIZE] = {0};

static unsigned char hal_serialPort_GPIO_Init(hal_serialPort_dev_t *device);
static unsigned char hal_serialPort_Basic_Init(hal_serialPort_dev_t *device);
static unsigned char hal_serialPort_Baudrate_Init(hal_serialPort_dev_t *device);
static unsigned char hal_serialPort_dataBit_Init(hal_serialPort_dev_t *device);
static unsigned char hal_serialPort_stopBit_Init(hal_serialPort_dev_t *device);
static unsigned char hal_serialPort_Parity_Init(hal_serialPort_dev_t *device);


static unsigned char (*initFunctions[SERIALPORT_FUNCTION_MAX])(hal_serialPort_dev_t *) =
{
	hal_serialPort_GPIO_Init,
	hal_serialPort_Basic_Init,
	hal_serialPort_Baudrate_Init,
	hal_serialPort_dataBit_Init,
	hal_serialPort_stopBit_Init,
	hal_serialPort_Parity_Init
};

//private 方法

static unsigned char hal_serialPort_GPIO_Init(hal_serialPort_dev_t *device)
{
	switch (device->index)
	{
		case tty0:	       
			        EPFR[35] &= MASK0;	//BIT0 为0，input from UART0_0
			        EPFR[35] |= BIT3;   //bit4,bit3 = 01,output from UART0_0
			        EPFR[35] &= MASK4;
					WriteKeyProtectedRegister( (unsigned int)&EPFR35, EPFR[35], BYTE_ACCESS );
					set_single_io_dir(12,7,IO_FUNC_OUTPUT);
					set_single_io_dir(12,6,IO_INPUT);
					break; 
					
		case tty1:	        
					EPFR[36] |= BIT3;     // SOT1_0 output enable,set 1
			        WriteKeyProtectedRegister((unsigned int)&EPFR36, EPFR[36], BYTE_ACCESS);
			        set_single_io_dir(0,0,IO_INPUT);
			        set_single_io_dir(0,1,IO_FUNC_OUTPUT);
					break; 
					
		default:	return 0;
	}
	return 1;
}

static unsigned char hal_serialPort_Basic_Init(hal_serialPort_dev_t *device)
{
	switch (device->index)
	{
		case tty0:	        //Reset UART Module
			        MFS0_UART_SCR_UPCL =1;	//reset UART ch 0	, important: it should be reset UART marco first
			        //SMR
			        MFS0_UART_SMR_MD  = 0;	//asynchronous normal mode	,important, it should be set once UART marco is reset
			        MFS0_UART_SMR_SBL = 0;	//Num Of Stop Bit, By Default is 1 bit
			        MFS0_UART_SMR_BDS = 0; // 0-->LSB-first, 1-->MSB first
			        MFS0_UART_SMR_SOE = 1;	//Enable SOUT

			        //FCR0
			        MFS0_UART_FCR0_FCL2=1;	//1--> Reset FIFO2
			        MFS0_UART_FCR0_FCL1=1;	//1--> Reset FIFO1

			        //FCR1
			        MFS0_UART_FCR1_FLSTE =0;	// 1-->Enable Data Lost Detection
			        MFS0_UART_FCR1_FRIIE =1;	// 1-->Enable Receive FIFO Idle Detection
			        MFS0_UART_FCR1_FSEL=0;	//0-->FIFO1 for TX and FIFO2 for RX, 1-->FIFO1 for RX and FIFO2 for TX

			        //FIFO_Disable
			        MFS0_UART_FCR0_FE2 =0;	//1-->Enable FIFO2
			        MFS0_UART_FCR0_FE1 =0;	//1-->Enable FIFO1
			        MFS0_UART_FCR1_FTIE=0;	//Disable TX FIFO interrupt enable bit
			        //FBYTE
			        if(MFS0_UART_FCR1_FSEL==0)
			            MFS0_UART_FBYTE= 0x0800;	//8 for FIFO2 (RX), 0 for FIFO1 (TX)
			        else
			            MFS0_UART_FBYTE = 0x0008;	//0 for FIFO2 (TX), 8 for FIFO1 (RX)

			        //ESCR
			        MFS0_UART_ESCR_INV = 0;	//0-->NRZ format , 1-->Inverted NRZ format       
					break; 
					
		case tty1:	        
					MFS1_UART_SCR_UPCL =1;	//reset UART ch 0	, important: it should be reset UART marco first
			        //SMR
			        MFS1_UART_SMR_MD  = 0;	//asynchronous normal mode	,important, it should be set once UART marco is reset
			        MFS1_UART_SMR_SBL = 0;	//Num Of Stop Bit, By Default is 1 bit
			        MFS1_UART_SMR_BDS = 0; // 0-->LSB-first, 1-->MSB first
			        MFS1_UART_SMR_SOE = 1;	//Enable SOUT

			        //FCR0
			        MFS1_UART_FCR0_FCL2=1;	//1--> Reset FIFO2
			        MFS1_UART_FCR0_FCL1=1;	//1--> Reset FIFO1

			        //FCR1
			        MFS1_UART_FCR1_FLSTE =0;	// 1-->Enable Data Lost Detection
			        MFS1_UART_FCR1_FRIIE =1;	// 1-->Enable Receive FIFO Idle Detection
			        MFS1_UART_FCR1_FSEL=0;	//0-->FIFO1 for TX and FIFO2 for RX, 1-->FIFO1 for RX and FIFO2 for TX


			        MFS1_UART_FCR0_FE2 =1;	//1-->Enable FIFO2
			        MFS1_UART_FCR0_FE1 =1;	//1-->Enable FIFO1
			        MFS1_UART_FCR1_FTIE=0;	//Disable TX FIFO interrupt enable bit
			        //FBYTE
			        if(MFS1_UART_FCR1_FSEL==0)
			            MFS1_UART_FBYTE= 0x0800;	//8 for FIFO2 (RX), 0 for FIFO1 (TX)
			        else
			            MFS1_UART_FBYTE = 0x0008;	//0 for FIFO2 (TX), 8 for FIFO1 (RX)

			        //ESCR
			        MFS1_UART_ESCR_INV = 0;	//0-->NRZ format , 1-->Inverted NRZ format        

					break; 
					
		default:	return 0;
	}
	return 1;
}

static unsigned char hal_serialPort_Baudrate_Init(hal_serialPort_dev_t *device)
{
	U32 l_nRLValue;

    l_nRLValue = SupplyClk/(device->baudrate) -1;

    if(l_nRLValue<1 || l_nRLValue>65535)
        return 0;

    switch(device->index)
    {
	    case tty0:
			        BGR0 = (__HWORD)(l_nRLValue);
			        break;
	    case tty1:
			        BGR1 = (__HWORD)(l_nRLValue);
			        break;
	    default: 	return 0;
    }
    return 1;
}


static unsigned char hal_set_dataBit_tty0(hal_serialPort_dev_t *device)
{
	switch(device->dataBit)
	{
		case 5:
				MFS0_UART_ESCR_L = 1;
				break;
		case 6:
				MFS0_UART_ESCR_L = 2;
				break;
		case 7:
				MFS0_UART_ESCR_L = 3;
				break;
		case 8:
				MFS0_UART_ESCR_L = 0;
				break;
		case 9:
				MFS0_UART_ESCR_L = 4;
				break;
		default:
				return 0;
	}
	return 1;
}

static unsigned char hal_set_dataBit_tty1(hal_serialPort_dev_t *device)
{
	switch(device->dataBit)
	{
		case 5:
				MFS1_UART_ESCR_L = 1;
				break;
		case 6:
				MFS1_UART_ESCR_L = 2;
				break;
		case 7:
				MFS1_UART_ESCR_L = 3;
				break;
		case 8:
				MFS1_UART_ESCR_L = 0;
				break;
		case 9:
				MFS1_UART_ESCR_L = 4;
				break;
		default:
				return 0;
	}
	return 1;
}

static unsigned char hal_set_stopBit_tty0(hal_serialPort_dev_t *device)
{
	switch(device->stopBit)
	{
        case 1:
	            MFS0_UART_SMR_SBL =0;
	            MFS0_UART_ESCR_ESBL = 0;
	            break;
        case 2:
	            MFS0_UART_SMR_SBL =1;
	            MFS0_UART_ESCR_ESBL = 0;
	            break;
        case 3:
	            MFS0_UART_SMR_SBL =0;
	            MFS0_UART_ESCR_ESBL = 1;
	            break;
        case 4:
	            MFS0_UART_SMR_SBL =1;
	            MFS0_UART_ESCR_ESBL = 1;
	            break;
        default:
            	return 0;
	}
	return 1;
}

static unsigned char hal_set_stopBit_tty1(hal_serialPort_dev_t *device)
{
	switch(device->stopBit)
	{
        case 1:
	            MFS1_UART_SMR_SBL =0;
	            MFS1_UART_ESCR_ESBL = 0;
	            break;
        case 2:
	            MFS1_UART_SMR_SBL =1;
	            MFS10_UART_ESCR_ESBL = 0;
	            break;
        case 3:
	            MFS1_UART_SMR_SBL =0;
	            MFS1_UART_ESCR_ESBL = 1;
	            break;
        case 4:
	            MFS1_UART_SMR_SBL =1;
	            MFS1_UART_ESCR_ESBL = 1;
	            break;
        default:return 0;
	}
	return 1;
}



static unsigned char hal_serialPort_dataBit_Init(hal_serialPort_dev_t *device)
{
	switch (device->index)
	{
		case tty0:
					if(!hal_set_dataBit_tty0(device))
						return 0;
					break;
		case tty1:	
					if(!hal_set_dataBit_tty1(device))
						return 0;
					break;
		default:	return 0;
	}
	return 1;
}

static unsigned char hal_serialPort_stopBit_Init(hal_serialPort_dev_t *device)
{
	switch (device->index)
	{
		case tty0:
					if(!hal_set_stopBit_tty0(device))
						return 0;
					break;
		case tty1:	
					if(!hal_set_stopBit_tty1(device))
						return 0;					
					break;	
		default:	return 0;
	}
	return 1;
}

static unsigned char hal_set_Parity_tty0(hal_serialPort_dev_t *device)
{
	switch (device->parity)
	{
        case 0:		//No Parity
	            MFS0_UART_ESCR_PEN=0;
	            break;
        case 1:		//Odd Parity
	            MFS0_UART_ESCR_PEN=1;
	            MFS0_UART_ESCR_P=1;
	            break;
        case 2:		//Even Parity
	            MFS0_UART_ESCR_PEN=1;
	            MFS0_UART_ESCR_P=0;
	            break;
        default:return 0;
	}
	return 1;
}

static unsigned char hal_set_Parity_tty1(hal_serialPort_dev_t *device)
{
	switch (device->parity)
	{
        case 0:		//No Parity
	            MFS1_UART_ESCR_PEN=0;
	            break;
        case 1:		//Odd Parity
	            MFS1_UART_ESCR_PEN=1;
	            MFS1_UART_ESCR_P=1;
	            break;
        case 2:		//Even Parity
	            MFS1_UART_ESCR_PEN=1;
	            MFS1_UART_ESCR_P=0;
	            break;
        default: return 0;
	}
	return 1;	
}

static unsigned char hal_serialPort_Parity_Init(hal_serialPort_dev_t *device)
{
	switch (device->index)
	{
		case tty0:
					if(!hal_set_Parity_tty0(device))
						return 0;
					break;
		case tty1:
					if(!hal_set_Parity_tty1(device))
						return 0;
					break;
		default:	return 0;
	}
	return 1;
}


static unsigned char hal_set_Status_Open(hal_serialPort_dev_t *device)
{
	switch (device->index)
	{
		case tty0:
					MFS0_UART_SCR_TXE = 1;	//Enable TX , should be set at last
					MFS0_UART_SCR_RXE = 1;	//Enable RX , should be set at last
					MFS0_UART_SCR_RIE = 1;    /* enable reception interrupt */
					break;
		case tty1:
					MFS1_UART_SCR_TXE = 1;	//Enable TX , should be set at last
					MFS1_UART_SCR_RXE = 1;	//Enable RX , should be set at last
					MFS1_UART_SCR_RIE = 1;	  /* enable reception interrupt */
					break;
		default:	device->status = tty_Close;
					return 0;
	}
	device->status = tty_Open;
	return 1;
}

static unsigned char hal_set_Status_Close(hal_serialPort_dev_t *device)
{
	switch (device->index)
	{
		case tty0:
					MFS0_UART_SCR_TXE = 0;	//Enable TX , should be set at last
					MFS0_UART_SCR_RXE = 0;	//Enable RX , should be set at last
					MFS0_UART_SCR_RIE = 0;    /* enable reception interrupt */

					break;
		case tty1:
					MFS1_UART_SCR_TXE = 0;	//Enable TX , should be set at last
					MFS1_UART_SCR_RXE = 0;	//Enable RX , should be set at last
					MFS1_UART_SCR_RIE = 0;	  /* enable reception interrupt */
					break;
		default:	device->status = tty_Open;
					return 0;
	}
	device->status = tty_Close;
	return 1;
}





void hal_uart_init(void)
{
    uart_queue_init(&uart_recv_queue,recv_buf,BUF_REC_SIZE);
    uart_queue_init(&uart_sent_queue,sent_buf,BUF_SENT_SIZE);
}


//public 方法
/* 打开成功返回TRUE 
 * 打开失败返回FALSE
 */

unsigned char hal_serialPort_Init(hal_serialPort_dev_t *device)
{
	int index=0;
	hal_uart_init();

	if(device->status == tty_Close)
	{
		for(index = 0;index<ARRAY_SIZE(initFunctions);++index)
		{
			if(initFunctions[index](device) == 0)
				return 0;
		}
		return 1;
	}
	return 0;
}


unsigned char hal_serialPort_Open(hal_serialPort_dev_t *device)
{
	return hal_set_Status_Open(device);
}

unsigned char hal_serialPort_Close(hal_serialPort_dev_t *device)
{
	return hal_set_Status_Close(device);
}

unsigned char hal_serialPort_Status(hal_serialPort_dev_t *device)
{
	return device->status;
}

unsigned char hal_serialPort_Write(hal_serialPort_dev_t *device,char *data)
{
	U32 ret = 1;

	U32  count = 0;

	switch(device->index)
	{
		case tty0:
					while(count++ < 50000)
					{
						if(MFS0_UART_SSR_TDRE==1 &&  MFS0_UART_SSR_TBI ==1)
						{
							if (*data == 0x00) // 字符串结束
							{
								ret = 0;
								break;
							}
							if (*data == 0x0A) // \n结尾的字符串
							{
								MFS0_UART_TDR0 = 0x0d;
								while (MFS0_UART_SSR_TDRE != 1 || MFS0_UART_SSR_TBI != 1);
							}
							MFS0_UART_TDR0=*data;
							data++;
						}
					}
					break;

		case tty1:
					while(count++ < 50000)
					{
						if(MFS1_UART_SSR_TDRE==1 &&  MFS1_UART_SSR_TBI ==1)
						{
							MFS1_UART_TDR0=*data;
							data++;
							ret = 0;
							break;
						}
					}
					break;
	}
	return ret;
}


__interrupt void uart1_rx_interrupt(void)
{
    char ch = 0;

    /*  Check for errors PE, ORE, FRE   */
    if(MFS1_UART_SSR_PE || MFS1_UART_SSR_FRE || MFS1_UART_SSR_ORE)
    {
        MFS1_UART_SSR_REC = 1;          	/*  Clear error flags    */
    }
    else if(MFS1_UART_SSR_RDRF == 1)  /* Reception end interrupt */
    {
        MFS1_UART_SSR_RDRF = 0;
        ch = MFS1_UART_RDR0;
        if (uart_en_queue(&uart_recv_queue, ch))//入队列错误
        {
            uart_de_queue(&uart_recv_queue);//先让一个数据出列
            uart_en_queue(&uart_recv_queue, ch);//把刚才的数据重新入队列
        }
    }

}


__interrupt void uart0_rx_interrupt(void)
{
    unsigned char ch = 0;


    /*  Check for errors PE, ORE, FRE   */
    if(MFS0_UART_SSR_PE || MFS0_UART_SSR_FRE || MFS0_UART_SSR_ORE)
    {
        MFS0_UART_SSR_REC = 1;          	/*  Clear error flags    */
    }
    else if(MFS0_UART_SSR_RDRF == 1)  /* Reception end interrupt */
    {
        MFS0_UART_SSR_RDRF = 0;
        ch = MFS0_UART_RDR0;
        if(uart_en_queue(&uart_recv_queue, ch))
        {
            drop_first_cmd(&uart_recv_queue);
            uart_en_queue(&uart_recv_queue, ch);
        }
    }

}





/* uart interrupt */
__interrupt void  uart1_tx_interrupt(void)
{
	signed short ch;
	ch = uart_de_queue(&uart_sent_queue);

	if(ch == -1)
	{
		MFS1_UART_SCR_TIE = 0;	//stop interrupt	
	}
	else
	{
		MFS1_UART_TDR0 = ch;
	}


}

void hal_enable_uart_tx_interrupt(U8         enable)
{
	if(enable)
	{
		MFS1_UART_SCR_TIE = 1;  //enable
		MFS1_UART_SSR_TDRE = 1;
	}  
	else
	{
		MFS1_UART_SCR_TIE = 0;  //disable
	}
	
}

//////////////////
void hal_enable_uart0_tx_interrupt(U8         enable)  
{
	if(enable)
	{
		MFS0_UART_SCR_TIE = 1;  //enable
		MFS0_UART_SSR_TDRE = 1;
	}
	else
	{
		MFS0_UART_SCR_TIE = 0;  //disable
	}
	
}


