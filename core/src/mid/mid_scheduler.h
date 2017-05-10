

#ifndef __MID_SCHEDULER_H_
#define __MID_SCHEDULER_H_

#include "comm_typedef.h"

/* ------ Public data type declarations ----------------------------*/

typedef void (* pTask)(void);

/* ------ Public function prototypes -------------------------------*/

/* Core scheduler functions */
extern void MidSchInit(void);	/*任务调度初始化*/
extern void MidSchStart(void);	/*启动任务调度，主要是启动时钟*/
extern U16  MidSchAddTask(const pTask task,const U16 period);/*添加一个任务*/
extern void MidSchDeleteTask(const pTask task);/*删除指定的任务*/
extern void MidSchDispatchTasks(void);/*任务调度处理*/
extern U16  MidSchFindTask(const pTask task);

#endif      /* __MID_SCHEDULER_H_ */
