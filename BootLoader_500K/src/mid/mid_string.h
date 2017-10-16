#ifndef _MID_STRING_H__
#define _MID_STRING_H__

#include "comm_typedef.h"

/*与数制转换相关三个宏定义*/
#define ISPRINT(ch) ((0x20 <= (ch)) && ((ch) <= 0x7E))
#define ISDIGIT(ch) ('0' <= (ch) && (ch) <= '9')
#define ISXDIGIT(ch) (('0' <= (ch) && (ch) <= '9') || ('A' <= (ch) && (ch) <= 'F'))

/*将数据转换为ASCII输出*/
extern void DTOA(U8 chn,U8 temp);
/*字符串转换为十六进制*/
extern S32 ATOX(const CHAR str[]);
/*将数据转换为ASCII输出*/
extern void DTOA(U8 chn,U8 temp);
/*十六进制转换为字符串*/
extern void XTOA(CHAR str[], S32 value);
U8	 sent_string(U8 chn, U8  *string);
void far_strcpy( CHAR *dest,const CHAR* src);
S16  far_strcmp(const CHAR* s1,const  CHAR* s2);
S16  far_strncmp(const CHAR* s1,const  CHAR* s2, S16 count);
S16  sent_data(U8 chn, U8 *string, U8 len);
void data_copy(U8 *dest,U8 *src,U8 count);
void byte_order_change(U8 *data,U8 len);

/*将字符串转换为10进制数*/
extern S32 str_to_dec(const U8 *str);
/*十进制转换为字符串*/
extern void dec_to_str(U8 *str,S32 value);

#ifdef WIN32 
int debug_out(char *msg,int first);
#endif

#endif


