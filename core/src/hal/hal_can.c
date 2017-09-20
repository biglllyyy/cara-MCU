#include <stdlib.h>
#include "_fr.h"
#include "hal_can.h"
#include "hal_gpio.h"
#include "KeyPortReg.h"
#include "mb91520.h"
#include "hal_clock.h"
#include "mid_dbg.h"

#define REC_MSG_BOX_START	    4
#define REC_MSG_NUM				8
#define SENT_MSG_BOX_START		1
#define SNE_MSG_NUM				1
#define  CHN_0					0

U32 sp_IO_CANn[] = {(U32)&IO_CAN0,(U32)&IO_CAN1,(U32)&IO_CAN2};


/* set parameter for CAN error state judge */
static U8 Error_State_0=0x00;

/* set parameter for CAN received data */

extern void can_msg_parse(can_msg_t *can_msg);
static void can_receive_object_init(U8 chn,U8 num,CAN_ID_FORMAT_e format,U32 id,U8 dlc,U32 id_mask);
static void can_sent_object_init(U8 chn,U8 num,CAN_ID_FORMAT_e format,U32 id,U8 dlc);

static U8 cntRecv = 0, cntSend = 0;
static can_msg_t*can_msg=0;

static can_msg_t* can1_rx_msg = NULL;
static can_msg_t* can1_tx_msg = NULL;

void hal_can_prepare(U8 nRecv, U8 nSend,can_msg_t*msg)
{
	
	if (cntRecv || cntSend)
	{
		dbg_string("Error: CAN already initialized\n");
		return;
	}
	cntRecv = nRecv;
	cntSend = nSend;
	can_msg=msg;
	if (!can_msg)
	{
		dbg_string("The Pointer of can_msg from mid_can is NULL\n");
		return;
	}
}
void hal_can1_prepare(can_msg_t*msg_tx,can_msg_t*msg_rx)
{
	can1_tx_msg = msg_tx;
	can1_rx_msg = msg_rx;
	if (NULL == can1_tx_msg)
	{
		dbg_string("The Pointer of can_msg from mid_can1 TX msg is NULL\n");
	}
	if (NULL == can1_rx_msg)
	{
		dbg_string("The Pointer of can_msg from mid_can1 RX msg is NULL\n");
	}
}

void hal_can_init(U8 chn)
{
    int i = 0;
    switch(chn)
    {
    case 0:
#if 0
        WriteKeyProtectedRegister((unsigned int)&DDR09, 0x00, BYTE_ACCESS);
        WriteKeyProtectedRegister((unsigned int)&PFR09, 0x20, BYTE_ACCESS);	/* TX0/P95,RX0/P96 */
        WriteKeyProtectedRegister((unsigned int)&EPFR86, 0xE1, BYTE_ACCESS);/* TX0,TX1 Output valid */
        WriteKeyProtectedRegister((unsigned int)&ADERL0, 0x0000, HALFWORD_ACCESS);
        WriteKeyProtectedRegister((unsigned int)&ADERH0, 0x0000, HALFWORD_ACCESS);
        WriteKeyProtectedRegister((unsigned int)&PORTEN, 0xFD, BYTE_ACCESS);
#endif
        Error_State_0 = 0;
        IO_ICR[18].byte =0x10;			/* CAN0 TX/RX/Error status Int level set */
#if (MCLK_FRQ == 4000000)
        IO_CANPRE.byte = 0x0E;			//select PLL,CAN clock=PLL/10
#elif(MCLK_FRQ == 8000000)
        IO_CANPRE.byte = 0x10;			//MCLK select,CAN clock=8M
#endif

//fsys = 16MHz
        /* enable digital input (RX0) */
        IO_CAN0.CTRLR.bit.Init = 1; 				/* CAN Controller Stop £¬test mode */


        for(i = 0; i<cntRecv; i++)
        {
          can_receive_object_init(chn,can_msg[i].buffer_num,can_msg[i].format,can_msg[i].id,can_msg[i].dlc,can_msg[i].id_mask);  //from msgbox 1 rec 
        }
		for(i= cntRecv;i<cntRecv+cntSend;i++)
		{
			can_sent_object_init(chn,can_msg[i].buffer_num,can_msg[i].format,can_msg[i].id,can_msg[i].dlc);		//·¢ËÍ
		}
        /* CAN bus setting */
        IO_CAN0.CTRLR.hword = 0x0041;		/* BTR/BRPE Write Enable */
#if (MCLK_FRQ == 4000000)
        IO_CAN0.BTR.hword   = MCLK_8M_BTR_250K;		/* 500kbps change the Baud rate to 250K by niujianlong*/
#elif(MCLK_FRQ == 8000000)
        IO_CAN0.BTR.hword   = MCLK_8M_BTR_500K;
#endif
        IO_CAN0.BRPER.hword = 0x0000;		/* bord rate extern  */
        IO_CAN0.CTRLR.hword = 0x0001;		/* BTR/BRPE Write Disable */
        /* Normal mode */
        IO_CAN0.CTRLR.hword = 0x0001;		/* Test=0 CCE=0 DAR=0 EIE=1 SIE=0 IE=1 Init=1*/	 //change 0x000B to 0x0001		
        /* Test mode */
        //IO_CAN0.CTRLR.hword = 0x000B;		/* BTR/BRPE Write Disable, Test mode ON */
        /*lookback mode */
        //IO_CAN0.TESTR.hword = 0x0010;		/* LoopBack mode ON */
        IO_CAN0.CTRLR.bit.Init = 0;				/* CAN Controller Start */
        break;
    case 1:
		IO_CAN1.CTRLR.bit.Init = 1;               /* CAN Controller Stop £¬test mode */
		for(i=0; can1_rx_msg[i].id != 0;i++)
		{
			can_receive_object_init(chn,can1_rx_msg[i].buffer_num,can1_rx_msg[i].format,can1_rx_msg[i].id,can1_rx_msg[i].dlc,can1_rx_msg[i].id_mask);  //from msgbox 1 rec 
		}
		for(i= 0; can1_tx_msg[i].id != 0;i++)
		{
			dbg_printf("init can buf = %d\n",can1_tx_msg[i].buffer_num);
			can_sent_object_init(chn,can1_tx_msg[i].buffer_num,can1_tx_msg[i].format,can1_tx_msg[i].id,can1_tx_msg[i].dlc);		//·¢ËÍ
		}
        /* CAN bus setting */
        IO_CAN1.CTRLR.hword = 0x0041;		/* BTR/BRPE Write Enable */
#if (MCLK_FRQ == 4000000)
        IO_CAN1.BTR.hword   = MCLK_8M_BTR_250K;		/* 500kbps change the Baud rate to 250K by niujianlong*/
#elif(MCLK_FRQ == 8000000)
        IO_CAN1.BTR.hword   = MCLK_8M_BTR_500K;
#endif
        IO_CAN1.BRPER.hword = 0x0000;		/* bord rate extern  */
        IO_CAN1.CTRLR.hword = 0x0001;		/* BTR/BRPE Write Disable */
        /* Normal mode */
        IO_CAN1.CTRLR.hword = 0x0001;		/* Test=0 CCE=0 DAR=0 EIE=1 SIE=0 IE=1 Init=1*/	 //change 0x000B to 0x0001		
        IO_CAN1.CTRLR.bit.Init = 0;				/* CAN Controller Start */		
        break;
    case 2:
        break;
    default:
        break;
    }
}

/*
**************************************************************************
Message Object make
U8 RxTx: receive or sent,0:receive,1:sent
num : message box num
id  : receive or sent id
dlc : data len

**************************************************************************
*/
#define CAN1_MSGB_SIZE 64
void can_receive_object_init(U8 chn,U8 num,CAN_ID_FORMAT_e format,U32 id,U8 dlc,U32 id_mask)
{
    switch(chn)
    {
    case 0:
        /*** Rx ***/
        /*MSK select*/
        IO_CAN0.IF2CMSK.hword = 0x00F0;			/*WR/RD=1 Mask=1 Arb=1 Control=1
												CIP=0 TxRqst/NewDat=0 DataA=0 DataB=0*/

	if(STAND_ID == format)
	{
        /*Arb Data*/
        /*MSK Data*/
        IO_CAN0.IF2MSK.word = 0xFFFC0000;				/*MXtd=1 MDir=1 res=1 MID28-MID18 all=1
														  MID17-MID0 all=0*/
		IO_CAN0.IF2MSK.bit.MXtd = 0;    /* extened frame */
        IO_CAN0.IF2ARB.word = 0x80000000|(id << 18);	/*MsgVal=1 Xtd=0 Dir=0 ID(28-18)=2*/
	}
	else
	{
		IO_CAN0.IF2MSK.word = id_mask;				/*MXtd=1 MDir=1 res=1 MID28-MID18 all=1
														MID17-MID0 all=0*/
		IO_CAN0.IF2MSK.bit.MXtd = 1;    /* extened frame */
        IO_CAN0.IF2ARB.word = 0xC0000000|(id & 0x1FFFFFFF);	/*MsgVal=1 Xtd=1 Dir=0 ID(28-18)=1 TestMode Only*/
	}

        /*MCTR*/
        IO_CAN0.IF2MCTR.hword = 0x1480 | dlc;	/*NewDat=Nouse MsgLst=0 IntPnd=0
												UMask=1 TxIE=0 RxIE=1 RmtEn=0
												TxRqst=Nouse EoB=1 DLC=8*/

        /*CREQ*/
        IO_CAN0.IF2CREQ.hword = num;		/*0x02,transmit IFx to message RAM use buffer2*/

        break;
    case 1:
		/*** Rx ***/
		/*MSK select*/
		IO_CAN1.IF2CMSK.hword = 0x00F0;			/*WR/RD=1 Mask=1 Arb=1 Control=1
													CIP=0 TxRqst/NewDat=0 DataA=0 DataB=0*/

		if(STAND_ID == format)
		{
			/*Arb Data*/
			/*MSK Data*/
			IO_CAN1.IF2MSK.word = 0xFFFC0000;				/*MXtd=1 MDir=1 res=1 MID28-MID18 all=1
																MID17-MID0 all=0*/
			IO_CAN1.IF2MSK.bit.MXtd = 0;    /* extened frame */
			IO_CAN1.IF2ARB.word = 0x80000000|(id << 18);	/*MsgVal=1 Xtd=0 Dir=0 ID(28-18)=2*/
		}
		else
		{
			IO_CAN1.IF2MSK.word = id_mask;				/*MXtd=1 MDir=1 res=1 MID28-MID18 all=1
															MID17-MID0 all=0*/
			IO_CAN1.IF2MSK.bit.MXtd = 1;    /* extened frame */
			IO_CAN1.IF2ARB.word = 0xC0000000|(id & 0x1FFFFFFF);	/*MsgVal=1 Xtd=1 Dir=0 ID(28-18)=1 TestMode Only*/
		}

		/*MCTR*/
		IO_CAN1.IF2MCTR.hword = 0x1480 | dlc;	/*NewDat=Nouse MsgLst=0 IntPnd=0
		UMask=1 TxIE=0 RxIE=1 RmtEn=0
		TxRqst=Nouse EoB=1 DLC=8*/

		/*CREQ*/
		IO_CAN1.IF2CREQ.hword = num;		/*0x02,transmit IFx to message RAM use buffer2*/
		break;
	case 2:
		break;
    default:
        break;
    }

}

void can_sent_object_init(U8 chn,U8 num,CAN_ID_FORMAT_e format,U32 id,U8 dlc)
{

    switch(chn)
    {
    case 0:
        /*MSK select*/
        IO_CAN0.IF1CMSK.hword = 0x00F0;		/*WR/RD=1 Mask=1 Arb=1 Control=1
											CIP=0 TxRqst/NewDat=0 DataA=0 DataB=0*/
        /*Arb Data*/
		if(STAND_ID == format)
		{
        IO_CAN0.IF1ARB.word = 0xA0000000|(id<<18);	/* 0xA0040000;	MsgVal=1:msg object valid
													Xtd=0:0:11 bit id,1:29 bit id
													Dir=1:0:Indicates the reception direction.,1:Indicates the transmission direction.
													ID(28-18)=1*/
		}
		else
		{
			IO_CAN0.IF1ARB.word = 0xE0000000|(id & 0x1FFFFFFF);  /* MsgVal:1,Xtd:1,Dir:1*/
		}

        /*MCTR*/
        IO_CAN0.IF1MCTR.hword = 0x0080 | dlc;		/*NewDat=Nouse MsgLst=0 IntPnd=0
													UMask=0 TxIE=0 RxIE=0 RmtEn=0
													TxRqst=0(Nouse) EoB=1 DLC=8*/
        /*Data*/
        IO_CAN0.IF1DTA1.hword = 0x0000;
        IO_CAN0.IF1DTA2.hword = 0x0000;
        IO_CAN0.IF1DTB1.hword = 0x0000;
        IO_CAN0.IF1DTB2.hword = 0x0000;

        /*CREQ*/
        IO_CAN0.IF1CREQ.hword = num;			/*transmit IFx to message RAM use buffer1*/
        break;
	case 1:
        /*MSK select*/
        IO_CAN1.IF1CMSK.hword = 0x00F0;		/*WR/RD=1 Mask=1 Arb=1 Control=1
											CIP=0 TxRqst/NewDat=0 DataA=0 DataB=0*/
        /*Arb Data*/
		if(STAND_ID == format)
		{
        IO_CAN1.IF1ARB.word = 0xA0000000|(id<<18);	/* 0xA0040000;	MsgVal=1:msg object valid
													Xtd=0:0:11 bit id,1:29 bit id
													Dir=1:0:Indicates the reception direction.,1:Indicates the transmission direction.
													ID(28-18)=1*/
		}
		else
		{
			IO_CAN1.IF1ARB.word = 0xE0000000|(id & 0x1FFFFFFF);  /* MsgVal:1,Xtd:1,Dir:1*/
		}

        /*MCTR*/
        IO_CAN1.IF1MCTR.hword = 0x0080 | dlc;		/*NewDat=Nouse MsgLst=0 IntPnd=0
													UMask=0 TxIE=0 RxIE=0 RmtEn=0
													TxRqst=0(Nouse) EoB=1 DLC=8*/
        /*Data*/
        IO_CAN1.IF1DTA1.hword = 0x0000;
        IO_CAN1.IF1DTA2.hword = 0x0000;
        IO_CAN1.IF1DTB1.hword = 0x0000;
        IO_CAN1.IF1DTB2.hword = 0x0000;

        /*CREQ*/
        IO_CAN1.IF1CREQ.hword = (CAN1_MSGB_SIZE-1) - num;			/*transmit IFx to message RAM use buffer1*/		
		break;
    default:
        break;
    }

}

void State_judge_0(void)
{
    U32 count = 0;

    if(IO_CAN0.STATR.bit.BOff==0x01)		//bus off
    {
        Error_State_0=0x01;

        /*Restart bus*/
        IO_CAN0.CTRLR.bit.Init = 0;		/*enable CAN controller*/
        while(((IO_CAN0.ERRCNT.bit.TEC!=0)||(IO_CAN0.ERRCNT.bit.REC!=0)) && (count++ <= 0xFFFFF));//see if recovered
    }
    if(IO_CAN0.STATR.bit.EWarn==0x01)		//error warning
    {
        Error_State_0 = 0x02;
    }
    if(!((IO_CAN0.STATR.bit.BOff)|(IO_CAN0.STATR.bit.EWarn)|(IO_CAN0.STATR.bit.EPass))) //error active
    {
        Error_State_0 = 0x03;				//error active
    }
}


void TxRx_Judge_0(void)
{
    S16   MsgNbr = 0x00;
    U32  MsgBuffer;
    U8  i =0;
    ///check///
    MsgNbr = IO_CAN0.INTR.hword;				//stor MsgNbr

    if( (MsgNbr>=1) && (MsgNbr<=0x20) ) // valid buffer number
    {
        MsgBuffer = 0x01 << (MsgNbr-1);
        // Check whether the interrupt source is a valid buffer

        if(((IO_CAN0.MSGVAL12.hword.MSGVAL1 & MsgBuffer) != 0) && ((IO_CAN0.INTPND12.word & MsgBuffer) != 0))
        {
            // Check whether the interrupt cause is receive or transmit
            if( (IO_CAN0.NEWDT12.word & MsgBuffer) != 0 ) // is a receive interrupt
            {

                /*fetch data from msg RAM*/
                IO_CAN0.IF2CMSK.hword = 0x007F;			/*WR/RD=0 Mask=1 Arb=1 Control=1
														CIP=1 TxRqst/NewDat=1 DataA=1 DataB=1*/

                IO_CAN0.IF2CREQ.bit.MN = MsgNbr;		//transmit msgRAM to IF

                if(IO_CAN0.IF2MCTR.bit.MsgLst==0x01)
                {
                    __wait_nop();  //mag lost
                    IO_CAN0.IF2MCTR.hword = 0x1488;		//NewDat=0 MSGLST=0 INTPND=0 UMSK=1 TXIE=0
                    //RXIE=1 RMTEN=0 TXRQST=0 EOB=1
                    IO_CAN0.IF2CMSK.hword = 0x0090;		//WRRD=1 CONTROL=1 other=0
                    //for clear MSGLST
                    IO_CAN0.IF2CREQ.bit.MN = MsgNbr;
                }
                else
                {
                    MsgNbr-=2;
                    if((MsgNbr>=0) && (MsgNbr < cntRecv))
                    {
                        can_msg[MsgNbr].dlc = (U8)CAN0_IF2MCTR_DLC;
                    }
                    switch(can_msg[MsgNbr].dlc)
                    {
                    case 8:
                        can_msg[MsgNbr].data[7] = CAN0_IF2DTB2_IF2DTB2L;//save data from buffer to RAM
                    case 7:
                        can_msg[MsgNbr].data[6] = CAN0_IF2DTB2_IF2DTB2H;
                    case 6:
                        can_msg[MsgNbr].data[5] = CAN0_IF2DTB1_IF2DTB1L;
                    case 5:
                        can_msg[MsgNbr].data[4] = CAN0_IF2DTB1_IF2DTB1H;
                    case 4:
                        can_msg[MsgNbr].data[3] = CAN0_IF2DTA2_IF2DTA2L;
                    case 3:
                        can_msg[MsgNbr].data[2] = CAN0_IF2DTA2_IF2DTA2H;
                    case 2:
                        can_msg[MsgNbr].data[1] = CAN0_IF2DTA1_IF2DTA1L;
                    case 1:
                        can_msg[MsgNbr].data[0] = CAN0_IF2DTA1_IF2DTA1H;
                        can_msg[MsgNbr].new_frame = 1;					/* new frame receive */
                        can_msg[MsgNbr].lost = 0;							/* clear lost flg */
                        can_msg[MsgNbr].count = 0;
                        break;
                    case 0:
                        break;
                    default:
                        break;
                    }
                    for(i=8; i>can_msg[MsgNbr].dlc; i--)
                    {
                        can_msg[MsgNbr].data[i-1] = 0;
                    }
                }
            }
            else if ( (IO_CAN0.TREQR12.word & MsgBuffer) == 0 ) // is transmission done
            {
                /* for clear Tx intPnd */
                IO_CAN0.IF1MCTR.hword = 0x0888;		//NEWDAT=0 MSGLST=0 INTPND=0 UMASK=0
                //TXIE=1 RXIE=0 RMTEN=0 TXRQST=0 EOB=1 DLC=8

                IO_CAN0.IF1CMSK.hword = 0x0090;		//WRRD=1 MASK=0 ARB=0 CONTROL=1
                //CIP=0 TXREQ=0 DTAA/B=0
                IO_CAN0.IF1CREQ.bit.MN = MsgNbr;	//IF->RAM
            }
        }
    }
}

void __interrupt CAN_0_int(void)
{
    if(IO_CAN0.INTR.hword == 0x8000)		/* status int */
    {
        State_judge_0();					//judge state only when INTR==0x8000
    }
    else if((IO_CAN0.INTR.hword >= 0x0001)&&(IO_CAN0.INTR.hword <= 0x0020))  /* buffer int */
    {
        TxRx_Judge_0();						//judge Tx or Rx interrupt
    }
    else
    {
        __wait_nop();
    }
}


void hal_can_sent(U8 chn,can_msg_t *can_msg)
{
    do
    {
        switch(chn)
        {
        case 0:
            if(IO_CAN0.INTR.hword == 0x8000)		/* status int */
            {
                State_judge_0();
            }
            if(Error_State_0 == 0x01)
            {
                break;
            }
            IO_CAN0.IF1CMSK.hword = 0x0087;	/* only updata DATA,0x87*/
            // WRRD    = 1 : Read information from buffer to IF
            // MASK    = 0 : no read of Mask regsiter
            // ARB     = 0 : Read Arbitration Register (ID Register)
            // CONTROL = 0 : Read object control register (e.g. Data lenght, IRQ enable, etc)
            // CIP     = 0 : (dont care for writing to object)
            // TXREQ   = 1 : 0:No Transmission Request
            // DATAA   = 1 : 0:Do not read Data A register 1:update data0-3
            // DATAB   = 1 : 0;Do not read Data B register 1:update data4-7
            CAN0_IF1ARB_Dir = 1;
            CAN0_IF1ARB_MsgVal = 1;
            CAN0_IF1DTA1_IF1DTA1H = can_msg->data[0];
            CAN0_IF1DTA1_IF1DTA1L = can_msg->data[1];
            CAN0_IF1DTA2_IF1DTA2H = can_msg->data[2];
            CAN0_IF1DTA2_IF1DTA2L = can_msg->data[3];
            CAN0_IF1DTB1_IF1DTB1H = can_msg->data[4];
            CAN0_IF1DTB1_IF1DTB1L = can_msg->data[5];
            CAN0_IF1DTB2_IF1DTB2H = can_msg->data[6];
            CAN0_IF1DTB2_IF1DTB2L = can_msg->data[7];
            IO_CAN0.IF1MCTR.hword = 0x2980|(can_msg->dlc);		//NEWDAT=0 MSGLST=0 INTPND=1 UMASK=0
            //TXIE=1 RXIE=0 RMTEN=0 TXRQST=1 EOB=1 DLC=8
            IO_CAN0.IF1CREQ.bit.MN= can_msg->buffer_num;		//IF -> RAM

            break;
        case 1:
            IO_CAN1.IF1CMSK.hword = 0x0087;	/* only updata DATA */
			CAN1_IF1ARB_Dir = 1;
            CAN1_IF1ARB_MsgVal = 1;
            CAN1_IF1DTA1_IF1DTA1H = can_msg->data[0];
            CAN1_IF1DTA1_IF1DTA1L = can_msg->data[1];
            CAN1_IF1DTA2_IF1DTA2H = can_msg->data[2];
            CAN1_IF1DTA2_IF1DTA2L = can_msg->data[3];
            CAN1_IF1DTB1_IF1DTB1H = can_msg->data[0];
            CAN1_IF1DTB1_IF1DTB1L = can_msg->data[1];
            CAN1_IF1DTB2_IF1DTB2H = can_msg->data[2];
            CAN1_IF1DTB2_IF1DTB2L = can_msg->data[3];
            IO_CAN1.IF1MCTR.hword = 0x2988;		//NEWDAT=0 MSGLST=0 INTPND=1 UMASK=0
            //TXIE=1 RXIE=0 RMTEN=0 TXRQST=1 EOB=1 DLC=8
            IO_CAN1.IF1CREQ.bit.MN=CAN1_MSGB_SIZE -1 - can_msg->buffer_num;		//IF -> RAM
            break;
        default:
            break;
        }
    }
    while (0);

}




U8 hal_can_get(can_msg_t *can_rx_ptr)
{
		IO_CAN0.IF2CMSK.byte.IF2CMSKL = 0x3F;
		CAN0_IF2CREQ_IF2CREQL = can_rx_ptr->buffer_num;

		if (IO_CAN0.IF2MCTR.bit.MsgLst)    /* check whether or not a message was lost */
		{
			IO_CAN0.IF2MCTR.bit.MsgLst = 0;   /*  clear MSGLST Flag */
			IO_CAN0.IF2CMSK.byte.IF2CMSKL = 0x90;
			CAN0_IF2CREQ_IF2CREQL = can_rx_ptr->buffer_num;
		}
		can_rx_ptr->dlc =  (U8)IO_CAN0.IF2MCTR.bit.DLC;       /* data length */
		can_rx_ptr->format = (IO_CAN0.IF2ARB.bit.Xtd == 1)?EXTERN_ID:STAND_ID;
		if(can_rx_ptr->format)	//extern id
		{
			can_rx_ptr->id = MSG2EXT( IO_CAN0.IF2ARB.word);   /* Arbitration */
			//dbg_printf("can_rx_ptr id = %08x",can_rx_ptr->id);
		}
		else
		{
			can_rx_ptr->id = MSG2STD(IO_CAN0.IF2ARB.word);
		}
		switch(can_rx_ptr->dlc)
		{
			case 8: can_rx_ptr->data[7] = CAN0_IF2DTB2_IF2DTB2L;//save data from buffer to RAM
            case 7: can_rx_ptr->data[6] = CAN0_IF2DTB2_IF2DTB2H;
            case 6: can_rx_ptr->data[5] = CAN0_IF2DTB1_IF2DTB1L;
            case 5: can_rx_ptr->data[4] = CAN0_IF2DTB1_IF2DTB1H;
            case 4: can_rx_ptr->data[3] = CAN0_IF2DTA2_IF2DTA2L;
            case 3: can_rx_ptr->data[2] = CAN0_IF2DTA2_IF2DTA2H;
            case 2: can_rx_ptr->data[1] = CAN0_IF2DTA1_IF2DTA1L;
            case 1: can_rx_ptr->data[0] = CAN0_IF2DTA1_IF2DTA1H;
            default:break;
		}       

	return SUCCESS;

}
U8 hal_can_n_get(U8 chn,can_msg_t *can_rx_ptr)
	{
	CANSTR *p_IO_CANn;
	if (chn>2)
	{
		return FAIL;
	}
	p_IO_CANn = (CANSTR *)sp_IO_CANn[chn];
	p_IO_CANn->IF2CMSK.byte.IF2CMSKL = 0x3F;
	p_IO_CANn->IF2CREQ.byte.IF2CREQL = can_rx_ptr->buffer_num;

	if (p_IO_CANn->IF2MCTR.bit.MsgLst)    /* check whether or not a message was lost */
	{
		p_IO_CANn->IF2MCTR.bit.MsgLst = 0;   /*  clear MSGLST Flag */
		p_IO_CANn->IF2CMSK.byte.IF2CMSKL = 0x90;
		p_IO_CANn->IF2CREQ.byte.IF2CREQL = can_rx_ptr->buffer_num;
	}
	can_rx_ptr->dlc =  (U8)p_IO_CANn->IF2MCTR.bit.DLC;       /* data length */
	can_rx_ptr->format = (p_IO_CANn->IF2ARB.bit.Xtd == 1)?EXTERN_ID:STAND_ID;
	if(can_rx_ptr->format)	//extern id
	{
		can_rx_ptr->id = MSG2EXT( p_IO_CANn->IF2ARB.word);   /* Arbitration */
		//dbg_printf("can_rx_ptr id = %08x",can_rx_ptr->id);
	}
	else
	{
		can_rx_ptr->id = MSG2STD(p_IO_CANn->IF2ARB.word);
	}
	switch(can_rx_ptr->dlc)
	{
		case 8: can_rx_ptr->data[7] = p_IO_CANn->IF2DTB2.byte.IF2DTB2L;//save data from buffer to RAM
	    case 7: can_rx_ptr->data[6] = p_IO_CANn->IF2DTB2.byte.IF2DTB2H;
	    case 6: can_rx_ptr->data[5] = p_IO_CANn->IF2DTB1.byte.IF2DTB1L;
	    case 5: can_rx_ptr->data[4] = p_IO_CANn->IF2DTB1.byte.IF2DTB1H;
	    case 4: can_rx_ptr->data[3] = p_IO_CANn->IF2DTA2.byte.IF2DTA2L;
	    case 3: can_rx_ptr->data[2] = p_IO_CANn->IF2DTA2.byte.IF2DTA2H;
	    case 2: can_rx_ptr->data[1] = p_IO_CANn->IF2DTA1.byte.IF2DTA1L;
	    case 1: can_rx_ptr->data[0] = p_IO_CANn->IF2DTA1.byte.IF2DTA1H;
	    default:break;
	}       

	return SUCCESS;

	}


U32 HalChkMob(void)
{
#ifndef WIN32
	return  (CAN0_NEWDT12);
#else

   return 0;
#endif

}
U32 HalCheckChnMob(U8 chn)
{
	CANSTR *p_IO_CANn;
	p_IO_CANn = (CANSTR *)sp_IO_CANn[chn];
#ifndef WIN32
	return p_IO_CANn->NEWDT12.word;
	//return  (CAN0_NEWDT12);
#else

   return 0;
#endif

}


U8 hal_canerror_statecheck(void)
{
	U8 u8Ret = 0;

	if(IO_CAN0.STATR.bit.BOff==0x01)		//bus off
	{
		u8Ret = CAN_BOFF;

		/*Restart bus*/
		IO_CAN0.CTRLR.bit.Init = 0;		/*enable CAN controller*/
		while((IO_CAN0.ERRCNT.bit.TEC!=0)||(IO_CAN0.ERRCNT.bit.REC!=0));
											//see if recovered
	}

	if(IO_CAN0.STATR.bit.EWarn==0x01)		//error warning
    {
		u8Ret = CAN_EWARN;
	}

	if(!((IO_CAN0.STATR.bit.BOff)|(IO_CAN0.STATR.bit.EWarn)|(IO_CAN0.STATR.bit.EPass))) //error active
    {
		u8Ret = CAN_EPASS;				//error active
	}


	return (u8Ret);
}


U8 hal_can_n_error_statecheck(U8 chn)
{
	U8 u8Ret = 0;
	CANSTR *p_IO_CANn;
	if (chn>2)
	{
		return CAN_BOFF;
	}
	p_IO_CANn = (CANSTR *)sp_IO_CANn[chn];
	if(p_IO_CANn->STATR.bit.BOff==0x01)		//bus off
	{
		u8Ret = CAN_BOFF;

		/*Restart bus*/
		p_IO_CANn->CTRLR.bit.Init = 0;		/*enable CAN controller*/
		while((p_IO_CANn->ERRCNT.bit.TEC!=0)||(p_IO_CANn->ERRCNT.bit.REC!=0));
											//see if recovered
	}

	if(p_IO_CANn->STATR.bit.EWarn==0x01)		//error warning
    {
		u8Ret = CAN_EWARN;
	}

	if(!((p_IO_CANn->STATR.bit.BOff)|(p_IO_CANn->STATR.bit.EWarn)|(p_IO_CANn->STATR.bit.EPass))) //error active
    {
		u8Ret = CAN_EPASS;				//error active
	}


	return (u8Ret);
}

t_CAN_ERR_TYPE hal_get_eer_type(void)
{
    U8 temp =0;
    temp = (U8)CAN0_STATR;
    temp &= 0x07;
    return ((t_CAN_ERR_TYPE)temp);
}


