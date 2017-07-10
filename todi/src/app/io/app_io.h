#ifndef  __APP_IO_H__
#define  __APP_IO_H__

#include "mid_pin.h"
#include "mid_filter.h"
#include "io_cfg.h"
#include "mid_can.h"

void app_cfg_io(void);

void DS1302_io_config(void);
extern void app_cfg_led_out_io(void);
extern void app_update_A20_ioconfg(void);
extern void app_test_update_core_board_100MS(void);

#if (DS1302_CFG == CFG_ON)
extern io_type_t  IO_RTC_RST;
extern io_type_t  IO_RTC_IO;
extern io_type_t  IO_RTC_SCK;
extern void		 DS1302_ioconfig(void);
#endif 

#if (PIN_CORE_POWER == CFG_ON)
extern  io_type_t  VCC_4V2_EN; 
extern  io_type_t  MCU_HOLD_ON;
#endif

#if (SIG_BUZ_CTL == CFG_ON)
extern io_type_t BUZ_CTL_IO;
#endif


#define    CAN_CHIP		TJA1042T   
#define    CAN_CFG      CFG_ON
#if	(CAN_CFG == CFG_ON)
extern io_type_t	io_can_stb;	 
extern io_type_t	io_can_en;

extern void      can_io_config(U8 chip);
#endif /* cfg on */

#if  (SIG_IN_DOORS  == CFG_ON)
extern  io_type_t DOOR_FL_IN;
extern  io_type_t DOOR_FR_IN;
extern  io_type_t DOOR_RL_IN;
extern  io_type_t DOOR_RR_IN;
extern  io_type_t DOOR_HOOD_IN;
extern  io_type_t DOOR_TRUNK_IN;
extern  void door_config_io_input(void);
#endif
void app_cfg_io_in(void);
void app_cfg_io_out(void);

#if (A20_UPDATE == CFG_ON)
extern  io_type_t   IO_MCU_CTL_START_UP;
extern  io_type_t	IO_MCU_CTL_STOP_UP;
extern  void app_update_A20_ioconfg(void);
#endif

/**********************************************************************************/

#if	   (SIG_BACKLIGHT_EN == CFG_ON)
extern io_type_t BL_POWER_CTRL;
extern io_type_t BL_GMA_SD; //sch0.4:add this line

#endif

extern  io_type_t	    pin_io_in[PIN_IN_ALL];
extern  pin_filter_t    pin_filter_in[PIN_IN_ALL];

void app_io_task(void);

#endif  /* the last */
