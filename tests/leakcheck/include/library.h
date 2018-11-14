#ifndef LEAKCHECK_LIBRARY_H
#define LEAKCHECK_LIBRARY_H

static void __attribute__ ((constructor)) lib_init(void);
static void __attribute__ ((destructor)) lib_fin(void);

#endif