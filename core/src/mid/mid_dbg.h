#ifndef __MID_DBG_H__
#define __MID_DBG_H__

#include <stdarg.h>

void dbg_init(void);
void dbg_hex(unsigned char*str,unsigned char len);


void dbg_string(const char*fmt,...);
void vp_dbg_string(const char*fmt,va_list vp,...);

#endif
