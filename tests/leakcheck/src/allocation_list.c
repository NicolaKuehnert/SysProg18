//
// Created by karl on 16.11.17.
//
#include <allocation_list.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <hooks/malloc_hook.h>
#include <hooks/free_hook.h>



allocation *allocation_list = NULL;


void insert_allocation(void *ptr, size_t len, void *base_calling_addr, char *module_path) {
    allocation  *new_allocation,
                *iter_allocation;


    enable_malloc_hook = 0;
    new_allocation = (allocation *) malloc(sizeof(allocation));
    enable_malloc_hook = 1;

    new_allocation->ptr = ptr;
    new_allocation->len = len;
    new_allocation->next = NULL;
    new_allocation->base_calling_addr = base_calling_addr;
    new_allocation->module_path = module_path;

    iter_allocation = allocation_list;
    if(!allocation_list) {
        allocation_list = new_allocation;
        return;
    }
    while(iter_allocation->next) {
        iter_allocation = iter_allocation->next;
    }

    iter_allocation->next = new_allocation;


    return;
}

void modify_allocation(void *ptr, size_t len) {
    allocation  *iter_allocation,
                *last_iter;


    if(!allocation_list) {
        return;
    }
    iter_allocation = allocation_list;

    last_iter = NULL;
    while(iter_allocation->ptr != ptr) {
        last_iter = iter_allocation;
        iter_allocation = iter_allocation->next;
        if(!iter_allocation) {
            return;
        }
    }


    if(len == 0) {
        if (iter_allocation == allocation_list) {
            if(iter_allocation->next == NULL) {
                allocation_list = NULL;
            }
            else {
                allocation_list = iter_allocation->next;
            }
        }
        else {
            last_iter->next = iter_allocation->next;
        }
        enable_free_hook = 0;
        free(iter_allocation);
        enable_free_hook = 1;
    }
    else {
        iter_allocation->len = len;
    }


    return;
}

void print_list() {
    allocation  *iter_allocation;
    char        *addr2line_command;


    setenv("LD_PRELOAD", "", 1);
    puts("\n ------ Unfreed memory ------");
    iter_allocation = allocation_list;
    while(iter_allocation) {
        if(iter_allocation->module_path && !strcmp(iter_allocation->module_path, "./libsut.so")) {
            printf("LEFT: %zu bytes at %p allocated at %p@%s\nLocation in code: \n", iter_allocation->len, iter_allocation->ptr,
                   iter_allocation->base_calling_addr, iter_allocation->module_path);
            addr2line_command = (char *) malloc(100 + strlen(iter_allocation->module_path));
            sprintf(addr2line_command, "addr2line -e %s %p", iter_allocation->module_path,
                iter_allocation->base_calling_addr);
            system(addr2line_command);
            free(addr2line_command);
        }
        else {
            printf("LEFT: %zu bytes at %p allocated\n", iter_allocation->len,
                   iter_allocation->ptr);
        }
        iter_allocation = iter_allocation->next;
    }
    puts(" ------ -------------- ------\n");
    
    return;
}


void free_list() {
    allocation  *iter_allocation,
                *saved_ptr;

    iter_allocation = allocation_list;
    while(iter_allocation) {
	    saved_ptr =  iter_allocation;
	    iter_allocation = iter_allocation->next;
	    free(saved_ptr->module_path);
        free(saved_ptr);
    }


    return;
}
