//
// Created by karl on 16.11.17.
//
#define _GNU_SOURCE

#include <hooks/free_hook.h>
#include <dlfcn.h>
#include <stdio.h>

#include <allocation_list.h>

int enable_free_hook = 0;
void (*orig_free)(void *) = NULL;


void free(void *ptr) {
    if(orig_free == NULL) {
        orig_free = (void (*) (void *)) dlsym (RTLD_NEXT, "free");
    }
    if(!enable_free_hook || ptr == NULL) {
        return (*orig_free)(ptr);
    }

    enable_free_hook = 0;

    (*orig_free)(ptr);
    modify_allocation(ptr, 0);
    //printf("free(%p)\n", ptr);

    enable_free_hook = 1;
}


