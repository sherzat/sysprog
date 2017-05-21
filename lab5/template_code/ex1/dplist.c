#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "dplist.h"

//#define DEBUG

#ifdef DEBUG
	#define DEBUG_PRINTF(...) 									\
		do {											\
			printf("\nIn %s - function %s at line %d: ", __FILE__, __func__, __LINE__);	\
			printf(__VA_ARGS__);								\
		} while(0)
#else
	#define DEBUG_PRINTF(...) (void)0
#endif


#define DPLIST_ERR_HANDLER(condition,dplist_errno_value,...)\
	do {						\
		if ((condition))			\
		{					\
		  dplist_errno = dplist_errno_value;	\
		  DEBUG_PRINTF(#condition "failed");	\
		  return __VA_ARGS__;			\
		}					\
		dplist_errno = DPLIST_NO_ERROR;			\
	} while(0)

/*
 * The real definition of struct list / struct node
 */

struct dplist_node {
  dplist_node_t * prev, * next;
  element_t element;
};

struct dplist {
  dplist_node_t * head;
  // more fields will be added later
};

dplist_t * dpl_create ()
{
  dplist_t * list;
  list = malloc(sizeof(struct dplist));
  DPLIST_ERR_HANDLER((list==NULL),DPLIST_MEMORY_ERROR,NULL);
  list->head = NULL;  
  return list;
}

void dpl_free( dplist_t ** list )
{
  // add your code here
	dplist_node_t *dummy, *temp;
	dummy = (*list)->head;

	while (NULL != dummy )
	{
		temp = dummy;
		dummy = dummy->next;
		free(temp);
	}
	
	free(*list);
	*list = NULL;
}

  /* Important note: to implement any list manipulation operator (insert, append, delete, sort, ...), always be aware of the following cases:
   * 1. empty list ==> avoid errors
   * 2. do operation at the start of the list ==> typically requires some special pointer manipulation
   * 3. do operation at the end of the list ==> typically requires some special pointer manipulation
   * 4. do operation in the middle of the list ==> default case with default pointer manipulation
   * ALWAYS check that you implementation works correctly in all these cases (check this on paper with list representation drawings!)
   **/ 

dplist_t * dpl_insert_at_index( dplist_t * list, element_t element, int index)
{
  dplist_node_t * ref_at_index, * list_node;
  DPLIST_ERR_HANDLER((list==NULL),DPLIST_INVALID_ERROR,list);
  list_node = malloc(sizeof(dplist_node_t));
  DPLIST_ERR_HANDLER((list_node==NULL),DPLIST_MEMORY_ERROR,NULL);
  list_node->element = element;
  if (list->head == NULL)  
  { // covers case 1 
    list_node->prev = NULL;
    list_node->next = NULL;
    list->head = list_node;
  } else if (index <= 0)  
  { // covers case 2 
    list_node->prev = NULL;
    list_node->next = list->head;
    list->head->prev = list_node;
    list->head = list_node;
  } else 
  {
    ref_at_index = dpl_get_reference_at_index(list, index);  
    assert( ref_at_index != NULL);
    if (index < dpl_size(list))
    { // covers case 4
      list_node->prev = ref_at_index->prev;
      list_node->next = ref_at_index;
      ref_at_index->prev->next = list_node;
      ref_at_index->prev = list_node;
    } else
    { // covers case 3 
      assert(ref_at_index->next == NULL);
      list_node->next = NULL;
      list_node->prev = ref_at_index;
      ref_at_index->next = list_node;    
    }
  }
  return list;
}

dplist_t * dpl_remove_at_index( dplist_t * list, int index)
{
  // add your code here
	dplist_node_t *dummy;

  DPLIST_ERR_HANDLER((list==NULL),DPLIST_INVALID_ERROR, 0);
  if (list->head == NULL ) return list;

	dummy = dpl_get_reference_at_index(list, index);
	if(dummy->prev != NULL)
	{	
		dummy->prev->next = dummy->next;
		if (dummy->next != NULL)
			dummy->next->prev = dummy->prev;
	}
	else 
	{
		list->head = dummy->next;
		if (dummy->next != NULL)
			dummy->next->prev = dummy->prev;
	}

	free(dummy);

	return list;
}

int dpl_size( dplist_t * list )
{
  // add your code here
	int count;
  dplist_node_t * dummy;
  DPLIST_ERR_HANDLER((list==NULL),DPLIST_INVALID_ERROR, 0);
  if (list->head == NULL ) return 0;

  for ( dummy = list->head, count = 0; dummy->next != NULL ; dummy = dummy->next, count++) ;
	return count+1;
	
}

dplist_node_t * dpl_get_reference_at_index( dplist_t * list, int index )
{
  int count;
  dplist_node_t * dummy;
  DPLIST_ERR_HANDLER((list==NULL),DPLIST_INVALID_ERROR,NULL);
  if (list->head == NULL ) return NULL;
  for ( dummy = list->head, count = 0; dummy->next != NULL ; dummy = dummy->next, count++) 
  { 
    if (count >= index) return dummy;
  }  
  return dummy;  
}

element_t dpl_get_element_at_index( dplist_t * list, int index )
{
  // add your code here
	dplist_node_t *dummy;
  DPLIST_ERR_HANDLER((list==NULL),DPLIST_INVALID_ERROR, 0);
  if (list->head == NULL ) return 0;
	dummy = dpl_get_reference_at_index(list, index);

	return dummy->element;
}

int dpl_get_index_of_element( dplist_t * list, element_t element )
{
  // add your code here
	int count;
	dplist_node_t *dummy;
  DPLIST_ERR_HANDLER((list==NULL),DPLIST_INVALID_ERROR, 0);
	dummy = list->head;

	for (dummy = list->head, count = 0; dummy != NULL; dummy = dummy->next, count++)
	{
		if( dummy->element == element) return count;
	}
	return -1;
}


    
