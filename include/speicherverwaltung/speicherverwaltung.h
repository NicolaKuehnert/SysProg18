#ifndef SPEICHERVERWALTUNG_H
#define SPEICHERVERWALTUNG_H

#include <stdlib.h>

#define MEM_POOL_SIZE 4096
#define MAGIC_INT 0xacdcacdc

typedef struct memblock{
	size_t size;
	struct memblock *next;
	unsigned short id;
} memblock;

memblock *freemem;
unsigned char mempool[];

int cm_init(void);
void *cm_malloc(size_t size);
void cm_free(void* ptr);

#endif /* SPEICHERVERWALTUNG_H */
