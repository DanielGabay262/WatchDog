/*
dev: Daniel Gabay
rev: Vitali
status: approved
date: 20.1.22
*/

#include <stdlib.h> /*malloc*/
#include <assert.h> /*assert*/

#include "scheduler.h"



#define ON 1
#define OFF 0
#define CLEAR_FLAG 2


int FindUID(const void *data, const void *param)
{
	task_t *task = (task_t *)data;
	ilrd_uid_t *uid_to_check = (ilrd_uid_t *)param;
	
	return (UIDIsSame(*uid_to_check, TaskGetUid(task)));
	 
}





/******************************************************************************************************************************************
                                        				create
******************************************************************************************************************************************/
scheduler_t *SchedulerCreate()
{
	scheduler_t *scheduler = (scheduler_t *)malloc(sizeof(scheduler_t));
	if (NULL == scheduler)
	{
		return NULL;
	}
	scheduler -> queue = PriorityQueueCreate(TaskCmp);
	if (NULL == scheduler -> queue)
	{
		free(scheduler);
		return NULL;
	}
	scheduler -> task = NULL;
	scheduler -> flag = ON;
	return scheduler;
}




/*****************************************************************************************************************************************
                                        				Destroy
*****************************************************************************************************************************************/
void SchedulerDestroy(scheduler_t *scheduler)
{
	assert(scheduler);
	
	SchedulerClear(scheduler);
	PriorityQueueDestroy(scheduler -> queue);
	scheduler -> queue = NULL;
	if (NULL != scheduler -> task)
	{
		TaskDestroy(scheduler -> task);
		scheduler -> task = NULL;
	} 
	free(scheduler);
}





/*****************************************************************************************************************************************
                                        				AddTask
*****************************************************************************************************************************************/
ilrd_uid_t SchedulerAddTask(scheduler_t *scheduler, size_t time_interval, int(*task_func)(void *param), void *param)
{
	task_t *new_task = NULL;
	
	assert(scheduler);
	assert(task_func);
	
	new_task = TaskCreate(time_interval, task_func, param);
	if (new_task == NULL)
	{
		return g_BadUid;
	}
	
	if (PriorityQueueEnqueue(scheduler -> queue, new_task) == 1)
	{
		TaskDestroy(new_task);
		return g_BadUid;
	}
	
	return (TaskGetUid(new_task));
	
}





/*****************************************************************************************************************************************
                                        				RemoveTask
*****************************************************************************************************************************************/
int SchedulerRemoveTask(scheduler_t *scheduler, ilrd_uid_t uid)
{
	task_t *return_task = NULL;
	assert(scheduler);
	
	return_task = (task_t *)PriorityQueueErase(scheduler -> queue, FindUID, &uid);
	if (NULL != return_task)
	{
		TaskDestroy(return_task);
		return_task = NULL;
		return 0;
	}
	return 1;
		
}





/****************************************************************************************************************************************
                                        				IsEmpty
****************************************************************************************************************************************/
int SchedulerIsEmpty(scheduler_t *scheduler)
{
	assert(scheduler);
	
	return (PriorityQueueIsEmpty(scheduler -> queue) && (NULL == scheduler -> task));
}






/****************************************************************************************************************************************
                                        				Size
****************************************************************************************************************************************/
size_t SchedulerSize(scheduler_t *scheduler)
{
	assert(scheduler);
	if (NULL != scheduler -> task)
	{
		return (PriorityQueueSize(scheduler -> queue) + 1);
	}
	return PriorityQueueSize(scheduler -> queue);
}








/****************************************************************************************************************************************
                                        				Clear
****************************************************************************************************************************************/
void SchedulerClear(scheduler_t *scheduler)
{
	task_t *return_task = NULL;
	
	assert(scheduler);
	
	if (NULL != scheduler -> task)
	{
		scheduler -> flag = CLEAR_FLAG;
	}
	
	while (PriorityQueueIsEmpty(scheduler -> queue) != 1)
	{
		return_task = (task_t *)PriorityQueueDequeue(scheduler -> queue);
		TaskDestroy(return_task);
		return_task = NULL;
	}
}





/****************************************************************************************************************************************
                                        				Run
****************************************************************************************************************************************/
int SchedulerRun(scheduler_t *scheduler)
{
	time_t time_till_task = 0;
	
	int return_run = 0;
	
	assert(scheduler);
	
	scheduler -> flag = ON;
	
	while (scheduler -> flag == ON)
	{
		if (SchedulerIsEmpty(scheduler) == 1)
		{
			return 1;
		}
		
		scheduler -> task = (task_t *)PriorityQueueDequeue(scheduler -> queue);
		time_till_task = TaskGetExeTime(scheduler -> task) - time(NULL);
		
		while (time_till_task > 0)
		{
			sleep(time_till_task);
			time_till_task = TaskGetExeTime(scheduler -> task) - time(NULL);
		}
		return_run = TaskRun(scheduler -> task);
		if (return_run == 0)
		{
			TaskUpdateTimeToRun(scheduler -> task);
			if (PriorityQueueEnqueue(scheduler -> queue, scheduler -> task) == 1)
			{
				TaskDestroy(scheduler -> task);
				scheduler -> task = NULL;
				return 2;
			}
			scheduler -> task = NULL;
		}
		
		else if (return_run == 1 || scheduler -> flag == CLEAR_FLAG)
		{
			TaskDestroy(scheduler -> task);
			scheduler -> task = NULL;
		}	
	}
	return 0;
}




/****************************************************************************************************************************************
                                        				Stop
****************************************************************************************************************************************/
void SchedulerStop(scheduler_t *scheduler)
{
	assert(scheduler);
	scheduler -> flag = OFF;
}















