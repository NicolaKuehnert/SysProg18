#ifndef SPEICHERVERWALTUNG_H
#def SPEICHERVERWALTUNG_H

#define MEM_POOL_SIZE 4096
#define MAGIC_INT oxacdcacdc

typedef struct memblock{
	size_t size;
	struct memblock *next;
	unsigned short id;
} memblock;

memblock *freemem;
unsigned char mempool[];

int cm_init(void);
void *cm_malloc(size_t size);

#endif /* SPEICHERVERWALTUNG_H */