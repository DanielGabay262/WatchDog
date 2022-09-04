#ifndef DOUBLY_LINKED_LIST_H
#define DOUBLY_LINKED_LIST_H

#include <stddef.h> /*size_t*/


typedef struct doubly_linked_list dll_t; 
typedef struct doubly_linked_list_node* dll_iterator_t;

                                                     


/***************************************************************************************************************************************
DESCRIPTION
	create a doubly liked list.
ARGUMENTS
	none
RETURN VALUE
	in sucsses - pointer to the new linked list.
	in failure - NULL.
complexity - o(1).
*/ 
dll_t *DoublyLinkedListCreate();
/***************************************************************************************************************************************/



/***************************************************************************************************************************************
DESCRIPTION
	free the linked list.
ARGUMENTS
	list - pointer to the linked list.
RETURN VALUE
	void.
complexity - o(n).
*/
void DoublyLinkedListDestroy(dll_t *list);
/***************************************************************************************************************************************/



/***************************************************************************************************************************************
DESCRIPTION
	check if the linked list is empty.
ARGUMENTS
	list - pointer to the linked list.
RETURN VALUE
	1 - empty.
	0 - not empty.
complexity - o(1).
*/
int DoublyLinkedListIsEmpty(const dll_t *list);
/***************************************************************************************************************************************/



/****************************************************************************************************************************************
DESCRIPTION
	get the head of the linked list (not dummy).
ARGUMENTS
	list - pionter to linked list.
RETURN VALUE
	pointer to the first iterator.
complexity - o(1).
*/
dll_iterator_t DoublyLinkedListBegin(const dll_t *list);
/***************************************************************************************************************************************/



/***************************************************************************************************************************************
DESCRIPTION
	check the numbers of iterators in a linked list.
ARGUMENTS
	list - pionter to linked list.
RETURN VALUE
	number of iterators.
complexity - o(n).
*/
size_t DoublyLinkedListSize(const dll_t *list);
/***************************************************************************************************************************************/



/***************************************************************************************************************************************
DESCRIPTION
	get the tail of the linked list
ARGUMENTS
	list - pionter to linked list.
RETURN VALUE
	pointer to the last iterator.
complexity - o(1).
*/
dll_iterator_t DoublyLinkedListEnd(const dll_t *list); 
/***************************************************************************************************************************************/



/***************************************************************************************************************************************
DESCRIPTION
	get the next iterator.
ARGUMENTS
	iterator - iterator.
RETURN VALUE
	pointer to the next iterator.
complexity - o(1).
*/
dll_iterator_t DoublyLinkedListNext(const dll_iterator_t iterator);
/***************************************************************************************************************************************/



/***************************************************************************************************************************************
DESCRIPTION
	get the previous iterator.
ARGUMENTS
	iterator - iterator.
RETURN VALUE
	pointer to the previous iterator.
complexity - o(1).
*/
dll_iterator_t DoublyLinkedListPrev(const dll_iterator_t iterator);
/***************************************************************************************************************************************/



/***************************************************************************************************************************************
DESCRIPTION
	checks if the 2 iterators are equal. 
ARGUMENTS
	iterator1 - iterator.
	iterator2 - iterator.
RETURN VALUE
	1 - equal.
	0 - not equal.
complexity - o(1).
*/
int DoublyLinkedLisSameIter(const dll_iterator_t iterator1,const dll_iterator_t iterator2);
/***************************************************************************************************************************************/



/***************************************************************************************************************************************
DESCRIPTION
	get data from iterator.
ARGUMENTS
	iterator - iterator.
RETURN VALUE
	pointer to data.
complexity - o(1).
*/
void *DoublyLinkedListGetData(const dll_iterator_t iterator);
/***************************************************************************************************************************************/



/***************************************************************************************************************************************
DESCRIPTION
	insert an iterator.
ARGUMENTS
	iterator - iterator. the insert will be before the iterator .
	data - data to insert.
RETURN VALUE
	in sucsses - pointer to the iterator.
	in failure - pointer to iterator end.
complexity - o(1).
*/
dll_iterator_t DoublyLinkedListInsertBefore(const dll_iterator_t iterator, void *data);
/***************************************************************************************************************************************/



/***************************************************************************************************************************************
DESCRIPTION
	insert an iterator to the head of the linked list.
ARGUMENTS
	list - pointer to the linked list.
	data - data to insert.
RETURN VALUE
	in sucsses - pointer to the iterator.
	in failure - pointer to iterator end.
complexity - o(1).
*/
dll_iterator_t DoublyLinkedListPushFront(dll_t *list, void *data);
/***************************************************************************************************************************************/



/***************************************************************************************************************************************
DESCRIPTION
	insert an iterator to the tail of the linked list.
ARGUMENTS
	list - pointer to the linked list.
	data - data to insert.
RETURN VALUE
	in sucsses - pointer to the iterator.
	in failure - pointer to iterator end.
complexity - o(1).
*/
dll_iterator_t DoublyLinkedListPushBack(dll_t *list, void *data);
/***************************************************************************************************************************************/



/***************************************************************************************************************************************
DESCRIPTION
	remove iterator. 
ARGUMENTS
	iterator - iterator to remove.
RETURN VALUE
	return the next iterator.
complexity - o(1).
*/
dll_iterator_t DoublyLinkedListRemove(dll_iterator_t iterator);
/***************************************************************************************************************************************/



/***************************************************************************************************************************************
DESCRIPTION
	remove thr tail iterator. 
ARGUMENTS
	list - pointer to linked list.
RETURN VALUE
	return the data that was in the poped iterator.
complexity - o(1).
*/
void *DoublyLinkedListPopBack(dll_t *list);
/***************************************************************************************************************************************/


/***************************************************************************************************************************************
DESCRIPTION
	remove thr thead iterator. 
ARGUMENTS
	list - pointer to linked list.
RETURN VALUE
	return the data that was in the poped iterator.
complexity - o(1).
*/
void *DoublyLinkedListPopFront(dll_t *list);
/***************************************************************************************************************************************/



/***************************************************************************************************************************************
DESCRIPTION
	for each iterator the function will do action_func. the function runs between "from" and "to", not including "to". 
ARGUMENTS
	from - firts iterator to check.
	to - last iterator to check.
	action_func - the action function.
	param - pointer to data.
RETURN VALUE
	in sucsses - 0.
	in failure - not 0.
complexity - o(n).
*/
int DoublyLinkedListForEach(dll_iterator_t from, dll_iterator_t to, int (*action_func)(void *data, void *param), void *param);
/***************************************************************************************************************************************/



/***************************************************************************************************************************************
DESCRIPTION
	find data in linked list, checking between "from" and "to", not including "to".
ARGUMENTS
	from - firts iterator to check.
	to size - last iterator to check.
	match_func - function that find the data in the iterators.
	param - the data to find in the iterators.
RETURN VALUE
	in sucsses - pointer to the iterator.
	in failure - pointer to iterator to.
complexity - o(n).
*/
dll_iterator_t DoublyLinkedListFind(dll_iterator_t from, dll_iterator_t to, int (*match_func)(const void *data, const void *param),const void *param);
/***************************************************************************************************************************************/



/***************************************************************************************************************************************
DESCRIPTION
	find data in linked list, checking between "from" and "to", not including "to".
ARGUMENTS
	from - firts iterator to check.
	to size - last iterator to check.
	match_func - function that find the data in the iterators.
	param - the data to find in the iterators.
	out_list - pointer to linked list to store the iterators that found.
RETURN VALUE
	in sucsses - 0.
	in failure - not 0.
complexity - o(n).
*/
int DoublyLinkedListMultiFind(dll_iterator_t from, dll_iterator_t to, int (*match_func)(const void *data, const void *param), const void *param, dll_t *out_list);
/***************************************************************************************************************************************/



/***************************************************************************************************************************************
DESCRIPTION
	connect iterators. where will connect last.
ARGUMENTS
	where - pointer to iterator.
	from - pointer to first iterator.
	to - pointer to last iterator.
RETURN VALUE
	the last copied iterator which is one before to.
complexity - o(n).
*/
dll_iterator_t DoublyLinkedListSplice(dll_iterator_t where, dll_iterator_t from, dll_iterator_t to);
/***************************************************************************************************************************************/




#endif /*DOUBLY_LINKED_LIST_H*/
