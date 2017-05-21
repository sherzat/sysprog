#ifndef DPLIST_H_
#define DPLIST_H_

extern int dplist_errno;

/*
 * definition of error codes
 * */
#define DPLIST_NO_ERROR 0
#define DPLIST_MEMORY_ERROR 1 // error due to mem alloc failure
//#define DPLIST_EMPTY_ERROR 2  //error due to an operation that can't be executed on an empty list
#define DPLIST_INVALID_ERROR 3 //error due to a list operation applied on a NULL list 

typedef int element_t;

typedef struct dplist dplist_t; // slist_t is a struct containing at least a head pointer to the start of the list; later function pointers to element_copy, element_compare, element_free, etc. will be added

typedef struct dplist_node dplist_node_t;

dplist_t * dpl_create ();
// Returns a pointer to a newly-allocated and initialized list.
// Returns NULL if memory allocation failed and dplist_errno is set to DPLIST_MEMORY_ERROR 

void dpl_free( dplist_t ** list );
// Every list node of the list needs to be deleted (free memory)
// The list itself also needs to be deleted (free all memory)
// '*list' must be set to NULL.
// If 'list' or '*list' is NULL, dplist_errno is set to DPLIST_INVALID_ERROR.

int dpl_size( dplist_t * list );
// Returns the number of elements in the list.
// Return 0 if 'list' is NULL and dplist_errno is set to DPLIST_INVALID_ERROR'.

dplist_t * dpl_insert_at_index( dplist_t * list, element_t element, int index);
// Inserts a new list node containing an 'element' in the list at position 'index'  and returns a pointer to the new list.
// Remark: the first list node has index 0.
// If 'index' is 0 or negative, the list node is inserted at the start of 'list'. 
// If 'index' is bigger than the number of elements in the list, the list node is inserted at the end of thelist.
// Return NULL if 'list' is NULL and dplist_errno is set to DPLIST_INVALID_ERROR 
// Return NULL if memory allocation failed and dplist_errno is set to DPLIST_MEMORY_ERROR 

dplist_t * dpl_remove_at_index( dplist_t * list, int index);
// Removes the list node at index 'index' from the list. 
// If 'index' is 0 or negative, the first list node is removed. 
// If 'index' is bigger than the number of elements in the list, the last list node is removed.
// If the list is empty, return the unmodifed list 
// Return NULL if 'list' is NULL and dplist_errno is set to DPLIST_INVALID_ERROR

dplist_node_t * dpl_get_reference_at_index( dplist_t * list, int index );
// Returns a reference to the list node with index 'index' in the list. 
// If 'index' is 0 or negative, a reference to the first list node is returned. 
// If 'index' is bigger than the number of list nodes in the list, a reference to the last list node is returned. 
// If the list is empty, NULL is returned.
// Return NULL if 'list' is NULL and dplist_errno is set to DPLIST_INVALID_ERROR

element_t dpl_get_element_at_index( dplist_t * list, int index );
// Returns the list element contained in the list node with index 'index' in the list.
// If 'index' is 0 or negative, the element of the first list node is returned. 
// If 'index' is bigger than the number of elements in the list, the element of the last list node is returned.
// If the list is empty, (void *)NULL is returned.
// Return NULL if 'list' is NULL and dplist_errno is set to DPLIST_INVALID_ERROR

int dpl_get_index_of_element( dplist_t * list, element_t element );
// Returns an index to the first list node in the list containing 'element'.
// If 'element' is not found in the list, -1 is returned.
// Return NULL if 'list' is NULL and dplist_errno is set to DPLIST_INVALID_ERROR

#endif  //DPLIST_H_

