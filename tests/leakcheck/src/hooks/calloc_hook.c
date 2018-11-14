//
// Created by karl on 16.11.17.
//
#define _GNU_SOURCE

#include <hooks/calloc_hook.h>
#include <dlfcn.h>
#include <stdio.h>
#include <memory.h>
#include <stdlib.h>

#include <allocation_list.h>



int enable_calloc_hook = 0;
void *(*orig_calloc)(size_t, size_t) = NULL;

#if __x86_64__
#define FIX_SIZE 32
#else
#define FIX_SIZE 20
#endif

static char dlsym_fix_mem[FIX_SIZE];

void *calloc(size_t len, size_t element_size) {
    void    *res,
            *base_addr_from_module,
            *calling_addr;
    Dl_info caller_info;
    char    *module_path;
    static int  dlsym_fixed = 0;



    if(orig_calloc == NULL) {
        if(!dlsym_fixed && len == 1 && element_size == FIX_SIZE) {
            dlsym_fixed = 1;
            memset(dlsym_fix_mem, 0, FIX_SIZE);
            return (void *) dlsym_fix_mem;
        }
        orig_calloc = (void *(*) (size_t, size_t)) dlsym (RTLD_NEXT, "calloc");
    }
    if(!enable_calloc_hook) {
        return (*orig_calloc)(len, element_size);
    }

    enable_calloc_hook = 0;

    res = (*orig_calloc)(len, element_size);
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
    //printf("calloc(%zu, %zu) = %p\n", len, element_size, res);

    enable_calloc_hook = 1;


    return res;
}

