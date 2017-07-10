#include "mid_string.h"
#include "hal_uart.h"
#ifdef WIN32
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#endif



S16  sent_data(U8 chn, U8 *string, U8 len)
{
    S16 ret = 0;
	hal_enable_uart_tx_interrupt(FALSE);
    while(len)
    {
		if(uart_en_queue(&uart_sent_queue,*string))
		{
			uart_de_queue(&uart_sent_queue);
		}
        string++;
        len--;
    }
    hal_enable_uart_tx_interrupt(TRUE); 
    return ret;
}




void far_strcpy( CHAR *dest,const CHAR* src)
{
    while ((*(dest++) = *(src++)) != '\0');
}

/*字符串比较函数*/
S16 far_strcmp(const CHAR* s1,const  CHAR* s2)
{
    while(*s1 == *s2)
    {
        if(*s1 == 0) break;
        if(*s2 == 0) break;

        s1++;
        s2++;
    }
    return (*s1 - *s2);
}

/*相等为0，不相等为1*/
S16 far_strncmp(const CHAR* s1,const  CHAR* s2, S16 count)
{
    while((count > 0) && (*s1 == *s2))
    {
        if(*s1 == 0) break;
        if(*s2 == 0) break;

        s1++;
        s2++;
        count--;
    }

    return count;
}

/* copy  function
   copy from src to dest,
	
*/
void data_copy(U8 *dest,U8 *src,U8 count)
{
	U8 index = 0;

	index = count;
	while(index>0)
	{
		*dest = *src;
		src++;
		dest++;
		index--;
	}
}


/* ANSI标准形式的声明方式，括号内的省略号表示可选参数 */
#ifdef  WIN32
int debug_out(char *msg,int first)
{
#if 0
    va_list  v1; /* 定义保存函数参数的结构 */
    int para = first; /* 存放取出的字符串参数 */

    /* argp指向传入的第一个可选参数，
    msg是最后一个确定的参数 */
    va_start(v1, first);
    while (para != -1)
    {
        /* 取出当前的参数，类型为 int */
        para = va_arg(v1, int);
        /* 采用空串指示参数输入结束 */
    }
    va_end(v1); /* 将argp置为NULL */
#endif
    if(!forbid_output)
    {
        printf(msg,first);
    }
    return 0;
}

#endif


S32 str_to_dec(const U8 *str)
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
/**************************************************************************************
*　函数名称: ATOX
*  功　　能: 字符串转换为十六进制
*  入口参数: 字符串
*  返 回 值: 十六进制数据
*  备    注: 
************************************************************************************/
S32 ATOX(const CHAR str[])
{
	U16 i;
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
			value = (U16)((U16)value << 4u);
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
/*************************************************************************************
*　函数名称: ITOA
*  功　　能: 十进制转换为字符串
*  入口参数: 十进制数据
*  返 回 值: 字符串
************************************************************************************/
void dec_to_str(U8 *str,S32 value)
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
			str[--pos] = '0' + (U8)(temp % 10);

			temp /= 10;
		} while (temp > 0);

	} while (0);
}
/************************************************************************************
*　函数名称: XTOA
*  功　　能: 十六进制转换为字符串
*  入口参数: 十六进制数据
*  返 回 值: 字符串
************************************************************************************/
void XTOA(CHAR str[], S32 value)
{
	CHAR hex[] = "0123456789ABCDEF";
	U32 temp, pos;

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
/************************************************************************************
*　函数名称: data_switch
*  功　　能: 两字解数据交换
*  入口参数: 输入数据地址
*  返 回 值: none
************************************************************************************/
void data_switch(U8 *data1, U8 *data2)
{
	U8 temp = 0;

	temp = *data1;
	*data1 = *data2;
	*data2= temp;
}
/************************************************************************************
*　函数名称: byte_order_change
*  功　　能: 改变多字节大小端顺序
*  入口参数: *data:输入数据地址,len:输入数据字节数 
*  返 回 值: none
************************************************************************************/
void byte_order_change(U8 *data,U8 len)
{
	switch(len)
	{
		case 2:
			 data_switch(data,data+1);
			 break;
		case 3:
			 data_switch(data,data+2);
			break;
		case 4:
			 data_switch(data,data+3);
			 data_switch(data+1,data+2);
			 break;
		default:break;
	}
}
