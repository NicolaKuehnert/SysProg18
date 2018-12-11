#ifndef RINGBUFFER_H
#define RINGBUFFER_H

#include <stdlib.h>
#define percent(x) ((100 / x->size) * count_elements(x))

typedef struct ring_buffer {
size_t size, count, head;
void **elems;
void (*free_callback)(void *p);
}ring_buffer;

ring_buffer *init_buffer(const size_t n, void (*f)(void *p));
void *read_buffer(ring_buffer *cb);
void write_buffer(ring_buffer *cb, void *data);
int free_buffer(ring_buffer *cb);
int count_elements(const ring_buffer *cb);

#endif 

ring_buffer *init_buffer(const size_t n, void (*f)(void *p));
void write_buffer(ring_buffer *cb, void *data);
void *read_buffer(ring_buffer *cb);
int free_buffer(ring_buffer *cb);
int count_elements(const ring_buffer *cb);
