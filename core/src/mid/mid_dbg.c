#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

#include "mid_dbg.h"
#include "hal_uart.h"
#include "version.h"

static char dbg_buf[256];

// initialize reserved UART for debugging
void dbg_init(void)
{
	uart_open(UART_DEBUG_CHN,115200, 8, 1, 0);
	dbg_string("Core lib version %d.%d.%d\n", CORE_VER_MAJOR, CORE_VER_MINOR, CORE_VER_REV);
}

#if 1
void dbg_string(const char*fmt,...)
{
	va_list vp;
	
	va_start(vp, fmt);
	vsprintf(dbg_buf, fmt, vp);
	va_end(vp);
	
	uart_write(UART_DEBUG_CHN, (U8*)dbg_buf);
	printf("%s", dbg_buf);
}
#else
void dbg_string(const char*fmt,...)
{
	return ;
}

#endif
void vp_dbg_string(const char*fmt,va_list vp,...)
{
	vsprintf(dbg_buf, fmt, vp);
	va_end(vp);
	
	uart_write(UART_DEBUG_CHN, (U8*)dbg_buf);
	printf("%s", dbg_buf);
}
void dbg_hex(unsigned char*str,unsigned char len)
{
	char temp1,temp2;
	char i=0;
	while(len!=0&&len<=85)
	{
		temp1=((*str)>>4)&0x0F;
		temp2=(*str)&0x0F;
		if(temp1<=9)
			dbg_buf[i++]=temp1+'0';
		else
			dbg_buf[i++]=temp1+('A'-10);
		
		if(temp2<=9)
			dbg_buf[i++]=temp2+'0';
		else
			dbg_buf[i++]=temp2+('A'-10);
		len--;
		dbg_buf[i++]=' ';
		str++;
	}
	dbg_buf[i]='\n';
	dbg_buf[i+1]=0x0;
	uart_write(UART_DEBUG_CHN, (U8*)dbg_buf);
}
#ifdef USE_FULL_ASSERT

/** 
  * @brief  Reports the name of the source file and the source line number
  *   where the assert_param error has occurred.
  * @param file: pointer to the source file name
  * @param line: assert_param error line source number
  * @retval : None
  */
void assert_failed(u8* file, u32 line)
{
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
     dbg_string("Wrong parameters value: file %s on line %d\r\n", file, line);
}
#endif

