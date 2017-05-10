#ifdef WIN32
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#endif

#include "comm_typedef.h"
#include "mid_command.h"
#include "mid_string.h"

#define CMD_HASH_RANGE  256
#define DATA_OFFSET		5

U8 find_str_number(const char* str,U8 offset);
void fill_frame_full(char* str,U8 data_byte,U8 sent_way);

static struct
{
	S16  id;       					/* ָ�� ��ʾ��ǰhash��Ӧ��id��-1��ʾû�ж�Ӧ */
	S16  nexthash; 					/* ָ����һhash��-1��ʾû�г�ͻ��0~255��ʾ��ͻ����һhash */
} s_cmdhash[CMD_HASH_RANGE];

#define HAIMA

const CHAR * s_id2cmd[CMD_ID_COUNT+1]= {0};

static const CHAR REQ__STRING[] = "REQ ";     /* ���� */
static const CHAR BUZ__STRING[] = "BUZ ";     /* ������ */
static const CHAR SPD__STRING[] = "SPD ";     /* ���� */
static const CHAR LEFT_STRING[] = "LEFT";     /* ��ת�� */
static const CHAR RIGT_STRING[] = "RIGT";     /* ��ת�� */
static const CHAR HIBM_STRING[] = "HIBM";     /* Զ��� */
static const CHAR FFOG_STRING[] = "FFOG";     /* ǰ��� */
static const CHAR RFOG_STRING[] = "RFOG";     /* ����� */
static const CHAR LOBM_STRING[] = "LOBM";     /* ���� */
static const CHAR ALS__STRING[] = "ALS ";     /* ALS�� */
static const CHAR POS__STRING[] = "POS ";     /* λ�õ� */
static const CHAR ABAG_STRING[] = "ABAG";     /* ��ȫ���� */
static const CHAR DRL__STRING[] = "DRL ";     /* �ռ��г��� */
static const CHAR ABS__STRING[] = "ABS ";     /* ABS�� */
static const CHAR DUBL_STRING[] = "DUBL";	  /* ˫�� */ 
static const CHAR RPM__STRING[] = "RPM ";	 /* ת����Ϣ*/
static const CHAR EPD__STRING[] = "EPD ";	 /* ת����Ϣ*/
/*follow for sent */
static const CHAR CGS__STRING[] = "CGS ";     /* ����ʻ��λ���� */
static const CHAR SBLT_STRING[] = "SBLT";     /* ����ȫ�� */
static const CHAR IGON_STRING[] = "IGON";     /* ��𿪹� */
static const CHAR PARK_STRING[] = "PARK";     /* ��е/������ɲ�� */
static const CHAR MBLT_STRING[] = "MBLT";     /* ����ȫ�� */
static const CHAR BRAK_STRING[] = "BRAK";     /* �ƶ�Һλ�� */
static const CHAR VATS_STRING[] = "VATS";     /* ��������쳣 */
static const CHAR REVE_STRING[] = "REVE";     /* �������� */
static const CHAR NUTR_STRING[] = "NUTR";     /* �ڵ����� */
static const CHAR FUEL_STRING[] = "FUEL";     /* ȼ�͵���ֵ */
static const CHAR RADR_STRING[] = "RADR";     /* �״���Ϣ */
static const CHAR RDSW_STRING[] = "RDSW";     /* �״￪�� */ 
static const CHAR PIN__STRING[] = "PIN ";	 /* PIN ��״̬ģ�� */
static const CHAR PINA_STRING[] = "PINA";	 /* �г�����PIN�� */
static const CHAR IMMO_STRING[] = "IMMO";	/* IMMO ���������� */

     

static void init_cmdstr(void)
{
#ifdef CMD_DEBUG
	S16  id;
#endif
	s_id2cmd[CMD_REQ ] = REQ__STRING;     /* ���� */
	s_id2cmd[CMD_BUZ ] = BUZ__STRING;     /* ������ */
	s_id2cmd[CMD_SPD ] = SPD__STRING;     /* ���� */
	s_id2cmd[CMD_LEFT] = LEFT_STRING;     /* ��ת�� */
	s_id2cmd[CMD_RIGT] = RIGT_STRING;     /* ��ת�� */
	s_id2cmd[CMD_HIBM] = HIBM_STRING;     /* Զ��� */
	s_id2cmd[CMD_FFOG] = FFOG_STRING;     /* ǰ��� */
	s_id2cmd[CMD_RFOG] = RFOG_STRING;     /* ����� */
	s_id2cmd[CMD_LOBM] = LOBM_STRING;     /* ���� */
	s_id2cmd[CMD_ALS ] = ALS__STRING;     /* ALS�� */
	s_id2cmd[CMD_POS ] = POS__STRING;     /* λ�õ� */
	s_id2cmd[CMD_ABAG] = ABAG_STRING;     /* ��ȫ���� */
	s_id2cmd[CMD_DRL ] = DRL__STRING;     /* �ռ��г��� */
	s_id2cmd[CMD_ABS ] = ABS__STRING;     /* ABS�� */
	s_id2cmd[CMD_DUBL] = DUBL_STRING;	  /* ˫�� */  
	s_id2cmd[CMD_RPM ] = RPM__STRING;	  /* ת�� */
	s_id2cmd[CMD_EPD ] = EPD__STRING;
	/*next for sent */
	s_id2cmd[CMD_CGS ] = CGS__STRING;     /* ����ʻ��λ���� */
	s_id2cmd[CMD_SBLT] = SBLT_STRING;     /* ����ȫ�� */
	s_id2cmd[CMD_IGON] = IGON_STRING;     /* ��𿪹� */
	s_id2cmd[CMD_PARK] = PARK_STRING;     /* ��е/������ɲ�� */
	s_id2cmd[CMD_MBLT] = MBLT_STRING;     /* ����ȫ�� */
	s_id2cmd[CMD_BRAK] = BRAK_STRING;     /* �ƶ�Һλ�� */
	s_id2cmd[CMD_VATS] = VATS_STRING;     /* ��������쳣 */
	s_id2cmd[CMD_REVE] = REVE_STRING;     /* �������� */
	s_id2cmd[CMD_NUTR] = NUTR_STRING;     /* �ڵ����� */
	s_id2cmd[CMD_FUEL] = FUEL_STRING;     /* ȼ�͵���ֵ */
	s_id2cmd[CMD_RADR] = RADR_STRING;     /* �״���Ϣ */
	s_id2cmd[CMD_RDSW] = RDSW_STRING;     /* �״￪�� */
	s_id2cmd[CMD_PIN ] = PIN__STRING;	  /* PIN ��״̬ģ�� */
	s_id2cmd[CMD_PINA] = PINA_STRING;	  /* �г�����PIN�� */
	s_id2cmd[CMD_IMMO] = IMMO_STRING;	 /* ������������֤ */
   
	s_id2cmd[CMD_ID_COUNT] = (const CHAR *)(NULL);

#ifdef CMD_DEBUG
	/*�˶μ�����������Ƿ����©������� */
	for(id = 0; id < CMD_ID_COUNT; id++)
	{
		if(s_id2cmd[id] == NULL)
		{
			printf("s_id2cmd[%d] is NULL,forgot to set cmdstr?\n", id);
		}
	}
	#endif
}


/*��ϣ���㣬��ÿ�������ֵɢ�е�0~255֮�� */
static U16  makehash(const CHAR *cmdstr)
{
	U16  i;
	U16  hash = 0;
	for(i = 0; i < 4; i++)
	{
		hash ^= cmdstr[i] << (3 * i);
	}
	hash ^= hash >> 8;
	return hash  & 0xFF;
}

/*���ڸ��������㷨������idֵ�����ظ�����������·��䣬�ٴμ��� */
static S16  cmd2hash(const  CHAR *cmdstr)
{
	/*�����ַ���������ʼhashֵ */
	S16 hash = makehash(cmdstr);

	do
	{
		/* ��������ڸ���������� */
		if(s_cmdhash[hash].id == -1)
		{
			hash = -1;
			break;
		}

		/* ���ƥ���������� */
		if(far_strncmp(cmdstr, s_id2cmd[s_cmdhash[hash].id],4)== FALSE)
		{
			break;
		}
		
		/* ȡ����һ���� */
		hash = s_cmdhash[hash].nexthash;
	} while(hash != -1);
	
	/* �ɹ�ƥ���򷵻�hashֵ�����򷵻�-1��ʾδ�ҵ� */
	return hash;
}



static void init_cmdhash(void)
{
#ifdef CMD_DEBUG
	S16  hashclash[CMD_HASH_RANGE] = { 0 };
	S16  hashdist[CMD_HASH_RANGE] = { 0 };
#endif

	S16  id, hash, nexthash;
	/* ��������ϣ�б� */
	for(hash = 0; hash < CMD_HASH_RANGE; hash++)
	{
		s_cmdhash[hash].id = -1;
		s_cmdhash[hash].nexthash = -1;
	}
	
	/* hash���䣬���ȷ���δ��ͻhash */
	for(id = 0; s_id2cmd[id] != NULL; id++)
	{
		hash = makehash(s_id2cmd[id]);
		
		/* δ��ͻ��ֱ��ʹ�ø�hash */
		if(s_cmdhash[hash].id == -1)
		{
			s_cmdhash[hash].id = id; 
			s_cmdhash[hash].nexthash = -1;
		}
	}
	
	/* hash���䣬Ȼ������ѳ�ͻhash */
	for(id = 0; s_id2cmd[id] != NULL; id++)
	{
		hash = makehash(s_id2cmd[id]);
		
		/* �ѳ�ͻ��ѭ��������һ����hash */
		if(s_cmdhash[hash].id != id)
		{
			/* ������һhash�Ĳ������ */
			/* ��������Ѿ����ֹ���ͻ�����ڳ�ͻhash�������� */
			while(s_cmdhash[hash].nexthash != -1)
			{
				hash = s_cmdhash[hash].nexthash;
			}
			
			/*ѭ��������һ������hashλ�� */
			nexthash = hash;
			do
			{
				nexthash = (nexthash + 1) % CMD_HASH_RANGE;
			} while(s_cmdhash[nexthash].id != -1);
			
			/* ���ҵ���IDλ�ã����ø����� */
			s_cmdhash[hash].nexthash = nexthash;
			s_cmdhash[nexthash].id = id; 
			s_cmdhash[nexthash].nexthash = -1; 
		}
	}
	
#ifdef CMD_DEBUG
	/* ���hash������ */
	printf("hash->id, cmd , nexthash\n");
	for(hash = 0; hash < CMD_HASH_RANGE; hash++)
	{
		if(s_cmdhash[hash].id == -1)
		{	continue;}		
		id = s_cmdhash[hash].id;
		
		printf("%3d, %3d, %s, %d\n", hash, id, s_id2cmd[id], s_cmdhash[hash].nexthash);
	}

	/* ���id������ */
	printf(" id->hash, cmd\n");
	for(id = 0; id < CMD_ID_COUNT; id++)
	{
		hash = cmd2hash(s_id2cmd[id]);		
		if(s_cmdhash[hash].id != id)
		{
			printf("ERROR id %d\n", hash);
		}
		printf("%3d, %4d, %s\n", id, hash, s_id2cmd[id]);
	}
	
	/* hash ��ͻ���� */
	for(id = 0; s_id2cmd[id] != NULL; id++)
	{
		hash = makehash(s_id2cmd[id]);
		hashclash[hash]++;
	}
	
	/* hash ռ����� */
	for(hash = 0; hash < CMD_HASH_RANGE; hash++)
	{
		if(hashclash[hash] == 0)
		{	
			continue;
		}	
		hashdist[hashclash[hash]]++;
		printf("%3d:%d\n", hash, hashclash[hash]);
	}

	/* hash �ֲ���� */
	for(hash = 0; hash < CMD_HASH_RANGE; hash++)
	{
		if(hashdist[hash] == 0)
		{
			continue;
		}	
		printf("%d:%2d=>%2d\n", hash, hashdist[hash], hash * hashdist[hash]);
	}
#endif
}
const  CHAR * id2cmd(S16  id)
{
	/* �������ֵ�����ڣ��򷵻ؿձ�ʾ���� */
	if((id < 0) || (id >= CMD_ID_COUNT))
	{
		return (const  CHAR *)NULL;
	}	
	
	/* ���������ַ��� */
	return s_id2cmd[id];
}
S16  cmd2id(const  CHAR *cmdstr)
{
	S16  hash = cmd2hash(cmdstr);
	/* �����������ڣ��򷵻�-1��ʾ���� */
	/* ���򷵻�����ֵ����ֵ */
	return (hash == -1) ? -1 : s_cmdhash[hash].id;
}
void init_cmdid(void)
{
	init_cmdstr();
	init_cmdhash();
}

S32 ATOI(const CHAR *str)
{
	S32 i;
	S32 value = 0;
	do
	{
		if((str == NULL) || (*str == '\0')) 
		{
			value = -1;
			break;
		}
		for(i = 0; str[i] != '\0'; i++)
		{
			if(!ISDIGIT(str[i])) 
			{
				value = -1;
				break;
			}
			value *= 10;
			value += str[i] - '0';
		}
	}while(0);	

	return value;
}

S32 ATOX(const CHAR *str)
{
	S32 i;
	S32 value = 0;
    do
	{
		if((str == NULL) || (*str == '\0')) 
		{
			value = -1;
			break;
		}
		for(i = 0; str[i] != '\0'; i++)
		{
			if(!ISXDIGIT(str[i]))
			{
				value = -1;
				break;
			}

			value <<= 4;
			if(ISDIGIT(str[i]))
			{
				value += str[i] - '0';
			}
			else
			{
				value += 10 + str[i] - 'A';
			}
		}
	}while(0);	
	return value;
}

void ITOA(CHAR *str, S32 value)
{
	S32 temp, pos;
	do 
	{
		if(str == (void*)0) {break;}
		if(value < 0) 
		{ 
			str[0] = 0; 
			break;
		}
		pos = 0;
		temp = value;
		do 
		{
			pos++;
			temp /= 10;
		} while (temp > 0);

		str[pos] = 0;
		temp = value;
		do 
		{
			str[--pos] = (CHAR )('0' + temp % 10);
			temp /= 10;
		} while (temp > 0);

	} while (0);
}

void XTOA(CHAR *str, S32 value)
{
	const char hex[] = "0123456789ABCDEF";
	S32 temp, pos;

	do 
	{
		if(str == (void*)0){ break;}
		if(value < 0) { str[0] = 0; break;}
		pos = 0;
		temp = value;
		do 
		{
			pos++;
			temp >>= 4;
		} while (temp > 0);

		str[pos] = 0;
		temp = value;
		do 
		{
			str[--pos] = hex[temp % 16];

			temp >>= 4;
		} while (temp > 0);

	} while (0);
}

void set_cmd_int(CHAR *cmd_line,U8 cmd_id,U32 value)
{
	//memcpy(cmd_line, id2cmd(cmd_id), 4);
	far_strcpy(cmd_line,id2cmd(cmd_id));
	ITOA(cmd_line + DATA_OFFSET, value);
}

void set_cmd_hex(CHAR *cmd_line, U8 cmd_id, U32 value)
{
	far_strcpy(cmd_line, id2cmd(cmd_id));
	XTOA(cmd_line + DATA_OFFSET, value);
}

void set_cmd_str(CHAR *cmd_line, U8 cmd_id, const  CHAR *str)
{
	U8 data_bytes = 0;
	//memcpy(cmd_line, id2cmd(cmd_id), 4);
	far_strcpy(cmd_line,id2cmd(cmd_id));
	far_strcpy(cmd_line + DATA_OFFSET, str);

	data_bytes = find_str_number(cmd_line,DATA_OFFSET);	/* find character numbers */
	fill_frame_full(cmd_line,data_bytes,1);
}

U8 find_str_value(const  CHAR *string,const  CHAR *str_values[],const U8 *int_values,U8 arr_size) 
{         
	U8 index = 0, value = 0;

	for(index = 0; index < arr_size; index++)
	{
		if(!far_strcmp(string + DATA_OFFSET, str_values[index]))
		{
			value = int_values[index]; 
			break;
		}
	}   
	
	return value;			
}


void clear_cmd(CMD_TYPE * cmd)
{
	cmd->cmd_id = -1;
	cmd->ncount = 0;
	cmd->cmd_line[0] = 0;
}

/*��ȡ���ÿ�δ���һ���ַ� */
/*��ȡ���س������ҵ�ǰ��������򷵻�����ṹ�壬����������ؿ� */
CMD_TYPE *get_cmd(CMD_TYPE * cmd, char ch)
{
	U8 need_return = 0;
	U8 need_clear = 0;
	do
	{
		/*��ȡ���س������򷵻�����ṹ�壬����������ؿ� */
		if(ch == '\r')
		{
			/* ���������С�����ֽ� */
			if(cmd->ncount < 4)
			{
				need_clear = 1;
				break;
			}
			
			/* ����������� */
			cmd->cmd_id = cmd2id(cmd->cmd_line);
			if(cmd->cmd_id == -1)
			{
				need_clear = 1;
				break;
			}

			/* �������ͬʱ׼���´ν��� */
			cmd->cmd_line[cmd->ncount] = '\0';
			cmd->ncount = 0;
			
			/* ������ȡ�����ȷ����Ҫ���� */
			need_return = 1;
		}
		/*��ȡ���ո񣬴�д��ĸ�����֣����ַ���ӵ������л��� */
		/*else if((ch == ' ' || ch == '-' || ch == '/' || ISUPPER(ch) || ISDIGIT(ch)) && (cmd->ncount < MAX_CMD_LINE)) */
		else if((ch >= ' ')&& (ch <= '~') &&( cmd->ncount < MAX_CMD_LINE))
		{
			cmd->cmd_line[cmd->ncount++] = ch;
			cmd->cmd_line[cmd->ncount] = '\0';
		}
		else
		{}
	} while(0);
	
	/* ���������������� */
	if(need_clear)
	{
		clear_cmd(cmd);
	}
	return (need_return) ? cmd : NULL;
}


void PutOutHex(U8 chn,U32  temp)
{
	const char hexstr[] = "0123456789ABCDEF";
	U8 data = 0;
	U8 byte = 0;
	hal_uart_put_char(chn,'0');
	hal_uart_put_char(chn,'X');
	if(temp <= 0xff)
	{
		byte = 1;
	}
	else if(temp <= 0xffff)
	{
		byte = 2;
	}
	else if(temp <= 0xffffff)
	{
		byte = 3;
	}
	else
	{
		byte = 4;
	}
	switch(byte)												/*ע��˴�switch��break */
	{
	case 4:   	data = (U8)((temp & 0xff000000) >> 24);	hal_uart_put_char(chn, hexstr[data >> 4]); hal_uart_put_char(chn, hexstr[data & 0x0F]);
	case 3: 	data = (U8)((temp & 0x00ff0000) >> 16);	hal_uart_put_char(chn, hexstr[data >> 4]); hal_uart_put_char(chn, hexstr[data & 0x0F]);
	case 2:		data = (U8)((temp & 0x0000ff00)	>> 8);	hal_uart_put_char(chn, hexstr[data >> 4]); hal_uart_put_char(chn, hexstr[data & 0x0F]);
	case 1:		data = (U8)((temp) & 0xff);				hal_uart_put_char(chn, hexstr[data >> 4]); hal_uart_put_char(chn, hexstr[data & 0x0F]);
	default:break;
	}
	hal_uart_put_char(chn,'\r');
}

/*
�ҳ����ַ������ַ�����
*/
U8 find_str_number(const char* str,U8 offset)
{
	U8 temp =0;
	while(*(str+offset) != 0)
	{
		temp++;
		str++;
	}
	return temp;
}


void  sent_char_to_buf(UartQueue *que,char ch)
{
	if(uart_en_queue(que,ch))
	{
		uart_de_queue(que);
		uart_en_queue(que,ch);
	}
}

void  sent_data_to_buf(UartQueue *que,const CHAR *str)
{
	while(*str)
	{
		sent_char_to_buf(que,*str);
		str++;
	}
}


void fill_frame_full(char* str,U8 data_byte,U8 sent_way)
{
	U16  check_sum = 0; 
	if(data_byte < 8)
	{
		str[DATA_OFFSET-1] =  (data_byte <<4)|(sent_way &0x03);
		check_sum = api_cal_crc16(str,data_byte+5);
		str[DATA_OFFSET+data_byte] = (check_sum&0xff00)>>8;
		str[DATA_OFFSET+data_byte+1] = check_sum&0xff;
	}

}