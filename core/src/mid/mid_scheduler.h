

#ifndef __MID_SCHEDULER_H_
#define __MID_SCHEDULER_H_

#include "comm_typedef.h"

/* ------ Public data type declarations ----------------------------*/

typedef void (* pTask)(void);

/* ------ Public function prototypes -------------------------------*/

/* Core scheduler functions */
extern void MidSchInit(void);	/*������ȳ�ʼ��*/
extern void MidSchStart(void);	/*����������ȣ���Ҫ������ʱ��*/
extern U16  MidSchAddTask(const pTask task,const U16 period);/*���һ������*/
extern void MidSchDeleteTask(const pTask task);/*ɾ��ָ��������*/
extern void MidSchDispatchTasks(void);/*������ȴ���*/
extern U16  MidSchFindTask(const pTask task);

#endif      /* __MID_SCHEDULER_H_ */
