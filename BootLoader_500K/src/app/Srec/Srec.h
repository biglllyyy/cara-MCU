/*******************************************************************************
* Freescale Semiconductor Inc.
* (c) Copyright 2004-2005 Freescale Semiconductor, Inc.
* ALL RIGHTS RESERVED.
********************************************************************************
Services performed by FREESCALE in this matter are performed AS IS and without 
any warranty. CUSTOMER retains the final decision relative to the total design 
and functionality of the end product. FREESCALE neither guarantees nor will be 
held liable by CUSTOMER for the success of this project.
FREESCALE DISCLAIMS ALL WARRANTIES, EXPRESSED, IMPLIED OR STATUTORY INCLUDING, 
BUT NOT LIMITED TO, IMPLIED WARRANTY OF MERCHANTABILITY OR FITNESS FOR 
A PARTICULAR PURPOSE ON ANY HARDWARE, SOFTWARE ORE ADVISE SUPPLIED TO THE PROJECT
BY FREESCALE, AND OR NAY PRODUCT RESULTING FROM FREESCALE SERVICES. IN NO EVENT
SHALL FREESCALE BE LIABLE FOR INCIDENTAL OR CONSEQUENTIAL DAMAGES ARISING OUT OF 
THIS AGREEMENT.

CUSTOMER agrees to hold FREESCALE harmless against any and all claims demands or 
actions by anyone on account of any damage, or injury, whether commercial, 
contractual, or tortuous, rising directly or indirectly as a result of an advise
or assistance supplied CUSTOMER in connection with product, services or goods 
supplied under this Agreement.
********************************************************************************
* File      Srec.h
* Owner     b01802
* Version   1.0
* Date      Feb-24-2010
* Classification   General Business Information
* Brief     Srecord utilities
********************************************************************************
* Detailed Description:
********************************************************************************
Revision History:
Version  Date          Author    Description of Changes
1.0      Feb-24-2010   b01802    Initial version
*******************************************************************************/
#ifndef _UTILITIES_H /* Prevent duplicated includes */
#define _UTILITIES_H
#include "comm_typedef.h"

#define DataRec 1
#define EndRec 2
#define HeaderRec 3

#define MaxSRecLen 128
#define BUFFER_SIZE		0x400

enum {
	noErr = 0,
	BadHexData,
	CheckSumErr,
	SRecTooLong,
	SRecOddError,
	SRecAdderr,
};
typedef struct {
                UINT8 RecType;					//!<类型
                UINT8 NumBytes;					//!<字节数;
                UINT32 LoadAddr;
                UINT8 Data[MaxSRecLen];
				UINT8 DataLen;
               } SRecDataRec;

typedef struct 
{
	UINT8 buf[BUFFER_SIZE];
	UINT32 r;
	UINT32 w;
}
UPDATA_FIFO;
extern volatile UPDATA_FIFO updata_buf;

extern UINT8 RcvSRecord(SRecDataRec *SRecData);
extern UINT8 ProgramFlash(void);
#endif
/******************************************************************************/
