#ifndef __TEMP_CFG_H__
#define __TEMP_CFG_H__

#define   TEMP_DIS_MAX					120
#define   TEMP_DIS_MIN					50
#define   TEMP_CAN_VALID_MAX			0xFE
#define   TEMP_CAN_INVALID_VALUE		0xFF
#define   TEMP_SCALE					0.75
#define   TEMP_OFFSET					(-48)
#define   TEMP_CAN_DIS_MAX				(U16)((TEMP_DIS_MAX-TEMP_OFFSET)/TEMP_SCALE)
#define   TEMP_CAN_DIS_MIN				(U16)((TEMP_DIS_MIN-TEMP_OFFSET)/TEMP_SCALE)
#define   TEMP_LOST_TIME				TimeMs(2000,10)
#define   TEMP_OVER_MAX_VALUE_TIME		TimeMs(2000,10)

#endif
