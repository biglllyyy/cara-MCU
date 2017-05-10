#include "hal_lin.h"
#include "KeyPortReg.h"
#include "_fr.h"
#include "hal_clock.h"

unsigned char LIN_Data_Send[8],LIN_Data_Rec[8];
U8 (*ptr_app_lin_msg)(U8, U8, U8);

static int uart_changeBaudRate(unsigned char nPortNum, unsigned long nBaudRate)
{
 unsigned long l_nRLValue;

 l_nRLValue = SupplyClk/ (nBaudRate)-1; 
 
 if(l_nRLValue<1 || l_nRLValue>65535)
  return 0;

 switch (nPortNum)
 {                                                                                                                                                                                                                                                                                              
  case 0:
   MFS0_LINUART_BGR = l_nRLValue;
   break;

  case 1:
   MFS1_LINUART_BGR = l_nRLValue;
   break;
	
	case 2:
	 MFS2_LINUART_BGR = l_nRLValue;
   break;
   
  case 3:
   MFS3_LINUART_BGR = l_nRLValue;
   break;
   
  case 4:
   MFS4_LINUART_BGR = l_nRLValue;
   break; 
   
  default:
   break;
 }
 return 1;
}

void hal_lin_init(void (*ptr), unsigned char nPortNum,unsigned char LIN_mode,unsigned long nBaudRate)
{
	unsigned char dummy;
	
	ptr_app_lin_msg = (U8*)ptr;
	
	switch (nPortNum)
	{
	//case 0:
  //{
  // /*WriteKeyProtectedRegister( (unsigned int)&PORTEN, 0x01, BYTE_ACCESS );//Port Input Enable*/
  // /*DDR*/
  // dummy=DDR12;
  // dummy|=0x80;
  // dummy&=0xbf;
  // WriteKeyProtectedRegister( (unsigned int)&DDR12, dummy, BYTE_ACCESS );/*P126 input P127 output*/
  // 
  // dummy=DDR13;
  // dummy|=0x01;
  // WriteKeyProtectedRegister( (unsigned int)&DDR13, dummy, BYTE_ACCESS );/*P130 output*/
  // 
  // /*PFR*/
  // dummy=PFR12;
  // dummy|=0x80;
  // dummy&=0xbf;
  // WriteKeyProtectedRegister( (unsigned int)&PFR12, dummy, BYTE_ACCESS );/*P126 as peripheral input pin*/ 
  // 																																			/*P127 as peripheral output pin */
  // 																																			
  // dummy=PFR13;
  // dummy&=0xfe;
  // WriteKeyProtectedRegister( (unsigned int)&PFR13, dummy, BYTE_ACCESS );/*P130 as port function*/ 
  // 
  // /*EPFR*/
  // WriteKeyProtectedRegister( (unsigned int)&EPFR35, 0xE8, BYTE_ACCESS );/*SOT0_0,SIN0_0,SCK0 no input*/
  // /*PDR*/
  // PDR13_P0 = 0;/*P130 output low ,disable tja1020*/
  // 
  // MFS0_LINUART_SCR_UPCL    = 1; /* Reset.*/
  // MFS0_LINUART_SMR_MD    = 3; /* Lin mode*/
  // MFS0_LINUART_SCR_MS    = LIN_mode; /* Master/Slave*/
  // MFS0_LINUART_SMR_SOE   = 1; /* Enable SOUT*/
  // MFS0_LINUART_ESCR_LBL   = 0; /* 13 bit time*/
  // MFS0_LINUART_ESCR_DEL   = 0; /* 1 bit*/
  // MFS0_LINUART_LAMCR_LCSTYP = 1; /* LIN checksum type setting£¨LAMCR:LCSTYP£©*/
  // MFS0_LINUART_SACSR_AUTE  = 0; /* Automatic baud rate adjustment disable£¨SACSR:AUTE=0£©*/
  // MFS0_LINUART_LAMCR_LAMEN = 1; /* LIN assist mode processing enable£¨LAMCR:LAMEN=1£©*/
  // MFS0_LINUART_LAMCR_LIDEN = 1; /* LIN ID register use enable£¨LAMCR:LIDEN=1£©*/
  // 
  // MFS0_LINUART_FCR1_FSEL  = 0; /* Transmission FIFO=FIFO1, Reception FIFO=FIFO2 setting£¨FCR1:FSEL=0£©*/
  // MFS0_LINUART_FBYTE1     = 0; /* valid data length setting £¨FBYTE2=8¡¢FBYTE1=0£©*/
  // MFS0_LINUART_FBYTE2    = 8;
  // MFS0_LINUART_FCR0_FE1   = 0; /* Transmission FIFO operation disable setting£¨FCR0:FE1=0£©*/
  // MFS0_LINUART_SCR_TXE    = 0; /* Transmission setting £¨SCR:TXE=0¡¢SCR:TIE=0£©*/
  // MFS0_LINUART_SCR_TIE    = 0;  
  // MFS0_LINUART_FCR0_FE2   = 0; /* reception FIFO operation disable setting£¨FCR0:FE2=0£©*/  
  // MFS0_LINUART_SCR_RXE    = 0; /* reception setting £¨SCR:RXE=0¡¢SCR:RIE=0£© */             
  // MFS0_LINUART_SCR_RIE    = 0;
  // 
  // MFS0_LINUART_SCR_TBIE    = 0; /* transmission bus idle interrupt disable£¨SCR:TBIE=0£©*/
  // MFS0_LINUART_FCR1_FTIE   = 0; /*Transmission FIFO interrupt disable£¨FCR1:FTIE=0£©*/
  // MFS0_LINUART_FCR1_FRIIE   = 0; /* reception FIFO idle state detect disable£¨FCR1:FRIIE=0£©*/
  // 
  // MFS0_LINUART_ESCR_LBIE   = 0; /* LIN Break interrupt disable£¨ESCR:LBIE=0£©*/
  // MFS0_LINUART_SACSR_SFDE  = 0; /* Sync Field interrupt disable£¨SACSR:SFDE=0£©*/
  // MFS0_LINUART_LAMIER_LCSCIE= 0; /* LIN checksum operation complete interrupt disable£¨LAMIER:LCSCIE=0£©*/
  // MFS0_LINUART_LAMIER_LAHCIE= 0; /* LIN automatic header complete interrupt disable(LAMIER:LAHCIE=0)*/
  // MFS0_LINUART_LAMIER_LBSERIE=0;
  // MFS0_LINUART_LAMIER_LSFERIE=0;
  // MFS0_LINUART_LAMIER_LPTERIE=0;
  // MFS0_LINUART_LAMIER_LCSERIE=0;
  // 
  // /*stop bit*/
  // MFS0_LINUART_SMR_SBL  = 0;
  // MFS0_LINUART_ESCR_ESBL= 0;
  // 
  // /*interrupt level*/
  // ICR4 = 0x10;
  // ICR5 = 0x10;
  // 
  // break;
  //}
  //case 1:
  //{
  // WriteKeyProtectedRegister( (unsigned int)&PORTEN, 0x01, BYTE_ACCESS );/*Port Input Enable*/
  // /*DDR*/
  // dummy=DDR00;
  // dummy&=0xfe;
  // dummy|=0x06;
  // WriteKeyProtectedRegister( (unsigned int)&DDR00, dummy, BYTE_ACCESS );/*P000 input P001 output P002 output*/
  // 
  // /*PFR*/
  // dummy=PFR00;
  // dummy|=0x02;
  // dummy&=0xfa;
  // WriteKeyProtectedRegister( (unsigned int)&PFR00, dummy, BYTE_ACCESS );/*P000 as peripheral input pin */
  //                                    /*P001 as peripheral output pin*/ 
  //                                    /*P002 as port function*/ 
  // /*EPFR*/
  // WriteKeyProtectedRegister( (unsigned int)&EPFR36, 0xf9, BYTE_ACCESS );/*SOT1_0,SIN1_0,SCK1 no input*/
  // 
  // /*PDR*/
  // PDR00_P2 = 0;/*P002 output low,tja1020 disabled*/
  // 
  // MFS1_LINUART_SCR_UPCL    = 1; /* Reset.*/
  // MFS1_LINUART_SMR_MD    = 3; /* Lin mode*/
  // MFS1_LINUART_SCR_MS    = LIN_mode; /* Master/Slave*/
  // MFS1_LINUART_SMR_SOE   = 1; /* Enable SOUT*/
  // MFS1_LINUART_ESCR_LBL   = 0; /* 13 bit time*/
  // MFS1_LINUART_ESCR_DEL   = 0; /* 1 bit*/
  // MFS1_LINUART_LAMCR_LCSTYP = 1; /* LIN checksum type setting£¨LAMCR:LCSTYP£©*/
  // MFS1_LINUART_SACSR_AUTE  = 0; /* Automatic baud rate adjustment disable£¨SACSR:AUTE=0£©*/
  // MFS1_LINUART_LAMCR_LAMEN = 1; /*LIN assist mode processing enable£¨LAMCR:LAMEN=1£©*/
  // MFS1_LINUART_LAMCR_LIDEN = 1; /* LIN ID register use enable£¨LAMCR:LIDEN=1£©*/
  // 
  // MFS1_LINUART_FCR1_FSEL  = 0; /* Transmission FIFO=FIFO1, Reception FIFO=FIFO2 setting£¨FCR1:FSEL=0£©*/
  // MFS1_LINUART_FBYTE1     = 0; /* valid data length setting £¨FBYTE2=8¡¢FBYTE1=0£©*/
  // MFS1_LINUART_FBYTE2    = 8;
  // MFS1_LINUART_FCR0_FE1   = 0; /* Transmission FIFO operation disable setting£¨FCR0:FE1=0£©*/
  // MFS1_LINUART_SCR_TXE    = 0; /* Transmission setting £¨SCR:TXE=0¡¢SCR:TIE=0£©*/
  // MFS1_LINUART_SCR_TIE    = 0;  
  // MFS1_LINUART_FCR0_FE2   = 0; /* reception FIFO operation disable setting£¨FCR0:FE2=0£©*/  
  // MFS1_LINUART_SCR_RXE    = 0; /* reception setting £¨SCR:RXE=0¡¢SCR:RIE=0£©*/              
  // MFS1_LINUART_SCR_RIE    = 0;
  // 
  // MFS1_LINUART_SCR_TBIE    = 0; /* transmission bus idle interrupt disable£¨SCR:TBIE=0£©*/
  // MFS1_LINUART_FCR1_FTIE   = 0; /* Transmission FIFO interrupt disable£¨FCR1:FTIE=0£©*/
  // MFS1_LINUART_FCR1_FRIIE   = 0; /* reception FIFO idle state detect disable£¨FCR1:FRIIE=0£©*/
  // 
  // MFS1_LINUART_ESCR_LBIE   = 0; /* LIN Break interrupt disable£¨ESCR:LBIE=0£©*/
  // MFS1_LINUART_SACSR_SFDE  = 0; /* Sync Field interrupt disable£¨SACSR:SFDE=0£©*/
  // MFS1_LINUART_LAMIER_LCSCIE= 0; /* LIN checksum operation complete interrupt disable£¨LAMIER:LCSCIE=0£©*/
  // MFS1_LINUART_LAMIER_LAHCIE= 0; /* LIN automatic header complete interrupt disable(LAMIER:LAHCIE=0)*/
  // MFS1_LINUART_LAMIER_LBSERIE=0;
  // MFS1_LINUART_LAMIER_LSFERIE=0;
  // MFS1_LINUART_LAMIER_LPTERIE=0;
  // MFS1_LINUART_LAMIER_LCSERIE=0;
  // 
  // /*stop bit*/
  // MFS1_LINUART_SMR_SBL  = 0;
  // MFS1_LINUART_ESCR_ESBL= 0;
  // 
  // /*interrupt level*/
  // ICR6 = 0x10;
  // ICR7 = 0x10;
  // 
  // break;
  //}
  case 4:
  {
   dummy=(unsigned char)ADERL1;
   dummy&=0xf1;
   WriteKeyProtectedRegister( (unsigned int)&ADERL1+1, dummy, BYTE_ACCESS);/*AD input disable*/
   
   WriteKeyProtectedRegister( (unsigned int)&PORTEN, 0x01, BYTE_ACCESS );/*Port Input Enable*/
   
   /*DDR*/
   dummy=DDR07;
   dummy&=0xfb;
   dummy|=0x0b;
   WriteKeyProtectedRegister( (unsigned int)&DDR07, dummy, BYTE_ACCESS );/*P072 input,P070 P071 P073 output*/
   
   /*PFR*/
   dummy=PFR07;
   dummy&=0xf8;
   dummy|=0x08;
   WriteKeyProtectedRegister( (unsigned int)&PFR07, dummy, BYTE_ACCESS );/*P070 as port function*/ 
                                      																	/*P071 as port function*/ 
   																																			/*P072 as peripheral input pin*/ 
                                      																	/*P073 as peripheral output pin */
   /*EPFR*/
   WriteKeyProtectedRegister( (unsigned int)&EPFR08, 0x08, BYTE_ACCESS );/*SOT4_0,SIN4_0,SCK4 no input/output*/
   /*PDR*/
   PDR07_P0 = 0;/*P070 output low ,disable tja1020*/
   PDR07_P1 = 1;/*P071 output high */
   
   MFS4_LINUART_SCR_UPCL    = 1; /* Reset.*/
   MFS4_LINUART_SMR_MD    = 3; /* Lin mode*/
   MFS4_LINUART_SCR_MS    = LIN_mode; /* Master/Slave*/
   MFS4_LINUART_SMR_SOE   = 1; /* Enable SOUT*/
   MFS4_LINUART_ESCR_LBL   = 0; /* 13 bit time*/
   MFS4_LINUART_ESCR_DEL   = 0; /* 1 bit*/
   MFS4_LINUART_LAMCR_LCSTYP = 1; /* LIN checksum type setting£¨LAMCR:LCSTYP£©*/
   MFS4_LINUART_SACSR_AUTE  = 0; /* Automatic baud rate adjustment disable£¨SACSR:AUTE=0£©*/
   MFS4_LINUART_LAMCR_LAMEN = 1; /* LIN assist mode processing enable£¨LAMCR:LAMEN=1£©*/
   MFS4_LINUART_LAMCR_LIDEN = 1; /* LIN ID register use enable£¨LAMCR:LIDEN=1£©*/
   
   MFS4_LINUART_FCR1_FSEL  = 0; /* Transmission FIFO=FIFO1, Reception FIFO=FIFO2 setting£¨FCR1:FSEL=0£©*/
   MFS4_LINUART_FBYTE1     = 0; /* valid data length setting £¨FBYTE2=8¡¢FBYTE1=0£©*/
   MFS4_LINUART_FBYTE2    = 8;
   MFS4_LINUART_FCR0_FE1   = 0; /* Transmission FIFO operation disable setting£¨FCR0:FE1=0£©*/
   MFS4_LINUART_SCR_TXE    = 0; /* Transmission setting £¨SCR:TXE=0¡¢SCR:TIE=0£©*/
   MFS4_LINUART_SCR_TIE    = 0;  
   MFS4_LINUART_FCR0_FE2   = 0; /* reception FIFO operation disable setting£¨FCR0:FE2=0£©*/  
   MFS4_LINUART_SCR_RXE    = 0; /* reception setting £¨SCR:RXE=0¡¢SCR:RIE=0£© */             
   MFS4_LINUART_SCR_RIE    = 0;
   
   MFS4_LINUART_SCR_TBIE    = 0; /* transmission bus idle interrupt disable£¨SCR:TBIE=0£©*/
   MFS4_LINUART_FCR1_FTIE   = 0; /*Transmission FIFO interrupt disable£¨FCR1:FTIE=0£©*/
   MFS4_LINUART_FCR1_FRIIE   = 0; /* reception FIFO idle state detect disable£¨FCR1:FRIIE=0£©*/
   
   MFS4_LINUART_ESCR_LBIE   = 0; /* LIN Break interrupt disable£¨ESCR:LBIE=0£©*/
   MFS4_LINUART_SACSR_SFDE  = 0; /* Sync Field interrupt disable£¨SACSR:SFDE=0£©*/
   MFS4_LINUART_LAMIER_LCSCIE= 0; /* LIN checksum operation complete interrupt disable£¨LAMIER:LCSCIE=0£©*/
   MFS4_LINUART_LAMIER_LAHCIE= 0; /* LIN automatic header complete interrupt disable(LAMIER:LAHCIE=0)*/
   MFS4_LINUART_LAMIER_LBSERIE=0;
   MFS4_LINUART_LAMIER_LSFERIE=0;
   MFS4_LINUART_LAMIER_LPTERIE=0;
   MFS4_LINUART_LAMIER_LCSERIE=0;
   
   /*stop bit*/
   MFS4_LINUART_SMR_SBL  = 0;
   MFS4_LINUART_ESCR_ESBL= 0;
   
   /*interrupt level*/
   ICR12 = 0x10;
   ICR13 = 0x10;
   
   break;
  }
  default:
   
   break;
 }
 
 /*set baudrate*/
 uart_changeBaudRate(nPortNum, nBaudRate);
}

void hal_lin_start(unsigned char nPortNum,unsigned char nLINID)
{
 volatile unsigned char dummy;
 
 switch(nPortNum)
 {
  //case 0:/*config MSFO*/
  // PDR13_P0 = 1;/*P130 output high ,enable tja1020*/
  // 
  // /*clear error flag*/
  // MFS0_LINUART_SSR_REC   = 1;/*clear uart error*/
  // MFS0_LINUART_LAMESR    = 0;/*clear lin error*/
  // 
  // MFS0_LINUART_LAMSR_LAHC = 0;/**/
  // MFS0_LINUART_LAMSR_LCSC = 0;
  // 
  // MFS0_LINUART_LAMCR_LTDRCL=1;/* Clear TDR*/
  // dummy = MFS0_LINUART_RDR0; /*clear RDR*/
  // 
  // MFS0_LINUART_SCR_TBIE  = 0;
  // MFS0_LINUART_SCR_TXE   = 0; /* Transmission operation disable£¨SCR:TXE=0£©*/
  // MFS0_LINUART_SCR_TIE   = 0; /* transmission interrupt disable£¨SCR:TIE=0£©*/
  // MFS0_LINUART_SCR_RXE   = 0; /* Reception operation disable£¨SCR:RXE=0£©*/
  // MFS0_LINUART_SCR_RIE   = 0; /* Reception interrupt disable£¨SCR:RIE=0£©*/
  // MFS0_LINUART_FCR0_FE1   = 0; /* Transmission FIFO=FIFO1 operation disable£¨FCR0:FE1=0£©*/
  // MFS0_LINUART_FCR0_FE2   = 0; /* Reception FIFO=FIFO2 operation disable (FCR0:FE2=0£©*/
  // MFS0_LINUART_FCR0_FCL1  = 1; /* FIFO1 reset*/
  // MFS0_LINUART_FCR0_FCL2  = 1; /* FIFO2 reset*/   
  // 
  // MFS0_LINUART_LAMIER_LCSCIE= 1;/* LIN checksum operation complete interrupt enable£¨LAMIER:LCSCIE=1£©*/
  // MFS0_LINUART_LAMIER_LAHCIE= 1;/* LIN automatic header complete interrupt enable(LAMIER:LAHCIE=1)*/
  // MFS0_LINUART_LAMIER_LBSERIE=1;
  // MFS0_LINUART_LAMIER_LSFERIE=1;
  // MFS0_LINUART_LAMIER_LPTERIE=1;
  // MFS0_LINUART_LAMIER_LCSERIE=1;
  // 
  // switch(MFS0_LINUART_SCR_MS)
  // {
  //  case 0:/*Master mode*/
  //   MFS0_LINUART_LAMID   = nLINID;/* LIN ID setting£¨LAMTID£©*/
  //
  //   MFS0_LINUART_SCR_LBR = 1;/* LIN Break Field transmission start£¨SCR:LBR=1£©*/
  //   
  //   break;
  //  case 1:/*Slave mode*/
  //   
  //   break; 
  // }
  // break;
  //case 1:/*config MFS1*/
  // PDR00_P2 = 1;/*P002 output high,tja1020 enable  */
  // 
  // /*clear error flag*/
  // MFS1_LINUART_SSR_REC   = 1;/*clear uart error*/
  // MFS1_LINUART_LAMESR    = 0;/*clear lin error*/
  // 
  // MFS1_LINUART_LAMSR_LAHC = 0;/**/
  // MFS1_LINUART_LAMSR_LCSC = 0;
  // 
  // MFS1_LINUART_LAMCR_LTDRCL=1;/* Clear TDR*/
  // dummy = MFS1_LINUART_RDR0; /*clear RDR*/
  // 
  // MFS1_LINUART_SCR_TBIE  = 0;
  // MFS1_LINUART_SCR_TXE   = 0; /* Transmission operation disable£¨SCR:TXE=0£©*/
  // MFS1_LINUART_SCR_TIE   = 0; /* transmission interrupt disable£¨SCR:TIE=0£©*/
  // MFS1_LINUART_SCR_RXE   = 0; /* Reception operation disable£¨SCR:RXE=0£©*/
  // MFS1_LINUART_SCR_RIE   = 0; /* Reception interrupt disable£¨SCR:RIE=0£©*/
  // MFS1_LINUART_FCR0_FE1   = 0; /* Transmission FIFO=FIFO1 operation disable£¨FCR0:FE1=0£©*/
  // MFS1_LINUART_FCR0_FE2   = 0; /* Reception FIFO=FIFO2 operation disable (FCR0:FE2=0£©*/
  // MFS1_LINUART_FCR0_FCL1  = 1; /* FIFO1 reset*/
  // MFS1_LINUART_FCR0_FCL2  = 1; /* FIFO2 reset*/
  // 
  // MFS1_LINUART_LAMIER_LCSCIE= 1;/* LIN checksum operation complete interrupt enable£¨LAMIER:LCSCIE=1£©*/
  // MFS1_LINUART_LAMIER_LAHCIE= 1;/* LIN automatic header complete interrupt enable(LAMIER:LAHCIE=1)*/
  // MFS1_LINUART_LAMIER_LBSERIE=1;
  // MFS1_LINUART_LAMIER_LSFERIE=1;
  // MFS1_LINUART_LAMIER_LPTERIE=1;
  // MFS1_LINUART_LAMIER_LCSERIE=1;
  // 
  // switch(MFS1_LINUART_SCR_MS)
  // {
  //  case 0:/*Master mode*/
  //   MFS1_LINUART_LAMID   = nLINID;/* LIN ID setting£¨LAMTID£©*/
  //   
  //   MFS1_LINUART_SCR_LBR = 1;/* LIN Break Field transmission start£¨SCR:LBR=1£©*/
  //   
  //   break;
  //  case 1:/*Slave mode*/
  //   
  //   break; 
  // }
  // break;
  case 4:/*config MFS4*/
   PDR07_P0 = 1;/*P070 output high,tja1020 enable  */
   
   /*clear error flag*/
   MFS4_LINUART_SSR_REC   = 1;/*clear uart error*/
   MFS4_LINUART_LAMESR    = 0;/*clear lin error*/
   
   MFS4_LINUART_LAMSR_LAHC = 0;/**/
   MFS4_LINUART_LAMSR_LCSC = 0;
   
   MFS4_LINUART_LAMCR_LTDRCL=1;/* Clear TDR*/
   dummy = MFS4_LINUART_RDR0; /*clear RDR*/
   
   MFS4_LINUART_SCR_TBIE  = 0;
   MFS4_LINUART_SCR_TXE   = 0; /* Transmission operation disable£¨SCR:TXE=0£©*/
   MFS4_LINUART_SCR_TIE   = 0; /* transmission interrupt disable£¨SCR:TIE=0£©*/
   MFS4_LINUART_SCR_RXE   = 0; /* Reception operation disable£¨SCR:RXE=0£©*/
   MFS4_LINUART_SCR_RIE   = 0; /* Reception interrupt disable£¨SCR:RIE=0£©*/
   MFS4_LINUART_FCR0_FE1   = 0; /* Transmission FIFO=FIFO1 operation disable£¨FCR0:FE1=0£©*/
   MFS4_LINUART_FCR0_FE2   = 0; /* Reception FIFO=FIFO2 operation disable (FCR0:FE2=0£©*/
   MFS4_LINUART_FCR0_FCL1  = 1; /* FIFO1 reset*/
   MFS4_LINUART_FCR0_FCL2  = 1; /* FIFO2 reset*/
   
   MFS4_LINUART_LAMIER_LCSCIE= 1;/* LIN checksum operation complete interrupt enable£¨LAMIER:LCSCIE=1£©*/
   MFS4_LINUART_LAMIER_LAHCIE= 1;/* LIN automatic header complete interrupt enable(LAMIER:LAHCIE=1)*/
   MFS4_LINUART_LAMIER_LBSERIE=1;
   MFS4_LINUART_LAMIER_LSFERIE=1;
   MFS4_LINUART_LAMIER_LPTERIE=1;
   MFS4_LINUART_LAMIER_LCSERIE=1;
   
   switch(MFS4_LINUART_SCR_MS)
   {
    case 0:/*Master mode*/
     MFS4_LINUART_LAMID   = nLINID;/* LIN ID setting£¨LAMTID£©*/
     
     MFS4_LINUART_SCR_LBR = 1;/* LIN Break Field transmission start£¨SCR:LBR=1£©*/
     
     break;
    case 1:/*Slave mode*/
     
     break; 
   }
   break;
   
  default:
   
   break;
 }
}

void hal_lin_stop(unsigned char nPortNum)
{
 switch(nPortNum)
 {
 //case 0:
 // PDR13_P0 = 0;/*P130 output low ,disable tja1020*/
 // 
 // MFS0_LINUART_LAMIER_LCSCIE= 0;/* LIN checksum operation complete interrupt disable£¨LAMIER:LCSCIE=0£©*/
 // MFS0_LINUART_LAMIER_LAHCIE= 0;/* LIN automatic header complete interrupt disable(LAMIER:LAHCIE=0)*/
 // break;
 //case 1:
 // PDR00_P2 = 0;/*P002 output low,tja1020 disable*/
 // 
 // MFS1_LINUART_LAMIER_LCSCIE= 0;/* LIN checksum operation complete interrupt disable£¨LAMIER:LCSCIE=0£©*/
 // MFS1_LINUART_LAMIER_LAHCIE= 0;/* LIN automatic header complete interrupt disable(LAMIER:LAHCIE=0)*/
 // break;
 	case 4:
   PDR07_P0 = 0;/*P070 output low,tja1020 disable*/
   
   MFS4_LINUART_LAMIER_LCSCIE= 0;/* LIN checksum operation complete interrupt disable£¨LAMIER:LCSCIE=0£©*/
   MFS4_LINUART_LAMIER_LAHCIE= 0;/* LIN automatic header complete interrupt disable(LAMIER:LAHCIE=0)*/
   break;
 	
  default:
   
   break;
 }
}

__interrupt void ISR_LIN0_Reception(void)
{
 unsigned char i,response_type;
 volatile unsigned char dummy;
 
 if((MFS0_LINUART_SSR&0x18)||(MFS0_LINUART_LAMESR&0x78))/*error occur*/
 {
  MFS0_LINUART_SSR_REC   = 1; /*clear uart error*/
  MFS0_LINUART_LAMESR    = 0; /*clear lin error*/
  
  MFS0_LINUART_LAMSR_LAHC = 0; /**/
  MFS0_LINUART_LAMSR_LCSC = 0;
  
  MFS0_LINUART_LAMCR_LTDRCL=1; /* Clear TDR*/
  dummy = MFS0_LINUART_RDR0;  /*clear RDR*/
  
  MFS0_LINUART_SCR_TBIE  = 0;
  MFS0_LINUART_SCR_TXE   = 0; /* Transmission operation disable£¨SCR:TXE=0£©*/
  MFS0_LINUART_SCR_TIE   = 0; /* transmission interrupt disable£¨SCR:TIE=0£©*/
  MFS0_LINUART_SCR_RXE   = 0; /* Reception operation disable£¨SCR:RXE=0£©*/
  MFS0_LINUART_SCR_RIE   = 0; /* Reception interrupt disable£¨SCR:RIE=0£©*/
  MFS0_LINUART_FCR0_FE1   = 0; /* Transmission FIFO=FIFO1 operation disable£¨FCR0:FE1=0£©*/
  MFS0_LINUART_FCR0_FE2   = 0; /* Reception FIFO=FIFO2 operation disable (FCR0:FE2=0£©*/
  MFS0_LINUART_FCR0_FCL1  = 1; /* FIFO1 reset*/
  MFS0_LINUART_FCR0_FCL2  = 1; /* FIFO2 reset*/
  
 }
 else if(MFS0_LINUART_LAMSR_LAHC)  /*Automatic header completion*/
 {
 	if (ptr_app_lin_msg)
		response_type=ptr_app_lin_msg(0,MFS0_LINUART_LAMID,HEADER);
  
	switch(response_type)
	{
		case TRANSMISSION:
			/*init FIFO*/
			MFS0_LINUART_SCR_TXE   = 0; /* Transmission operation disable£¨SCR:TXE=0£©*/
    MFS0_LINUART_SCR_RXE   = 0; /* Reception operation disable£¨SCR:RXE=0£©*/
    MFS0_LINUART_FCR0_FCL1  = 1; /* FIFO1 reset*/
    MFS0_LINUART_FCR0_FCL2  = 1; /* FIFO2 reset*/
    /*enable Transmission FIFO*/
    MFS0_LINUART_FCR0_FE1 = 1; /* Transmission FIFO operation enable setting£¨FCR0:FE1=1£©*/
    MFS0_LINUART_FCR0_FE2 = 0; /* reception FIFO operation disable setting£¨FCR0:FE2=0£©*/
    
    /*Transmission FIFO data M byte setting£¨TDR£¨FIFO1£©£©*/
    for(i = 0; i < MFS0_LINUART_LAMCR_LDL; i ++)
    {
     MFS0_LINUART_TDR0 = LIN_Data_Send[i];
    }
    
    MFS0_LINUART_SCR_TXE  = 1; /* Transmission setting £¨SCR:TXE=1¡¢SCR:TIE=0¡¢SCR:RXE=0¡¢SCR:RIE=0¡¢SCR:TBIE=0£©*/
    
    break;
   case RECEPTION:
    /*init FIFO*/
    MFS0_LINUART_SCR_TXE   = 0; /* Transmission operation disable£¨SCR:TXE=0£©*/
    MFS0_LINUART_SCR_RXE   = 0; /* Reception operation disable£¨SCR:RXE=0£©*/
    MFS0_LINUART_FCR0_FCL1  = 1; /* FIFO1 reset*/
    MFS0_LINUART_FCR0_FCL2  = 1; /* FIFO2 reset*/
    /*enable reception FIFO*/
    MFS0_LINUART_FCR0_FE1  = 0; /* Transmission FIFO operation disable setting£¨FCR0:FE1=0£©*/
    MFS0_LINUART_FCR0_FE2  = 1; /* reception FIFO operation enable setting£¨FCR0:FE2=1£©*/

    /**/
    MFS0_LINUART_SCR_RXE   = 1; /*Reception operation enable£¨SCR:RXE=1£©*/
    break;
   default:
    
    break;
  }
  
  MFS0_LINUART_LAMSR_LAHC = 0;/*clear flag*/
 }
 else if(MFS0_LINUART_LAMSR_LCSC) /* Checksum has been received*/
 {
 	 	if (ptr_app_lin_msg)
  response_type=ptr_app_lin_msg(0,MFS0_LINUART_LAMID,RESPONSE);
  
  switch(response_type)
  {
   case TRANSMISSION:
    MFS0_LINUART_FCR0_FE1 = 0; /* Transmission FIFO operation disable setting£¨FCR0:FE1=0£©*/
    MFS0_LINUART_SCR_TXE  = 0; /* Transmission setting £¨SCR:TXE=0¡¢SCR:TIE=0£©*/
    MFS0_LINUART_SCR_TIE  = 0;  
    MFS0_LINUART_SCR_TBIE = 0;
    
    break;
   case RECEPTION:
    MFS0_LINUART_FCR0_FE2 = 0; /* reception FIFO operation disable setting£¨FCR0:FE2=0£©*/
    MFS0_LINUART_SCR_RXE  = 0;
    MFS0_LINUART_SCR_RIE  = 0;
    
    break;
   default:
    
    break;
  }
  
  MFS0_LINUART_LAMSR_LCSC = 0;/*clear flag*/
 }                                                         
 /**/
 else if(MFS0_LINUART_SSR_RDRF)
 {
  dummy = MFS0_LINUART_RDR0;
 }
 else
 {}
}
__interrupt void ISR_LIN0_Transmission(void)
{}
__interrupt void ISR_LIN1_Reception(void)
{
 unsigned char i,response_type;
 volatile unsigned char dummy;
 
 if((MFS1_LINUART_SSR&0x18)||(MFS1_LINUART_LAMESR&0x78))/*error occur*/
 {
  MFS1_LINUART_SSR_REC   = 1; /*clear uart error*/
  MFS1_LINUART_LAMESR    = 0; /*clear lin error*/
  
  MFS1_LINUART_LAMSR_LAHC = 0; /**/
  MFS1_LINUART_LAMSR_LCSC = 0;
  
  MFS1_LINUART_LAMCR_LTDRCL=1; /* Clear TDR*/
  dummy = MFS1_LINUART_RDR0;  /*clear RDR*/
  
  MFS1_LINUART_SCR_TBIE  = 0;
  MFS1_LINUART_SCR_TXE   = 0; /* Transmission operation disable£¨SCR:TXE=0£©*/
  MFS1_LINUART_SCR_TIE   = 0; /* transmission interrupt disable£¨SCR:TIE=0£©*/
  MFS1_LINUART_SCR_RXE   = 0; /* Reception operation disable£¨SCR:RXE=0£©*/
  MFS1_LINUART_SCR_RIE   = 0; /* Reception interrupt disable£¨SCR:RIE=0£©*/
  MFS1_LINUART_FCR0_FE1   = 0; /* Transmission FIFO=FIFO1 operation disable£¨FCR0:FE1=0£©*/
  MFS1_LINUART_FCR0_FE2   = 0; /* Reception FIFO=FIFO2 operation disable (FCR0:FE2=0£©*/
  MFS1_LINUART_FCR0_FCL1  = 1; /* FIFO1 reset*/
  MFS1_LINUART_FCR0_FCL2  = 1; /* FIFO2 reset*/
  
 }
 else if(MFS1_LINUART_LAMSR_LAHC)  /*Automatic header completion*/
 {
 	if (ptr_app_lin_msg) 	
  response_type=ptr_app_lin_msg(1,MFS1_LINUART_LAMID,HEADER);
  
  switch(response_type)
  {
   case TRANSMISSION:
    /*init FIFO*/
    MFS1_LINUART_SCR_TXE   = 0; /* Transmission operation disable£¨SCR:TXE=0£©*/
    MFS1_LINUART_SCR_RXE   = 0; /* Reception operation disable£¨SCR:RXE=0£©*/
    MFS1_LINUART_FCR0_FCL1  = 1; /* FIFO1 reset*/
    MFS1_LINUART_FCR0_FCL2  = 1; /* FIFO2 reset*/
    /*enable Transmission FIFO*/
    MFS1_LINUART_FCR0_FE1 = 1; /* Transmission FIFO operation enable setting£¨FCR0:FE1=1£©*/
    MFS1_LINUART_FCR0_FE2 = 0; /* reception FIFO operation disable setting£¨FCR0:FE2=0£©*/
    
    /*Transmission FIFO data M byte setting£¨TDR£¨FIFO1£©£©*/
    for(i = 0; i < MFS1_LINUART_LAMCR_LDL; i ++)
    {
     MFS1_LINUART_TDR0 = LIN_Data_Send[i];
    }
    
    MFS1_LINUART_SCR_TXE  = 1; /* Transmission setting £¨SCR:TXE=1¡¢SCR:TIE=0¡¢SCR:RXE=0¡¢SCR:RIE=0¡¢SCR:TBIE=0£©*/
    break;
   case RECEPTION:
    /*init FIFO*/
    MFS1_LINUART_SCR_TXE   = 0; /* Transmission operation disable£¨SCR:TXE=0£©*/
    MFS1_LINUART_SCR_RXE   = 0; /* Reception operation disable£¨SCR:RXE=0£©*/
    MFS1_LINUART_FCR0_FCL1  = 1; /* FIFO1 reset*/
    MFS1_LINUART_FCR0_FCL2  = 1; /* FIFO2 reset*/
    /*enable reception FIFO*/
    MFS1_LINUART_FCR0_FE1  = 0; /* Transmission FIFO operation disable setting£¨FCR0:FE1=0£©*/
    MFS1_LINUART_FCR0_FE2  = 1; /* reception FIFO operation enable setting£¨FCR0:FE2=1£©*/

    /**/
    MFS1_LINUART_SCR_RXE   = 1; /*Reception operation enable£¨SCR:RXE=1£©*/
    break;
   default:
    
    break;
  }
  
  MFS1_LINUART_LAMSR_LAHC = 0;/*clear flag*/
 }
 else if(MFS1_LINUART_LAMSR_LCSC) /* Checksum has been received*/
 {
 	if (ptr_app_lin_msg)
  response_type=response_type=ptr_app_lin_msg(1,MFS1_LINUART_LAMID,RESPONSE);
  
  switch(response_type)
  {
   case TRANSMISSION:
    MFS1_LINUART_FCR0_FE1 = 0; /* Transmission FIFO operation disable setting£¨FCR0:FE1=0£©*/
    MFS1_LINUART_SCR_TXE  = 0; /* Transmission setting £¨SCR:TXE=0¡¢SCR:TIE=0£©*/
    MFS1_LINUART_SCR_TIE  = 0;  
    MFS1_LINUART_SCR_TBIE = 0;
    
    break;
   case RECEPTION:
    MFS1_LINUART_FCR0_FE2 = 0; /* reception FIFO operation disable setting£¨FCR0:FE2=0£©*/
    MFS1_LINUART_SCR_RXE  = 0;
    MFS1_LINUART_SCR_RIE  = 0;
    
    break;
   default:
    
    break;
  }
  
  MFS1_LINUART_LAMSR_LCSC = 0;/*clear flag*/
 }                                                         
 /**/
 else if(MFS1_LINUART_SSR_RDRF)
 {
  dummy = MFS1_LINUART_RDR0;
 }
 else
 {}
}
__interrupt void ISR_LIN1_Transmission(void)
{}
__interrupt void ISR_LIN4_Reception(void)
{
 unsigned char i,response_type;
 volatile unsigned char dummy;
 
 if((MFS4_LINUART_SSR&0x18)||(MFS4_LINUART_LAMESR&0x78))/*error occur*/
 {
  MFS4_LINUART_SSR_REC   = 1; /*clear uart error*/
  MFS4_LINUART_LAMESR    = 0; /*clear lin error*/
  
  MFS4_LINUART_LAMSR_LAHC = 0; /**/
  MFS4_LINUART_LAMSR_LCSC = 0;
  
  MFS4_LINUART_LAMCR_LTDRCL=1; /* Clear TDR*/
  dummy = MFS4_LINUART_RDR0;  /*clear RDR*/
  
  MFS4_LINUART_SCR_TBIE  = 0;
  MFS4_LINUART_SCR_TXE   = 0; /* Transmission operation disable£¨SCR:TXE=0£©*/
  MFS4_LINUART_SCR_TIE   = 0; /* transmission interrupt disable£¨SCR:TIE=0£©*/
  MFS4_LINUART_SCR_RXE   = 0; /* Reception operation disable£¨SCR:RXE=0£©*/
  MFS4_LINUART_SCR_RIE   = 0; /* Reception interrupt disable£¨SCR:RIE=0£©*/
  MFS4_LINUART_FCR0_FE1   = 0; /* Transmission FIFO=FIFO1 operation disable£¨FCR0:FE1=0£©*/
  MFS4_LINUART_FCR0_FE2   = 0; /* Reception FIFO=FIFO2 operation disable (FCR0:FE2=0£©*/
  MFS4_LINUART_FCR0_FCL1  = 1; /* FIFO1 reset*/
  MFS4_LINUART_FCR0_FCL2  = 1; /* FIFO2 reset*/
  
 }
 else if(MFS4_LINUART_LAMSR_LAHC)  /*Automatic header completion*/
 {
 	if (ptr_app_lin_msg)
  response_type=ptr_app_lin_msg(4,MFS4_LINUART_LAMID,HEADER);
  
  switch(response_type)
  {
   case TRANSMISSION:
    /*init FIFO*/
    MFS4_LINUART_SCR_TXE   = 0; /* Transmission operation disable£¨SCR:TXE=0£©*/
    MFS4_LINUART_SCR_RXE   = 0; /* Reception operation disable£¨SCR:RXE=0£©*/
    MFS4_LINUART_FCR0_FCL1  = 1; /* FIFO1 reset*/
    MFS4_LINUART_FCR0_FCL2  = 1; /* FIFO2 reset*/
    /*enable Transmission FIFO*/
    MFS4_LINUART_FCR0_FE1 = 1; /* Transmission FIFO operation enable setting£¨FCR0:FE1=1£©*/
    MFS4_LINUART_FCR0_FE2 = 0; /* reception FIFO operation disable setting£¨FCR0:FE2=0£©*/
    
    /*Transmission FIFO data M byte setting£¨TDR£¨FIFO1£©£©*/
    for(i = 0; i < MFS4_LINUART_LAMCR_LDL; i ++)
    {
     MFS4_LINUART_TDR0 = LIN_Data_Send[i];
    }
    
    MFS4_LINUART_SCR_TXE  = 1; /* Transmission setting £¨SCR:TXE=1¡¢SCR:TIE=0¡¢SCR:RXE=0¡¢SCR:RIE=0¡¢SCR:TBIE=0£©*/
    break;
   case RECEPTION:
    /*init FIFO*/
    MFS4_LINUART_SCR_TXE   = 0; /* Transmission operation disable£¨SCR:TXE=0£©*/
    MFS4_LINUART_SCR_RXE   = 0; /* Reception operation disable£¨SCR:RXE=0£©*/
    MFS4_LINUART_FCR0_FCL1  = 1; /* FIFO1 reset*/
    MFS4_LINUART_FCR0_FCL2  = 1; /* FIFO2 reset*/
    /*enable reception FIFO*/
    MFS4_LINUART_FCR0_FE1  = 0; /* Transmission FIFO operation disable setting£¨FCR0:FE1=0£©*/
    MFS4_LINUART_FCR0_FE2  = 1; /* reception FIFO operation enable setting£¨FCR0:FE2=1£©*/

    /**/
    MFS4_LINUART_SCR_RXE   = 1; /*Reception operation enable£¨SCR:RXE=1£©*/
    break;
   default:
    
    break;
  }
  
  MFS4_LINUART_LAMSR_LAHC = 0;/*clear flag*/
 }
 else if(MFS4_LINUART_LAMSR_LCSC) /* Checksum has been received*/
 {
 	if (ptr_app_lin_msg)
  		response_type=ptr_app_lin_msg(4,MFS4_LINUART_LAMID,RESPONSE);
  
  switch(response_type)
  {
   case TRANSMISSION:
    MFS4_LINUART_FCR0_FE1 = 0; /* Transmission FIFO operation disable setting£¨FCR0:FE1=0£©*/
    MFS4_LINUART_SCR_TXE  = 0; /* Transmission setting £¨SCR:TXE=0¡¢SCR:TIE=0£©*/
    MFS4_LINUART_SCR_TIE  = 0;  
    MFS4_LINUART_SCR_TBIE = 0;
    
    break;
   case RECEPTION:
    MFS4_LINUART_FCR0_FE2 = 0; /* reception FIFO operation disable setting£¨FCR0:FE2=0£©*/
    MFS4_LINUART_SCR_RXE  = 0;
    MFS4_LINUART_SCR_RIE  = 0;
    
    break;
   default:
    
    break;
  }
  
  MFS4_LINUART_LAMSR_LCSC = 0;/*clear flag*/
 }                                                         
 /**/
 else if(MFS4_LINUART_SSR_RDRF)
 {
  dummy = MFS4_LINUART_RDR0;
 }
 else
 {}
}
__interrupt void ISR_LIN4_Transmission(void)
{}
