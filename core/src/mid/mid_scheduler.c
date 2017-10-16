#include <stdlib.h>
#include <string.h>
#include "mid_scheduler.h"
#include "hal_reload_timer.h"

/* The array of tasks */
//#define ABS_TIME16(GOAL_TIME,CUR_TIME)    ((GOAL_TIME) >= (CUR_TIME))?((GOAL_TIME)-(CUR_TIME)):(0xFFFF-(CUR_TIME)+(GOAL_TIME)+1)

#define  MAX_TICK_VALUE		0xFFFF
typedef struct
{

    pTask Task;      /* Pointer to the task function */

    U32 Timeout;      /* Timeout (ticks) */

    U16 Period;       /* Interval (ticks) between subsequent runs */

} TASK_TYPE;

/* ------ Public constants -----------------------------------------

The maximum number of tasks required at any one time
during the execution of the program
MUST BE ADJUSTED FOR EACH NEW PROJECT ---------------------------*/

#define MAX_TASKS  (50u)

/*TASK_TYPE s_task.Tasks[MAX_TASKS];*/

static struct
{
    U16		TasksCount;
	U16     OncesMaxTasks;
    TASK_TYPE Tasks[MAX_TASKS];
} s_task;


void MidSchInit(void)
{
    TickInit();                     /* tick = 10ms */
    s_task.TasksCount = 0;
    memset(&s_task.Tasks, 0, sizeof(s_task.Tasks));
}


void MidSchStart(void)
{
    TickStart();
}

U16 MidSchFindTask(const pTask task)
{
    U16 index;
    U16 ret = 0xff;
    /* First find a gap in the array (if there is one) */
    for(index = 0u; index < s_task.TasksCount; index++)
    {
        /* if find that task, return the index */
        if(s_task.Tasks[index].Task == task)
        {
            ret = index;
            break;
        }
    }
    return ret;
}


U16 MidSchAddTask(const pTask task, const U16 period)
{
    U16 index, top;

    do
    {
        /* if already added, return success */
        index = MidSchFindTask(task);
        if(index != 0xFFu)  /*如果该任务存在*/
        {
            index = 0x00u;   /*已经成功添加 任务*/
            break;
        }

        /* if no space, return failed */
        if(s_task.TasksCount == MAX_TASKS)
        {
            index = 0xFFu;           /*添加任务失败*/
            break;
        }
        /* add a new task at top */
        top = s_task.TasksCount++;
        s_task.Tasks[top].Task = task;
        s_task.Tasks[top].Timeout = GetTick();
        s_task.Tasks[top].Period = period / 10;
        /* indicate add success */
        index = 0x00u;
    }
    while(0);
    return index;
}

void MidSchDeleteTask(const pTask task)
{
    U16 index, top;

    do
    {
        /* if not added, do nothing */
        index = MidSchFindTask(task);
        if(index == 0xFFu)
        {
            break;
        }              /*任务不存在*/
        /* replace top task to this task */
        top = --s_task.TasksCount;
        if(index != top)
        {
            s_task.Tasks[index].Task = s_task.Tasks[top].Task;
            s_task.Tasks[index].Timeout = s_task.Tasks[top].Timeout;
            s_task.Tasks[index].Period = s_task.Tasks[top].Period;
        }
    }
    while(0);
}

void MidSchDispatchTasks(void)
{
    U16 index=0;
	U16 count=0;

    /* Dispatches (runs) the next task (if one is ready) */
    for (index = 0; index < s_task.TasksCount; index++)
    {
        /* HalTimerGetTick() INCREASE AT 10MS INTERVAL, OVERFLOW EVERY 655.36 SECONDS at 16bit hal timer */
        if(hal_timer_passed(s_task.Tasks[index].Timeout) >= s_task.Tasks[index].Period)
        {
            s_task.Tasks[index].Timeout = GetTick();
            s_task.Tasks[index].Task();                  /* Run the task  */
			count++;
        }
    }
	if(count > s_task.OncesMaxTasks)
	{
		s_task.OncesMaxTasks = count;
	}
	count = 0;
}
