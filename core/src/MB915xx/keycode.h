/******************************************************************************
 * $Revision$
 * $Date$
 *****************************************************************************/
/************************************************************************/
/*               (C) Fujitsu Semiconductor Europe GmbH                  */
/*                                                                      */
/* The following software deliverable is intended for and must only be  */
/* used for reference and in an evaluation laboratory environment.      */
/* It is provided on an as-is basis without charge and is subject to    */
/* alterations.                                                         */
/* It is the user’s obligation to fully test the software in its        */
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



/*** Port Key Code ***/
typedef union{
    __HWORD word;
    struct{
    __HWORD _KEY1:1;
    __HWORD _KEY0:1;
    __HWORD _SIZE:1;
    __HWORD _RADR12:1;
    __HWORD _RADR11:1;
    __HWORD _RADR10:1;
    __HWORD _RADR9:1;
    __HWORD _RADR8:1;
    __HWORD _RADR7:1;
    __HWORD _RADR6:1;
    __HWORD _RADR5:1;
    __HWORD _RADR4:1;
    __HWORD _RADR3:1;
    __HWORD _RADR2:1;
    __HWORD _RADR1:1;
    __HWORD _RADR0:1;
    }bit;
    struct{
    __HWORD _KEY:2; 
    __HWORD _SIZE:1; 
    __HWORD _RADR:13; 
    }bitc;
}KEYCDRSTR;
 
#ifndef __KEY_CODE_REG_H__
#define __KEY_CODE_REG_H__

#define        WPREG(n,d)                            \
{                                                \
    KEYCDRSTR wk;                                \
    unsigned long adr = (unsigned long)&n;        \
    wk.bitc._RADR = (unsigned short)adr;            \
    wk.bitc._SIZE = (sizeof(n) - 1);                \
    __DI();                                        \
    wk.bitc._KEY = 0;                                \
    KEYCDR = wk.word;                            \
    wk.bitc._KEY = 1;                                \
    KEYCDR = wk.word;                            \
    wk.bitc._KEY = 2;                                \
    KEYCDR = wk.word;                            \
    wk.bitc._KEY = 3;                                \
    KEYCDR = wk.word;                            \
    n = d;                                        \
    __EI();                                        \
}

#define        WPREGBIT(n,b,d)                        \
{                                                \
    KEYCDRSTR wk;                                \
    unsigned long adr = (unsigned long)&n;        \
    unsigned short wkdata;                        \
    wk.bitc._RADR = (unsigned short)adr;            \
    wk.bitc._SIZE = (sizeof(n) - 1);                \
    wkdata = n;                                    \
    if(d)                                        \
        wkdata |= (1<<b);                        \
    else                                        \
        wkdata &= ~(1<<b);                        \
    __DI();                                        \
    wk.bitc._KEY = 0;                                \
    KEYCDR = wk.word;                            \
    wk.bitc._KEY = 1;                                \
    KEYCDR = wk.word;                            \
    wk.bitc._KEY = 2;                                \
    KEYCDR = wk.word;                            \
    wk.bitc._KEY = 3;                                \
    KEYCDR = wk.word;                            \
    n = wkdata;                                    \
    __EI();                                        \
}

#endif /* KEY_CODE_REG_H */
