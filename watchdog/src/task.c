/*
dev: Daniel Gabay
rev: Vitali
status: approved
date: 20.1.22
*/


#include <stdlib.h> /*malloc*/
#include <assert.h> /*assert*/

#include "task.h"



struct task
{
	ilrd_uid_t uid;
	time_t time_exe;
	size_t time_interval;
	int(*task_func)(void *param);
	void *param;
};



/******************************************************************************************************************************************
                                        				create
                                        				*******************************************************************************************************************************************/
task_t *TaskCreate(size_t time_interval, int(*task_func)(void *param), void *param)
{
	task_t *task = NULL;
	
	assert(task_func);
	
	task = (task_t *)malloc(sizeof(task_t));
	if (NULL == task)
	{
		return NULL;
	}
	task -> uid = UIDCreate();
	task -> time_interval = time_interval;
	task -> task_func = task_func;
	task -> param = param;
	TaskUpdateTimeToRun(task);
	return task;
}



/*****************************************************************************************************************************************
                                        				Destroy
*****************************************************************************************************************************************/
void TaskDestroy(task_t *task)
{
	assert(task);
	free(task);
	task = NULL;
}




/****************************************************************************************************************************************
                                        				Run
****************************************************************************************************************************************/
int TaskRun(task_t *task)
{
	assert(task);
	return (task -> task_func(task -> param));
}




/****************************************************************************************************************************************
                                        				GetExeTime
****************************************************************************************************************************************/
time_t TaskGetExeTime(const task_t *task)
{
	assert(task);
	return (task -> time_exe);
}






/****************************************************************************************************************************************
                                        				GetUid
****************************************************************************************************************************************/
ilrd_uid_t TaskGetUid(const task_t *task)
{
	assert(task);
	return (task -> uid);
}






/****************************************************************************************************************************************
                                        				UpdateTimeToRun
****************************************************************************************************************************************/
void TaskUpdateTimeToRun(task_t *task)
{
	assert(task);
	task -> time_exe = time(NULL) + task -> time_interval;
}






/****************************************************************************************************************************************
                                        				Cmp
****************************************************************************************************************************************/
int TaskCmp(const void *task1, const void *task2)
{
	
	task_t *task_1 = NULL;
	task_t *task_2 = NULL;
	
	assert(task1);
	assert(task2);
	
	task_1 = (task_t *)task1;
	task_2 = (task_t *)task2;
	
	return (task_1 -> time_exe - task_2 -> time_exe);
}












