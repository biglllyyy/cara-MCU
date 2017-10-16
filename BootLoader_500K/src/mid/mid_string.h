#ifndef _MID_STRING_H__
#define _MID_STRING_H__

#include "comm_typedef.h"

/*������ת����������궨��*/
#define ISPRINT(ch) ((0x20 <= (ch)) && ((ch) <= 0x7E))
#define ISDIGIT(ch) ('0' <= (ch) && (ch) <= '9')
#define ISXDIGIT(ch) (('0' <= (ch) && (ch) <= '9') || ('A' <= (ch) && (ch) <= 'F'))

/*������ת��ΪASCII���*/
extern void DTOA(U8 chn,U8 temp);
/*�ַ���ת��Ϊʮ������*/
extern S32 ATOX(const CHAR str[]);
/*������ת��ΪASCII���*/
extern void DTOA(U8 chn,U8 temp);
/*ʮ������ת��Ϊ�ַ���*/
extern void XTOA(CHAR str[], S32 value);
U8	 sent_string(U8 chn, U8  *string);
void far_strcpy( CHAR *dest,const CHAR* src);
S16  far_strcmp(const CHAR* s1,const  CHAR* s2);
S16  far_strncmp(const CHAR* s1,const  CHAR* s2, S16 count);
S16  sent_data(U8 chn, U8 *string, U8 len);
void data_copy(U8 *dest,U8 *src,U8 count);
void byte_order_change(U8 *data,U8 len);

/*���ַ���ת��Ϊ10������*/
extern S32 str_to_dec(const U8 *str);
/*ʮ����ת��Ϊ�ַ���*/
extern void dec_to_str(U8 *str,S32 value);

#ifdef WIN32 
int debug_out(char *msg,int first);
#endif

#endif


