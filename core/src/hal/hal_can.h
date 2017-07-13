#ifndef __HAL_CAN_H__
#define __HAL_CAN_H__

#include "comm_typedef.h"
/*

SEG = TSeg2 + Tseg1 + 3
tq=([BRPE, BRP]+1) / fsys
BTR = SEG*

TSeg2   TSeg1  SJW
2		11		1
*/


/*************************************/
#define  MCLK_8M_BTR_500K   		0x2B40
#define  MCLK_8M_BTR_250K       0x2B41
#define  MCLK_8M_BTR_125K       0x2B43
#define  MCLK_8M_BTR_100K       0x2B44

#define  MCLK_4M_BTR_250K       0x2B40
#define  MCLK_4M_BTR_125K       0x2B41


#define MSG2STD(msg) ((msg & 0x000007FFL) << 18)
#define MSG2EXT(msg) (msg & 0x1FFFFFFFL)
#define STD2MSG(reg) (((reg & 0x1FFFFFFFL) >> 18) & 0x000007FFL)
#define EXT2MSG(reg) (reg & 0x1FFFFFFFL)

#define CAN_NO_EWARN		    (0u)
#define CAN_EWARN				(1u)
#define CAN_EPASS				(2u)
#define CAN_BOFF				(3u)


typedef enum
{
	STAND_ID  = 0,
	EXTERN_ID = 1
}CAN_ID_FORMAT_e;


typedef struct
{
    U32  		 	id;
	U32				id_mask;
    CAN_ID_FORMAT_e format;
    U8           	dlc;
    U8           	data[8];
    U8  			buffer_num;
    U8			 	new_frame;
    U8			 	lost;
    U16			 	count;				/* signal lost count */
    U16 		 	period_ms;			/* signal period */ 
} can_msg_t;

typedef enum
{
    CAN_NORMAL = 0,
    STUFF_ERR =1,
    FORM_ERR= 2,
    ACK_ERR = 3,
    BIT1_ERR = 4,
    BIT0_ERR = 5,
    CRC_ERR = 6,
    UN_KNOWN_ERR = 7
} t_CAN_ERR_TYPE;



extern  void hal_can_prepare(U8 nRecv, U8 nSend,can_msg_t*can_msg);
extern  void hal_can_init(U8 chn);
extern  U8   hal_can_get(can_msg_t *can_rx_ptr);
extern  void hal_can_sent(U8 chn,can_msg_t *can_msg);
extern  void State_judge_0(void);
extern  U32 HalChkMob(void);
extern  t_CAN_ERR_TYPE hal_get_eer_type(void);
extern  U8 hal_canerror_statecheck(void);

#endif
