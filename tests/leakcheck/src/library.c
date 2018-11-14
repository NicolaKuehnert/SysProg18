#include <library.h>

#include <hooks/malloc_hook.h>
#include <hooks/calloc_hook.h>
#include <hooks/realloc_hook.h>
#include <hooks/free_hook.h>

#include <allocation_list.h>

#include <stdio.h>



static void lib_init(void) {
    printf("MemLeakCheck loaded. \n");

    enable_malloc_hook = 1;
    enable_calloc_hook = 1;
    enable_realloc_hook = 1;
    enable_free_hook = 1;


    return;
}

static void lib_fin(void) {
    enable_malloc_hook = 0;
    enable_calloc_hook = 0;
    enable_realloc_hook = 0;
    enable_free_hook = 0;

    printf("MemLeakCheck finished.\n");

    print_list();

    free_list();

    
    return;
}
