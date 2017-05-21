#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "dplist.h"


dplist_t * list = NULL;

int dplist_errno;

void dpl_print( dplist_t * list )
{
  element_t element;
  int i, length;

  length = dpl_size(list);
  assert( dplist_errno == DPLIST_NO_ERROR );
  for ( i = 0; i < length; i++)    
  {
    element = dpl_get_element_at_index( list, i );
    assert( dplist_errno == DPLIST_NO_ERROR );
    printf("index element %d = %d\n", i, element);
  }
}


int main(void)
{
  list = dpl_create();
  assert(dplist_errno == DPLIST_NO_ERROR);
  
  dpl_insert_at_index(list,1,0);
  assert(dplist_errno == DPLIST_NO_ERROR);

  dpl_insert_at_index(list,2,1);
  assert(dplist_errno == DPLIST_NO_ERROR);

  dpl_insert_at_index(list,4,5);
  assert(dplist_errno == DPLIST_NO_ERROR);
  
  printf("list size = %d\n", dpl_size(list) );
  assert(dplist_errno == DPLIST_NO_ERROR);

  dpl_insert_at_index(list,3,2);
  assert(dplist_errno == DPLIST_NO_ERROR);    
  
  dpl_print( list );
  
  printf("element '4' has index = %d\n", dpl_get_index_of_element(list, 4) );
  
  list = dpl_remove_at_index(list,-5);
  assert(dplist_errno == DPLIST_NO_ERROR);   
  
  list = dpl_remove_at_index(list, 20);
  assert(dplist_errno == DPLIST_NO_ERROR); 
  
  dpl_print(list);
  
  dpl_free(&list);
  assert(dplist_errno == DPLIST_NO_ERROR);
  
  return 0;
}
