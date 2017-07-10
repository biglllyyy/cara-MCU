#include <stdarg.h>
#include "mid_dbg.h"


void dbg_printf(const char*fmt,...)
{
#ifdef __DEBUG__ 
	va_list vp;
	
	va_start(vp, fmt);
	vp_dbg_string(fmt, vp); 
#endif
}

void rel_printf(const char*fmt,...)
{
	va_list vp;
	
	va_start(vp, fmt);
	vp_dbg_string(fmt, vp); 
}

