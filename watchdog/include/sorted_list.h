	
#ifndef SORTED_LINKED_LIST_H
#define SORTED_LINKED_LIST_H

#include "doubly_linked_list.h"


typedef struct sorted_list sorted_list_t;
typedef struct sorted_list_iterator sorted_list_iterator_t;

struct sorted_list_iterator 
{
    dll_iterator_t doubly_iter;
    #ifndef NDEBUG
    sorted_list_t *list;/*gdb*/
    #endif
    
};/*sorted_list_iterator_t*/



/*************************************************************************************************************************
DESCRIPTION
	create a sorted list.
ARGUMENTS
	cmp - compare function.
RETURN VALUE
	in sucsses - pointer to the new sorted list.
	in failure - NULL.
complexity - o(1).
***********************************************************************************************************/
sorted_list_t *SortedLinkedListCreate(int(*cmp)(const void* data1, const void* data2));



/*************************************************************************************************************************
DESCRIPTION
	free the sorted list.
ARGUMENTS
	list - pointer to the sorted list.
RETURN VALUE
	void.
complexity - o(n).
************************************************************************************************************************/
void SortedLinkedListDestroy(sorted_list_t *list);


/*************************************************************************************************************************
DESCRIPTION
	check if the sorted list is empty.
ARGUMENTS
	list - pointer to the sorted list.
RETURN VALUE
	1 - empty.
	0 - not empty.
complexity - o(1).
***********************************************************************************************************/
int SortedLinkedListIsEmpty(const sorted_list_t *list);


/*************************************************************************************************************************
DESCRIPTION
	get the head of the sorted list (not dummy).
ARGUMENTS
	list - pointer to sorted list.
RETURN VALUE
	pointer to the first iterator.
complexity - o(1).
***********************************************************************************************************/
sorted_list_iterator_t SortedLinkedListBegin(const sorted_list_t *list);


/*************************************************************************************************************************
DESCRIPTION
	check the numbers of iterators in a sorted list.
ARGUMENTS
	list - pionter to sorted list.
RETURN VALUE
	number of iterators.
complexity - o(n). 
***********************************************************************************************************/
size_t SortedLinkedListSize(const sorted_list_t *list);


/*************************************************************************************************************************
DESCRIPTION
	get the tail of the sorted list
ARGUMENTS
	list - pionter to sorted list.
RETURN VALUE
	pointer to the last iterator.
complexity - o(1).
***********************************************************************************************************/
sorted_list_iterator_t SortedLinkedListEnd(const sorted_list_t *list); 



/*************************************************************************************************************************
DESCRIPTION
	get the next iterator.
ARGUMENTS
	iterator - iterator.
RETURN VALUE
	pointer to the next iterator.
complexity - o(1).
***********************************************************************************************************/
sorted_list_iterator_t SortedLinkedListNext(const sorted_list_iterator_t iterator);


/*************************************************************************************************************************
DESCRIPTION
	get the previous iterator.
ARGUMENTS
	iterator - iterator.
RETURN VALUE
	pointer to the previous iterator.
complexity - o(1).
***********************************************************************************************************/

sorted_list_iterator_t SortedLinkedListPrev(const sorted_list_iterator_t iterator);



/*************************************************************************************************************************
DESCRIPTION
	insert an iterator.
ARGUMENTS
	list - pionter to sorted list.
	data - data to insert.
RETURN VALUE
	in sucsses - pointer to the iterator.
	in failure - pointer to iterator end.
complexity - o(n).
***************************************************************************************************************************/
sorted_list_iterator_t SortedLinkedListInsert(sorted_list_t *list, void *data);



/*************************************************************************************************************************
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
***************************************************************************************************************************/
int SortedLinkedListForEach(sorted_list_iterator_t from, sorted_list_iterator_t to, int (*action_func)(void *data, void *param), void *param);




/*************************************************************************************************************************
DESCRIPTION
	checks if the 2 iterators are equal. 
ARGUMENTS
	iterator1 - iterator.
	iterator2 - iterator.
RETURN VALUE
	1 - equal.
	0 - not equal.
complexity - o(1).
***************************************************************************************************************************/
int SortedLinkedLisSameIter(const sorted_list_iterator_t iterator1,const sorted_list_iterator_t iterator2);


/*************************************************************************************************************************
DESCRIPTION
	get data from iterator.
ARGUMENTS
	iterator - iterator.
RETURN VALUE
	pointer to data.
complexity - o(1).
***************************************************************************************************************************/
void *SortedLinkedListGetData(const sorted_list_iterator_t iterator);



/*************************************************************************************************************************
DESCRIPTION
	remove iterator. 
ARGUMENTS
	iterator - iterator to remove.
RETURN VALUE
	return the next iterator.
complexity - o(1).
***************************************************************************************************************************/
sorted_list_iterator_t SortedLinkedListRemove(sorted_list_iterator_t iterator);



/*************************************************************************************************************************
DESCRIPTION
	remove thr last iterator. 
ARGUMENTS
	list - pointer to sorted list.
RETURN VALUE
	return the data that was in the poped iterator.
complexity - o(1).
***************************************************************************************************************************/
void *SortedLinkedListPopBack(sorted_list_t *list);



/*************************************************************************************************************************
DESCRIPTION
	remove the first iterator. 
ARGUMENTS
	list - pointer to sorted list.
RETURN VALUE
	return the data that was in the poped iterator.
complexity - o(1).
***************************************************************************************************************************/
void *SortedLinkedListPopFront(sorted_list_t *list);



/*************************************************************************************************************************
DESCRIPTION
	find data in sorted list, checking between "from" and "to", not including "to".
ARGUMENTS
	from - firts iterator to check.
	to size - last iterator to check.
	match_func - function that find the data in the iterators.
	param - the data to find in the iterators.
RETURN VALUE
	in sucsses - the iterator found.
	in failure - the iterator to.
complexity - o(n).

***************************************************************************************************************************/
sorted_list_iterator_t SortedLinkedListFindIf(sorted_list_iterator_t from, sorted_list_iterator_t to, int (*match_func)(const void *data, const void *param),const void *param); 										




/*************************************************************************************************************************
DESCRIPTION
	merge between 2 sorted lists.
ARGUMENTS
	dest - pointer to destination sort list.
	src - pointer to source sort list.
RETURN VALUE
	void.
complexity - o(n).

***************************************************************************************************************************/
void SortedLinkedListMerge(sorted_list_t *dest, sorted_list_t *src);



/*************************************************************************************************************************
DESCRIPTION
	find data in sorted list base on the cmp function, checking between "from" and "to", not including "to".
ARGUMENTS
	list - pointer to sorted list.
	from - firts iterator to check.
	to size - last iterator to check.
	param - the data to find in the iterators.
RETURN VALUE
	in sucsses - the iterator found.
	in failure - the iterator to.
complexity - o(n).
***************************************************************************************************************************/
sorted_list_iterator_t SortedLinkedListFind(sorted_list_t *list, sorted_list_iterator_t from, sorted_list_iterator_t to, const void *data); 	




#endif /*SORTED_LINKED_LIST_H*/
















