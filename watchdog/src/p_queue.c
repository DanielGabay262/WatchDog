#include <assert.h> /*assert*/
#include <stdlib.h> /*malloc */


#include "sorted_list.h"
#include "p_queue.h"


struct priority_queue
{
	sorted_list_t *list;
	int(*cmp)(const void *, const void *);
};

priority_queue_t *PriorityQueueCreate(int(*cmp)(const void* data1, const void* data2))
{
	priority_queue_t *queue;
	
	assert(NULL != cmp);
	
	queue = (priority_queue_t *)malloc(sizeof(priority_queue_t));
	
	if (NULL == queue)
	{
		return NULL;
	}
	
	queue->list = SortedLinkedListCreate(cmp);
	queue->cmp = cmp;
	
	if(NULL == queue->list)
	{
		free(queue);
		return NULL;
	}
	return queue;
}

void PriorityQueueDestroy(priority_queue_t *queue)
{
	assert(NULL != queue);
	
	SortedLinkedListDestroy(queue->list);
	
	queue->list = NULL;
	
	free(queue);
}
	
int PriorityQueueEnqueue(priority_queue_t *queue, void *data)
{
	sorted_list_iterator_t tail_node;
	sorted_list_iterator_t inserted_node;
	
	assert(NULL != queue);
	
	inserted_node = SortedLinkedListInsert(queue->list, data);
	
	tail_node = SortedLinkedListEnd(queue->list);
	
	return SortedLinkedLisSameIter(tail_node, inserted_node);
}

void *PriorityQueueDequeue(priority_queue_t *queue)
{
	assert(NULL != queue);
	
	return SortedLinkedListPopFront(queue->list);
}

size_t PriorityQueueSize(const priority_queue_t *queue)
{
	assert(NULL != queue);
	
	return SortedLinkedListSize(queue->list);
}

int PriorityQueueIsEmpty(const priority_queue_t *queue)
{
	assert(NULL != queue);
	
	return SortedLinkedListIsEmpty(queue->list);
}

void PriorityQueueClear(priority_queue_t *queue)
{
	size_t size = 0; 
	
	assert(NULL != queue);
	
	size = PriorityQueueSize(queue);
	
	while(0 != size)
	{
		PriorityQueueDequeue(queue);
		--size;
	}	
}

void *PriorityQueueErase(priority_queue_t *queue, int (*match_func)(const void *data, const void *param), const void *param)
{
	sorted_list_iterator_t tail_node;
	sorted_list_iterator_t head_node;
	sorted_list_iterator_t found_node;
	
	void *data = NULL;
	
	assert(NULL != queue);
	assert(NULL != match_func);
	
	tail_node = SortedLinkedListEnd(queue->list);
	head_node = SortedLinkedListBegin(queue->list);
	
	found_node = SortedLinkedListFindIf(head_node, tail_node, match_func, param);
	
	data = SortedLinkedListGetData(found_node);
	
	SortedLinkedListRemove(found_node);
	
	return data;
}

void *PriorityQueuePeek(const priority_queue_t *queue)
{
	assert(NULL != queue);
	
	return SortedLinkedListGetData(SortedLinkedListBegin(queue->list));
}

