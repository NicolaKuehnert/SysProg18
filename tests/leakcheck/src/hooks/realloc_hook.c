//
// Created by karl on 16.11.17.
//
#define _GNU_SOURCE

#include <hooks/realloc_hook.h>
#include <dlfcn.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <allocation_list.h>


int enable_realloc_hook = 0;
void *(*orig_realloc)(void *, size_t) = NULL;


void *realloc(void *ptr, size_t len) {
    void    *res,
            *base_addr_from_module,
            *calling_addr;
    Dl_info caller_info;
    char    *module_path;


    if(orig_realloc == NULL) {
        orig_realloc = (void *(*) (void *, size_t)) dlsym (RTLD_NEXT, "realloc");
    }
    if(!enable_realloc_hook) {
        return (*orig_realloc)(ptr, len);
    }

    enable_realloc_hook = 0;

    res = (*orig_realloc)(ptr, len);
    if(ptr == NULL) {
        calling_addr = __builtin_return_address(0);
        if(!dladdr(calling_addr, &caller_info)) {
            puts("No debug information stored\n");
            insert_allocation(res, len, NULL, NULL);
        }
        else {
            module_path = (char *) malloc(strlen(caller_info.dli_fname)+1);
            strcpy(module_path, caller_info.dli_fname);
            module_path[strlen(caller_info.dli_fname)] = 0x00;

            base_addr_from_module = (void *) (__builtin_return_address(0) - caller_info.dli_fbase);
            insert_allocation(res, len, base_addr_from_module, module_path);
        }
    }
    else {
        modify_allocation(ptr, len);
    }

    //printf("realloc(%p, %zu) = %p\n", ptr, len, res);

    enable_realloc_hook = 1;


    return res;
}
