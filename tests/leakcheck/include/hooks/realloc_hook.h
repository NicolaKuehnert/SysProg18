//
// Created by karl on 16.11.17.
//

#ifndef LEAKCHECK_REALLOC_HOOK_H
#define LEAKCHECK_REALLOC_HOOK_H


#include <stddef.h>


extern int  enable_realloc_hook;
void        *realloc(void *, size_t);


#endif //LEAKCHECK_REALLOC_HOOK_H
