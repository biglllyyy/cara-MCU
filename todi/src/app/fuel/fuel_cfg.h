#ifndef __FUEL_CFG_H__
#define __FUEL_CFG_H__


/****************************************************************************/
/*
	_Vcc
	|
----+-----ad1
	|
	-
   | |
   |_|
    |
----+-----ad2
	|
    _
   | |
   |_|Rx = signal input
    |
	+-gnd
*/
#define  CONSUMPTION_SCALE  1
#define  DECIMAL_BITS			  1
#define	 AD_11_MAX				  0x7FF
#define  VOL_VCC				  5
#define  UP_RES					  100
//#define  K_PAR					  77
//#define  CONST_DATA1			 (AD_11_MAX*(UP_RES*K_PAR/500))

#define   FUEL_CHN1				(12)
#define   FUEL_FULL_VOLUM_ML	(58000)
#define   SERIOUS_RES 			(100)
	 

#define  VALID_RES_FUEL_OPEN 200
#define  VALID_RES_FUEL_SHORT 10

#define RES_SHIFT	2	/* 电阻值的放大倍数 */

/* 满油、空箱的阻值 */
#define RES_FUEL_EMPTY	(120<<RES_SHIFT)
#define RES_FUEL_FULL	(23<<RES_SHIFT)

/* 判断是否加过油的阈值 */
#define REFUEL_THRESHOLD		12000

/* AD值上下限 */
#define AD_FUEL_FULL	(715)
#define AD_FUEL_EMPTY	(2240)
#define AD_FUEL_RESERVE (10) //加满油后


#endif
