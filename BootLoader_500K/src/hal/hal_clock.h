#ifndef _HAL_CLOCK_H__
#define _HAL_CLOCK_H__

#include "_fr.h"
#include "comm_typedef.h"

#define  MCLK_FRQ1		4000000
#define  MCLK_FRQ2		8000000
#define  _40M			(40000000ul)
#define  _80M			(80000000ul)
#define  _4M			(4000000ul)
#define  _8M			(8000000ul)
#define  _12M			(12000000ul)
#define  _32_7K			(32768ul)
#define  MAIN_OSC		_4M
/********MCLK select  ****************/
#define  MCLK_FRQ		MCLK_FRQ1
#define  PCLK			_40M
#define  HCLK			_40M
#define  SupplyClk	    _40M	//40Mhz

#define SYSTEM_MAINCLOCK_FREQ	_4M
#define SYSTEM_SUBCLOCK_FREQ	_32_7K
/*
PLL config logic

                |~~~~~~~~~~~~|		 |~~~~~~~~~~~~|			  |~~~~~~~~~~~~~~~~|
MCLK		    | Divider    |		 |   PLL      |			  | Divider        |
----------------+            +-------+(NON  SSCG) +-----------+ CCPSDIVR.PODS  +------->PLLCLK
                | PLLCR.PDS  |		 |	          |			  |________________|
                |____________|		 |____________|
                      |
                      |
               |~~~~~~~~~~~~~|
               |   *Mul	     |
               |CCPLLFBR.IDIV|
               |_____________|
*/

/*
Source CLK cfg

            |~~~~~~~|           |~~~~~|           |~~~ \
MCLK--------|  1/2  |-----------|MCLK2|-----------|0/1  |
            |_______|           |_____|           |     |
                                                  |     |
PLLCLK------------------------------------------->|2    +------------------>Source CLK
                                                  |     |
SBCLK-------------------------------------------->|3    |
(sub clock)					                      |_____|
                                                    |
                                                    |
                                              |~~~~~~~~~~~~|
                                              |clock select|
                                              | CSELR.CKS  |
                                              |CMONR..CKM  |
                                              |____________|
*/
/*

CCLK,PCLK1,PCLK2 cfg
                                            |~~~~~~~~~~~~~~|
                |~~~~~~~~~~~~~~|            |      1       |--------------------------->HCLK
Source CLK      |              |base clock  |      1       |---------------------------->CCLK
----------------|DIVR0.DIVB    +----------->|  DIVR2.DIVP  +----------+----------------->PCLK1
                |(1/1~1/8)     |            |  (1/1~1/16)  |          |
                |______________|	        |  DIVR1.DIVT  +----------|----------------->TCLK
                                            |  (1/1~1/8)   |          |
                                            |______________|          |
                                                                      |
                                                                      |
                                                                      |
                                                                      |
                                                                      |
                                                                      |
               |~~~~~~~~~~~~~~|			                              |        |~~~~~~~~~~~|
               |  PICD.PDIV   |			                              +------->|  SACR.M   |
PLLCLK---------|   1/1~1/16   +----------------------------------------------->|           +---->PCLK2
               |______________|						                           |___________|
*/

#define PLL_MUL_MIN		(13)
#define PLL_MUL_MAX		(100)

/*select PLL clk */
#define  PLL_CLK_MCLK_4M_PLL_80M_CCLK_80M_PCLK_TCLK_80M()			SelectPLLClock(40,DIV_1,PLL_OUT_DIV_2,BASE_DIV_1,DIV_1,BASE_DIV_1)
#define  PLL_CLK_MCLK_4M_PLL_80M_CCLK_80M_PCLK_TCLK_40M()           SelectPLLClock(40,DIV_1,PLL_OUT_DIV_2,BASE_DIV_1,DIV_2,BASE_DIV_2)
#define  PLL_CLK_MCLK_4M_PLL_80M_CCLK_40M_PCLK_TCLK_40M()			SelectPLLClock(40,DIV_1,PLL_OUT_DIV_2,BASE_DIV_2,DIV_1,BASE_DIV_1)
#define  PLL_CLK_MCLK_8M_PLL_80M_CCLK_80M_PCLK_TCLK_80M()			SelectPLLClock(20,DIV_1,PLL_OUT_DIV_2,BASE_DIV_1,DIV_1,BASE_DIV_1)
#define  PLL_CLK_MCLK_8M_PLL_80M_CCLK_40M_PCLK_TCLK_40M()			SelectPLLClock(20,DIV_1,PLL_OUT_DIV_2,BASE_DIV_2,DIV_1,BASE_DIV_1)
#define  PLL_CLK_MCLK_8M_PLL_40M_CCLK_40M_PCLK_TCLK_40M()			SelectPLLClock(10,DIV_1,PLL_OUT_DIV_2,BASE_DIV_1,DIV_1,BASE_DIV_1)
/* select main clk */
#define  MAIN_CLK_MCLK_4M_CCLK_4M_PCLK_TCLK_4M()					SelectMainClock(BASE_DIV_1,DIV_1,BASE_DIV_1)
#define  MAIN_CLK_MCLK_8M_CCLK_4M_PCLK_TCLK_4M()					SelectMainClock(BASE_DIV_2,DIV_1,BASE_DIV_1)
#define  MAIN_CLK_MCLK_8M_CCLK_8M_PCLK_TCLK_8M()				    SelectMainClock(BASE_DIV_1,DIV_1,BASE_DIV_1)


typedef enum
{
    /* PLL in_n = MCLK/n;n=1-16,set PLLCR.PDS*/
    DIV_1 = 0,
    DIV_2 = 1,
    DIV_3 = 2,
    DIV_4 = 3,
    DIV_5 = 4,
    DIV_6 = 5,
    DIV_7 = 6,
    DIV_8 = 7,
    DIV_9 = 8,
    DIV_10 = 9,
    DIV_11 = 10,
    DIV_12= 11,
    DIV_13= 12,
    DIV_14 = 13,
    DIV_15 = 14,
    DIV_16= 15
} e_MCLK_2_PLL_DIV;

typedef enum
{
    /* set CCPSDIVR.PODS */
    PLL_OUT_DIV_2 = 0,
    PLL_OUT_DIV_4 = 1,
    PLL_OUT_DIV_6 = 2,
    PLL_OUT_DIV_8 = 3,
    PLL_OUT_DIV_10 =4,
    PLL_OUT_DIV_12 =5,
    PLL_OUT_DIV_14 =6,
    PLL_OUT_DIV_16 =7
} e_PLL_OUT_DIV;

typedef enum
{
    /* set DIVR2.DIVP;base clk = source_clk/_n */
    BASE_DIV_1 = 0,     /* no divide,init value  */
    BASE_DIV_2 = 1,
    BASE_DIV_3 = 2,
    BASE_DIV_4 = 3,
    BASE_DIV_5 = 4,
    BASE_DIV_6 = 5,
    BASE_DIV_7 = 6,
    BASE_DIV_8 = 7
} e_SOURCE_2_BASE_DIV;
typedef   e_MCLK_2_PLL_DIV       e_BASE_2_PCLK1_DIV;    /* set DIVR2.DIVP  */
typedef   e_SOURCE_2_BASE_DIV    e_BASE_2_TCLK_DIV;	   /* set DIVR1.DIVT */




void SelectPLLClock(
    U8 u8PLLMultiplicationFactor,
    e_MCLK_2_PLL_DIV	 ePLLInputClockDivider,
    e_PLL_OUT_DIV		 ePLLOutputDivider,
    e_SOURCE_2_BASE_DIV  eSource_Base_Div,
    e_BASE_2_PCLK1_DIV   eBase_Pclk1_Div,
    e_BASE_2_TCLK_DIV    eBase_Tclk_Div);


void SelectMainClock(e_SOURCE_2_BASE_DIV  eSource_Base_Div,
                     e_BASE_2_PCLK1_DIV   eBase_Pclk1_Div,
                     e_BASE_2_TCLK_DIV    eBase_Tclk_Div);
#ifndef __CPU_MB91F577S__
void SelectSubClock(e_SOURCE_2_BASE_DIV  eSource_Base_Div,
                    e_BASE_2_PCLK1_DIV   eBase_Pclk1_Div,
                    e_BASE_2_TCLK_DIV    eBase_Tclk_Div);
#endif // __CPU_MB91F577S__



/**********   fun declear *******************************/
void hal_pll_init(void);
void hal_clock_init(void);
void cpu_goto_main_run(void);


#endif /* __hal_clock_h_ */

/* end if header file */
