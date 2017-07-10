#include "hal_uart.h"
#include "comm_typedef.h"
#include "KeyPortReg.h"
#include "mb91520.h"
#include "hal_gpio.h"
#include "uart_queue.h"
#include "hal_clock.h"
#ifdef WIN32
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#endif

UartQueue	uart_recv_queue = {0};
static U8	recv_buf[BUF_REC_SIZE] = {0};
UartQueue	uart_sent_queue = {0};
static U8	sent_buf[BUF_SENT_SIZE] = {0};


void hal_uart_init(void)
{
    uart_queue_init(&uart_recv_queue,recv_buf,BUF_REC_SIZE);
    uart_queue_init(&uart_sent_queue,sent_buf,BUF_SENT_SIZE);
}



U8   forbid_output = 0;

/*================================================================================
** input
				nPortNum:端口编号（0-1）
				nBaudRate:波特率
				nDataBits:数据长度位数
				nMumOfStopBit:1-4
				nParity: 0: 不校验
						 1：奇校验
						 2：偶校验

** output	    1：ERR 0:OK
*/
int uart_open(U8 nPortNum, U32 nBaudRate, U8 nWordLength, U8 nNumOfStopBit, U8 nParity)
{
    int l_nReturn=1;

    //ICR6 = 22;
    //unsigned int l_nTemp =0;
    hal_uart_init();
    //Reset related UART module & GPIO register
    switch(nPortNum)
    {
    case 0:
        //Reset UART Module
        MFS0_UART_SCR_UPCL =1;	//reset UART ch 0	, important: it should be reset UART marco first
        //GPIO Pin
        EPFR[35] &= MASK0;	//BIT0 为0，input from UART0_0
        EPFR[35] |= BIT3;   //bit4,bit3 = 01,output from UART0_0
        EPFR[35] &= MASK4;
        WriteKeyProtectedRegister( (unsigned int)&EPFR35, EPFR[35], BYTE_ACCESS );
        set_single_io_dir(12,7,IO_FUNC_OUTPUT);
        set_single_io_dir(12,6,IO_INPUT);
        //MFS1_UART_SCR_TXE = 0;		//disable sent


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

        if(uart_changeBaudRate(nPortNum,nBaudRate)==0)
            return 0;

        switch(nWordLength)
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
            //break;
        }

        switch(nNumOfStopBit)
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
            //break;
        }

        switch(nParity)
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
        default:
            return 0;
            //break;
        }


        //SCR
        MFS0_UART_SCR_TXE=1;	//Enable TX	, should be set at last
        MFS0_UART_SCR_RXE=1;	//Enable RX	, should be set at last
        MFS0_UART_SCR_RIE  = 1;    /* enable reception interrupt */
        //SSR
        MFS0_UART_SSR_REC=1;	//Clear error flag for reception , SCR4_CRE=1;

        break;

    case 1:
        //Reset UART Module
        MFS1_UART_SCR_UPCL =1;	//reset UART ch 0	, important: it should be reset UART marco first
        //GPIO Pin
        EPFR[36] |= BIT3;     // SOT1_0 output enable,set 1
        WriteKeyProtectedRegister((unsigned int)&EPFR36, EPFR[36], BYTE_ACCESS);

        set_single_io_dir(0,0,IO_INPUT);
        set_single_io_dir(0,1,IO_FUNC_OUTPUT);


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

        //FIFO_Disable
        //FCR0
        //UART0_UART0_FCR00_FLST = 0;
        //UART0_UART0_FCR00_FLD =0;
        //UART0_UART0_FCR00_FSET=0;
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

        if(uart_changeBaudRate(nPortNum,nBaudRate)==0)
            return 0;

        switch(nWordLength)
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
            //break;
        }

        switch(nNumOfStopBit)
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
        default:
            return 0;
            //break;
        }

        switch(nParity)
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
        default:
            return 0;
            //break;
        }


        //SCR
        MFS1_UART_SCR_TXE=1;	//Enable TX	, should be set at last
        MFS1_UART_SCR_RXE=1;	//Enable RX	, should be set at last
        MFS1_UART_SCR_RIE  = 1;    /* enable reception interrupt */
        //SSR
        //MFS1_UART_SSR_REC=1;	//Clear error flag for reception , SCR4_CRE=1;
        break;
    case 2:
        l_nReturn=0;
        break;
        //UPCL2=1;	//reset UART module
        //REST2=1;	//reset roload counter
        //break;
    default:
        l_nReturn=0;
        break;

        //break;
    }
    return l_nReturn;
}

/*
** input
				nPortNum:端口编号（0-1）
				nBaudRate: 波特率
** output	    1：ERR 0:OK
================================================================================
*/
int uart_changeBaudRate(U8 nPortNum, U32 nBaudRate)
{
    U32 l_nRLValue;
    //unsigned long l_nTemp;

    l_nRLValue = SupplyClk/(nBaudRate) -1;

    if(l_nRLValue<1 || l_nRLValue>65535)
        return 0;

    //set PLL value
    switch(nPortNum)
    {
    case 0:
        BGR0 = (__HWORD)(l_nRLValue);
        break;
    case 1:
        BGR1 = (__HWORD)(l_nRLValue);
        break;
    default:
        return 0;
        //break;
    }
    return 1;
}
/*
** input
				nPortNum:端口编号（0-1）
				* pChar: 写入字符指针
				a_nStringLen:字符长度
				a_IsCheckLen：数据长度检查 0：不检查 1：检查
** output	    1：ERR 0:OK
*/
int uart_write(U8 nPortNum, U8 * pChar)
{
    //Suggest to be called anytime except ISR
    U32 ret = 1;
#ifndef WIN32
    U32  count = 0;

    switch(nPortNum)
    {
    case 0:
        while(count++ < 50000)
        {
            //TX immedicately if TX register is all empty and UART TX register is also empty
            if(MFS0_UART_SSR_TDRE==1 &&  MFS0_UART_SSR_TBI ==1)
            {
            	if (*pChar == 0x00)	// 字符串结束
            	{
            		ret = 0;
					break;
            	}
				if (*pChar == 0x0A)	// \n结尾的字符串
				{
					MFS0_UART_TDR0 = 0x0d;
					while (MFS0_UART_SSR_TDRE != 1 || MFS0_UART_SSR_TBI != 1);
				}
                MFS0_UART_TDR0=*pChar;
                pChar++;
            }
        }
        break;

    case 1:
        while(count++ < 50000)
        {
            //TX immedicately if TX register is all empty and UART TX register is also empty
            if(MFS1_UART_SSR_TDRE==1 &&  MFS1_UART_SSR_TBI ==1)
            {
                MFS1_UART_TDR0=*pChar;
                pChar++;
                ret = 0;
                break;
            }
        }
        break;
    }
    return ret;
#else
    if(!forbid_output)
    {
        //putchar(*pChar);
        printf("%x ",*pChar);
    }
    return 0;
#endif

}

/*
** input
				nPortNum:端口编号（0-1）
				* r_pChar:读取数据存放的地址
** output	    1：ERR 0:OK
*/

U8 uart_read(U8 nPortNum, U8 * r_pChar)
{
    U8 ret = 1;
#if 1
    if(r_pChar==0)
        return 1;

    *r_pChar=0xff;

    switch(nPortNum)
    {
    case 0:
        if(MFS0_UART_SSR_RDRF==1)
        {
            *r_pChar=MFS0_UART_RDR0;
            //MFS0_UART_SSR_RDRF = 0;
            ret = 0;
        }
        break;
    case 1:
        if(MFS1_UART_SSR_RDRF==1)
        {
            *r_pChar=MFS1_UART_RDR0;
            //MFS1_UART_SSR_RDRF = 0;
            ret = 0;
        }
        break;
    default:
        ret = 1;
        break;
    }
#endif
    return ret;
}

/* 输出一个字符 */
/*
ret: 1,putout errr
	 0:putout OK
*/
U8  hal_uart_put_char(U8 chn, U8 data)
{
    U8 ret = 1;
    U8 temp = data;
    if(1 == uart_write(chn, &temp))
    {
        ret = 1;
    }
    else
    {
        ret = 0;
    }
    return (ret);
}


__interrupt void uart1_rx_interrupt(void)
{
    S8 ch = 0;
#ifdef WIN32
    MFS1_UART_SSR_RDRF =1;
#endif
    do
    {
        /*  Check for errors PE, ORE, FRE   */
        if(MFS1_UART_SSR_PE || MFS1_UART_SSR_FRE || MFS1_UART_SSR_ORE)
        {
            MFS1_UART_SSR_REC = 1;          	/*  Clear error flags    */
        }
        else if(MFS1_UART_SSR_RDRF == 1)  /* Reception end interrupt */
        {
            MFS1_UART_SSR_RDRF = 0;

#ifndef WIN32

            ch = MFS1_UART_RDR0;
#else
            if(_kbhit())
            {
                ch = _getch();
            }
            if(ch <= 0)
            {
                break;
            }
#endif
            if (uart_en_queue(&uart_recv_queue, ch))//入队列错误
            {
                uart_de_queue(&uart_recv_queue);//先让一个数据出列
                uart_en_queue(&uart_recv_queue, ch);//把刚才的数据重新入队列
            }
        }
    }
    while (0);
}


__interrupt void uart0_rx_interrupt(void)
{
    U8 ch = 0;

    do
    {
        /*  Check for errors PE, ORE, FRE   */
        if(MFS0_UART_SSR_PE || MFS0_UART_SSR_FRE || MFS0_UART_SSR_ORE)
        {
            MFS0_UART_SSR_REC = 1;          	/*  Clear error flags    */
        }
        else if(MFS0_UART_SSR_RDRF == 1)  /* Reception end interrupt */
        {
            MFS0_UART_SSR_RDRF = 0;
#ifndef WIN32

            ch = MFS0_UART_RDR0;
#else
            if(_kbhit())
            {
                ch = _getch();
            }
            if(ch <= 0)
            {
                break;
            }
            else if(ch == 27)		/* 按下ESC键盘停止输出 */
            {
                forbid_output = 1;
            }
            else if(ch == '\r')		/* 按回车键继续输出 */
            {
                forbid_output = 0;
            }
#endif
            if(uart_en_queue(&uart_recv_queue, ch))
            {
                drop_first_cmd(&uart_recv_queue);
                uart_en_queue(&uart_recv_queue, ch);
            }
        }
    }
    while(0);
}




void uart_sent_task10ms(void)
{
	S8  ch;
	U8  index = 0;
	for(index = 0;index <10;index++)
	{	
		ch = (S8)uart_de_queue(&uart_sent_queue);
		if(ch < 0)	//queue empty
		{
			break;
		}
		else
		{
			 hal_uart_put_char(UART_A20_CHN, ch);
		}
	}
}

/* uart interrupt */
__interrupt void  uart1_tx_interrupt(void)
{
	S16  ch;
	ch = uart_de_queue(&uart_sent_queue);

	if(ch == -1)
	{
		MFS1_UART_SCR_TIE = 0;	//stop interrupt	
	}
	else
	{
#ifndef WIN32	
		MFS1_UART_TDR0 = ch;
#else
		if(ch == FRAME_HEAD)
		{
			printf("\n");
		}
		printf("%02X ",ch);
		if((ch == FRAME_REAR))
		{
			printf("\n");
		}
#endif
	}


}

void hal_enable_uart_tx_interrupt(U8  enable)
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
