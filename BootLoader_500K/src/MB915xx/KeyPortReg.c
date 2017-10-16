/* module KeyProtReg.c MB91520 */

/* history */
/* v1.0 RSchum  Initial Version */

/************************************************************************/
/*               (C) Fujitsu Semiconductor Europe GmbH                  */
/*                                                                      */
/* The following software deliverable is intended for and must only be  */
/* used for reference and in an evaluation laboratory environment.      */
/* It is provided on an as-is basis without charge and is subject to    */
/* alterations.                                                         */
/* It is the user obligation to fully test the software in its        */
/* environment and to ensure proper functionality, qualification and    */
/* compliance with component specifications.                            */
/*                                                                      */
/* In the event the software deliverable includes the use of open       */
/* source components, the provisions of the governing open source       */
/* license agreement shall apply with respect to such software          */
/* deliverable.                                                         */
/* FSEU does not warrant that the deliverables do not infringe any      */
/* third party intellectual property right (IPR). In the event that     */
/* the deliverables infringe a third party IPR it is the sole           */
/* responsibility of the customer to obtain necessary licenses to       */
/* continue the usage of the deliverable.                               */
/*                                                                      */
/* To the maximum extent permitted by applicable law FSEU disclaims all */
/* warranties, whether express or implied, in particular, but not       */
/* limited to, warranties of merchantability and fitness for a          */
/* particular purpose for which the deliverable is not designated.      */
/*                                                                      */
/* To the maximum extent permitted by applicable law, FSEU’s liability  */
/* is restricted to intention and gross negligence.                     */
/* FSEU is not liable for consequential damages.                        */
/*                                                                      */
/* (V1.4)                                                               */
/************************************************************************/
#ifndef WIN32

#pragma asm

	.export _WriteKeyProtectedRegister

	_WriteKeyProtectedRegister:
	MOV     R4, R0
	LDI:32  #0x00001FFF, R1
	LDI:20  #0xF44, R2
	LDI:20  #0x4000, R3

	;get the lower 13 bits of traget address
	AND     R1, R0
	ADD     R6, R0 ;-> lower 13 bits + size information + key = 00
    STH     R0, @R2
    ADD     R3, R0 ;-> lower 13 bits + size information + key = 01
    STH     R0, @R2
    ADD     R3, R0 ;-> lower 13 bits + size information + key = 10
    STH     R0, @R2
    ADD     R3, R0 ;-> lower 13 bits + size information + key = 11
    STH     R0, @R2

    CMP     #0, R6 ; check for access size (0x0000 => 8Bit, 0x2000 => 16Bit)
    BEQ     _WKPR_8Bit
    STH     R5, @R4
    BRA     _WKPR_end
_WKPR_8Bit:
    STB     R5, @R4
_WKPR_end:
    RET

#pragma endasm

#else

void WriteKeyProtectedRegister(unsigned int u16Address, unsigned short u16Value, unsigned short u16Size)
{
    u16Address = (unsigned int)&u16Value;
}

#endif

