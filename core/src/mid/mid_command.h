
#ifndef _MID_COMMAND_H_
#define _MID_COMMAND_H_

#include "hal_uart.h"
#include "comm_typedef.h"
#include "uart_queue.h"

#define  MAX_CMD_LINE   40
#define HEX_VALUE_PROC(var, min_value, max_value)             \
	do {                                                      \
	if((VALUE = ATOX(cmd->cmd_line + 4)) == -1) break;        \
	if(VALUE < min_value || VALUE > max_value) { VALUE = -1; break; } \
	var = VALUE;                                              \
	} while(0)

#define INT_VALUE_PROC(var, min_value, max_value)             \
	do {                                                          \
	if((VALUE = ATOI(cmd->cmd_line + 4)) == -1) break;        \
	if(VALUE < min_value || VALUE > max_value) { VALUE = -1; break; } \
	var = VALUE;                                              \
	} while(0)

#define STR_VALUE_PROC(var, str_values, int_values, arr_size) \
	for(INDEX = arr_size - 1; INDEX >= 0; INDEX--)                \
{                                                             \
	if(!strcmp(cmd->cmd_line + 4, str_values[INDEX]))         \
	{                                                         \
	var = int_values[INDEX];                              \
	break;                                                \
	}                                                         \
}

#define STR_VALUE_PROC2(var, str_values, arr_size)            \
	for(INDEX = arr_size - 1; INDEX >= 0; INDEX--)                \
{                                                             \
	if(!far_strcmp(cmd->cmd_line + 4, str_values[INDEX]))         \
	{                                                         \
	var = INDEX;                                          \
	break;                                                \
	}                                                         \
}

#define TOUPPER(ch) (ISLOWER(ch) ? (ch) - 0x20 : (ch))
#define ISLOWER(ch) (('a' <= (ch)) && ((ch) <= 'z'))
#define ISUPPER(ch) (('A' <= (ch)) && ((ch) <= 'Z'))
#define ISPRINT(ch) ((0x20 <= (ch)) && ((ch) <= 0x7E))
#define ISDIGIT(ch) (('0' <= (ch)) && ((ch) <= '9'))
#define ISXDIGIT(ch) ((('0' <= (ch)) && ((ch) <= '9')) || (('A' <= (ch)) && ((ch) <= 'F')))


#define HEX_SUCCESS() (VALUE != -1)
#define INT_SUCCESS() (VALUE != -1)
#define STR_SUCCESS() (INDEX >= 0)

/*���������е������±�*/
typedef enum                                              
{   
	CMD_REQ,     /* ���� */ 
	CMD_BUZ,     /* ������ */
	CMD_SPD,     /* ���� */
	CMD_LEFT,    /* ��ת�� */
	CMD_RIGT,    /* ��ת�� */
	CMD_HIBM,    /* Զ��� */
	CMD_FFOG,    /* ǰ��� */
	CMD_RFOG,    /* ����� */
	CMD_LOBM,    /* ���� */
	CMD_ALS,     /* ALS�� */
	CMD_POS,     /* λ�õ� */
	CMD_ABAG,    /* ��ȫ���� */
	CMD_DRL,     /* �ռ��г��� */
	CMD_ABS,     /* ABS�� */  
	CMD_DUBL,	 /* ˫�� */ 
	CMD_RPM,	 /* ת�� */
	CMD_PIN,	 /* PIN��״̬ģ�� */
	CMD_PINA,	 /* PIN���о� */
	CMD_EPD,	 /* other cmd */

/*	follow send */         
	CMD_CGS,     /* ����ʻ��λ���� */
	CMD_SBLT,    /* ����ȫ�� */
	CMD_IGON,    /* ��𿪹� */
	CMD_PARK,    /* ��е/������ɲ�� */
	CMD_MBLT,    /* ����ȫ�� */
	CMD_BRAK,    /* �ƶ�Һλ�� */
	CMD_VATS,    /* ��������쳣 */
	CMD_REVE,    /* �������� */
	CMD_NUTR,    /* �ڵ����� */
	CMD_FUEL,    /* ȼ�͵���ֵ */
	CMD_RADR,    /* �״���Ϣ */
	CMD_RDSW,    /* �״￪�� */ 
	CMD_IMMO,	
	CMD_ID_COUNT      
} CMD_NAME_TYPE;

typedef  struct
{
	S16  cmd_id;
	U16  ncount;
	CHAR cmd_line[MAX_CMD_LINE + 1];
}CMD_TYPE;

S16 cmd2id(const CHAR *cmdstr);
void init_cmdid(void);

extern void		clear_cmd(CMD_TYPE * cmd);
extern CMD_TYPE *get_cmd(CMD_TYPE * cmd, char ch);
extern S32		ATOI(const CHAR *str);
extern S32		ATOX(const CHAR *str);
extern void		ITOA(CHAR *str, S32 value);
extern void		XTOA(CHAR *str, S32 value);
extern void		set_cmd_int(CHAR *cmd_line, U8 cmd_id, U32 value);
extern void		set_cmd_hex(CHAR *cmd_line, U8 cmd_id, U32 value);
extern void		set_cmd_str(CHAR *cmd_line, U8 cmd_id, const CHAR *str);
extern U8		find_str_value(const CHAR *string, const CHAR *str_values[], const U8 *int_values, U8 arr_size);
extern void		sent_data_to_buf(UartQueue *que,const CHAR *str);
//extern void		sent_char_to_buf(UartQueue *que,char ch);
//extern void		sent_data_to_buf(UartQueue *que,const CHAR *str);

#endif
