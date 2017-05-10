/*=========================================================================
; FR Family Softune Workbench sample,
; ALL RIGHTS RESERVED, COPYRIGHT (C) FUJITSU SEMICODUCTOR LIMITED 2011
; LICENSED MATERIAL - PROGRAM PROPERTY OF FUJITSU SEMICODUCTOR LIMITED
=========================================================================*/

#include <unistd.h>
#include <string.h>

/*#define NOUSE_TRANSBUFFER */
/*#define NOCHECK_MBRBUSY*/
/*
 *  NOUSE_TRANSBUFFER
 *    If this macro is defined, the transfer buffer is not used.
 *    The output data is transfered single byte.
 *
 *  NOCHECK_MBRBUSY
 *    If this macro is defined, MBR:BUSY is not checked. 
 *    Output data is often dropped in this setting. 
 *    Refer to HWM(chapter of OCD) if you want to know details of MBR. 
 *    If NOUSE_TRANSBUFFER macro is defined, this macro is not effected.
 *
 *  MBRBUFFER_LENGTH
 *    This macro is defined the transfer buffer length. (max 255)
 *    If NOUSE_TRANSBUFFER macro is not defined, this macro is not effected.
 *   
 */

#define MBR_BUSY	0x8000U
#define MBR_DMAREQ	0x4000U
#pragma asm
_MBR	.equ	0BFAH
#pragma endasm

#ifndef NOUSE_TRANSBUFFER
  #ifndef MBRBUFFER_LENGTH
    #define MBRBUFFER_LENGTH 255
  #endif
  #if MBRBUFFER_LENGTH > 255
    #define MBRBUFFER_LENGTH 255
  #endif
char _SemiHostBuffer[MBRBUFFER_LENGTH];
#endif

int write (int fileno, char *buf, int size)
{
  extern volatile short MBR;

  /* accept stdio or stderr only */
  if ((fileno!=1)&&(fileno!=2)) return -1;

#ifdef NOUSE_TRANSBUFFER	/* SET SEMIHOSTING /MBR */
  {
    int i;

    for (i = 0; i < size; i++) {
#ifndef NOCHECK_MBRBUSY
      while (MBR & MBR_BUSY);	/* wait for MBR not busy */
#endif
      MBR = *buf++;		/* write data body */
    }
    return i;
  }
#else				/* SET SEMIHOSTING /BUFFER */
  { 
    int remain;
    int piece;

    remain = size;
    while (remain > 0) {
      while (MBR & (MBR_BUSY|MBR_DMAREQ));	/* wait for MBR not busy */

      piece = (MBRBUFFER_LENGTH<remain)?MBRBUFFER_LENGTH:remain;
      memcpy(_SemiHostBuffer, buf, piece);
      buf    += piece;
      remain -= piece;
      MBR = MBR_DMAREQ | (piece&0xFFU);	/* set DMQREQ & data length */
    }
    return size -= remain;
  }
#endif
}
