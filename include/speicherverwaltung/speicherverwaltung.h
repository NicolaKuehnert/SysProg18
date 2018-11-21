#ifndef SPEICHERVERWALTUNG_H
#define SPEICHERVERWALTUNG_H

#include <stdlib.h>

#define MEM_POOL_SIZE 4096
#define MAGIC_INT 0xacdcacdc
#define toMemblockPtr(x) ((memblock *)x)
#define inRange(x) (toMemblockPtr(x) >= toMemblockPtr(mempool) && toMemblockPtr(x) <= toMemblockPtr(mempool) + MEM_POOL_SIZE)
#define nextBlock(x) ((memblock *) (((char *) (x + 1)) + x->size))
 
typedef struct memblock{
	size_t size;
	struct memblock *next;
	unsigned short id;
} memblock;

memblock *freemem;
char mempool[MEM_POOL_SIZE];

int init_heap(void);
void *cm_malloc(size_t size);
void cm_free(void* ptr);
void cm_defrag(void);
void cm_defrag20(void);
void *cm_memcpy(void *dest, const void *src, size_t n);
void *cm_realloc(void *ptr, size_t size);

#endif /* SPEICHERVERWALTUNG_H */
