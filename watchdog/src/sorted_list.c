/*
dev: Daniel Gabay
rev: Yaniv
status: approved
date: 17.1.22
*/


#include <assert.h>
#include <stdlib.h>

#include "sorted_list.h"
	

struct sorted_list /*only in c file*/
{
    dll_t *list;
    int(*cmp)(const void *, const void *);
};/*sorted_list_t*/


typedef struct cmp 
{
	int(*cmp)(const void *, const void *);
	void *data;
}cmp_t;



static int IsEqual(const void *data, const void *param)
{
	cmp_t *cmp_equal = (cmp_t *)param;
	if (0 == cmp_equal -> cmp(data, cmp_equal -> data))
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

static int IsBigger(const void *data, const void *param)
{
	cmp_t *cmp_bigger = (cmp_t *)param;
	if (0 < cmp_bigger -> cmp(data, cmp_bigger -> data))
	{
		return 1;
	}
	else
	{
		return 0;
	}
}



static sorted_list_iterator_t DebugVersion(sorted_list_iterator_t iterator, const sorted_list_t *list)
{
	#ifndef NDEBUG
	iterator.list = (sorted_list_t *)list;
	#endif
	return iterator;
}

/***************************************************************************************************************************************
                                                  create
****************************************************************************************************************************************/
sorted_list_t *SortedLinkedListCreate(int(*cmp)(const void *data1, const void *data2))
{
	sorted_list_t *sort_list = (sorted_list_t *)malloc(sizeof(sorted_list_t));
	assert(cmp);
	if (NULL == sort_list)
	{
		return NULL;
	}
	sort_list -> list = DoublyLinkedListCreate();
	if (NULL == sort_list -> list)
	{
		free(sort_list);
		return NULL;
	}
	sort_list -> cmp = cmp;
	return sort_list;
}


/****************************************************************************************************************************************
                                                  Destroy
****************************************************************************************************************************************/
void SortedLinkedListDestroy(sorted_list_t *list)
{
	assert(list);
	DoublyLinkedListDestroy(list -> list);
	list -> list = NULL;
	free(list);
}




/****************************************************************************************************************************************
                                                  IsEmpty
****************************************************************************************************************************************/
int SortedLinkedListIsEmpty(const sorted_list_t *list)
{
	assert(list);
	return (DoublyLinkedListIsEmpty(list -> list));
}




/****************************************************************************************************************************************
                                                  Begin
****************************************************************************************************************************************/
sorted_list_iterator_t SortedLinkedListBegin(const sorted_list_t *list)
{
	sorted_list_iterator_t iterator;
	assert(list);
	iterator.doubly_iter = DoublyLinkedListBegin(list -> list);
	return (DebugVersion(iterator, list));
}




/****************************************************************************************************************************************
                                                  Size
****************************************************************************************************************************************/
size_t SortedLinkedListSize(const sorted_list_t *list)
{
	assert(list);
	return DoublyLinkedListSize(list -> list);
}




/****************************************************************************************************************************************
                                                  End
****************************************************************************************************************************************/
sorted_list_iterator_t SortedLinkedListEnd(const sorted_list_t *list)
{
	sorted_list_iterator_t iterator;
	assert(list);
	iterator.doubly_iter = DoublyLinkedListEnd(list -> list);
	return (DebugVersion(iterator, list));
}




/****************************************************************************************************************************************
                                                  Next
****************************************************************************************************************************************/
sorted_list_iterator_t SortedLinkedListNext(const sorted_list_iterator_t iterator)
{
	sorted_list_iterator_t next_iterator;
	next_iterator.doubly_iter = DoublyLinkedListNext(iterator.doubly_iter);
	#ifndef NDEBUG
	next_iterator.list = iterator.list;
	#endif
	return next_iterator;
}




/****************************************************************************************************************************************
                                                  Prev
****************************************************************************************************************************************/
sorted_list_iterator_t SortedLinkedListPrev(const sorted_list_iterator_t iterator)
{
	sorted_list_iterator_t prev_iterator;
	prev_iterator.doubly_iter = DoublyLinkedListPrev(iterator.doubly_iter);
	#ifndef NDEBUG
	prev_iterator.list = iterator.list;
	#endif
	return prev_iterator;
}




/****************************************************************************************************************************************
                                                  ForEach
****************************************************************************************************************************************/
int SortedLinkedListForEach(sorted_list_iterator_t from, sorted_list_iterator_t to, int (*action_func)(void *data, void *param), void *param)
{
	return (DoublyLinkedListForEach(from.doubly_iter, to.doubly_iter, action_func, param));
}




/****************************************************************************************************************************************
                                                  SameIter
****************************************************************************************************************************************/
int SortedLinkedLisSameIter(const sorted_list_iterator_t iterator1, const sorted_list_iterator_t iterator2)
{
	return (DoublyLinkedLisSameIter(iterator1.doubly_iter, iterator2.doubly_iter));
}




/****************************************************************************************************************************************
                                                  GetData
****************************************************************************************************************************************/
void *SortedLinkedListGetData(const sorted_list_iterator_t iterator)
{
	return (DoublyLinkedListGetData(iterator.doubly_iter));
}



/****************************************************************************************************************************************
                                                  Remove
****************************************************************************************************************************************/
sorted_list_iterator_t SortedLinkedListRemove(sorted_list_iterator_t iterator)
{
	sorted_list_iterator_t return_iterator =  SortedLinkedListNext(iterator);
	DoublyLinkedListRemove(iterator.doubly_iter);
	return return_iterator;
}




/****************************************************************************************************************************************
                                                  PopBack
****************************************************************************************************************************************/
void *SortedLinkedListPopBack(sorted_list_t *list)
{
	assert(list);
	return (DoublyLinkedListPopBack(list -> list));
}




/****************************************************************************************************************************************
                                                  PopFront
****************************************************************************************************************************************/
void *SortedLinkedListPopFront(sorted_list_t *list)
{
	assert(list);
	return (DoublyLinkedListPopFront(list -> list));
}




/****************************************************************************************************************************************
                                                  FindIf
****************************************************************************************************************************************/
sorted_list_iterator_t SortedLinkedListFindIf(sorted_list_iterator_t from, sorted_list_iterator_t to, int (*match_func)(const void *data, const void *param),const void *param)
{
	sorted_list_iterator_t return_iter;
	assert(match_func);
	#ifndef NDEBUG
	assert(from.list == to.list);
	return_iter.list = from.list;
	#endif
	return_iter.doubly_iter = DoublyLinkedListFind(from.doubly_iter, to.doubly_iter, match_func, param);
	return return_iter;
}




/****************************************************************************************************************************************
                                                  Insert
****************************************************************************************************************************************/
sorted_list_iterator_t SortedLinkedListInsert(sorted_list_t *list, void *data)
{
	sorted_list_iterator_t begin_iterator;
	sorted_list_iterator_t end_iterator;
	sorted_list_iterator_t new_iterator;
	
	cmp_t cmp_bigger;
	
	assert(list);
	
	cmp_bigger.cmp = list -> cmp;
	cmp_bigger.data = data;
	
	begin_iterator = SortedLinkedListBegin(list);
	end_iterator = SortedLinkedListEnd(list);
	new_iterator.doubly_iter = DoublyLinkedListInsertBefore(DoublyLinkedListFind(begin_iterator.doubly_iter, end_iterator.doubly_iter, IsBigger, &cmp_bigger), data);
	return (DebugVersion(new_iterator, list));
}




/****************************************************************************************************************************************
                                                  Find
****************************************************************************************************************************************/
sorted_list_iterator_t SortedLinkedListFind(sorted_list_t *list, sorted_list_iterator_t from, sorted_list_iterator_t to, const void *data)
{
	sorted_list_iterator_t iterator_found;
	cmp_t cmp_equal;
	
	assert(list);
	
	#ifndef NDEBUG
	assert(list == from.list);
	assert(list == to.list);
	iterator_found.list = list;
	#endif
	
	cmp_equal.cmp = list -> cmp;
	cmp_equal.data = (void *)data;
	
	iterator_found.doubly_iter = DoublyLinkedListFind(from.doubly_iter, to.doubly_iter, IsEqual, &cmp_equal);
	return iterator_found;
}





/****************************************************************************************************************************************
                                                  Merge
****************************************************************************************************************************************/
void SortedLinkedListMerge(sorted_list_t *dest, sorted_list_t *src)
{
	sorted_list_iterator_t to;
	sorted_list_iterator_t from;
	sorted_list_iterator_t where;
	
	cmp_t cmp_if;
	
	assert(dest);
	assert(src);
	
	cmp_if.cmp = dest -> cmp;
	
	where = SortedLinkedListBegin(dest);
	
	while (SortedLinkedListSize(src) > 0)
	{
		from = SortedLinkedListBegin(src);
		to = SortedLinkedListNext(from);
		cmp_if.data = SortedLinkedListGetData(SortedLinkedListBegin(src));
		where = SortedLinkedListFindIf(where, SortedLinkedListEnd(dest), IsBigger, &cmp_if);   /* search from where (the last place found) */
		DoublyLinkedListSplice(where.doubly_iter, from.doubly_iter, to.doubly_iter);
	}
}






















