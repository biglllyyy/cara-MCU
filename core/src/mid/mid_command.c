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
	S16  id;       					/* 指向 表示当前hash对应的id，-1表示没有对应 */
	S16  nexthash; 					/* 指向下一hash，-1表示没有冲突，0~255表示冲突的下一hash */
} s_cmdhash[CMD_HASH_RANGE];

#define HAIMA

const CHAR * s_id2cmd[CMD_ID_COUNT+1]= {0};

static const CHAR REQ__STRING[] = "REQ ";     /* 请求 */
static const CHAR BUZ__STRING[] = "BUZ ";     /* 蜂鸣器 */
static const CHAR SPD__STRING[] = "SPD ";     /* 车速 */
static const CHAR LEFT_STRING[] = "LEFT";     /* 左转向 */
static const CHAR RIGT_STRING[] = "RIGT";     /* 右转向 */
static const CHAR HIBM_STRING[] = "HIBM";     /* 远光灯 */
static const CHAR FFOG_STRING[] = "FFOG";     /* 前雾灯 */
static const CHAR RFOG_STRING[] = "RFOG";     /* 后雾灯 */
static const CHAR LOBM_STRING[] = "LOBM";     /* 金光灯 */
static const CHAR ALS__STRING[] = "ALS ";     /* ALS灯 */
static const CHAR POS__STRING[] = "POS ";     /* 位置灯 */
static const CHAR ABAG_STRING[] = "ABAG";     /* 安全气嚷 */
static const CHAR DRL__STRING[] = "DRL ";     /* 日间行车灯 */
static const CHAR ABS__STRING[] = "ABS ";     /* ABS灯 */
static const CHAR DUBL_STRING[] = "DUBL";	  /* 双闪 */ 
static const CHAR RPM__STRING[] = "RPM ";	 /* 转速信息*/
static const CHAR EPD__STRING[] = "EPD ";	 /* 转速信息*/
/*follow for sent */
static const CHAR CGS__STRING[] = "CGS ";     /* 副驾驶座位有人 */
static const CHAR SBLT_STRING[] = "SBLT";     /* 副安全带 */
static const CHAR IGON_STRING[] = "IGON";     /* 点火开关 */
static const CHAR PARK_STRING[] = "PARK";     /* 机械/电子手刹打开 */
static const CHAR MBLT_STRING[] = "MBLT";     /* 主安全带 */
static const CHAR BRAK_STRING[] = "BRAK";     /* 制动液位低 */
static const CHAR VATS_STRING[] = "VATS";     /* 车身防盗异常 */
static const CHAR REVE_STRING[] = "REVE";     /* 倒挡开关 */
static const CHAR NUTR_STRING[] = "NUTR";     /* 在挡开关 */
static const CHAR FUEL_STRING[] = "FUEL";     /* 燃油电阻值 */
static const CHAR RADR_STRING[] = "RADR";     /* 雷达信息 */
static const CHAR RDSW_STRING[] = "RDSW";     /* 雷达开关 */ 
static const CHAR PIN__STRING[] = "PIN ";	 /* PIN 脚状态模拟 */
static const CHAR PINA_STRING[] = "PINA";	 /* 列出所有PIN脚 */
static const CHAR IMMO_STRING[] = "IMMO";	/* IMMO 发动机防盗 */

     

static void init_cmdstr(void)
{
#ifdef CMD_DEBUG
	S16  id;
#endif
	s_id2cmd[CMD_REQ ] = REQ__STRING;     /* 请求 */
	s_id2cmd[CMD_BUZ ] = BUZ__STRING;     /* 蜂鸣器 */
	s_id2cmd[CMD_SPD ] = SPD__STRING;     /* 车速 */
	s_id2cmd[CMD_LEFT] = LEFT_STRING;     /* 左转向 */
	s_id2cmd[CMD_RIGT] = RIGT_STRING;     /* 右转向 */
	s_id2cmd[CMD_HIBM] = HIBM_STRING;     /* 远光灯 */
	s_id2cmd[CMD_FFOG] = FFOG_STRING;     /* 前雾灯 */
	s_id2cmd[CMD_RFOG] = RFOG_STRING;     /* 后雾灯 */
	s_id2cmd[CMD_LOBM] = LOBM_STRING;     /* 金光灯 */
	s_id2cmd[CMD_ALS ] = ALS__STRING;     /* ALS灯 */
	s_id2cmd[CMD_POS ] = POS__STRING;     /* 位置灯 */
	s_id2cmd[CMD_ABAG] = ABAG_STRING;     /* 安全气嚷 */
	s_id2cmd[CMD_DRL ] = DRL__STRING;     /* 日间行车灯 */
	s_id2cmd[CMD_ABS ] = ABS__STRING;     /* ABS灯 */
	s_id2cmd[CMD_DUBL] = DUBL_STRING;	  /* 双闪 */  
	s_id2cmd[CMD_RPM ] = RPM__STRING;	  /* 转速 */
	s_id2cmd[CMD_EPD ] = EPD__STRING;
	/*next for sent */
	s_id2cmd[CMD_CGS ] = CGS__STRING;     /* 副驾驶座位有人 */
	s_id2cmd[CMD_SBLT] = SBLT_STRING;     /* 副安全带 */
	s_id2cmd[CMD_IGON] = IGON_STRING;     /* 点火开关 */
	s_id2cmd[CMD_PARK] = PARK_STRING;     /* 机械/电子手刹打开 */
	s_id2cmd[CMD_MBLT] = MBLT_STRING;     /* 主安全带 */
	s_id2cmd[CMD_BRAK] = BRAK_STRING;     /* 制动液位低 */
	s_id2cmd[CMD_VATS] = VATS_STRING;     /* 车身防盗异常 */
	s_id2cmd[CMD_REVE] = REVE_STRING;     /* 倒挡开关 */
	s_id2cmd[CMD_NUTR] = NUTR_STRING;     /* 在挡开关 */
	s_id2cmd[CMD_FUEL] = FUEL_STRING;     /* 燃油电阻值 */
	s_id2cmd[CMD_RADR] = RADR_STRING;     /* 雷达信息 */
	s_id2cmd[CMD_RDSW] = RDSW_STRING;     /* 雷达开关 */
	s_id2cmd[CMD_PIN ] = PIN__STRING;	  /* PIN 脚状态模拟 */
	s_id2cmd[CMD_PINA] = PINA_STRING;	  /* 列出所有PIN脚 */
	s_id2cmd[CMD_IMMO] = IMMO_STRING;	 /* 发动机防盗认证 */
   
	s_id2cmd[CMD_ID_COUNT] = (const CHAR *)(NULL);

#ifdef CMD_DEBUG
	/*此段检测上面命令是否存在漏掉的情况 */
	for(id = 0; id < CMD_ID_COUNT; id++)
	{
		if(s_id2cmd[id] == NULL)
		{
			printf("s_id2cmd[%d] is NULL,forgot to set cmdstr?\n", id);
		}
	}
	#endif
}


/*哈希计算，将每个命令的值散列到0~255之间 */
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

/*对于根据上述算法产生的id值存在重复的情况，重新分配，再次计算 */
static S16  cmd2hash(const  CHAR *cmdstr)
{
	/*根据字符串产生初始hash值 */
	S16 hash = makehash(cmdstr);

	do
	{
		/* 如果不存在该命令，则跳出 */
		if(s_cmdhash[hash].id == -1)
		{
			hash = -1;
			break;
		}

		/* 如果匹配上则跳出 */
		if(far_strncmp(cmdstr, s_id2cmd[s_cmdhash[hash].id],4)== FALSE)
		{
			break;
		}
		
		/* 取得下一个项 */
		hash = s_cmdhash[hash].nexthash;
	} while(hash != -1);
	
	/* 成功匹配则返回hash值，否则返回-1表示未找到 */
	return hash;
}



static void init_cmdhash(void)
{
#ifdef CMD_DEBUG
	S16  hashclash[CMD_HASH_RANGE] = { 0 };
	S16  hashdist[CMD_HASH_RANGE] = { 0 };
#endif

	S16  id, hash, nexthash;
	/* 清空命令哈希列表 */
	for(hash = 0; hash < CMD_HASH_RANGE; hash++)
	{
		s_cmdhash[hash].id = -1;
		s_cmdhash[hash].nexthash = -1;
	}
	
	/* hash分配，首先分配未冲突hash */
	for(id = 0; s_id2cmd[id] != NULL; id++)
	{
		hash = makehash(s_id2cmd[id]);
		
		/* 未冲突，直接使用该hash */
		if(s_cmdhash[hash].id == -1)
		{
			s_cmdhash[hash].id = id; 
			s_cmdhash[hash].nexthash = -1;
		}
	}
	
	/* hash分配，然后分配已冲突hash */
	for(id = 0; s_id2cmd[id] != NULL; id++)
	{
		hash = makehash(s_id2cmd[id]);
		
		/* 已冲突，循环查找下一可用hash */
		if(s_cmdhash[hash].id != id)
		{
			/* 查找下一hash的查找起点 */
			/* 如果该项已经出现过冲突，则在冲突hash后续查找 */
			while(s_cmdhash[hash].nexthash != -1)
			{
				hash = s_cmdhash[hash].nexthash;
			}
			
			/*循环查找下一个可用hash位置 */
			nexthash = hash;
			do
			{
				nexthash = (nexthash + 1) % CMD_HASH_RANGE;
			} while(s_cmdhash[nexthash].id != -1);
			
			/* 在找到的ID位置，设置该命令 */
			s_cmdhash[hash].nexthash = nexthash;
			s_cmdhash[nexthash].id = id; 
			s_cmdhash[nexthash].nexthash = -1; 
		}
	}
	
#ifdef CMD_DEBUG
	/* 输出hash分配结果 */
	printf("hash->id, cmd , nexthash\n");
	for(hash = 0; hash < CMD_HASH_RANGE; hash++)
	{
		if(s_cmdhash[hash].id == -1)
		{	continue;}		
		id = s_cmdhash[hash].id;
		
		printf("%3d, %3d, %s, %d\n", hash, id, s_id2cmd[id], s_cmdhash[hash].nexthash);
	}

	/* 输出id分配结果 */
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
	
	/* hash 冲突分析 */
	for(id = 0; s_id2cmd[id] != NULL; id++)
	{
		hash = makehash(s_id2cmd[id]);
		hashclash[hash]++;
	}
	
	/* hash 占用输出 */
	for(hash = 0; hash < CMD_HASH_RANGE; hash++)
	{
		if(hashclash[hash] == 0)
		{	
			continue;
		}	
		hashdist[hashclash[hash]]++;
		printf("%3d:%d\n", hash, hashclash[hash]);
	}

	/* hash 分布输出 */
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
	/* 如果索引值不存在，则返回空表示错误 */
	if((id < 0) || (id >= CMD_ID_COUNT))
	{
		return (const  CHAR *)NULL;
	}	
	
	/* 返回正常字符串 */
	return s_id2cmd[id];
}
S16  cmd2id(const  CHAR *cmdstr)
{
	S16  hash = cmd2hash(cmdstr);
	/* 如果该命令不存在，则返回-1表示错误 */
	/* 否则返回正常值索引值 */
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

/*读取命令，每次传入一个字符 */
/*读取到回车符，且当前命令缓冲区则返回命令结构体，其他情况返回空 */
CMD_TYPE *get_cmd(CMD_TYPE * cmd, char ch)
{
	U8 need_return = 0;
	U8 need_clear = 0;
	do
	{
		/*读取到回车符，则返回命令结构体，其他情况返回空 */
		if(ch == '\r')
		{
			/* 错误：命令长度小于四字节 */
			if(cmd->ncount < 4)
			{
				need_clear = 1;
				break;
			}
			
			/* 错误：命令不存在 */
			cmd->cmd_id = cmd2id(cmd->cmd_line);
			if(cmd->cmd_id == -1)
			{
				need_clear = 1;
				break;
			}

			/* 保存命令，同时准备下次接收 */
			cmd->cmd_line[cmd->ncount] = '\0';
			cmd->ncount = 0;
			
			/* 正常读取到命令，确定需要返回 */
			need_return = 1;
		}
		/*读取到空格，大写字母，数字，将字符添加到命令行缓冲 */
		/*else if((ch == ' ' || ch == '-' || ch == '/' || ISUPPER(ch) || ISDIGIT(ch)) && (cmd->ncount < MAX_CMD_LINE)) */
		else if((ch >= ' ')&& (ch <= '~') &&( cmd->ncount < MAX_CMD_LINE))
		{
			cmd->cmd_line[cmd->ncount++] = ch;
			cmd->cmd_line[cmd->ncount] = '\0';
		}
		else
		{}
	} while(0);
	
	/* 如果命令有误，则清除 */
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
	switch(byte)												/*注意此处switch无break */
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
找出该字符串中字符个数
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