/*
dev: Daniel
rev: Idan
status: Approved
date: 15.1.22
*/

#include <stdlib.h> /* malloc */
#include <assert.h> /* assert */

#include "doubly_linked_list.h"

#define SUCCESS 0
#define FAILURE 1
#define YES 1
#define NO 0

struct doubly_linked_list_node
{
    void *data;
    struct doubly_linked_list_node *next;
    struct doubly_linked_list_node *prev;
};

typedef struct doubly_linked_list_node dll_node_t;

struct doubly_linked_list
{
    dll_node_t head;
    dll_node_t tail;
};

dll_node_t *GetIteratorEnd(dll_node_t *iterator)
{
	assert(iterator);
	while (iterator -> next != NULL)
	{
		iterator = iterator -> next;
	}
	return iterator;
}

dll_t *DoublyLinkedListCreate()
{
	dll_t *list = (dll_t *)malloc(sizeof(dll_t));
	if (NULL == list)
	{
		return NULL;
	}
	list -> head.data = NULL;
	list -> head.next = &(list -> tail);
	list -> head.prev = NULL;
	list -> tail.data = NULL;
	list -> tail.next = NULL;
	list -> tail.prev = &(list -> head);
	return list;
}

void DoublyLinkedListDestroy(dll_t *list)
{
	assert(list);
	while (list -> head.next != &(list -> tail))
	{
		DoublyLinkedListRemove(list -> head.next);
	}
	free(list);
}

dll_node_t *DoublyLinkedListRemove(dll_node_t * iterator)
{
	dll_node_t *return_iterator = NULL;
	
	assert(iterator);
	
	return_iterator = iterator -> next;
	iterator -> prev -> next = iterator -> next;
	iterator -> next -> prev = iterator -> prev;
	free(iterator);
	return return_iterator;
}

int DoublyLinkedListIsEmpty(const dll_t *list)
{
	assert(list);
	return (list -> head.next == &(list -> tail));
}

dll_node_t *DoublyLinkedListBegin(const dll_t *list)
{
	assert(list);
	return (list ->head.next);
}

size_t DoublyLinkedListSize(const dll_t *list)
{
	size_t count = 0;
	dll_node_t *ptr = NULL;
	
	assert(list);
	ptr = list -> head.next;
	
	while (ptr != &(list -> tail))
	{
		++count;
		ptr = ptr -> next;
	}
	return count;
}

dll_node_t *DoublyLinkedListEnd(const dll_t *list)
{
	assert(list);
	return (dll_node_t *)(&(list -> tail));
}

dll_node_t *DoublyLinkedListNext(const dll_iterator_t iterator)
{
	assert(iterator);
	return (iterator -> next);
}

dll_node_t *DoublyLinkedListPrev(const dll_iterator_t iterator)
{
	assert(iterator);
	return (iterator -> prev);
}

int DoublyLinkedLisSameIter(const dll_iterator_t iterator1, const dll_iterator_t iterator2)
{
	assert(iterator1);
	assert(iterator2);
	return (iterator1 == iterator2);
}

void *DoublyLinkedListGetData(const dll_iterator_t iterator)
{
	assert(iterator);
	return(iterator -> data);
}

dll_node_t *DoublyLinkedListInsertBefore(const dll_iterator_t iterator, void *data)
{
	dll_node_t *new_node = NULL;
	
	assert(iterator);
	new_node = (dll_node_t *)malloc(sizeof(dll_node_t));
	if (NULL == new_node)
	{
		return (GetIteratorEnd(iterator));
	}
	
	new_node -> next = iterator;
	new_node -> prev = iterator -> prev;
	new_node -> data = data;
	iterator -> prev -> next = new_node;
	iterator -> prev = new_node;
	return new_node;
}


dll_node_t *DoublyLinkedListPushFront(dll_t *list, void *data)
{
	assert(list);
	return (DoublyLinkedListInsertBefore(list -> head.next, data));
}

dll_node_t *DoublyLinkedListPushBack(dll_t *list, void *data)
{
	assert(list);
	return (DoublyLinkedListInsertBefore((&list -> tail), data));
}

void *DoublyLinkedListPopBack(dll_t *list)
{
	void *data = NULL;
	assert(list);
	data = list -> tail.prev -> data;
	DoublyLinkedListRemove(list -> tail.prev);
	return data;
}


void *DoublyLinkedListPopFront(dll_t *list)
{
	void *data = NULL;
	assert(list);
	data = list -> head.next -> data;
	DoublyLinkedListRemove(list -> head.next);
	return data;
}


int DoublyLinkedListForEach(dll_iterator_t from, dll_iterator_t to, int (*action_func)(void *data, void *param), void *param)
{
	int ans = SUCCESS;
	assert(from);
	assert(to);
	assert(action_func);
	
	while (from != to && ans == SUCCESS)
	{
		ans = action_func(from -> data, param);
		from = from -> next;
	}
	if (from == to)
	{
		return SUCCESS;
	}
	return ans;
}


dll_node_t *DoublyLinkedListFind(dll_iterator_t from, dll_iterator_t to, int (*match_func)(const void *data, const void *param),const void *param)
{
	assert(from);
	assert(to);
	assert(match_func);

	while (from != to && match_func(from -> data, param) != YES)
	{
		from = from -> next;
	}
	return from;
}


int DoublyLinkedListMultiFind(dll_iterator_t from, dll_iterator_t to, int (*match_func)(const void *data, const void *param), const void *param, dll_t *out_list)
{
	dll_node_t * check_success = NULL;

	assert(from);
	assert(to);
	assert(match_func);
	assert(out_list);
	
	while (from != to)
	{
		from = DoublyLinkedListFind(from, to, match_func, param);
		if (from != to)
		{
			check_success = DoublyLinkedListPushBack(out_list, from -> data);
			if (check_success -> next == NULL)
			{
				return NO;
			}
		 from = from -> next;
		}
	}
	return YES;
}


dll_node_t *DoublyLinkedListSplice(dll_iterator_t where, dll_iterator_t from, dll_iterator_t to)
{
	dll_node_t *temp = NULL;
	
	assert(where);
	assert(from);
	assert(to);
	
	temp = to -> prev;
	
	from -> prev -> next = to;
	to -> prev = from -> prev;
	where -> prev -> next = from;
	from -> prev = where -> prev;
	where -> prev = temp;
	temp -> next = where;
	return where;
}
