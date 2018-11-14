//
// Created by karl on 16.11.17.
//

#ifndef LEAKCHECK_ALLOCATION_LIST_H
#define LEAKCHECK_ALLOCATION_LIST_H

#include <stddef.h>

typedef struct allocation allocation;

struct allocation {
    void        *ptr;
    size_t      len;
    void        *base_calling_addr;
    char        *module_path;
    allocation  *next;
};

extern allocation *allocation_list;


void insert_allocation(void *, size_t, void *, char *);
void modify_allocation(void *, size_t);
void print_list();
void free_list();

#endif //LEAKCHECK_ALLOCATION_LIST_H
