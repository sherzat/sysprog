#include <string.h>
#include <stdio.h>
#include <assert.h>
#include "ma_malloc.h"
//#include "memdump.h"


#define MEM_POOL_SIZE 600 //in bytes
typedef unsigned char byte;

typedef enum {ALLOCATED, FREE} mem_status;

typedef struct {
  size size;
  mem_status status;
} mem_chunk_header;

static byte mem_pool[MEM_POOL_SIZE];


/*
** Allocates array of bytes (memory pool) and initializes the memory allocator. 
** If some bytes have been used after calling ma_malloc(size), calling to ma_init() will result in clearing up the memory pool.
*/
void ma_init()
{
	memset(mem_pool, '*', MEM_POOL_SIZE);

	mem_chunk_header header;
	mem_chunk_header footer;

	header.size = (MEM_POOL_SIZE - 2*sizeof(mem_chunk_header) );
	header.status = FREE;
	memcpy(mem_pool, &header, sizeof(mem_chunk_header));	

	footer.size = (MEM_POOL_SIZE - 2*sizeof(mem_chunk_header) );
	footer.status = FREE;
	memcpy(&mem_pool[sizeof(mem_chunk_header) + header.size], &footer, sizeof(mem_chunk_header));	
}

/*
** Requesting for the tsize bytes from memory pool. 
** If the request is possible, the pointer to the first possible address byte (right after its header) in memory pool is returned.
*/
void* ma_malloc(size tsize)
{
	int i = 0;
	mem_chunk_header *f_s;
	byte *h, *new_h, *new_f;
	while ( i < MEM_POOL_SIZE)
	{
		mem_chunk_header *h_s;
		byte *f;
		h = mem_pool;
		h += i;
		h_s = (mem_chunk_header *)h;

		f = h + (sizeof(mem_chunk_header) + h_s->size);
		f_s = (mem_chunk_header *)f;

		if (h_s->status == FREE && h_s->size >= tsize)
		{
			h_s->size = tsize;
			h_s->status = ALLOCATED;
			new_f = h + (sizeof(mem_chunk_header) + h_s->size);
			memcpy(new_f, h, sizeof(mem_chunk_header));

			f_s->status = FREE;
			f_s->size -=  (h_s->size + 2 * sizeof(mem_chunk_header));
			new_h = new_f + sizeof(mem_chunk_header); 
			memcpy(new_h, f, sizeof(mem_chunk_header));

			return h + sizeof(mem_chunk_header);
		}
		else 
		{
			i += 2 * sizeof(mem_chunk_header) + h_s->size;
		}
	}

	return NULL;
}

/*
** Releasing the bytes in memory pool which was hold by ptr, meaning makes those bytes available for other uses. 
** Implement also the coalescing behavior.
*/
void ma_free(void* ptr)
{
	byte *h, *f, *next_h, *next_f, *pre_h, *pre_f;
	mem_chunk_header *h_s, *f_s, *next_h_s, *next_f_s, *pre_h_s, *pre_f_s;

	h = (byte*)ptr - sizeof(mem_chunk_header);
	h_s = (mem_chunk_header *)h;

	f = (byte*)ptr + h_s->size;
	f_s = (mem_chunk_header *)f;

	next_h = f + sizeof(mem_chunk_header);
	next_h_s = (mem_chunk_header *)next_h;

	next_f = next_h + (sizeof(mem_chunk_header) + next_h_s->size);
	next_f_s = (mem_chunk_header *)next_f;

	pre_f = h - sizeof(mem_chunk_header);
	pre_f_s = (mem_chunk_header *)pre_f;

	pre_h = pre_f - (pre_f_s->size + sizeof(mem_chunk_header));
	pre_h_s = (mem_chunk_header *)pre_h;

	//free ALLOCATED memory.
	h_s->status = FREE;
	f_s->status = FREE;
	memset(h + sizeof(mem_chunk_header), '*', h_s->size);

	
	//left seek free blocks
	if ((next_h < (mem_pool + MEM_POOL_SIZE)) && (next_h_s->status == FREE))
	{
		h_s->size +=  next_h_s->size + 2*sizeof(mem_chunk_header);
		next_f_s->size = h_s->size;
		memset(h + sizeof(mem_chunk_header), '*', h_s->size);
	}
	//right seek free blocks
	if ((pre_h > mem_pool) && (pre_h_s->status == FREE))
	{
		pre_h_s->size += h_s->size + 2*sizeof(mem_chunk_header);
		f_s->size = pre_h_s->size;
		memset(pre_h + sizeof(mem_chunk_header), '*', pre_h_s->size);
	}
}


/*
** This function is only for debugging. It prints out the entire memory pool. 
** Use the code from the memdump tool to do this.
*/

/*
void ma_print(void)
{
  
	memdump(mem_pool, 600);
  
}
*/ 
  
