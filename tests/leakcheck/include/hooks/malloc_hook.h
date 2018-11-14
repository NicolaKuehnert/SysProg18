//
// Created by karl on 16.11.17.
//

#ifndef LEAKCHECK_MALLOC_HOOK_H
#define LEAKCHECK_MALLOC_HOOK_H


#include <stddef.h>


extern int  enable_malloc_hook;
void        *malloc(size_t);


#endif //LEAKCHECK_MALLOC_HOOK_H
