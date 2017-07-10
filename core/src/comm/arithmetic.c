#include <stdlib.h>
#include  "arithmetic.h"
U8  find_max_value(U8 *data,U8 len)
{
	U8 index = 0;
	U8 max = 0;

	if(data != NULL)
	{
		for(index=0;index<len;index++)
		{
			if(data[index]>max)
			{
				max = data[index];
			}
		}
	}

	return max;
}

U16 linear_interpolation(const T_LINEAR_INTERPOLATION *tbl,U16 vector,U8 src_order,U16 goal)
{
	//deault src up order
	U8  low = 0;
	U8  high = vector-1;
	U8  target_range = 0;		//range [ );
	U16  mid_value = 0;
	U16  ret = 0;

	//up  order
	if(SOURCE_UP_ORDER == src_order)
	{
		if(goal <= tbl[0].src)
		{
			ret = tbl[0].target;
		}
		else if(goal >= tbl[high].src)
		{
			ret = tbl[high].target;
		}
		else
		{
			do 
			{
				target_range = (low+high)/2;
				mid_value = tbl[target_range].src;
				if(goal == mid_value)
				{
					ret = tbl[target_range].target;
					return (ret);
				}
				else if(goal > mid_value)
				{
					low = target_range;
				}
				else
				{
					high = target_range;
				}
			}while (high-low != 1);
			ret = (U16)(((S32)(goal - tbl[low].src)* (S16)(tbl[high].target-tbl[low].target))/(S16)(tbl[high].src-tbl[low].src) + tbl[low].target);
		}


	}
	else  //down order
	{
		if(goal >= tbl[0].src)
		{
			ret = tbl[0].target;
		}
		else if(goal <= tbl[high].src)
		{
			ret = tbl[high].target;
		}
		else
		{
			do 
			{
				target_range = (low+high)/2;
				mid_value = tbl[target_range].src;
				if(goal == mid_value)
				{
					ret = tbl[target_range].target;
					return (ret);
				}
				else if(goal > mid_value)
				{
					high = target_range;
				}
				else
				{
					low = target_range;
				}
			}while (high-low != 1);
			ret = (U16)(((S32)(goal - tbl[low].src)* (S16)(tbl[high].target-tbl[low].target))/(S16)(tbl[high].src-tbl[low].src) + tbl[low].target);
		}
	}

	return ret;
}
