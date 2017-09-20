#ifndef __MID_DBG_H__
#define __MID_DBG_H__

#include <stdarg.h>

void dbg_init(void);
void dbg_hex(unsigned char*str,unsigned char len);


void dbg_string(const char*fmt,...);
void vp_dbg_string(const char*fmt,va_list vp,...);


/* Exported macro ------------------------------------------------------------*/
#ifdef  USE_FULL_ASSERT

/**
  * @brief  The assert_param macro is used for function's parameters check.
  * @param expr: If expr is false, it calls assert_failed function
  *   which reports the name of the source file and the source
  *   line number of the call that failed.
  *   If expr is true, it returns no value.
  * @retval : None
  */
#define assert_param(expr) ((expr) ? (void)0 : assert_failed((uint8_t *)__FILE__, __LINE__))
/* Exported functions ------------------------------------------------------- */
void assert_failed(uint8_t* file, uint32_t line);
#else
#define assert_param(expr) ((void)0)
#endif /* USE_FULL_ASSERT */


#endif
