//
// Created by karl on 16.11.17.
//

#ifndef LEAKCHECK_FREE_HOOK_H
#define LEAKCHECK_FREE_HOOK_H


#include <stddef.h>

extern int  enable_free_hook;
void        free(void *);


#endif //LEAKCHECK_FREE_HOOK_H
