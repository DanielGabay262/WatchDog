#ifndef PRIORITY_QUEUE_H
#define PRIORITY_QUEUE_H

#include <stddef.h> /*size_t*/

#include "sorted_list.h"

typedef struct priority_queue priority_queue_t;




/*************************************************************
DESCRIPTION
	create a priority queue.
ARGUMENTS
	cmp - pointer to function.
RETURN VALUE
	in success - pointer to the new priority queue.
	in failure - NULL.
complexity - o(1).
*************************************************************/
priority_queue_t *PriorityQueueCreate(int(*cmp)(const void* data1, const void* data2));



/*************************************************************************************************************************
DESCRIPTION
	free the priority queue.
ARGUMENTS
	queue - pointer to priority queue.
RETURN VALUE
	none.
complexity - o(n).
************************************************************************************************************************/
void PriorityQueueDestroy(priority_queue_t *queue);


/*************************************************************************************************************************
DESCRIPTION
	add data to queue.
ARGUMENTS
	queue - pointer to priority queue.
	data - pointer to data.
RETURN VALUE
	in success - 0.
	in failure - 1.
complexity - o(n).
***********************************************************************************************************/
int PriorityQueueEnqueue(priority_queue_t *queue, void *data);


/*************************************************************************************************************************
DESCRIPTION
	remove data from the priority queue.
ARGUMENTS
	queue - pointer to priority queue.
RETURN VALUE
	
complexity - o(n).
***********************************************************************************************************/
void *PriorityQueueDequeue(priority_queue_t *queue);


/*************************************************************************************************************************
DESCRIPTION
	return the size of the queue.
ARGUMENTS
	queue - pointer to priority queue.
RETURN VALUE
	number of element in the queue.
complexity - o(n).
***********************************************************************************************************/
size_t PriorityQueueSize(const priority_queue_t *queue);


/*************************************************************************************************************************
DESCRIPTION
	check if the queue is empry.
ARGUMENTS
	queue - pointer to priority queue.
RETURN VALUE
	1 - empty.
	0 - not empty.
complexity - o(1).
***********************************************************************************************************/
int PriorityQueueIsEmpty(const priority_queue_t *queue); 



/*************************************************************************************************************************
DESCRIPTION
	clear the queue.
ARGUMENTS
	queue - pointer to priority queue.
RETURN VALUE
	none.
complexity - o(n).
***********************************************************************************************************/
void PriorityQueueClear(priority_queue_t *queue);


/*************************************************************************************************************************
DESCRIPTION
	rempve element from the queue based on special criterion.
ARGUMENTS
	queue - pointer to priority queue.
	match_func - pointer to function.
	param - pointer to data.
RETURN VALUE
	pointer to the removed data.
complexity - o(n).
***********************************************************************************************************/

void *PriorityQueueErase(priority_queue_t *queue, int (*match_func)(const void *data, const void *param), const void *param);




/*************************************************************************************************************************
DESCRIPTION
	return the data of the first element in the queue.
ARGUMENTS
	queue - pointer to priority queue.
RETURN VALUE
	pointer to the data of the first element in the queue.
complexity 
***************************************************************************************************************************/
void *PriorityQueuePeek(const priority_queue_t *queue);

 	




#endif /*PRIORITY_QUEUE_H*/







