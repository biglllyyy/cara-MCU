#ifndef APP_INFO_H
#define APP_INFO_H

#include "comm_typedef.h"

#define MCU_IN1     PDDR02_P0
#define MCU_IN2     PDDR02_P1
#define MCU_IN3     PDDR02_P2
#define MCU_IN4     PDDR02_P3
#define MCU_IN5     PDDR02_P5
#define MCU_IN6     PDDR02_P6
#define MCU_IN7     PDDR02_P7
#define MCU_IN8     PDDR03_P0

#define MCU_IN9      PDDR03_P1
#define MCU_IN10     PDDR03_P2
#define MCU_IN11     PDDR03_P3
#define MCU_IN12     PDDR03_P4
#define MCU_IN13     PDDR03_P5
#define MCU_IN14     PDDR03_P6
#define MCU_IN15     PDDR03_P7
#define MCU_IN16     PDDR15_P0

#define MCU_IN17     PDDR15_P1
#define MCU_IN18     PDDR04_P0
#define MCU_IN19     PDDR04_P1
#define MCU_IN20     PDDR04_P3
#define MCU_IN21     PDDR04_P4
#define MCU_IN22     PDDR04_P5

#define MCU_IN23     PDDR04_P6

#define MCU_IN24     PDDR04_P7

#define MCU_IN25     PDDR05_P0
#define MCU_IN26     PDDR05_P1
#define MCU_IN27     PDDR05_P2
#define MCU_IN28     PDDR05_P3
#define MCU_IN29     PDDR05_P4
#define MCU_IN30     PDDR05_P5
#define MCU_IN31     PDDR05_P6

#define MCU_IN32     PDDR05_P7
extern U16 single_miles;
extern U32 total_miles;
extern U8  key_info_value;
extern U8  CurrentMenu;

extern void cal_info_task_100ms(void);

#endif
