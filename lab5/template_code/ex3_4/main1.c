#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "dplist.h"

dplist_t * list = NULL;

int dplist_errno;

typedef struct element{
	int a;
	int b;
} element_node_t;

void dpl_print( dplist_t * list )
{
  int i, length;

  length = dpl_size(list);
  assert(dplist_errno == DPLIST_NO_ERROR);
  for ( i = 0; i < length; i++)    
  {
  	int* element = dpl_get_element_at_index(list, i);
    assert(dplist_errno == DPLIST_NO_ERROR);
    printf("element at index %d = %d\n", i, *element);
  }
}

void * element_copy(void * src_element)
{
	element_node_t *copy;
	copy = malloc(sizeof(*copy));
	assert(copy != NULL);
	
	copy->a = ((element_node_t *)src_element)->a;
	copy->b = ((element_node_t *)src_element)->b;
	return copy;
}

void element_free(void ** element)
{
	free(*element);
	*element = NULL;
}

int element_compare(void *x, void *y)
{
	if (((element_node_t *)x)->a == ((element_node_t *)y)->a)
		return 0;
	else if (((element_node_t *)x)->a > ((element_node_t *)y)->a)
		return 1;

//	if (*((int *)x) < *((int *)y))
		return -1;
}

int main(void)
{
  element_node_t * a;
//  int x;
//  int index;
  a = malloc( sizeof(*a) );
  assert(a != NULL);
  
  list = dpl_create( element_copy, element_free, element_compare);

	printf("\n********testing search methods for references of elements***************\n");
	for (int i = 0; i <3; i++)
	{
		a->a=i;
		a->b=i;
		dpl_insert_at_index(list, a, dpl_size(list), true);
	}
	puts("0\n");

	dpl_remove_at_index(list, dpl_size(list), true);
	dpl_remove_at_index(list, dpl_size(list), true);
	dpl_remove_at_index(list, dpl_size(list), true);
	puts("1\n");
	for (int i = 0; i <3; i++)
	{
		a->a=i;
		a->b=i;
		dpl_insert_at_index(list, a, dpl_size(list), true);
	}
	puts("2\n");
	for (int i = 0; i <3; i++)
	dpl_remove_at_index(list, dpl_size(list), true);
	dpl_remove_at_index(list, dpl_size(list), true);
	dpl_remove_at_index(list, dpl_size(list), true);
dpl_free(&list);
free(a);
	/*
	for (int i = 0; i <10; i++)
	{
		*a = i;
		dpl_insert_at_index(list, a, i, true);
	}
	*/
/*
	for (int i = 0; i < dpl_size(list); i++)
	{
		dplist_node_t *ref = dpl_get_reference_at_index(list, i);
		printf("reference at index %d, %p\n", dpl_get_index_of_reference(list, ref) , ref);
		int* element = dpl_get_element_at_reference(list, ref);
		printf("element at reference%p is %d\n", ref, *element);
		ref = dpl_get_reference_of_element(list, element);
		printf("the reference of element%d is %p\n", *element, ref);
		element = dpl_get_element_at_reference(list, NULL);
		printf("element at reference NULL is %d\n", *element);
		
	}
	printf("\n******************test*****************\n");
	for (int i = 0; i < 5; i++)
	{
  *a = i;
  dpl_insert_at_index(list,a,i, true);
	printf("element %d is inserted at index %d\n", i, i);
	int *ret = dpl_get_element_at_index(list, i);
	printf("get element at index %d: %d\n",i, *ret);
	printf("size of list is: %d\n", dpl_size(list));
	printf("\n\n");
	}


	printf("\n******************test*****************\n");
	for (int i = 5; i < 10; i++)
	{
  *a = i;
 // dpl_insert_at_index(list,a,i, true);
//	printf("element %d is inserted at index %d\n", i, i);
	dpl_remove_at_index(list, i, true);
	int *ret = dpl_get_element_at_index(list, i);
	printf("get element at index %d: %d\n",i, *ret);
	printf("size of list is: %d\n", dpl_size(list));
	printf("\n\n");
	}
	
	printf("\n******************test*****************\n");
	for (int i = 5; i < 10; i++)
	{
  *a = i;
  dpl_insert_at_index(list,a,i, true);
	printf("element %d is inserted at index %d\n", i, i);
	dpl_remove_at_index(list, i, true);
	int ret = dpl_get_index_of_element(list, a);
	if (ret < 0)
		printf("element not found.\n");
	printf("size of list is: %d\n", dpl_size(list));
	printf("\n\n");
	}
	dpl_free(&list);
	free(a);
*/
	/*
	printf("\n******************search reference methods****************\n");
	for (int i = 5; i < 10; i++)
	{
  *a = i;
  dpl_insert_at_index(list,a,i, true);
	printf("element %d is inserted at index %d\n", i, i);

	printf("size of list is: %d\n", dpl_size(list));
	printf("\n\n");
	}

	dpl_print(list);
	for (int i = 0; i < dpl_size(list); i++)
	{
		dplist_node_t *ref = dpl_get_reference_at_index(list, i);
		printf("reference at index%d, %p\n",i , ref);
	}

	dplist_node_t *ref = dpl_get_first_reference(list);
	printf("first reference %p\n", ref);
	while ((ref = dpl_get_next_reference(list, ref)) != NULL)
	{
		printf("next reference %p\n", ref);
	}

	ref = dpl_get_last_reference(list);
	printf("last reference %p\n", ref);
	while ((ref = dpl_get_previous_reference(list, ref)) != NULL)
	{
		printf("previous reference %p\n", ref);
	}

	dpl_remove_at_reference(list, ref, true);
*/


/*
	printf("\n********testing inserted at a reference***************\n");
	for (int i = 0; i < 5; i++)
	{
  *a = i;
  dpl_insert_at_index(list,a,i, true);
	printf("element %d is inserted at index %d\n", i, i);
	printf("\n");
	}
	dpl_print(list);
	printf("size of list is: %d\n", dpl_size(list));

	ref = dpl_get_reference_at_index(list, 2);
	int tmp = 12;
	dpl_insert_at_reference(list, &tmp, ref, true);
	printf("dpl_insert_at_reference\n");
	dpl_print(list);
	printf("size of list is: %d\n", dpl_size(list));
	printf("index of ref%p, is %d\n", ref, dpl_get_index_of_reference(list, ref));
	//dpl_remove_at_reference(list, ref, true);
	dpl_remove_element(list, &tmp,true);
	printf("index of ref%p, is %d\n", ref, dpl_get_index_of_reference(list, ref));
	dpl_print(list);

	dpl_insert_sorted(list, &tmp, true);
	dpl_print(list);
	free(a);
  dpl_free( &list );


*/

/*
  *a = 2;
  dpl_insert_at_index(list,a,-1, true);

  *a = 3;
  dpl_insert_at_index(list,a,-1, true);

	*a = 12;
	dpl_insert_sorted(list, a, true);
 	printf("dpl_insert_sorted\n");
  dpl_print(list);
	free(a);
  a = malloc( sizeof(int) );
  assert(a != NULL);

	*a = 7;
	dpl_insert_at_reference(list, a, NULL, true);
  
  *a = 4;
  dpl_insert_at_index(list,a,10, true);  
	
	*a = 12;
	dpl_insert_sorted(list, a, true);
 	printf("dpl_insert_sorted\n");
  dpl_print(list);

  *a = 5;
  dpl_insert_at_index(list,a,10, true); 

	//dpl_insert_at_reference
	*a = 6;
	dpl_insert_at_reference(list, a, dpl_get_first_reference(list), true);

	*a = 7;
	dpl_insert_at_reference(list, a, NULL, true);
  
	*a = 7;
	dpl_insert_at_reference(list, a, NULL, true);
  
	*a = 9;
	dpl_insert_at_reference(list, a, NULL, true);
	
	*a = 12;
	dpl_insert_sorted(list, a, true);
 	printf("dpl_insert_sorted\n");
  dpl_print(list);

	*a = 8;
	dpl_insert_at_reference(list, a, NULL, true);

  dpl_print(list);
//	sort list
	dpl_sort(list);

 	printf("list is sorted \n");
  dpl_print(list);
 
  x = 3;
  index = dpl_get_index_of_element(list, &x);
  if (index == -1)
  {
    printf("element %d is not found\n", x);
  }
  else
  {
    printf("element %d found at index %d\n", x, index);
  }

	//get first reference

	printf("first %d\n",*((int*)dpl_get_element_at_reference(list, dpl_get_first_reference(list))));
	printf("last %d\n",*((int*)dpl_get_element_at_reference(list, dpl_get_last_reference(list))));
	printf("next %d\n",*((int*)dpl_get_element_at_reference(list, dpl_get_next_reference(list, dpl_get_first_reference(list)))));
	printf("previous %d\n",*((int*)dpl_get_element_at_reference(list, dpl_get_previous_reference(list, dpl_get_last_reference(list)))));
	x = 2;
	printf("element 3 %d\n",*((int*)dpl_get_element_at_reference(list, dpl_get_reference_of_element(list, &x))));
	printf("index %d\n",dpl_get_index_of_reference(list,  dpl_get_reference_of_element(list, &x)));

  // remove elements from the list
  list = dpl_remove_at_index(list, -1, true);
  list = dpl_remove_at_index(list, 10, true);
  list = dpl_remove_at_index(list, -1, true);

  dpl_print(list);
  dpl_free( &list );
  free(a); 
  
  //dpl_print(list);
  // check Valgrind: after freeing the entire list, there is still a memory leak!!!
*/  
  return 0;
}
