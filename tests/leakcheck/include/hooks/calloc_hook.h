//
// Created by karl on 16.11.17.
//

#ifndef LEAKCHECK_CALLOC_HOOK_H
#define LEAKCHECK_CALLOC_HOOK_H

#include <stddef.h>


extern int  enable_calloc_hook;
void        *calloc(size_t, size_t);

#endif //LEAKCHECK_CALLOC_HOOK_H
