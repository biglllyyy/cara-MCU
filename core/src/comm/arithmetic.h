#ifndef   __ARITHMETIC_H__
#define	  __ARITHMETIC_H__

#include "comm_typedef.h"

#define   SOURCE_UP_ORDER		0
#define   SOURCE_DOWN_ORDER		1
U8  find_max_value(U8 *data,U8 len);
typedef struct{
	U16 src;
	U16 target;
}T_LINEAR_INTERPOLATION;

U16 linear_interpolation(
	const T_LINEAR_INTERPOLATION *tbl,
	U16 vector,
	U8 src_order,
	U16 goal
);
#endif
