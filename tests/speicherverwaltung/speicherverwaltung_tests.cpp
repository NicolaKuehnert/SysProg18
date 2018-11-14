#include <string.h>
#include <math.h>
#include <gtest/gtest.h>

extern "C" {
#include "speicherverwaltung/speicherverwaltung.h"
}

namespace testing {
namespace internal {
enum GTestColor {
    COLOR_DEFAULT, COLOR_RED, COLOR_GREEN, COLOR_YELLOW
};
extern void ColoredPrintf(GTestColor color, const char* fmt, ...);
}
}

#define PRINT_INFO(x)   testing::internal::ColoredPrintf(testing::internal::COLOR_GREEN, "[          ] "); testing::internal::ColoredPrintf(testing::internal::COLOR_YELLOW, x)

#define CHECK_MALLOC(p) if (!p) {fprintf(stderr, "NULL pointer: %s (line %d)!\n", __FILE__, __LINE__); exit(EXIT_FAILURE);}
#define TESTSTART freemem=(memblock*)mempool;freemem->size=MEM_POOL_SIZE-sizeof(memblock);freemem->next=NULL;

/* ============================TESTS======================================== */

/*
 * Testet ob die Konstante MEM_POOL_SIZE definiert wurde und diese die
 * korrekte Groesse hat.
 *
 * @param void
 * @return void
 */
TEST(Speicherverwaltung, test_MEM_POOL_SIZE) {
    PRINT_INFO("MEM_POOL_SIZE: Wurde MEM_POOL richtig definiert?\n");

    ASSERT_EQ(MEM_POOL_SIZE, 16384);
}

/*
 * Testet ob die Konstante MAGIC_INT definiert wurde und diese die
 * korrekte Groesse hat.
 *
 * @param void
 * @return void
 */
TEST(Speicherverwaltung, test_MAGIC_INT) {
    PRINT_INFO("MAGIC_INT: Wurde MAGIC_INT richtig definiert?\n");

    ASSERT_EQ(MAGIC_INT, 0xacdcacdc);
}

/*
 * Testet ob die globale Variable mempool angelegt wurde und diese die
 * korrekte Groesse hat.
 *
 * @param void
 * @return void
 */
TEST(Speicherverwaltung, test_mempool_size) {
    PRINT_INFO(
            "mempool: Wurde die globale Variabel mempool richtig definiert?\n");

    ASSERT_EQ(sizeof(mempool), MEM_POOL_SIZE);
}

/*
 * Testet ob die globale Variable freemem angelegt worden ist.
 *
 * @param void
 * @return void
 */
TEST(Speicherverwaltung, test_freemem) {
    PRINT_INFO("free_Mem: Wurde die globale Varible freemem angelegt?\n");

    memblock *x = freemem;

    ASSERT_EQ(x, freemem);
}

/*
 * Testet ob der Datentyp memblock definiert wurde und welche groesse er hat.
 *
 * @param void
 * @return void
 */
TEST(Speicherverwaltung, test_memblock) {
    PRINT_INFO("memblock: Wurde memblock richitg angelegt?\n");

    memblock mb;
    mb.size = 16;
    mb.next = &mb;

    ASSERT_EQ(mb.size, 16);
    ASSERT_EQ(mb.next, &mb);
}

/*
 * Testet ob init_heap den Heap initiiert.
 * Precondition: init_heap() darf noch nicht gelaufen sein!
 *
 * @param void
 * @return void
 */
TEST(Speicherverwaltung, test_init_heap) { /* MUSS als erster Heap-aendernder Test laufen */
    PRINT_INFO("init_heap: Wird der Heap initialisiert?\n");

    memset(mempool, 2, MEM_POOL_SIZE);
    freemem = NULL;

    init_heap();

    ASSERT_EQ(freemem, (memblock* ) mempool);
    ASSERT_EQ(freemem->size, MEM_POOL_SIZE - sizeof(memblock));
    ASSERT_EQ(NULL, freemem->next);
}

/*
 * Testet ob ein doppelter Aufruf von init_heap nichts tut.
 * - zur Kontrolle mit fremem=NULL nach dem ersten Mal.
 *
 * @param void
 * @return void
 */
TEST(Speicherverwaltung, test_init_heap_double_call) {
    PRINT_INFO(
            "init_heap: Wird verhindert, dass man den Heap doppelt initalisiert?\n");

    char dummy[MEM_POOL_SIZE];

    init_heap(); /* mind. ein Aufruf */

    memset(mempool, 2, MEM_POOL_SIZE);
    memset(dummy, 2, MEM_POOL_SIZE);
    freemem = NULL;

    init_heap(); /* tut nix mehr */

    ASSERT_TRUE(memcmp(mempool, dummy, MEM_POOL_SIZE) == 0);
    ASSERT_EQ(NULL, freemem);
}

/*
 * Testet ob ein doppelter Aufruf von init_heap nichts tut.
 * - zur Kontrolle mit size/next==0 nach dem ersten Mal.
 *
 * @param void
 * @param void
 */
TEST(Speicherverwaltung, test_init_heap_double_call_blocks) {
    PRINT_INFO(
            "init_heap: Wird verhindert, dass man den Heap doppelt initalisiert?\n");

    char dummy[MEM_POOL_SIZE];

    init_heap(); /* mind. ein Aufruf */

    memset(mempool, 2, MEM_POOL_SIZE);
    memset(dummy, 2, MEM_POOL_SIZE);
    ((memblock*) mempool)->size = 0;
    ((memblock*) mempool)->next = 0;
    ((memblock*) dummy)->size = 0;
    ((memblock*) dummy)->next = 0;
    freemem = (memblock*) mempool;

    init_heap(); /* tut nix mehr */

    ASSERT_TRUE(memcmp(mempool, dummy, MEM_POOL_SIZE) == 0);
    ASSERT_EQ(freemem, (memblock* ) mempool);
}

/*
 * Testet ob ein doppelter Aufruf von init_heap nichts tut.
 * - zur Kontrolle mit next==MAGIC_INT nach dem ersten Mal.
 *
 * @param void
 * @param void
 */
TEST(Speicherverwaltung, test_init_heap_double_call_blocks_next_magic) {
    PRINT_INFO(
            "init_heap: Wird verhindert, dass man den Heap doppelt initalisiert?\n");

    char dummy[MEM_POOL_SIZE];

    init_heap(); /* mind. ein Aufruf */

    memset(mempool, 2, MEM_POOL_SIZE);
    memset(dummy, 2, MEM_POOL_SIZE);
    ((memblock*) mempool)->size = MEM_POOL_SIZE - sizeof(memblock);
    ((memblock*) mempool)->next = (memblock*) MAGIC_INT;
    ((memblock*) dummy)->size = MEM_POOL_SIZE - sizeof(memblock);
    ((memblock*) dummy)->next = (memblock*) MAGIC_INT;
    freemem = (memblock*) mempool;

    init_heap(); /* tut nix mehr */

    ASSERT_TRUE(memcmp(mempool, dummy, MEM_POOL_SIZE) == 0);
    ASSERT_EQ(freemem, (memblock* ) mempool);
}

/*
 * Testet ob ein doppelter Aufruf von init_heap nichts tut.
 *
 * @param void
 * @param void
 */
TEST(Speicherverwaltung, test_init_heap_double_call_first_char) {
    PRINT_INFO(
            "init_heap: Wird verhindert, dass man den Heap doppelt initalisiert?\n");

    TESTSTART

    init_heap(); /* mind. ein Aufruf */

    ASSERT_EQ(freemem, (memblock* ) mempool);
    ASSERT_EQ(freemem->size, MEM_POOL_SIZE - sizeof(memblock));
    ASSERT_EQ(NULL, freemem->next);

    mempool[0] = 0;

    init_heap(); /* tut nix mehr */

    ASSERT_EQ(mempool[0], 0);
    ASSERT_EQ(freemem, (memblock* ) mempool);
}

/*
 * Testet ob ein doppelter Aufruf von init_heap nichts tut.
 *
 * @param void
 * @param void
 */
TEST(Speicherverwaltung, test_init_heap_double_call_first_char_magic) {
    PRINT_INFO(
            "init_heap: Wird verhindert, dass man den Heap doppelt initalisiert?\n");

    TESTSTART

    init_heap(); /* mind. ein Aufruf */

    ASSERT_EQ(freemem, (memblock* ) mempool);
    ASSERT_EQ(freemem->size, MEM_POOL_SIZE - sizeof(memblock));
    ASSERT_EQ(NULL, freemem->next);

    *((int*) mempool) = MAGIC_INT;

    init_heap(); /* tut nix mehr */

    ASSERT_EQ(*((int* ) mempool), MAGIC_INT);
    ASSERT_EQ(freemem, (memblock* ) mempool);
}

/*
 * Testet ob ein Aufruf von cm_malloc mit null Bytes nichts tut.
 *
 * @param void
 * @return void
 */
TEST(Speicherverwaltung, test_cm_malloc_null_bytes) {
    PRINT_INFO("cm_malloc: Kann man 0 Byte Speicher reservieren?\n");

    TESTSTART

    void *tmp = cm_malloc(0);

    ASSERT_EQ(NULL, tmp);

    ASSERT_EQ(freemem, (memblock* ) mempool);
    ASSERT_EQ(freemem->size, MEM_POOL_SIZE - sizeof(memblock));
    ASSERT_EQ(NULL, freemem->next);
}

#ifndef MALLOCSPLIT
/*
 * Test von cm_malloc mit 1 Byte.
 *
 * @param void
 * @return void
 */
TEST(Speicherverwaltung, test_cm_malloc_one_byte) {
    PRINT_INFO("cm_malloc: Kann man 1 Byte Speicher reservieren?\n");

    TESTSTART

    void *tmp = cm_malloc(1);

    ASSERT_EQ(NULL, freemem);

    ASSERT_EQ((memblock* ) tmp, ((memblock* ) mempool) + 1);
    ASSERT_EQ((((memblock* )tmp) - 1)->size, MEM_POOL_SIZE - sizeof(memblock));
    ASSERT_EQ((((memblock* )tmp) - 1)->next, (memblock* ) MAGIC_INT);
}
#endif

#ifdef MALLOCSPLIT
/*
 * Test von cm_malloc mit 1 Byte.
 *
 * @param void
 * @return void
 */
TEST(Speicherverwaltung, test_cm_malloc_one_byte_with_split) {
    PRINT_INFO("cm_malloc: Kann man 1 Byte Speicher reservieren?\n");

    TESTSTART

    void *tmp = cm_malloc(1);

    ASSERT_EQ(freemem, (memblock* ) (mempool + sizeof(memblock) + 1));
    ASSERT_EQ(freemem->size, MEM_POOL_SIZE - 2 * sizeof(memblock) - 1);
    ASSERT_EQ(NULL, freemem->next);

    ASSERT_EQ((memblock* ) tmp, ((memblock* ) mempool) + 1);
    ASSERT_EQ((((memblock* )tmp) - 1)->size, 1);
    ASSERT_EQ((((memblock* )tmp) - 1)->next, (memblock* ) MAGIC_INT);
}
#endif

/*
 * Test von cm_malloc mit dem gesamten Speicher.
 *
 * @param void
 * @return void
 */
TEST(Speicherverwaltung, test_cm_malloc_all_bytes) {
    PRINT_INFO("cm_malloc: Reservieren des ganzen Speichern moeglich?\n");

    TESTSTART

    void *tmp = cm_malloc(MEM_POOL_SIZE - sizeof(memblock));

    ASSERT_EQ((memblock* ) tmp, ((memblock* ) mempool) + 1);
    ASSERT_EQ((((memblock* )tmp) - 1)->size, MEM_POOL_SIZE - sizeof(memblock));
    ASSERT_EQ((((memblock* )tmp) - 1)->next, (memblock* ) MAGIC_INT);

    ASSERT_EQ(NULL, freemem);
}

/*
 * Test von cm_malloc mit dem gesamten Speicher plus ein Byte.
 *
 * @param void
 * @return void
 */
TEST(Speicherverwaltung, test_cm_malloc_all_plus_one_bytes) {
    PRINT_INFO(
            "cm_malloc: Kann man mehr als die maximale groesse reservieren?\n");

    TESTSTART

    void *tmp = cm_malloc(MEM_POOL_SIZE - sizeof(memblock) + 1);

    ASSERT_EQ(NULL, tmp);

    ASSERT_EQ(freemem, (memblock* ) mempool);
    ASSERT_EQ(freemem->size, MEM_POOL_SIZE - sizeof(memblock));
    ASSERT_EQ(NULL, freemem->next);
}

/*
 * Test von cm_malloc mit ohne freiem Speicher.
 *
 * @param void
 * @return void
 */
TEST(Speicherverwaltung, test_cm_malloc_no_free_mem) {
    PRINT_INFO(
            "cm_malloc: Kann man Speicher reservieren wenn man keinen mehr hat?\n");

    TESTSTART

    freemem = NULL;

    void *tmp = cm_malloc(10);

    ASSERT_EQ(NULL, tmp);
    ASSERT_EQ(NULL, freemem);
}

/*
 * Test von cm_malloc ohne Split.
 *
 * @param void
 * @return void
 */
TEST(Speicherverwaltung, test_cm_malloc_without_split) {
    PRINT_INFO("cm_malloc: Reagiert cm_malloc bei nicht Split?\n");

    TESTSTART

    size_t size = MEM_POOL_SIZE - sizeof(memblock) - 2 * sizeof(memblock);
    void *tmp = cm_malloc(size);

    ASSERT_EQ((memblock* ) tmp, ((memblock* ) mempool) + 1);
    ASSERT_EQ((((memblock* )tmp) - 1)->size, MEM_POOL_SIZE - sizeof(memblock));
    ASSERT_EQ((((memblock* )tmp) - 1)->next, (memblock* ) MAGIC_INT);

    ASSERT_EQ(NULL, freemem);
}

/*
 * Test von cm_malloc ohne Split (limit).
 *
 * @param void
 * @return void
 */
TEST(Speicherverwaltung, test_cm_malloc_without_split_limit) {
    PRINT_INFO("cm_malloc: Split Grenzwert richtig eingehalten?\n");

    TESTSTART

    size_t size = MEM_POOL_SIZE - sizeof(memblock) - 2 * sizeof(memblock) - 32;
    void *tmp = cm_malloc(size);

    ASSERT_EQ((memblock* ) tmp, ((memblock* ) mempool) + 1);
    ASSERT_EQ((((memblock* )tmp) - 1)->size, MEM_POOL_SIZE - sizeof(memblock));
    ASSERT_EQ((((memblock* )tmp) - 1)->next, (memblock* ) MAGIC_INT);

    ASSERT_EQ(NULL, freemem);
}

#ifdef MALLOCSPLIT
/*
 * Test von cm_malloc mit Split (limit).
 *
 * @param void
 * @return void
 */
TEST(Speicherverwaltung, test_cm_malloc_with_split_limit) {
    PRINT_INFO("cm_malloc: Funktioniert der Split?\n");

    TESTSTART

    size_t size = MEM_POOL_SIZE - sizeof(memblock) - 2 * sizeof(memblock) - 33;
    void *tmp = cm_malloc(size);

    ASSERT_EQ((memblock* ) tmp, ((memblock* ) mempool) + 1);
    ASSERT_EQ((((memblock* )tmp) - 1)->size, size);
    ASSERT_EQ((((memblock* )tmp) - 1)->next, (memblock* ) MAGIC_INT);

    ASSERT_EQ(freemem,
            (memblock* ) (mempool + sizeof(memblock) + size));
    ASSERT_EQ(freemem->size,
            MEM_POOL_SIZE - sizeof(memblock) - sizeof(memblock) - size);
    ASSERT_EQ(NULL, freemem->next);
}
#endif

/*
 * Test von cm_malloc (letzer Block alloziert wird).
 *
 * @param void
 * @return void
 */
TEST(Speicherverwaltung, test_cm_malloc_last_block) {
    PRINT_INFO("cm_malloc: Kann man den letzten Block reservieren?\n");

    TESTSTART

    memblock *a, *b, *c, *d, *e, *f;
    void *tmp;
    a = (memblock*) mempool;
    freemem = a;

    b = (memblock*) ((char*) (a + 1) + 100);
    a->size = 100;
    a->next = b;

    c = (memblock*) ((char*) (b + 1) + 140);
    b->size = 140;
    b->next = c;

    d = (memblock*) ((char*) (c + 1) + 100);
    c->size = 100;
    c->next = d;

    e = (memblock*) ((char*) (d + 1) + 500);
    d->size = 500;
    d->next = e;

    f = (memblock*) ((char*) (e + 1) + 160);
    e->size = 160;
    e->next = f;

    f->size = &mempool[MEM_POOL_SIZE - 1] - ((char*) (f + 1));
    f->next = NULL;

    tmp = cm_malloc(&mempool[MEM_POOL_SIZE - 1] - ((char*) (f + 1)));

    ASSERT_EQ(a, freemem);
    ASSERT_EQ(a->next, b);
    ASSERT_EQ(b->next, c);
    ASSERT_EQ(c->next, d);
    ASSERT_EQ(d->next, e);
    ASSERT_EQ(NULL, e->next);
    ASSERT_EQ((char* )tmp, (char* )(f + 1));
    ASSERT_EQ((f->next), (memblock* ) MAGIC_INT);
    ASSERT_EQ(f->size, &mempool[MEM_POOL_SIZE - 1] - ((char*) (f + 1)));
}

/*
 * Test von cm_malloc (kein Split, Grenze).
 *
 * @param void
 * @return void
 */
TEST(Speicherverwaltung, test_cm_malloc_list_no_split_first) {
    PRINT_INFO("cm_malloc: Reagiert Split am Grenzwert richtig?\n");

    TESTSTART

    memblock *a, *b, *c, *d, *e, *f, *g;
    void *tmp;
    a = (memblock*) mempool;
    freemem = a;

    b = (memblock*) ((char*) (a + 1) + 100);
    a->size = 100;
    a->next = b;

    c = (memblock*) ((char*) (b + 1) + 140);
    b->size = 140;
    b->next = c;

    d = (memblock*) ((char*) (c + 1) + 100);
    c->size = 100;
    c->next = d;

    e = (memblock*) ((char*) (d + 1) + 500);
    d->size = 500;
    d->next = e;

    f = (memblock*) ((char*) (e + 1) + 160);
    e->size = 160;
    e->next = f;

    f->size = &mempool[MEM_POOL_SIZE - 1] - ((char*) (f + 1));
    f->next = NULL;

    tmp = cm_malloc(90);
    g = (memblock*) tmp - 1;

    ASSERT_EQ(b->next, c);
    ASSERT_EQ(c->next, d);
    ASSERT_EQ(d->next, e);
    ASSERT_EQ(e->next, f);
    ASSERT_EQ(NULL, f->next);
    ASSERT_EQ((char* )tmp, ((char* )(a + 1)));
    ASSERT_EQ((g->next), (memblock* ) MAGIC_INT);
    ASSERT_EQ(g->size, 100);
    ASSERT_EQ(b, freemem);
}

/*
 * Test von cm_malloc - keine freien Bloecke.
 *
 * @param void
 * @return void
 */
TEST(Speicherverwaltung, test_cm_malloc_no_block_in_list_fits) {
    PRINT_INFO(
            "cm_malloc: Passender freier Block am Ende, der nicht in der Liste ist\n");

    TESTSTART

    memblock *a, *b, *c, *d, *e, *f;
    void *tmp;
    a = (memblock*) mempool;
    freemem = a;

    b = (memblock*) ((char*) (a) + 180);
    a->size = 100;
    a->next = b;

    c = (memblock*) ((char*) (b) + 220);
    b->size = 140;
    b->next = c;

    d = (memblock*) ((char*) (c) + 180);
    c->size = 100;
    c->next = d;

    e = (memblock*) ((char*) (d) + 580);
    d->size = 500;
    d->next = e;

    f = (memblock*) ((char*) (e) + 240);
    e->size = 160;
    e->next = NULL;

    f->size = 1000;
    f->next = NULL;

    tmp = cm_malloc(800);

    ASSERT_EQ(b->next, c);
    ASSERT_EQ(c->next, d);
    ASSERT_EQ(d->next, e);
    ASSERT_EQ(NULL, e->next);
    ASSERT_EQ(NULL, f->next);
    ASSERT_EQ(NULL, tmp);
    ASSERT_EQ(a, freemem);
}

/*
 * Test von cm_malloc - keine freien Bloecke
 * und ein nicht eingehaengter Block in der mitte.
 *
 * @param void
 * @return void
 */
TEST(Speicherverwaltung, test_cm_malloc_no_listed_block_fits_unlisted_in_middle) {
    PRINT_INFO(
            "cm_malloc: Passender freier Block in der Mitte, der nicht in der Liste ist\n");

    TESTSTART

    memblock *a, *b, *c, *d, *e, *f;
    void *tmp;
    a = (memblock*) mempool;
    freemem = a;

    b = (memblock*) ((char*) (a) + 180);
    a->size = 100;
    a->next = b;

    c = (memblock*) ((char*) (b) + 220);
    b->size = 140;
    b->next = c;

    d = (memblock*) ((char*) (c) + 180);
    c->size = 100;
    d->next = NULL;

    e = (memblock*) ((char*) (d) + 580);
    d->size = 500;
    c->next = e;

    f = (memblock*) ((char*) (e) + 240);
    e->size = 160;
    e->next = f;

    f->size = 220;
    f->next = NULL;

    tmp = cm_malloc(400);

    ASSERT_EQ(b->next, c);
    ASSERT_EQ(c->next, e);
    ASSERT_EQ(NULL, d->next);
    ASSERT_EQ(e->next, f);
    ASSERT_EQ(NULL, f->next);
    ASSERT_EQ(NULL, tmp);
    ASSERT_EQ(a, freemem);
}

#ifdef MALLOCSPLIT
/*
 * Test von cm_malloc (mehrere Allokationen nacheinander).
 *
 * @param void
 * @return void
 */
TEST(Speicherverwaltung, test_cm_malloc_iteration) {
    PRINT_INFO("cm_malloc: Kann man cm_malloc mehr als einmal ausfuehren?\n");

    TESTSTART

    memblock *a, *b, *c, *d, *e, *f, *g;
    void *tmp;
    a = (memblock*) mempool;
    freemem = a;

    b = (memblock*) ((char*) (a + 1) + 100);
    a->size = 100;
    a->next = b;

    c = (memblock*) ((char*) (b + 1) + 140);
    b->size = 140;
    b->next = c;

    d = (memblock*) ((char*) (c + 1) + 100);
    c->size = 100;
    c->next = d;

    e = (memblock*) ((char*) (d + 1) + 500);
    d->size = 500;
    d->next = e;

    f = (memblock*) ((char*) (e + 1) + 160);
    e->size = 160;
    e->next = f;

    f->size = &mempool[MEM_POOL_SIZE - 1] - ((char*) (f + 1));
    f->next = NULL;

    tmp = cm_malloc(300);
    g = (memblock*) ((char*) (d + 1) + 300);

    ASSERT_EQ(a, freemem);
    ASSERT_EQ(a->next, b);
    ASSERT_EQ(b->next, c);
    ASSERT_EQ(c->next, g);
    ASSERT_EQ(e->next, f);
    ASSERT_EQ(NULL, f->next);
    ASSERT_EQ(g->next, e);
    ASSERT_EQ(g->size, 200 - sizeof(memblock));
    ASSERT_EQ((char* )tmp, (char* )(d + 1));
    ASSERT_EQ(d->next, (memblock* ) MAGIC_INT);
    ASSERT_EQ(d->size, 300);
}

/*
 * Test von cm_malloc (Split).
 *
 * @param void
 * @return void
 */
TEST(Speicherverwaltung, test_cm_malloc_list_split_first) {
    PRINT_INFO("cm_malloc: Reagiert Split von cm_mallocrichtig?\n");

    TESTSTART

    memblock *a, *b, *c, *d, *e, *f, *g;
    void *tmp;
    a = (memblock*) mempool;
    freemem = a;

    b = (memblock*) ((char*) (a + 1) + 100);
    a->size = 100;
    a->next = b;

    c = (memblock*) ((char*) (b + 1) + 140);
    b->size = 140;
    b->next = c;

    d = (memblock*) ((char*) (c + 1) + 100);
    c->size = 100;
    c->next = d;

    e = (memblock*) ((char*) (d + 1) + 500);
    d->size = 500;
    d->next = e;

    f = (memblock*) ((char*) (e + 1) + 160);
    e->size = 160;
    e->next = f;

    f->size = &mempool[MEM_POOL_SIZE - 1] - ((char*) (f + 1));
    f->next = NULL;

    tmp = cm_malloc(20);
    g = (memblock*) ((char*) (a + 1) + 20);

    ASSERT_EQ(b->next, c);
    ASSERT_EQ(c->next, d);
    ASSERT_EQ(d->next, e);
    ASSERT_EQ(e->next, f);
    ASSERT_EQ(NULL, f->next);
    ASSERT_EQ((char* )tmp, (char* )(a + 1));
    ASSERT_EQ((a->next), (memblock* ) MAGIC_INT);
    ASSERT_EQ(a->size, 20);
    ASSERT_EQ(g, freemem);
    ASSERT_EQ(g->next, b);
    ASSERT_EQ(g->size, 80 - sizeof(memblock));
}
#endif

/*
 * Test von cm_malloc wobei freemem ausserhalb von mempool ist.
 *
 * @param void
 * @return void
 */
TEST(Speicherverwaltung, test_cm_malloc_mem_in_array) {
    PRINT_INFO("cm_malloc: Speicher inherhalb des Arrays?\n");

    TESTSTART

    char dummy[200];

    freemem = (memblock*) dummy;
    freemem->size = 200 - sizeof(memblock);
    freemem->next = NULL;

    void* a = cm_malloc(150);

    ASSERT_EQ(NULL, a);

    ASSERT_EQ(freemem, (memblock* )dummy);
    ASSERT_EQ(freemem->size, 200 - sizeof(memblock));
    ASSERT_EQ(NULL, freemem->next);
}

/*
 * Test von cm_malloc wobei fremem in mempool.
 * Erster freier Block ausserhalb von mempool.
 *
 * @param void
 * @return void
 */
TEST(Speicherverwaltung, test_cm_malloc_outside_mempool) {
    PRINT_INFO("cm_malloc: Speicher ausserhalb des mempool?\n");

    TESTSTART

    char dummy[200];
    memblock *a, *out;

    out = (memblock*) dummy;
    out->size = 300 - sizeof(memblock);
    out->next = NULL;

    a = (memblock*) mempool;
    freemem = a;
    a->size = 100;
    a->next = out;

    void* t = cm_malloc(200);

    ASSERT_EQ(NULL, t);

    ASSERT_EQ(out, (memblock* )dummy);
    ASSERT_EQ(out->size, 300 - sizeof(memblock));
    ASSERT_EQ(NULL, out->next);
}

/*
 * Test von cm_free(): Null-Pointer
 * 4b, 1
 * @return void
 * @param void
 */
TEST(Speicherverwaltung, test_cm_free_null) {
    PRINT_INFO("cm_free: Free mit NULL-pointer?\n");

    TESTSTART

    char mempoolcopy[MEM_POOL_SIZE];
    memcpy(mempoolcopy, mempool, MEM_POOL_SIZE);

    cm_free(NULL);

    ASSERT_EQ((memblock* ) mempool, freemem);
    ASSERT_EQ(memcmp(mempoolcopy, mempool, MEM_POOL_SIZE), 0);
}

/*
 * Test von cm_free() (Ersten Block free'en)
 * 4b, 1
 * @return void
 * @param void
 */
TEST(Speicherverwaltung, test_cm_free_first_block) {
    PRINT_INFO(
            "cm_free: Reagiert freemem richtig wenn man den ersten Blockt freigibt?\n");

    TESTSTART

    memblock *a, *b;
    void *ptr;

    a = (memblock*) mempool;
    b = (memblock*) (mempool + 200);

    a->size = 200 - sizeof(memblock);
    a->next = (memblock*) MAGIC_INT;

    b->size = MEM_POOL_SIZE - 200 - sizeof(memblock);
    b->next = NULL;

    freemem = b;
    ptr = (void*) (mempool + sizeof(memblock));

    cm_free(ptr);

    ASSERT_EQ(freemem, (memblock* ) mempool);
    ASSERT_EQ(a->size, 200 - sizeof(memblock));
    ASSERT_EQ(a->next, b);
    ASSERT_EQ(b->size, MEM_POOL_SIZE - 200 - sizeof(memblock));
    ASSERT_EQ(NULL, b->next);
}

/*
 * Test von cm_free() (In der Mitte free'en)
 * 4b, 1
 * @param void
 * @return void
 */
TEST(Speicherverwaltung, test_cm_free_second_block) {
    PRINT_INFO(
            "cm_free: Reagiert freemem richtig wenn man einen Blockt in der mitte freigibt?\n");

    TESTSTART

    memblock *a, *b, *c;
    void *ptr;

    a = (memblock*) mempool;
    b = (memblock*) (mempool + 200);
    c = (memblock*) (mempool + 500);

    a->size = 200 - sizeof(memblock);
    a->next = NULL;

    b->size = 300 - sizeof(memblock);
    b->next = (memblock*) MAGIC_INT;

    c->size = MEM_POOL_SIZE - 500 - sizeof(memblock);
    c->next = a;

    freemem = c;
    ptr = (void*) (b + 1);

    cm_free(ptr);

    ASSERT_EQ(freemem, b);
    ASSERT_EQ(NULL, a->next);
    ASSERT_EQ(a->size, 200 - sizeof(memblock));
    ASSERT_EQ(b->next, c);
    ASSERT_EQ(b->size, 300 - sizeof(memblock));
    ASSERT_EQ(c->next, a);
    ASSERT_EQ(c->size, MEM_POOL_SIZE - 500 - sizeof(memblock));
}

/*
 * Test von free (kein Speicher).
 * 4b, 1
 * @param void
 * @return void
 */
TEST(Speicherverwaltung, test_cm_free_no_freemem) {
    PRINT_INFO("cm_free: Keine freien Bloecke\n");

    TESTSTART

    memblock *a;
    void *ptr;

    a = (memblock*) mempool;
    a->size = MEM_POOL_SIZE - sizeof(memblock);
    a->next = (memblock*) MAGIC_INT;

    freemem = NULL;
    ptr = (void*) (mempool + sizeof(memblock));

    cm_free(ptr);

    ASSERT_EQ(freemem, a);
    ASSERT_EQ(NULL, a->next);
    ASSERT_EQ(a->size, MEM_POOL_SIZE - sizeof(memblock));
}

/*
 * Test von cm_free() (Fehlermeldung wenn next nicht MAGIC_INT)
 * 4b, 1
 * @param void
 * @return void
 */
TEST(Speicherverwaltung, test_cm_free_wrong_next_null) {
    PRINT_INFO("cm_free: Falscher next Pointer?\n");

    TESTSTART

    memblock *a, *b;
    void *ptr;

    a = (memblock*) mempool;
    b = (memblock*) (mempool + 200);

    a->size = 200 - sizeof(memblock);
    a->next = NULL;

    b->size = MEM_POOL_SIZE - 200 - sizeof(memblock);
    b->next = NULL;

    freemem = b;
    ptr = (void*) (mempool + sizeof(memblock));

    cm_free(ptr);

    ASSERT_EQ(freemem, b);
    ASSERT_EQ(NULL, a->next);
    ASSERT_EQ(a->size, 200 - sizeof(memblock));
    ASSERT_EQ(NULL, b->next);
    ASSERT_EQ(b->size, MEM_POOL_SIZE - 200 - sizeof(memblock));
}

/*
 * Test von cm_free() (Fehlermeldung wenn next nicht MAGIC_INT)
 * 4b, 1
 * @param void
 * @return void
 */
TEST(Speicherverwaltung, test_cm_free_wrong_next_b) {
    PRINT_INFO("cm_free: Falscher Wert im next Pointer?\n");

    TESTSTART

    memblock *a, *b;
    void *ptr;

    a = (memblock*) mempool;
    b = (memblock*) (mempool + 200);

    a->size = 200 - sizeof(memblock);
    a->next = b;

    b->size = MEM_POOL_SIZE - 200 - sizeof(memblock);
    b->next = NULL;

    freemem = b;
    ptr = (void*) (mempool + sizeof(memblock));

    cm_free(ptr);

    ASSERT_EQ(freemem, b);
    ASSERT_EQ(NULL, b->next);
    ASSERT_EQ(b->size, MEM_POOL_SIZE - 200 - sizeof(memblock));
    ASSERT_EQ(a->next, b);
    ASSERT_EQ(a->size, 200 - sizeof(memblock));
}

/*
 * Test von free (Array-Grenzen).
 *
 * Pointer auf anscheinend belegten Block ausserhalb vom Array
 *
 * 4b, 1
 * @param void
 * @return void
 */
TEST(Speicherverwaltung, test_cm_free_wrongblock_outside) {
    PRINT_INFO("cm_free: Arraygrenzen\n");

    TESTSTART

    char reserved_space[100];
    memblock *a;
    void *ptr;

    a = (memblock*) reserved_space;
    a->size = 100 - sizeof(memblock);
    a->next = (memblock*) MAGIC_INT;

    ptr = (void*) (reserved_space + sizeof(memblock));
    cm_free(ptr);

    ASSERT_EQ(freemem, (memblock* ) mempool);
    ASSERT_EQ(NULL, freemem->next);
    ASSERT_EQ(freemem->size, MEM_POOL_SIZE - sizeof(memblock));
    ASSERT_EQ(a->next, (memblock*) MAGIC_INT);
    ASSERT_EQ(a->size, 100 - sizeof(memblock));
}

/*
 * Test von free (Adresse von lokaler Variable +1).
 * 4b, 1
 * @param void
 * @return void
 */
TEST(Speicherverwaltung, test_cm_free_wrong_pointer_localvariable) {
    PRINT_INFO("cm_free: Arraygrenzen\n");

    TESTSTART

    memblock a;

    a.size = 10;
    a.next = (memblock*) MAGIC_INT;

    cm_free(&a + 1);

    ASSERT_EQ(freemem, (memblock* ) mempool);
    ASSERT_EQ(NULL, freemem->next);
    ASSERT_EQ(freemem->size, MEM_POOL_SIZE - sizeof(memblock));
    ASSERT_EQ(a.next, (memblock*) MAGIC_INT);
    ASSERT_EQ(a.size, 10);
}

/*
 * Test von free (ptr zeigt nicht auf den Anfang eines Blocks).
 * 4b, 1
 * @param void
 * @return void
 */
TEST(Speicherverwaltung, test_cm_free_wrong_pointer_mempool) {
    PRINT_INFO("cm_free: ptr zeigt nicht auf Anfang eines Speicherblocks\n");

    TESTSTART

    memblock *a;
    void *ptr;

    a = (memblock*) mempool;
    a->size = MEM_POOL_SIZE - sizeof(memblock);
    a->next = (memblock*) MAGIC_INT;

    ptr = (void*) (mempool + 1);

    cm_free(ptr);

    ASSERT_EQ(freemem, (memblock* ) mempool);
    ASSERT_EQ(a->next, (memblock*) MAGIC_INT);
    ASSERT_EQ(a->size, MEM_POOL_SIZE - sizeof(memblock));
}

/* ************************************************************************* */

/*
 * Test von cm_defrag() (freemem ist NULL)
 * 4b, 2
 * @param void
 * @return void
 */
TEST(Speicherverwaltung, test_cm_defrag_free_mem_null) {
    PRINT_INFO("cm_defrag: Wie reagiert cm_defrag wenn freemem Null ist?\n");

    TESTSTART

    memblock *a, *b;

    a = (memblock*) mempool;
    b = (memblock*) (mempool + 200);
    a->size = 200 - sizeof(memblock);
    a->next = (memblock*) MAGIC_INT;
    b->size = 200 - sizeof(memblock);
    b->next = (memblock*) MAGIC_INT;

    char mempoolcopy[MEM_POOL_SIZE];
    memcpy(mempoolcopy, mempool, MEM_POOL_SIZE);

    freemem = NULL;

    cm_defrag();

    ASSERT_EQ(NULL, freemem);
    ASSERT_EQ(memcmp(mempoolcopy, mempool, MEM_POOL_SIZE), 0);
}

/*
 * Test von cm_defrag() (kein Defrag moeglich)
 * 4b, 2
 * @param void
 * @return void
 */
TEST(Speicherverwaltung, test_cm_defrag_defrag_not_possible) {
    PRINT_INFO("cm_defrag: Was passiert wenn kein Defrag moeglich ist?\n");

    TESTSTART

    memblock *a, *b, *c, *d, *e;

    a = (memblock*) mempool;
    b = (memblock*) (mempool + 200);
    c = (memblock*) (mempool + 400);
    d = (memblock*) (mempool + 600);
    e = (memblock*) (mempool + 1000);

    a->size = 200 - sizeof(memblock);
    a->next = c;

    b->size = 200 - sizeof(memblock);
    b->next = (memblock*) MAGIC_INT;

    c->size = 200 - sizeof(memblock);
    c->next = e;

    d->size = 200 - sizeof(memblock);
    d->next = (memblock*) MAGIC_INT;

    e->size = 400 - sizeof(memblock);
    e->next = NULL;

    freemem = a;

    char mempoolcopy[MEM_POOL_SIZE];
    memcpy(mempoolcopy, mempool, MEM_POOL_SIZE);

    cm_defrag();

    ASSERT_EQ(freemem, a);
    ASSERT_EQ(memcmp(mempoolcopy, mempool, MEM_POOL_SIZE), 0);
}

/*
 * Test von cm_defrag() (Ptr sind aufeinanderfolgend)
 *
 * Beispiel 1 vom Blatt (freispeicher_pre1)
 *
 * 4b, 2
 * @param void
 * @return void
 */
TEST(Speicherverwaltung, test_cm_defrag_ptr_blocks_sorted) {
    PRINT_INFO(
            "cm_defrag: Reagiert cm_defrag wenn die Pointer aufeinander folgenden Blocks?\n");

    TESTSTART

    memblock *a, *b;

    a = (memblock*) mempool;
    b = (memblock*) (mempool + 200);

    a->size = 200 - sizeof(memblock);
    a->next = b;

    b->size = 200;
    b->next = NULL;

    freemem = a;

    cm_defrag();

    ASSERT_EQ(freemem, a);
    ASSERT_EQ(a->size, 200 - sizeof(memblock) + 200 + sizeof(memblock));
    ASSERT_EQ(NULL, a->next);
}

/*
 * Test von cm_defrag() (Ptr sind aufeinanderfolgend)
 *
 * Beispiel 2 vom Blatt (freispeicher_pre2)
 *
 * 4b, 2
 * @param void
 * @return void
 */
TEST(Speicherverwaltung, test_cm_defrag_ptr_blocks_not_sorted2) {
    PRINT_INFO(
            "cm_defrag: Reagiert cm_defrag wenn die Pointer nicht aufeinander folgenden Blocks?\n");

    TESTSTART

    memblock *a, *b;

    a = (memblock*) mempool;
    b = (memblock*) (mempool + 200);

    a->size = 200 - sizeof(memblock);
    a->next = NULL;

    b->size = 200;
    b->next = a;

    freemem = b;

    cm_defrag();

    ASSERT_EQ(freemem, a);
    ASSERT_EQ(a->size, 200 - sizeof(memblock) + 200 + sizeof(memblock));
    ASSERT_EQ(NULL, a->next);
}

/*
 * Test von cm_defrag() (Ptr sind nicht aufeinander folgend)
 * 4b, 2
 * @param void
 * @return void
 */
TEST(Speicherverwaltung, test_cm_defrag_ptr_not_sorted) {
    PRINT_INFO(
            "cm_defrag: Reagiert cm_defrag wenn die Blocks nicht aufeinander folgen?\n");

    TESTSTART

    memblock *a, *b, *c, *d;

    a = (memblock*) mempool;
    b = (memblock*) (mempool + 200);
    c = (memblock*) (mempool + 400);
    d = (memblock*) (mempool + 600);

    a->size = 200 - sizeof(memblock);
    a->next = d;

    b->size = 200 - sizeof(memblock);
    b->next = NULL;

    c->size = 200 - sizeof(memblock);
    c->next = (memblock*) MAGIC_INT;

    d->size = 1000;
    d->next = b;

    freemem = a;

    cm_defrag();

    ASSERT_EQ(freemem, a);
    ASSERT_EQ(a->size, 200 - sizeof(memblock) + b->size + sizeof(memblock));
    ASSERT_EQ(a->next, d);
    ASSERT_EQ(d->size, 1000);
    ASSERT_EQ(NULL, d->next);
    ASSERT_EQ(c->size, 200 - sizeof(memblock));
    ASSERT_EQ(c->next, (memblock*) MAGIC_INT);
}

/*
 * Test von cm_defrag() mit zwei Zusammenfuehrungen.
 * Fuenf Bloecke: der mittlere ist belegt und die jeweils linken
 * und rechten Bloecke sind nicht belegt.
 * 4b, 2
 * @param void
 * @return void
 */
TEST(Speicherverwaltung, test_cm_defrag_two_separate_defrags) {
    PRINT_INFO(
            "cm_defrag: Funktionieren zwei Zusammenfuehrungen mit einem belegten Block in der Mitte?\n");

    TESTSTART

    memblock *a, *b, *c, *d, *e;

    a = (memblock*) mempool;
    b = (memblock*) (mempool + 200);
    c = (memblock*) (mempool + 400);
    d = (memblock*) (mempool + 600);
    e = (memblock*) (mempool + 1000);

    a->size = 200 - sizeof(memblock);
    a->next = d;

    b->size = 200 - sizeof(memblock);
    b->next = e;

    c->size = 200 - sizeof(memblock);
    c->next = (memblock*) MAGIC_INT;

    d->size = 400 - sizeof(memblock);
    d->next = b;

    e->size = 100;
    e->next = NULL;

    freemem = a;

    cm_defrag();

    ASSERT_EQ(freemem, a);
    ASSERT_EQ(a->size, 200 - sizeof(memblock) + b->size + sizeof(memblock));
    ASSERT_EQ(a->next, d);
    ASSERT_EQ(d->size, 400 - sizeof(memblock) + e->size + sizeof(memblock));
    ASSERT_EQ(NULL, d->next);
    ASSERT_EQ(c->size, 200 - sizeof(memblock));
    ASSERT_EQ(c->next, (memblock*) MAGIC_INT);
}

/*
 * Test von cm_defrag() (Defrag mit 3 freien Bloecken)
 * 4b, 2
 * @param void
 * @return void
 */
TEST(Speicherverwaltung, test_cm_defrag_with_three_free_blocks) {
    PRINT_INFO(
            "cm_defrag: Werden drei aneinader grenzende Bloecke zusdammengefasst?\n");

    TESTSTART

    memblock *a, *b;

    a = (memblock*) (mempool + sizeof(memblock) + 100);
    a->size = 100;
    a->next = NULL;

    b = (memblock*) (mempool + sizeof(memblock) * 2 + 200);
    b->size = MEM_POOL_SIZE - (sizeof(memblock) * 3 + 200);
    b->next = a;

    freemem = (memblock*) mempool;
    freemem->size = 100;
    freemem->next = b;

    cm_defrag();

    ASSERT_EQ(freemem, (memblock* ) mempool);
    ASSERT_EQ(freemem->size, MEM_POOL_SIZE - sizeof(memblock));
    ASSERT_EQ(NULL, freemem->next);
}

/*
 * Test von cm_defrag() (Ptr sind nicht aufeinander folgend)
 * 4b, 2
 * @param void
 * @return void
 */
TEST(Speicherverwaltung, test_cm_defrag_ptr_nested) {
    PRINT_INFO(
            "cm_defrag: Reagiert cm_defrag wenn die Bloecke verschachtelt sind (kompletter Pool)?\n");

    TESTSTART

    memblock *a, *b, *c, *d;

    d = (memblock*) mempool;
    b = (memblock*) (mempool + 200);
    a = (memblock*) (mempool + 500);
    c = (memblock*) (mempool + 900);

    d->size = 200 - sizeof(memblock);
    d->next = NULL;

    b->size = 300 - sizeof(memblock);
    b->next = c;

    a->size = 400 - sizeof(memblock);
    a->next = b;

    c->size = MEM_POOL_SIZE - 900 - sizeof(memblock);
    c->next = d;

    freemem = a;

    cm_defrag();

    ASSERT_EQ(freemem, d);
    ASSERT_EQ(freemem->size, MEM_POOL_SIZE - sizeof(memblock));
    ASSERT_EQ(NULL, freemem->next);
}

/*
 * Test von cm_defrag() (Ptr sind nicht aufeinander folgend)
 * 4b, 2
 * @param void
 * @return void
 */
TEST(Speicherverwaltung, test_cm_defrag_ptr_nested_front) {
    PRINT_INFO(
            "cm_defrag: Reagiert cm_defrag wenn die Bloecke verschachtelt sind (belegter Block vorn)?\n");

    TESTSTART

    memblock *a, *b, *c, *d, *x;

    x = (memblock*) mempool;
    d = (memblock*) (mempool + 100);
    b = (memblock*) (mempool + 200);
    a = (memblock*) (mempool + 500);
    c = (memblock*) (mempool + 900);

    x->size = 100 - sizeof(memblock);
    x->next = (memblock*) MAGIC_INT;

    d->size = 100 - sizeof(memblock);
    d->next = NULL;

    b->size = 300 - sizeof(memblock);
    b->next = c;

    a->size = 400 - sizeof(memblock);
    a->next = b;

    c->size = MEM_POOL_SIZE - 900 - sizeof(memblock);
    c->next = d;

    freemem = a;

    cm_defrag();

    ASSERT_EQ(freemem, d);
    ASSERT_EQ(freemem->size, MEM_POOL_SIZE - 100 - sizeof(memblock));
    ASSERT_EQ(NULL, freemem->next);
    ASSERT_EQ(x->size, 100 - sizeof(memblock));
    ASSERT_EQ(x->next, (memblock*) MAGIC_INT);
}

/*
 * Test von cm_defrag() (Ptr sind nicht aufeinander folgend)
 * 4b, 2
 * @param void
 * @return void
 */
TEST(Speicherverwaltung, test_cm_defrag_ptr_nested_end) {
    PRINT_INFO(
            "cm_defrag: Reagiert cm_defrag wenn die Bloecke verschachtelt sind (belegter Block hinten)?\n");

    TESTSTART

    memblock *a, *b, *c, *d, *x;

    d = (memblock*) mempool;
    b = (memblock*) (mempool + 200);
    a = (memblock*) (mempool + 500);
    c = (memblock*) (mempool + 900);
    x = (memblock*) (mempool + 1400);

    d->size = 200 - sizeof(memblock);
    d->next = NULL;

    b->size = 300 - sizeof(memblock);
    b->next = c;

    a->size = 400 - sizeof(memblock);
    a->next = b;

    c->size = 500 - sizeof(memblock);
    c->next = d;

    x->size = MEM_POOL_SIZE - 1400 - sizeof(memblock);
    x->next = (memblock*) MAGIC_INT;

    freemem = a;

    cm_defrag();

    ASSERT_EQ(freemem, d);
    ASSERT_EQ(freemem->size, 1400 - sizeof(memblock));
    ASSERT_EQ(NULL, freemem->next);
    ASSERT_EQ(x->size, MEM_POOL_SIZE - 1400 - sizeof(memblock));
    ASSERT_EQ(x->next, (memblock*) MAGIC_INT);
}

/*
 * Test von cm_defrag() (Ptr sind nicht aufeinander folgend)
 * 4b, 2
 * @param void
 * @return void
 */
TEST(Speicherverwaltung, test_cm_defrag_ptr_nested2) {
    PRINT_INFO(
            "cm_defrag: Reagiert cm_defrag wenn die Bloecke komplexer verschachtelt sind (kompletter Pool)?\n");

    TESTSTART

    memblock *a, *b, *c, *d;

    d = (memblock*) mempool;
    a = (memblock*) (mempool + 200);
    c = (memblock*) (mempool + 500);
    b = (memblock*) (mempool + 900);

    d->size = 200 - sizeof(memblock);
    d->next = NULL;

    a->size = 300 - sizeof(memblock);
    a->next = b;

    c->size = 400 - sizeof(memblock);
    c->next = d;

    b->size = MEM_POOL_SIZE - 900 - sizeof(memblock);
    b->next = c;

    freemem = a;

    cm_defrag();

    ASSERT_EQ(freemem, d);
    ASSERT_EQ(freemem->size, MEM_POOL_SIZE - sizeof(memblock));
    ASSERT_EQ(NULL, freemem->next);
}

/*
 * Test von cm_defrag() (Ptr sind nicht aufeinander folgend)
 * 4b, 2
 * @param void
 * @return void
 */
TEST(Speicherverwaltung, test_cm_defrag_ptr_nested2_front) {
    PRINT_INFO(
            "cm_defrag: Reagiert cm_defrag wenn die Bloecke komplexer verschachtelt sind (belegter Block vorn)?\n");

    TESTSTART

    memblock *a, *b, *c, *d, *x;

    x = (memblock*) mempool;
    d = (memblock*) (mempool + 100);
    a = (memblock*) (mempool + 200);
    c = (memblock*) (mempool + 500);
    b = (memblock*) (mempool + 900);

    x->size = 100 - sizeof(memblock);
    x->next = (memblock*) MAGIC_INT;

    d->size = 100 - sizeof(memblock);
    d->next = NULL;

    a->size = 300 - sizeof(memblock);
    a->next = b;

    c->size = 400 - sizeof(memblock);
    c->next = d;

    b->size = MEM_POOL_SIZE - 900 - sizeof(memblock);
    b->next = c;

    freemem = a;

    cm_defrag();

    ASSERT_EQ(freemem, d);
    ASSERT_EQ(freemem->size, MEM_POOL_SIZE - 100 - sizeof(memblock));
    ASSERT_EQ(NULL, freemem->next);
    ASSERT_EQ(x->size, 100 - sizeof(memblock));
    ASSERT_EQ(x->next, (memblock*) MAGIC_INT);
}

/*
 * Test von cm_defrag() (Ptr sind nicht aufeinander folgend)
 * 4b, 2
 * @param void
 * @return void
 */
TEST(Speicherverwaltung, test_cm_defrag_ptr_nested2_middle) {
    PRINT_INFO(
            "cm_defrag: Reagiert cm_defrag wenn die Bloecke komplexer verschachtelt sind (belegter Block mitte)?\n");

    TESTSTART

    memblock *a, *b, *c, *d, *x;

    d = (memblock*) mempool;
    a = (memblock*) (mempool + 200);
    x = (memblock*) (mempool + 400);
    c = (memblock*) (mempool + 500);
    b = (memblock*) (mempool + 900);

    d->size = 200 - sizeof(memblock);
    d->next = NULL;

    a->size = 200 - sizeof(memblock);
    a->next = b;

    x->size = 100 - sizeof(memblock);
    x->next = (memblock*) MAGIC_INT;

    c->size = 400 - sizeof(memblock);
    c->next = d;

    b->size = MEM_POOL_SIZE - 900 - sizeof(memblock);
    b->next = c;

    freemem = a;

    cm_defrag();

    ASSERT_EQ(freemem, d);
    ASSERT_EQ(freemem->size, 400 - sizeof(memblock));
    ASSERT_EQ(freemem->next, c);
    ASSERT_EQ(c->size, MEM_POOL_SIZE - 500 - sizeof(memblock));
    ASSERT_EQ(NULL, c->next);
    ASSERT_EQ(x->size, 100 - sizeof(memblock));
    ASSERT_EQ(x->next, (memblock*) MAGIC_INT);
}

/*
 * Test von cm_defrag() (Ptr sind nicht aufeinander folgend)
 * 4b, 2
 * @param void
 * @return void
 */
TEST(Speicherverwaltung, test_cm_defrag_ptr_nested2_end) {
    PRINT_INFO(
            "cm_defrag: Reagiert cm_defrag wenn die Bloecke komplexer verschachtelt sind (belegter Block hinten)?\n");

    TESTSTART

    memblock *a, *b, *c, *d, *x;

    d = (memblock*) mempool;
    a = (memblock*) (mempool + 200);
    c = (memblock*) (mempool + 500);
    b = (memblock*) (mempool + 900);
    x = (memblock*) (mempool + 1400);

    d->size = 200 - sizeof(memblock);
    d->next = NULL;

    a->size = 300 - sizeof(memblock);
    a->next = b;

    c->size = 400 - sizeof(memblock);
    c->next = d;

    b->size = 500 - sizeof(memblock);
    b->next = c;

    x->size = MEM_POOL_SIZE - 1400 - sizeof(memblock);
    x->next = (memblock*) MAGIC_INT;

    freemem = a;

    cm_defrag();

    ASSERT_EQ(freemem, d);
    ASSERT_EQ(freemem->size, 1400 - sizeof(memblock));
    ASSERT_EQ(NULL, freemem->next);
    ASSERT_EQ(x->size, MEM_POOL_SIZE - 1400 - sizeof(memblock));
    ASSERT_EQ(x->next, (memblock*) MAGIC_INT);
}

/*
 * Test von cm_defrag() (Ptr sind nicht aufeinander folgend)
 * 4b, 2
 * @param void
 * @return void
 */
TEST(Speicherverwaltung, test_cm_defrag_ptr_nested3) {
    PRINT_INFO(
            "cm_defrag: Reagiert cm_defrag wenn die Bloecke noch komplexer verschachtelt sind (kompletter Pool)?\n");

    TESTSTART

    memblock *a, *b, *c, *d;

    b = (memblock*) mempool;
    c = (memblock*) (mempool + 200);
    d = (memblock*) (mempool + 500);
    a = (memblock*) (mempool + 900);

    b->size = 200 - sizeof(memblock);
    b->next = c;

    c->size = 300 - sizeof(memblock);
    c->next = d;

    d->size = 400 - sizeof(memblock);
    d->next = NULL;

    a->size = MEM_POOL_SIZE - 900 - sizeof(memblock);
    a->next = b;

    freemem = a;

    cm_defrag();

    ASSERT_EQ(freemem, b);
    ASSERT_EQ(freemem->size, MEM_POOL_SIZE - sizeof(memblock));
    ASSERT_EQ(NULL, freemem->next);
}

/*
 * Test von cm_defrag() (Ptr sind nicht aufeinander folgend)
 * 4b, 2
 * @param void
 * @return void
 */
TEST(Speicherverwaltung, test_cm_defrag_ptr_nested4) {
    PRINT_INFO(
            "cm_defrag: Reagiert cm_defrag wenn die Bloecke noch komplexer verschachtelt sind (belegter Block)?\n");

    TESTSTART

    memblock *a, *b, *c, *d, *e, *x;

    d = (memblock*) mempool;
    b = (memblock*) (mempool + 100);
    x = (memblock*) (mempool + 300);
    e = (memblock*) (mempool + 400);
    c = (memblock*) (mempool + 700);
    a = (memblock*) (mempool + 1100);

    d->size = 100 - sizeof(memblock);
    d->next = e;

    b->size = 200 - sizeof(memblock);
    b->next = c;

    x->size = 100 - sizeof(memblock);
    x->next = (memblock*) MAGIC_INT;

    e->size = 300 - sizeof(memblock);
    e->next = NULL;

    c->size = 400 - sizeof(memblock);
    c->next = d;

    a->size = MEM_POOL_SIZE - 1100 - sizeof(memblock);
    a->next = b;

    freemem = a;

    cm_defrag();

    ASSERT_EQ(freemem, e);

    ASSERT_EQ(freemem->size, MEM_POOL_SIZE - 400 - sizeof(memblock));
    ASSERT_EQ(freemem->next, d);

    ASSERT_EQ(d->size, 300 - sizeof(memblock));
    ASSERT_EQ(NULL, d->next);

    ASSERT_EQ(x->size, 100 - sizeof(memblock));
    ASSERT_EQ(x->next, (memblock*) MAGIC_INT);
}

/*
 * Test von cm_defrag() (Ptr sind nicht aufeinander folgend)
 * 4b, 2
 * @param void
 * @return void
 */
TEST(Speicherverwaltung, test_cm_defrag_ptr_nested4_nasty) {
    PRINT_INFO(
            "cm_defrag: Reagiert cm_defrag wenn die Bloecke noch komplexer verschachtelt sind (belegter+freier Block)?\n");

    TESTSTART

    memblock *a, *b, *c, *d, *e, *w, *x;

    d = (memblock*) mempool;
    b = (memblock*) (mempool + 100);
    w = (memblock*) (mempool + 200);
    x = (memblock*) (mempool + 300);
    e = (memblock*) (mempool + 400);
    c = (memblock*) (mempool + 700);
    a = (memblock*) (mempool + 1100);

    d->size = 100 - sizeof(memblock);
    d->next = e;

    b->size = 100 - sizeof(memblock);
    b->next = c;

    w->size = 100 - sizeof(memblock);
    w->next = NULL;

    x->size = 100 - sizeof(memblock);
    x->next = (memblock*) MAGIC_INT;

    e->size = 300 - sizeof(memblock);
    e->next = NULL;

    c->size = 400 - sizeof(memblock);
    c->next = d;

    a->size = MEM_POOL_SIZE - 1100 - sizeof(memblock);
    a->next = b;

    freemem = a;

    cm_defrag();

    ASSERT_EQ(freemem, e);

    ASSERT_EQ(freemem->size, MEM_POOL_SIZE - 400 - sizeof(memblock));
    ASSERT_EQ(freemem->next, d);

    ASSERT_EQ(d->size, 200 - sizeof(memblock));
    ASSERT_EQ(NULL, d->next);

    ASSERT_EQ(w->size, 100 - sizeof(memblock));
    ASSERT_EQ(NULL, w->next);

    ASSERT_EQ(x->size, 100 - sizeof(memblock));
    ASSERT_EQ(x->next, (memblock*) MAGIC_INT);
}

/*
 * Test von cm_defrag() (Ptr sind nicht aufeinander folgend)
 *
 * Beispiel 3 vom Blatt (freispeicher_pre)
 *
 * 4b, 2
 * @param void
 * @return void
 */
TEST(Speicherverwaltung, test_cm_defrag_sketch) {
    PRINT_INFO("cm_defrag: Beispiel vom Uebungsblatt?\n");

    TESTSTART

    memblock *a, *b, *c, *w, *x;

    w = (memblock*) mempool;
    b = (memblock*) (mempool + 100);
    x = (memblock*) (mempool + 300);
    c = (memblock*) (mempool + 600);
    a = (memblock*) (mempool + 1000);

    w->size = 100 - sizeof(memblock);
    w->next = (memblock*) MAGIC_INT;

    b->size = 200 - sizeof(memblock);
    b->next = c;

    x->size = 300 - sizeof(memblock);
    x->next = (memblock*) MAGIC_INT;

    c->size = 400 - sizeof(memblock);
    c->next = NULL;

    a->size = MEM_POOL_SIZE - 1000 - sizeof(memblock);
    a->next = b;

    freemem = a;

    cm_defrag();

    ASSERT_EQ(freemem, c);

    ASSERT_EQ(freemem->size, MEM_POOL_SIZE - 600 - sizeof(memblock));
    ASSERT_EQ(freemem->next, b);

    ASSERT_EQ(b->size, 200 - sizeof(memblock));
    ASSERT_EQ(NULL, b->next);

    ASSERT_EQ(w->size, 100 - sizeof(memblock));
    ASSERT_EQ(w->next, (memblock*) MAGIC_INT);

    ASSERT_EQ(x->size, 300 - sizeof(memblock));
    ASSERT_EQ(x->next, (memblock*) MAGIC_INT);
}

/*
 * Test von cm_defrag() (Defrag mit 3 freien Bloecken, einer nicht verlinkt)
 * 4b, 2
 * @param void
 * @return void
 */
TEST(Speicherverwaltung, test_cm_defrag_with_three_free_blocks_one_not_in_freememlist) {
    PRINT_INFO(
            "cm_defrag: Werden Bloecke die frei sind aber nicht in der Freispeicherliste sind ignoriert?\n");

    TESTSTART

    memblock *a, *b;

    a = (memblock*) (mempool + sizeof(memblock) + 100);
    a->size = 100;
    a->next = NULL;

    b = (memblock*) (mempool + sizeof(memblock) * 2 + 200);
    b->size = MEM_POOL_SIZE - (sizeof(memblock) * 3 + 200);
    b->next = NULL;

    freemem = (memblock*) mempool;
    freemem->size = 100;
    freemem->next = b;

    cm_defrag();

    ASSERT_EQ(freemem, (memblock* ) mempool);
    ASSERT_EQ(freemem->size, 100);
    ASSERT_EQ(freemem->next, b);

    ASSERT_EQ(b, (memblock* )(mempool + sizeof(memblock) * 2 + 200));
    ASSERT_EQ(b->size, MEM_POOL_SIZE - (sizeof(memblock) * 3 + 200));
    ASSERT_EQ(NULL, b->next);

    ASSERT_EQ(a, (memblock* )(mempool + sizeof(memblock) + 100));
    ASSERT_EQ(a->size, 100);
    ASSERT_EQ(NULL, a->next);
}

/*
 * Test, ob cm_defrag() die Grenzen des Arrays beruecksichtigt.
 * 4b, 2
 * @param void
 * @return void
 */
TEST(Speicherverwaltung, test_cm_defrag_freemems_outof_bounds) {
    PRINT_INFO("cm_defrag: Ignoriert Defrag Bloecke auserhalb von mempool?\n");

    TESTSTART

    char dummy[200];
    memset(mempool, 0, MEM_POOL_SIZE);

    memblock* a = (memblock*) (mempool + sizeof(memblock) + 100);
    a->size = 200 - sizeof(memblock);
    a->next = NULL;

    memblock* b = (memblock*) dummy;
    b->size = 200 - sizeof(memblock);
    b->next = a;

    freemem = (memblock*) mempool;
    freemem->size = 100 - sizeof(memblock);
    freemem->next = b;

    cm_defrag();

    ASSERT_EQ(a, (memblock* )(mempool + sizeof(memblock) + 100));
    ASSERT_EQ(NULL, a->next);
    ASSERT_EQ(a->size, 200 - sizeof(memblock));

    ASSERT_EQ(b, (memblock* )dummy);
    ASSERT_EQ(b->next, a);
    ASSERT_EQ(b->size, 200 - sizeof(memblock));

    ASSERT_EQ(freemem, (memblock* )mempool);
    ASSERT_EQ(freemem->next, b);
    ASSERT_EQ(freemem->size, 100 - sizeof(memblock));
}

/*
 * Test von cm_defrag20() (freemem ist NULL)
 * 4b, 2
 * @param void
 * @return void
 */
TEST(Speicherverwaltung, test_cm_defrag20_free_mem_null) {
    PRINT_INFO("cm_defrag20: Wie reagiert cm_defrag wenn freemem Null ist?\n");

    TESTSTART

    memblock *a, *b;

    a = (memblock*) mempool;
    b = (memblock*) (mempool + 200);

    a->size = 200 - sizeof(memblock);
    a->next = (memblock*) MAGIC_INT;

    b->size = MEM_POOL_SIZE - a->size - 2 * sizeof(memblock);
    b->next = (memblock*) MAGIC_INT;

    char mempoolcopy[MEM_POOL_SIZE];
    memcpy(mempoolcopy, mempool, MEM_POOL_SIZE);

    freemem = NULL;

    cm_defrag20();

    ASSERT_EQ(NULL, freemem);
    ASSERT_EQ(memcmp(mempoolcopy, mempool, MEM_POOL_SIZE), 0);
}

/*
 * Test von cm_defrag20() (kein Defrag moeglich)
 * 4b, 2
 * @param void
 * @return void
 */
TEST(Speicherverwaltung, test_cm_defrag20_not_possible) {
    PRINT_INFO("cm_defrag20: Was passiert wenn kein Defrag moeglich ist?\n");

    TESTSTART

    memblock *a, *b, *c, *d, *e;

    a = (memblock*) mempool;
    b = (memblock*) (mempool + 200);
    c = (memblock*) (mempool + 400);
    d = (memblock*) (mempool + 600);
    e = (memblock*) (mempool + 800);

    a->size = 200 - sizeof(memblock);
    a->next = c;

    b->size = 200 - sizeof(memblock);
    b->next = (memblock*) MAGIC_INT;

    c->size = 200 - sizeof(memblock);
    c->next = e;

    d->size = 200 - sizeof(memblock);
    d->next = (memblock*) MAGIC_INT;

    e->size = MEM_POOL_SIZE - a->size - b->size - c->size - d->size
            - 5 * sizeof(memblock);
    e->next = NULL;

    freemem = a;

    char mempoolcopy[MEM_POOL_SIZE];
    memcpy(mempoolcopy, mempool, MEM_POOL_SIZE);

    cm_defrag20();

    ASSERT_EQ(freemem, a);
    ASSERT_EQ(memcmp(mempoolcopy, mempool, MEM_POOL_SIZE), 0);
}

/*
 * Test von cm_defrag20() (Ptr sind aufeinanderfolgend)
 * 4b, 2
 * @param void
 * @return void
 */
TEST(Speicherverwaltung, test_cm_defrag20_ptr_blocks_sorted) {
    PRINT_INFO(
            "cm_defrag20: Reagiert cm_defrag wenn die Pointer aufeinander folgenden Blocks?\n");

    TESTSTART

    memblock *a, *b;

    a = (memblock*) mempool;
    b = (memblock*) (mempool + 200);

    a->size = 200 - sizeof(memblock);
    a->next = b;

    b->size = MEM_POOL_SIZE - a->size - 2 * sizeof(memblock);
    b->next = NULL;

    freemem = a;

    cm_defrag20();

    ASSERT_EQ(freemem, a);
    ASSERT_EQ(a->size, MEM_POOL_SIZE - sizeof(memblock));
    ASSERT_EQ(NULL, a->next);
}

/*
 * Test von cm_defrag20() (Ptr sind nicht aufeinander folgend)
 * 4b, 2
 * @param void
 * @return void
 */
TEST(Speicherverwaltung, test_cm_defrag20_ptr_not_sorted) {
    PRINT_INFO(
            "cm_defrag20: Reagiert cm_defrag wenn die Blocks nicht aufeinander folgen?\n");

    TESTSTART

    memblock *a, *b, *c, *d;

    a = (memblock*) mempool;
    b = (memblock*) (mempool + 200);
    c = (memblock*) (mempool + 400);
    d = (memblock*) (mempool + 600);

    a->size = 200 - sizeof(memblock);
    a->next = d;

    b->size = 200 - sizeof(memblock);
    b->next = NULL;

    c->size = 200 - sizeof(memblock);
    c->next = (memblock*) MAGIC_INT;

    d->size = MEM_POOL_SIZE - a->size - b->size - c->size
            - 4 * sizeof(memblock);
    d->next = b;

    freemem = a;

    cm_defrag20();

    ASSERT_EQ(freemem, a);
    ASSERT_EQ(a->size, 200 - sizeof(memblock) + b->size + sizeof(memblock));
    ASSERT_EQ(a->next, d);
    ASSERT_EQ(d->size,
            MEM_POOL_SIZE - a->size - c->size - 3 * sizeof(memblock));
    ASSERT_EQ(NULL, d->next);
    ASSERT_EQ(c->size, 200 - sizeof(memblock));
    ASSERT_EQ(c->next, (memblock*) MAGIC_INT);
}

/*
 * Test von cm_defrag20() mit zwei Zusammenfuehrungen.
 * Fuenf Bloecke: der mittlere ist belegt und die jeweils linken
 * und rechten Bloecke sind nicht belegt.
 * 4b, 2
 * @param void
 * @return void
 */
TEST(Speicherverwaltung, test_cm_defrag20_two_separate_defrags) {
    PRINT_INFO(
            "cm_defrag20: Funktionieren zwei Zusammenfuehrungen mit einem belegten Block in der Mitte?\n");

    TESTSTART

    memblock *a, *b, *c, *d, *e;

    a = (memblock*) mempool;
    b = (memblock*) (mempool + 200);
    c = (memblock*) (mempool + 400);
    d = (memblock*) (mempool + 600);
    e = (memblock*) (mempool + 1000);

    a->size = 200 - sizeof(memblock);
    a->next = d;

    b->size = 200 - sizeof(memblock);
    b->next = e;

    c->size = 200 - sizeof(memblock);
    c->next = (memblock*) MAGIC_INT;

    d->size = 400 - sizeof(memblock);
    d->next = b;

    e->size = MEM_POOL_SIZE - a->size - b->size - c->size - d->size
            - 5 * sizeof(memblock);
    e->next = NULL;

    freemem = a;

    cm_defrag20();

    ASSERT_EQ(freemem, a);
    ASSERT_EQ(a->size, 200 - sizeof(memblock) + b->size + sizeof(memblock));
    ASSERT_EQ(a->next, d);
    ASSERT_EQ(c->size, 200 - sizeof(memblock));
    ASSERT_EQ(c->next, (memblock*) MAGIC_INT);
    ASSERT_EQ(d->size,
            MEM_POOL_SIZE - a->size - c->size - 3 * sizeof(memblock));
    ASSERT_EQ(NULL, d->next);
}

/*
 * Test von cm_defrag20() (Defrag mit 3 freien Bloecken)
 * 4b, 2
 * @param void
 * @return void
 */
TEST(Speicherverwaltung, test_cm_defrag20_defrag_with_three_free_blocks) {
    PRINT_INFO(
            "cm_defrag20: Funktioniert cm_defrag mit 3 freien Bloecken hintereinander?\n");

    TESTSTART

    memblock *a, *b, *c, *d, *e;

    a = (memblock*) mempool;
    b = (memblock*) (mempool + 200);
    c = (memblock*) (mempool + 400);
    d = (memblock*) (mempool + 600);
    e = (memblock*) (mempool + 1000);

    a->size = 200 - sizeof(memblock);
    a->next = e;

    b->size = 200 - sizeof(memblock);
    b->next = c;

    c->size = 200 - sizeof(memblock);
    c->next = NULL;

    d->size = 400 - sizeof(memblock);
    d->next = (memblock*) MAGIC_INT;

    e->size = MEM_POOL_SIZE - a->size - b->size - c->size - d->size
            - 5 * sizeof(memblock);
    e->next = b;

    freemem = a;

    cm_defrag20();

    ASSERT_EQ(freemem, a);
    ASSERT_EQ(a->size,
            200 - sizeof(memblock) + b->size + sizeof(memblock) + c->size
                    + sizeof(memblock));
    ASSERT_EQ(a->next, e);
    ASSERT_EQ(d->size, 400 - sizeof(memblock));
    ASSERT_EQ(d->next, (memblock*) MAGIC_INT);
    ASSERT_EQ(e->size,
            MEM_POOL_SIZE - a->size - d->size - 3 * sizeof(memblock));
    ASSERT_EQ(NULL, c->next);
}

/*
 * Test von cm_defrag20() (Ptr sind nicht aufeinander folgend)
 * 4b, 2
 * @param void
 * @return void
 */
TEST(Speicherverwaltung, test_cm_defrag20_ptr_nested) {
    PRINT_INFO(
            "cm_defrag20: Reagiert cm_defrag20 wenn die Bloecke verschachtelt sind (kompletter Pool)?\n");

    TESTSTART

    memblock *a, *b, *c, *d;

    d = (memblock*) mempool;
    b = (memblock*) (mempool + 200);
    a = (memblock*) (mempool + 500);
    c = (memblock*) (mempool + 900);

    d->size = 200 - sizeof(memblock);
    d->next = NULL;

    b->size = 300 - sizeof(memblock);
    b->next = c;

    a->size = 400 - sizeof(memblock);
    a->next = b;

    c->size = MEM_POOL_SIZE - 900 - sizeof(memblock);
    c->next = d;

    freemem = a;

    cm_defrag20();

    ASSERT_EQ(freemem, d);
    ASSERT_EQ(freemem->size, MEM_POOL_SIZE - sizeof(memblock));
    ASSERT_EQ(NULL, freemem->next);
}

/*
 * Test von cm_defrag20() (Ptr sind nicht aufeinander folgend)
 * 4b, 2
 * @param void
 * @return void
 */
TEST(Speicherverwaltung, test_cm_defrag20_ptr_nested_front) {
    PRINT_INFO(
            "cm_defrag20: Reagiert cm_defrag20 wenn die Bloecke verschachtelt sind (belegter Block vorn)?\n");

    TESTSTART

    memblock *a, *b, *c, *d, *x;

    x = (memblock*) mempool;
    d = (memblock*) (mempool + 100);
    b = (memblock*) (mempool + 200);
    a = (memblock*) (mempool + 500);
    c = (memblock*) (mempool + 900);

    x->size = 100 - sizeof(memblock);
    x->next = (memblock*) MAGIC_INT;

    d->size = 100 - sizeof(memblock);
    d->next = NULL;

    b->size = 300 - sizeof(memblock);
    b->next = c;

    a->size = 400 - sizeof(memblock);
    a->next = b;

    c->size = MEM_POOL_SIZE - 900 - sizeof(memblock);
    c->next = d;

    freemem = a;

    cm_defrag20();

    ASSERT_EQ(freemem, d);
    ASSERT_EQ(freemem->size, MEM_POOL_SIZE - 100 - sizeof(memblock));
    ASSERT_EQ(NULL, freemem->next);
    ASSERT_EQ(x->size, 100 - sizeof(memblock));
    ASSERT_EQ(x->next, (memblock*) MAGIC_INT);
}

/*
 * Test von cm_defrag20() (Ptr sind nicht aufeinander folgend)
 * 4b, 2
 * @param void
 * @return void
 */
TEST(Speicherverwaltung, test_cm_defrag20_ptr_nested_end) {
    PRINT_INFO(
            "cm_defrag20: Reagiert cm_defrag20 wenn die Bloecke verschachtelt sind (belegter Block hinten)?\n");

    TESTSTART

    memblock *a, *b, *c, *d, *x;

    d = (memblock*) mempool;
    b = (memblock*) (mempool + 200);
    a = (memblock*) (mempool + 500);
    c = (memblock*) (mempool + 900);
    x = (memblock*) (mempool + 1400);

    d->size = 200 - sizeof(memblock);
    d->next = NULL;

    b->size = 300 - sizeof(memblock);
    b->next = c;

    a->size = 400 - sizeof(memblock);
    a->next = b;

    c->size = 500 - sizeof(memblock);
    c->next = d;

    x->size = MEM_POOL_SIZE - 1400 - sizeof(memblock);
    x->next = (memblock*) MAGIC_INT;

    freemem = a;

    cm_defrag20();

    ASSERT_EQ(freemem, d);
    ASSERT_EQ(freemem->size, 1400 - sizeof(memblock));
    ASSERT_EQ(NULL, freemem->next);
    ASSERT_EQ(x->size, MEM_POOL_SIZE - 1400 - sizeof(memblock));
    ASSERT_EQ(x->next, (memblock*) MAGIC_INT);
}

/*
 * Test von cm_defrag20() (Ptr sind nicht aufeinander folgend)
 * 4b, 2
 * @param void
 * @return void
 */
TEST(Speicherverwaltung, test_cm_defrag20_ptr_nested2) {
    PRINT_INFO(
            "cm_defrag20: Reagiert cm_defrag20 wenn die Bloecke komplexer verschachtelt sind (kompletter Pool)?\n");

    TESTSTART

    memblock *a, *b, *c, *d;

    d = (memblock*) mempool;
    a = (memblock*) (mempool + 200);
    c = (memblock*) (mempool + 500);
    b = (memblock*) (mempool + 900);

    d->size = 200 - sizeof(memblock);
    d->next = NULL;

    a->size = 300 - sizeof(memblock);
    a->next = b;

    c->size = 400 - sizeof(memblock);
    c->next = d;

    b->size = MEM_POOL_SIZE - 900 - sizeof(memblock);
    b->next = c;

    freemem = a;

    cm_defrag20();

    ASSERT_EQ(freemem, d);
    ASSERT_EQ(freemem->size, MEM_POOL_SIZE - sizeof(memblock));
    ASSERT_EQ(NULL, freemem->next);
}

/*
 * Test von cm_defrag20() (Ptr sind nicht aufeinander folgend)
 * 4b, 2
 * @param void
 * @return void
 */
TEST(Speicherverwaltung, test_cm_defrag20_ptr_nested2_front) {
    PRINT_INFO(
            "cm_defrag20: Reagiert cm_defrag20 wenn die Bloecke komplexer verschachtelt sind (belegter Block vorn)?\n");

    TESTSTART

    memblock *a, *b, *c, *d, *x;

    x = (memblock*) mempool;
    d = (memblock*) (mempool + 100);
    a = (memblock*) (mempool + 200);
    c = (memblock*) (mempool + 500);
    b = (memblock*) (mempool + 900);

    x->size = 100 - sizeof(memblock);
    x->next = (memblock*) MAGIC_INT;

    d->size = 100 - sizeof(memblock);
    d->next = NULL;

    a->size = 300 - sizeof(memblock);
    a->next = b;

    c->size = 400 - sizeof(memblock);
    c->next = d;

    b->size = MEM_POOL_SIZE - 900 - sizeof(memblock);
    b->next = c;

    freemem = a;

    cm_defrag20();

    ASSERT_EQ(freemem, d);
    ASSERT_EQ(freemem->size, MEM_POOL_SIZE - 100 - sizeof(memblock));
    ASSERT_EQ(NULL, freemem->next);
    ASSERT_EQ(x->size, 100 - sizeof(memblock));
    ASSERT_EQ(x->next, (memblock*) MAGIC_INT);
}

/*
 * Test von cm_defrag20() (Ptr sind nicht aufeinander folgend)
 * 4b, 2
 * @param void
 * @return void
 */
TEST(Speicherverwaltung, test_cm_defrag20_ptr_nested2_middle) {
    PRINT_INFO(
            "cm_defrag20: Reagiert cm_defrag20 wenn die Bloecke komplexer verschachtelt sind (belegter Block mitte)?\n");

    TESTSTART

    memblock *a, *b, *c, *d, *x;

    d = (memblock*) mempool;
    a = (memblock*) (mempool + 200);
    x = (memblock*) (mempool + 400);
    c = (memblock*) (mempool + 500);
    b = (memblock*) (mempool + 900);

    d->size = 200 - sizeof(memblock);
    d->next = NULL;

    a->size = 200 - sizeof(memblock);
    a->next = b;

    x->size = 100 - sizeof(memblock);
    x->next = (memblock*) MAGIC_INT;

    c->size = 400 - sizeof(memblock);
    c->next = d;

    b->size = MEM_POOL_SIZE - 900 - sizeof(memblock);
    b->next = c;

    freemem = a;

    cm_defrag20();

    ASSERT_EQ(freemem, d);
    ASSERT_EQ(freemem->size, 400 - sizeof(memblock));
    ASSERT_EQ(freemem->next, c);
    ASSERT_EQ(c->size, MEM_POOL_SIZE - 500 - sizeof(memblock));
    ASSERT_EQ(NULL, c->next);
    ASSERT_EQ(x->size, 100 - sizeof(memblock));
    ASSERT_EQ(x->next, (memblock*) MAGIC_INT);
}

/*
 * Test von cm_defrag20() (Ptr sind nicht aufeinander folgend)
 * 4b, 2
 * @param void
 * @return void
 */
TEST(Speicherverwaltung, test_cm_defrag20_ptr_nested2_end) {
    PRINT_INFO(
            "cm_defrag20: Reagiert cm_defrag20 wenn die Bloecke komplexer verschachtelt sind (belegter Block hinten)?\n");

    TESTSTART

    memblock *a, *b, *c, *d, *x;

    d = (memblock*) mempool;
    a = (memblock*) (mempool + 200);
    c = (memblock*) (mempool + 500);
    b = (memblock*) (mempool + 900);
    x = (memblock*) (mempool + 1400);

    d->size = 200 - sizeof(memblock);
    d->next = NULL;

    a->size = 300 - sizeof(memblock);
    a->next = b;

    c->size = 400 - sizeof(memblock);
    c->next = d;

    b->size = 500 - sizeof(memblock);
    b->next = c;

    x->size = MEM_POOL_SIZE - 1400 - sizeof(memblock);
    x->next = (memblock*) MAGIC_INT;

    freemem = a;

    cm_defrag20();

    ASSERT_EQ(freemem, d);
    ASSERT_EQ(freemem->size, 1400 - sizeof(memblock));
    ASSERT_EQ(NULL, freemem->next);
    ASSERT_EQ(x->size, MEM_POOL_SIZE - 1400 - sizeof(memblock));
    ASSERT_EQ(x->next, (memblock*) MAGIC_INT);
}

/*
 * Test von cm_defrag20() (Ptr sind nicht aufeinander folgend)
 * 4b, 2
 * @param void
 * @return void
 */
TEST(Speicherverwaltung, test_cm_defrag20_ptr_nested3) {
    PRINT_INFO(
            "cm_defrag20: Reagiert cm_defrag20 wenn die Bloecke noch komplexer verschachtelt sind (kompletter Pool)?\n");

    TESTSTART

    memblock *a, *b, *c, *d;

    b = (memblock*) mempool;
    c = (memblock*) (mempool + 200);
    d = (memblock*) (mempool + 500);
    a = (memblock*) (mempool + 900);

    b->size = 200 - sizeof(memblock);
    b->next = c;

    c->size = 300 - sizeof(memblock);
    c->next = d;

    d->size = 400 - sizeof(memblock);
    d->next = NULL;

    a->size = MEM_POOL_SIZE - 900 - sizeof(memblock);
    a->next = b;

    freemem = a;

    cm_defrag20();

    ASSERT_EQ(freemem, b);
    ASSERT_EQ(freemem->size, MEM_POOL_SIZE - sizeof(memblock));
    ASSERT_EQ(NULL, freemem->next);
}

/*
 * Test von cm_defrag20() (Ptr sind nicht aufeinander folgend)
 * 4b, 2
 * @param void
 * @return void
 */
TEST(Speicherverwaltung, test_cm_defrag20_ptr_nested4) {
    PRINT_INFO(
            "cm_defrag20: Reagiert cm_defrag20 wenn die Bloecke noch komplexer verschachtelt sind (belegter Block)?\n");

    TESTSTART

    memblock *a, *b, *c, *d, *e, *x;

    d = (memblock*) mempool;
    b = (memblock*) (mempool + 100);
    x = (memblock*) (mempool + 300);
    e = (memblock*) (mempool + 400);
    c = (memblock*) (mempool + 700);
    a = (memblock*) (mempool + 1100);

    d->size = 100 - sizeof(memblock);
    d->next = e;

    b->size = 200 - sizeof(memblock);
    b->next = c;

    x->size = 100 - sizeof(memblock);
    x->next = (memblock*) MAGIC_INT;

    e->size = 300 - sizeof(memblock);
    e->next = NULL;

    c->size = 400 - sizeof(memblock);
    c->next = d;

    a->size = MEM_POOL_SIZE - 1100 - sizeof(memblock);
    a->next = b;

    freemem = a;

    cm_defrag20();

    ASSERT_EQ(freemem, d);

    ASSERT_EQ(freemem->size, 300 - sizeof(memblock));
    ASSERT_EQ(freemem->next, e);

    ASSERT_EQ(e->size, MEM_POOL_SIZE - 400 - sizeof(memblock));
    ASSERT_EQ(NULL, e->next);

    ASSERT_EQ(x->size, 100 - sizeof(memblock));
    ASSERT_EQ(x->next, (memblock*) MAGIC_INT);
}

/*
 * Test von cm_defrag20() (Ptr sind nicht aufeinander folgend)
 * 4b, 2
 * @param void
 * @return void
 */
TEST(Speicherverwaltung, test_cm_defrag20_ptr_nested4_nasty) {
    PRINT_INFO(
            "cm_defrag20: Reagiert cm_defrag20 wenn die Bloecke noch komplexer verschachtelt sind (belegter+freier Block)?\n");

    TESTSTART

    memblock *a, *b, *c, *d, *e, *w, *x;

    d = (memblock*) mempool;
    b = (memblock*) (mempool + 100);
    w = (memblock*) (mempool + 200);
    x = (memblock*) (mempool + 300);
    e = (memblock*) (mempool + 400);
    c = (memblock*) (mempool + 700);
    a = (memblock*) (mempool + 1100);

    d->size = 100 - sizeof(memblock);
    d->next = e;

    b->size = 100 - sizeof(memblock);
    b->next = c;

    w->size = 100 - sizeof(memblock);
    w->next = NULL;

    x->size = 100 - sizeof(memblock);
    x->next = (memblock*) MAGIC_INT;

    e->size = 300 - sizeof(memblock);
    e->next = NULL;

    c->size = 400 - sizeof(memblock);
    c->next = d;

    a->size = MEM_POOL_SIZE - 1100 - sizeof(memblock);
    a->next = b;

    freemem = a;

    cm_defrag20();

    ASSERT_EQ(freemem, d);

    ASSERT_EQ(freemem->size, 300 - sizeof(memblock));
    ASSERT_EQ(freemem->next, e);

    ASSERT_EQ(e->size, MEM_POOL_SIZE - 400 - sizeof(memblock));
    ASSERT_EQ(NULL, e->next);

    ASSERT_EQ(x->size, 100 - sizeof(memblock));
    ASSERT_EQ(x->next, (memblock*) MAGIC_INT);
}

/*
 * Test von cm_defrag20() (kein Defrag moeglich, Umsortierung Liste)
 * 4b, 2
 * @param void
 * @return void
 */
TEST(Speicherverwaltung, test_cm_defrag20_not_possible_nasty) {
    PRINT_INFO(
            "cm_defrag20: Was passiert wenn kein Defrag moeglich ist (Umsortierung Liste)?\n");

    TESTSTART

    memblock *a, *b, *c, *d, *e;

    a = (memblock*) mempool;
    b = (memblock*) (mempool + 200);
    c = (memblock*) (mempool + 400);
    d = (memblock*) (mempool + 600);
    e = (memblock*) (mempool + 800);

    a->size = 200 - sizeof(memblock);
    a->next = NULL;

    b->size = 200 - sizeof(memblock);
    b->next = (memblock*) MAGIC_INT;

    c->size = 200 - sizeof(memblock);
    c->next = a;

    d->size = 200 - sizeof(memblock);
    d->next = (memblock*) MAGIC_INT;

    e->size = MEM_POOL_SIZE - a->size - b->size - c->size - d->size
            - 5 * sizeof(memblock);
    e->next = c;

    freemem = e;

    cm_defrag20();

    ASSERT_EQ(freemem, a);
    ASSERT_EQ(freemem->size, 200 - sizeof(memblock));
    ASSERT_EQ(freemem->next, c);

    ASSERT_EQ(c->size, 200 - sizeof(memblock));
    ASSERT_EQ(c->next, e);

    ASSERT_EQ(e->size, MEM_POOL_SIZE - 800 - sizeof(memblock));
    ASSERT_EQ(NULL, e->next);

    ASSERT_EQ(b->size, 200 - sizeof(memblock));
    ASSERT_EQ(b->next, (memblock*) MAGIC_INT);

    ASSERT_EQ(d->size, 200 - sizeof(memblock));
    ASSERT_EQ(d->next, (memblock*) MAGIC_INT);
}

/*
 * Test von cm_defrag20() (Ptr sind nicht aufeinander folgend)
 * Beispiel (Skizze) vom Uebungsblatt
 * 4b, 2
 * @param void
 * @return void
 */
TEST(Speicherverwaltung, test_cm_defrag20_sketch) {
    PRINT_INFO("cm_defrag20: Beispiel vom Uebungsblatt?\n");

    TESTSTART

    memblock *a, *b, *c, *w, *x;

    w = (memblock*) mempool;
    b = (memblock*) (mempool + 100);
    x = (memblock*) (mempool + 300);
    c = (memblock*) (mempool + 600);
    a = (memblock*) (mempool + 1000);

    w->size = 100 - sizeof(memblock);
    w->next = (memblock*) MAGIC_INT;

    b->size = 200 - sizeof(memblock);
    b->next = c;

    x->size = 300 - sizeof(memblock);
    x->next = (memblock*) MAGIC_INT;

    c->size = 400 - sizeof(memblock);
    c->next = NULL;

    a->size = MEM_POOL_SIZE - 1000 - sizeof(memblock);
    a->next = b;

    freemem = a;

    cm_defrag20();

    ASSERT_EQ(freemem, b);

    ASSERT_EQ(freemem->size, 200 - sizeof(memblock));
    ASSERT_EQ(freemem->next, c);

    ASSERT_EQ(c->size, MEM_POOL_SIZE - 600 - sizeof(memblock));
    ASSERT_EQ(NULL, c->next);

    ASSERT_EQ(w->size, 100 - sizeof(memblock));
    ASSERT_EQ(w->next, (memblock*) MAGIC_INT);

    ASSERT_EQ(x->size, 300 - sizeof(memblock));
    ASSERT_EQ(x->next, (memblock*) MAGIC_INT);
}

/* ************************************************************************* */

char* cm_memcpy_helpsentence_32(char *d) {
    *d = 'D';
    *(d + 1) = 'I';
    *(d + 2) = 'E';
    *(d + 3) = 'S';
    *(d + 4) = ' ';
    *(d + 5) = 'I';
    *(d + 6) = 'S';
    *(d + 7) = 'T';
    *(d + 8) = ' ';
    *(d + 9) = 'E';
    *(d + 10) = 'I';
    *(d + 11) = 'N';
    *(d + 12) = ' ';
    *(d + 13) = 'T';
    *(d + 14) = 'E';
    *(d + 15) = 'S';
    *(d + 16) = 'T';
    *(d + 17) = ' ';
    *(d + 18) = 'S';
    *(d + 19) = 'U';
    *(d + 20) = 'P';
    *(d + 21) = 'E';
    *(d + 22) = 'R';
    *(d + 23) = ' ';
    *(d + 24) = 'W';
    *(d + 25) = 'I';
    *(d + 26) = 'C';
    *(d + 27) = 'H';
    *(d + 28) = 'T';
    *(d + 29) = 'I';
    *(d + 30) = 'G';
    *(d + 31) = '\0';

    return d;
}

char* cm_memcpy_helpsentence_random(char *randomstring, int size) {
    static char charset[] = "abcdefghijklmnopqrstuvwxyz";

    if (size) {
        if (randomstring) {
            for (int n = 0; n < size; n++) {
                int key = rand() % (int) (sizeof(charset) - 1);
                randomstring[n] = charset[key];
            }
            randomstring[size] = '\0';
        }
    }
    return randomstring;
}

/*
 * Test von cm_memcpy() (size = 0)
 * 4b, 3
 * @param void
 * @param void
 */
TEST(Speicherverwaltung, test_cm_memcpy_size_null) {
    PRINT_INFO("cm_memcpy: Kann man nichts kopieren?\n");

    TESTSTART

    memblock *dBlock, *sBlock;
    void *dest, *src, *ptr;
    char *d, *s;
    int rtc = 32; /* range to compare */

    memset(mempool, 0, sizeof(mempool));

    dBlock = (memblock*) mempool;
    dBlock->size = 35;
    dBlock->next = (memblock*) MAGIC_INT;
    dest = mempool + sizeof(memblock);
    d = (char*) dest;
    d = cm_memcpy_helpsentence_32(d);

    sBlock = (memblock*) (mempool + dBlock->size + sizeof(memblock));
    sBlock->size = 35;
    sBlock->next = (memblock*) MAGIC_INT;
    src = mempool + 2 * sizeof(memblock) + dBlock->size;
    s = (char*) src;
    s = cm_memcpy_helpsentence_random(s, sBlock->size);

    ptr = cm_memcpy(dest, src, 0);

    ASSERT_NE(memcmp(d, s, rtc), 0);
    ASSERT_EQ(ptr, dest);
}

/*
 * Test von cm_memcpy() (dest = 0)
 * 4b, 3
 * @param void
 * @param void
 */
TEST(Speicherverwaltung, test_cm_memcpy_dest_null) {
    PRINT_INFO("cm_memcpy: Kann die groesse des Zielorts 0 sein?\n");

    TESTSTART

    memblock *sBlock;
    void *src, *ptr;
    char *s;
    int rtc = 32; /* range to compare */

    memset(mempool, 0, sizeof(mempool));

    sBlock = (memblock*) mempool;
    sBlock->size = rtc;
    sBlock->next = (memblock*) MAGIC_INT;
    src = mempool + 1;
    s = (char*) src;
    s = cm_memcpy_helpsentence_random(s, rtc);

    char c[rtc];
    memcpy(c, s, rtc);

    ptr = cm_memcpy(NULL, src, rtc);

    ASSERT_EQ(memcmp(c, s, rtc), 0);
    ASSERT_EQ(NULL, ptr);
}

/*
 * Test von cm_memcpy() (src = 0)
 * 4b, 3
 * @param void
 * @param void
 */
TEST(Speicherverwaltung, test_cm_memcpy_src_null) {
    PRINT_INFO("cm_memcpy: Kann man nichts kopieren?\n");

    TESTSTART

    memblock *dBlock;
    void *dest, *ptr;
    char *d;
    int rtc = 32; /* range to compare */

    memset(mempool, 0, sizeof(mempool));

    dBlock = (memblock*) mempool;
    dBlock->size = rtc;
    dBlock->next = (memblock*) MAGIC_INT;
    dest = mempool + sizeof(memblock);
    d = (char*) dest;
    d = cm_memcpy_helpsentence_random(d, rtc);

    char c[rtc];
    memcpy(c, d, rtc);

    ptr = cm_memcpy(dest, NULL, rtc);

    ASSERT_EQ(memcmp(c, d, rtc), 0);
    ASSERT_EQ(ptr, dest);
}

/*
 * Test von cm_memcpy() (copy moeglich)
 * 4b, 3
 * @param void
 * @param void
 */
TEST(Speicherverwaltung, test_cm_memcpy) {
    PRINT_INFO("cm_memcpy: Funktioniert das kopieren?\n");

    TESTSTART

    memblock *dBlock, *sBlock;
    void *dest, *src, *ptr;
    char *d, *s;

    memset(mempool, 0, sizeof(mempool));

    dBlock = (memblock*) mempool;
    dBlock->size = 20;
    dBlock->next = (memblock*) MAGIC_INT;
    dest = mempool + sizeof(memblock);
    d = (char*) dest;
    *d = 'x';
    *(d + 1) = 'x';
    *(d + 2) = 'x';
    *(d + 3) = 'x';
    *(d + 4) = 'x';
    *(d + 5) = 'x';
    *(d + 6) = 'x';
    *(d + 7) = 'x';
    *(d + 8) = 'x';

    sBlock = (memblock*) (mempool + dBlock->size + sizeof(memblock));
    sBlock->size = 100;
    sBlock->next = (memblock*) MAGIC_INT;
    src = mempool + 2 * sizeof(memblock) + dBlock->size;
    s = (char*) src;
    *s = 'H';
    *(s + 1) = 'A';
    *(s + 2) = 'L';
    *(s + 3) = 'L';
    *(s + 4) = 'O';
    *(s + 5) = 0;
    *(s + 6) = 'A';
    *(s + 7) = 'B';
    *(s + 8) = 'C';

    ptr = cm_memcpy(dest, src, 7);

    ASSERT_EQ(ptr, dest);
    ASSERT_EQ(*d, 'H');
    ASSERT_EQ(*(d + 1), 'A');
    ASSERT_EQ(*(d + 2), 'L');
    ASSERT_EQ(*(d + 3), 'L');
    ASSERT_EQ(*(d + 4), 'O');
    ASSERT_EQ(*(d + 5), 0);
    ASSERT_EQ(*(d + 6), 'A');
    ASSERT_EQ(*(d + 7), 'x');
    ASSERT_EQ(*(d + 8), 'x');
    ASSERT_EQ(*s, 'H');
    ASSERT_EQ(*(s + 1), 'A');
    ASSERT_EQ(*(s + 2), 'L');
    ASSERT_EQ(*(s + 3), 'L');
    ASSERT_EQ(*(s + 4), 'O');
    ASSERT_EQ(*(s + 5), 0);
    ASSERT_EQ(*(s + 6), 'A');
    ASSERT_EQ(*(s + 7), 'B');
    ASSERT_EQ(*(s + 8), 'C');
}

/*
 * Test von cm_memcpy() (dest ist kleiner als size)
 *
 * 5, 3
 * @param void
 * @param void
 */
TEST(Speicherverwaltung, test_cm_memcpy_dest_too_small) {
    PRINT_INFO("cm_memcpy: Reicht der Speicher im Ziel?\n");

    TESTSTART

    memblock *dBlock, *sBlock, *zBlock;
    void *dest, *src, *ptr;
    char *d, *s;
    int rtc = 60; /* range to compare */

    memset(mempool, 0, sizeof(mempool));

    dBlock = (memblock*) mempool;
    dBlock->size = 32;
    dBlock->next = (memblock*) MAGIC_INT;
    dest = dBlock + 1;
    d = (char*) dest;
    d = cm_memcpy_helpsentence_32(d);

    sBlock = (memblock*) (&mempool[MEM_POOL_SIZE - 1] - 100 - sizeof(memblock));
    sBlock->size = 100;
    sBlock->next = (memblock*) MAGIC_INT;
    src = sBlock + 1;
    s = (char*) src;
    s = cm_memcpy_helpsentence_random(s, sBlock->size);
    *(s + 8) = 'T';
    *(s + 28) = 'F';

    zBlock = (memblock*) ((char*) (dBlock + 1) + dBlock->size);
    zBlock->size = MEM_POOL_SIZE - 2 * sizeof(memblock) - dBlock->size
            - sBlock->size;

    ASSERT_NE(memcmp(d, s, rtc), 0);

    ptr = cm_memcpy(dest, src, rtc);

    ASSERT_EQ(memcmp(d, s, rtc), 0);
    ASSERT_EQ(ptr, dest);
}

/*
 * Test von cm_memcpy() (src->size kleiner als size (Heartbleed-Bug!))
 * 4b, 3
 * @param void
 * @param void
 */
TEST(Speicherverwaltung, test_cm_memcpy_heart_bleed_bug) {
    PRINT_INFO("cm_memcpy: Heartbleed Bug (siehe Wikipedia)?\n");

    TESTSTART

    memblock *dBlock, *sBlock, *eBlock;
    void *dest, *src, *ptr;
    char *d, *s;
    int rtc = 31; /* range to compare; trailing zero wird bewusst nicht mitgenommen */

    memset(mempool, 0, sizeof(mempool));

    dBlock = (memblock*) mempool;
    dBlock->size = 100;
    dBlock->next = (memblock*) MAGIC_INT;
    dest = dBlock + 1;
    d = (char*) dest;
    for (int i = 0; (size_t) i < dBlock->size; i++) {
        *(d + i) = 'X';
    }

    sBlock = (memblock*) (mempool + dBlock->size + sizeof(memblock));
    sBlock->size = 25;
    sBlock->next = (memblock*) MAGIC_INT;
    src = sBlock + 1;
    s = (char*) src;
    s = cm_memcpy_helpsentence_32(s);

    sBlock = (memblock*) (mempool + dBlock->size + sBlock->size
            + (2 * sizeof(memblock)));
    sBlock->size = 100;
    sBlock->next = (memblock*) MAGIC_INT;

    eBlock = (memblock*) (mempool + dBlock->size + sBlock->size
            + (2 * sizeof(memblock)));
    eBlock->size = 200;
    eBlock->next = (memblock*) MAGIC_INT;

    ASSERT_NE(memcmp(d, s, rtc), 0);

    ptr = cm_memcpy(dest, src, rtc);

    ASSERT_EQ(memcmp(d, s, rtc), 0);
    ASSERT_EQ(ptr, dest);
}

/*
 * Test von cm_memcpy() (dest und src ueberlappen)
 * 4b, 3
 * @param void
 * @param void
 */
TEST(Speicherverwaltung, test_cm_memcpy_dest_overlaps_src) {
    PRINT_INFO("cm_memcpy: Ueberlappen sich Dest und Src?\n");

    TESTSTART

    memblock *dBlock, *sBlock;
    void *dest, *src, *ptr;
    char *d, *s;
    int rtc = 20; /* range to compare */

    memset(mempool, 0, sizeof(mempool));

    dBlock = (memblock*) mempool;
    dBlock->size = rtc;
    dBlock->next = (memblock*) MAGIC_INT;
    dest = dBlock + 1;
    d = (char*) dest;
    d = cm_memcpy_helpsentence_random(d, dBlock->size);

    sBlock = (memblock*) (mempool + dBlock->size + sizeof(memblock));
    sBlock->size = rtc * 3;
    sBlock->next = (memblock*) MAGIC_INT;
    src = sBlock + 1;
    s = (char*) src;
    s = cm_memcpy_helpsentence_random(s, sBlock->size);

    /* s komplett sichern */
    char s_alt[rtc * 3];
    memcpy(s_alt, s, rtc * 3);

    /* sBlock sichern um spaeter vergleichen zu koennen*/
    memblock c;
    memcpy(&c, sBlock, sizeof(memblock));

    /* dBlock sichern um spaeter vergleichen zu koennen*/
    memblock c1;
    memcpy(&c1, dBlock, sizeof(memblock));

    ptr = cm_memcpy(dest, src, rtc * 3);

    /* sBlock an der urspruenglichen Adresse finden */
    ASSERT_NE(memcmp(&c, sBlock, sizeof(memblock)), 0);

    /* dBlock an der urspruenglichen Adresse finden */
    ASSERT_EQ(memcmp(&c1, dBlock, sizeof(memblock)), 0);

    /* erster Teil */
    ASSERT_EQ(memcmp(d, s_alt, rtc), 0);

    /* zweiter Teil */
    size_t x = 2 * rtc - sizeof(memblock);
    ASSERT_EQ(memcmp(s, s_alt + rtc + sizeof(memblock), x), 0);

    /* dritter Teil */
    size_t y = rtc + sizeof(memblock);
    ASSERT_EQ(
            memcmp(s + 2 * rtc - sizeof(memblock),
                    s_alt + 2 * rtc - sizeof(memblock), y), 0);

    /* Rueckgabe sollte auch passen */
    ASSERT_EQ(ptr, dest);
}

/*
 * Test von cm_memcpy() (src und dest ueberlappen)
 * 4b, 3
 * @param void
 * @param void
 */
TEST(Speicherverwaltung, test_cm_memcpy_src_overlaps_dest) {
    PRINT_INFO("cm_memcpy: Ueberlappen sich Src und Dest?\n");

    TESTSTART

    memblock *dBlock, *sBlock;
    void *dest, *src, *ptr;
    char *d, *s, *a;
    int rtc = 30; /* range to compare */

    memset(mempool, 0, sizeof(mempool));

    sBlock = (memblock*) mempool;
    sBlock->size = rtc + 2;
    sBlock->next = (memblock*) MAGIC_INT;
    src = sBlock + 1;
    s = (char*) src;
    s = cm_memcpy_helpsentence_random(s, sBlock->size);

    dBlock = (memblock*) (mempool + sBlock->size + sizeof(memblock));
    dBlock->size = rtc - 1;
    dBlock->next = (memblock*) MAGIC_INT;
    dest = dBlock + 1;
    d = (char*) dest;
    d = cm_memcpy_helpsentence_random(d, dBlock->size);

    a = d + sBlock->size;
    a = cm_memcpy_helpsentence_random(a, dBlock->size + 2);
    char a_alt[dBlock->size + 2];
    memcpy(a_alt, a, dBlock->size + 2);

    /* sBlock sichern um spaeter vergleichen zu koennen*/
    memblock c;
    memcpy(&c, sBlock, sizeof(memblock));

    /* dBlock sichern um spaeter vergleichen zu koennen*/
    memblock c1;
    memcpy(&c1, dBlock, sizeof(memblock));

    ptr = cm_memcpy(dest, src, sBlock->size + dBlock->size);

    /* sBlock an der urspruenglichen Adresse finden */
    ASSERT_EQ(memcmp(&c, sBlock, sizeof(memblock)), 0);

    /* dBlock an der urspruenglichen Adresse finden */
    ASSERT_EQ(memcmp(&c1, dBlock, sizeof(memblock)), 0);

    /* erster Teil */
    ASSERT_EQ(memcmp(d, s, sBlock->size), 0);

    /* dBlock an der neuen (zweiten) Adresse finden */
    ASSERT_EQ(memcmp(&c1, d + sBlock->size, sizeof(memblock)), 0);

    /* zweiter Teil */
    ASSERT_EQ(
            memcmp(d + sBlock->size + sizeof(memblock), d,
                    dBlock->size - sizeof(memblock)), 0);

    /* stimmen die letzten manuell eingefuegten bytes noch ueberein */
    ASSERT_EQ(memcmp(d + sBlock->size + dBlock->size, a_alt + dBlock->size, 2),
            0);

    /* Rueckgabe sollte auch passen */
    ASSERT_EQ(ptr, dest);
}

/*
 * Test von cm_memcpy() (dest==src)
 *
 * @param void
 * @param void
 */
TEST(Speicherverwaltung, test_cm_memcpy_dest_src_equal) {
    PRINT_INFO("cm_memcpy: Was passiert wenn dest==src ist?\n");

    TESTSTART

    memblock *dBlock;
    void *dest, *src, *ptr;
    char *d, *s;
    int rtc = 32; /* range to compare */

    memset(mempool, 0, sizeof(mempool));

    dBlock = (memblock*) mempool;
    dBlock->size = rtc;
    dBlock->next = (memblock*) MAGIC_INT;
    dest = mempool + sizeof(memblock);
    d = (char*) dest;

    src = mempool + sizeof(memblock);
    s = (char*) src;
    s = cm_memcpy_helpsentence_32(s);

    ptr = cm_memcpy(dest, src, rtc);

    ASSERT_EQ(memcmp(d, s, rtc), 0);
    ASSERT_EQ(ptr, dest);
    ASSERT_EQ(ptr, src);
}

/*
 * Test von cm_memcpy() (dest-nextPtr nicht MAGIC_INT)
 * 4b, 3
 * @param void
 * @param void
 */
TEST(Speicherverwaltung, test_cm_memcpy_dest_wrong_next) {
    PRINT_INFO("cm_memcpy: Next Pointer von Dest nicht Magic Int?\n");

    TESTSTART

    memblock *dBlock, *sBlock;
    void *dest, *src, *ptr;
    char *d, *s;
    int rtc = 32; /* range to compare */

    memset(mempool, 0, sizeof(mempool));

    dBlock = (memblock*) mempool;
    dBlock->size = rtc;
    dBlock->next = NULL;
    dest = mempool + sizeof(memblock);
    d = (char*) dest;
    d = cm_memcpy_helpsentence_random(d, rtc);

    sBlock = (memblock*) (mempool + dBlock->size + sizeof(memblock));
    sBlock->size = rtc;
    sBlock->next = (memblock*) MAGIC_INT;
    src = mempool + 2 * sizeof(memblock) + dBlock->size;
    s = (char*) src;
    s = cm_memcpy_helpsentence_32(s);

    char c1[rtc];
    memcpy(c1, d, rtc);

    char c2[rtc];
    memcpy(c2, s, rtc);

    ptr = cm_memcpy(dest, src, rtc);

    ASSERT_EQ(memcmp(c1, d, rtc), 0);
    ASSERT_EQ(memcmp(c2, s, rtc), 0);
    ASSERT_NE(memcmp(d, s, rtc), 0);
    ASSERT_EQ(ptr, dest);
}

/*
 * Test von cm_memcpy() (src-nextPtr nicht MAGIC_INT)
 * 4b, 3
 * @param void
 * @param void
 */
TEST(Speicherverwaltung, test_cm_memcpy_src_wrong_next) {
    PRINT_INFO("cm_memcpy: Next Pointer von Src nicht Magic Int?\n");

    TESTSTART

    memblock *dBlock, *sBlock;
    void *dest, *src, *ptr;
    char *d, *s;
    int rtc = 32; /* range to compare */

    memset(mempool, 0, sizeof(mempool));

    dBlock = (memblock*) mempool;
    dBlock->size = rtc;
    dBlock->next = (memblock*) MAGIC_INT;
    dest = mempool + sizeof(memblock);
    d = (char*) dest;
    d = cm_memcpy_helpsentence_random(d, rtc);

    sBlock = (memblock*) (mempool + dBlock->size + sizeof(memblock));
    sBlock->size = rtc;
    sBlock->next = NULL;
    src = mempool + 2 * sizeof(memblock) + dBlock->size;
    s = (char*) src;
    s = cm_memcpy_helpsentence_32(s);

    char c1[rtc];
    memcpy(c1, d, rtc);

    char c2[rtc];
    memcpy(c2, s, rtc);

    ptr = cm_memcpy(dest, src, rtc);

    ASSERT_EQ(memcmp(c1, d, rtc), 0);
    ASSERT_EQ(memcmp(c2, s, rtc), 0);
    ASSERT_NE(memcmp(d, s, rtc), 0);
    ASSERT_EQ(ptr, dest);

}

/*
 * Test von cm_memcpy() (dest ist nicht in mempool)
 *
 * @param void
 * @param void
 */
TEST(Speicherverwaltung, test_cm_memcpy_dest_local) {
    PRINT_INFO("cm_memcpy: Was passiert wenn dest nicht in mempool ist?\n");

    TESTSTART

    char dummy[100];
    memblock *dBlock, *sBlock;
    void *dest, *src, *ptr;
    char *d, *s;
    int rtc = 32; /* range to compare */

    memset(mempool, 0, sizeof(mempool));
    memset(dummy, 0, sizeof(dummy));

    dBlock = (memblock*) dummy;
    dBlock->size = rtc;
    dBlock->next = (memblock*) MAGIC_INT;
    dest = dummy + sizeof(memblock);
    d = (char*) dest;
    d = cm_memcpy_helpsentence_random(d, rtc);

    sBlock = (memblock*) (mempool + dBlock->size + sizeof(memblock));
    sBlock->size = rtc;
    sBlock->next = (memblock*) MAGIC_INT;
    src = mempool + 2 * sizeof(memblock) + dBlock->size;
    s = (char*) src;
    s = cm_memcpy_helpsentence_32(s);

    char c1[rtc];
    memcpy(c1, d, rtc);

    char c2[rtc];
    memcpy(c2, s, rtc);

    ptr = cm_memcpy(dest, src, rtc);

    ASSERT_EQ(memcmp(c1, d, rtc), 0);
    ASSERT_EQ(memcmp(c2, s, rtc), 0);
    ASSERT_NE(memcmp(d, s, rtc), 0);
    ASSERT_EQ(ptr, dest);

}

/*
 * Test von cm_memcpy() (src ist nicht in mempool)
 *
 * @param void
 * @param void
 */
TEST(Speicherverwaltung, test_cm_memcpy_src_local) {
    PRINT_INFO("cm_memcpy: Was passiert wenn src nicht in mempool ist?\n");

    TESTSTART

    char dummy[100];
    memblock *dBlock, *sBlock;
    void *dest, *src, *ptr;
    char *d, *s;
    int rtc = 32; /* range to compare */

    memset(mempool, 0, sizeof(mempool));
    memset(dummy, 0, sizeof(dummy));

    dBlock = (memblock*) mempool;
    dBlock->size = rtc;
    dBlock->next = (memblock*) MAGIC_INT;
    dest = mempool + sizeof(memblock);
    d = (char*) dest;
    d = cm_memcpy_helpsentence_random(d, rtc);

    sBlock = (memblock*) dummy;
    sBlock->size = rtc;
    sBlock->next = (memblock*) MAGIC_INT;
    src = dummy + sizeof(memblock);
    s = (char*) src;
    s = cm_memcpy_helpsentence_32(s);

    char c1[rtc];
    memcpy(c1, d, rtc);

    char c2[rtc];
    memcpy(c2, s, rtc);

    ptr = cm_memcpy(dest, src, rtc);

    ASSERT_EQ(memcmp(c1, d, rtc), 0);
    ASSERT_EQ(memcmp(c2, s, rtc), 0);
    ASSERT_NE(memcmp(d, s, rtc), 0);
    ASSERT_EQ(ptr, dest);

}

/* ************************************************************************* */

/*
 * Test von cm_realloc() (ptr != NULL, ptr->Nutzbereich == size)
 * 4b, 4
 * @param void
 * @param void
 */
TEST(Speicherverwaltung, test_cm_realloc_do_nothing) {
    PRINT_INFO("cm_realloc: Null Pointer und Size kleiner 0?\n");

    init_heap(); // wichtig, damit malloc() nicht den speziell praeparierten mempool zuruecksetzt!
    TESTSTART

    memblock *block, *tmp;
    void *t, *s;

    freemem = (memblock*) (mempool + 20 + sizeof(memblock));
    freemem->size = 100;
    freemem->next = NULL;

    block = (memblock*) mempool;
    block->size = 20;
    block->next = (memblock*) MAGIC_INT;
    s = mempool + sizeof(memblock);

    char mempoolcopy[MEM_POOL_SIZE];
    memcpy(mempoolcopy, mempool, MEM_POOL_SIZE);
    tmp = freemem;

    t = cm_realloc(s, 20);

    ASSERT_EQ(t, s);
    ASSERT_EQ(((memblock* ) t - 1)->size, block->size);
    ASSERT_EQ(freemem, tmp);
    ASSERT_EQ(memcmp(mempoolcopy, mempool, MEM_POOL_SIZE), 0);
}

/*
 * Test von cm_realloc() (ptr == NULL, size!=0 -> cm_malloc())
 * 4b, 4
 * @param void
 * @param void
 */
TEST(Speicherverwaltung, test_cm_realloc_call_cm_malloc) {
    PRINT_INFO("cm_realloc: Null Pointer und Size ungleich 0?\n");

    init_heap(); // wichtig, damit malloc() nicht den speziell praeparierten mempool zuruecksetzt!
    TESTSTART

    void *tmp = NULL;

    freemem = (memblock*) mempool;
    freemem->size = 100;
    freemem->next = NULL;

    tmp = cm_realloc(tmp, 100);

    ASSERT_EQ(tmp, (memblock* ) mempool + 1);
    ASSERT_EQ(((memblock* ) tmp - 1)->size, 100);
    ASSERT_EQ(((memblock* ) tmp - 1)->next, (memblock*) MAGIC_INT);
    ASSERT_EQ(NULL, freemem);
}

/*
 * Test von cm_realloc() (size == 0 -> cm_free())
 * 4b, 4
 * @param void
 * @param void
 */
TEST(Speicherverwaltung, test_cm_realloc_call_cm_free) {
    PRINT_INFO("cm_realloc: Kein Null Pointer und Size gleich 0?\n");

    init_heap(); // wichtig, damit malloc() nicht den speziell praeparierten mempool zuruecksetzt!
    TESTSTART

    memblock *a, *oldFreeMem;
    void *b, *ptr;

    a = (memblock*) mempool;
    a->size = 100;
    a->next = (memblock*) MAGIC_INT;
    b = (void*) (a + 1);

    freemem = (memblock*) (mempool + 100 + sizeof(memblock));
    freemem->size = MEM_POOL_SIZE - 100 + 2 * sizeof(memblock);
    freemem->next = NULL;
    oldFreeMem = freemem;

    ptr = cm_realloc(b, 0);

    ASSERT_EQ(NULL, ptr);
    ASSERT_EQ(freemem, a);
    ASSERT_EQ(a->size, 100);
    ASSERT_EQ(a->next, oldFreeMem);
    ASSERT_EQ(oldFreeMem->size, MEM_POOL_SIZE - 100 + 2 * sizeof(memblock));
    ASSERT_EQ(NULL, oldFreeMem->next);
}

/*
 * TEST MIT cm_realloc(NULL, 0) -> sollte nix tun
 */
TEST(Speicherverwaltung, test_cm_realloc_null_0) {
    PRINT_INFO("cm_realloc: Null Pointer und Size gleich 0\n");

    init_heap(); // wichtig, damit malloc() nicht den speziell praeparierten mempool zuruecksetzt!
    TESTSTART

    char mempoolcopy[MEM_POOL_SIZE];
    memcpy(mempoolcopy, mempool, MEM_POOL_SIZE);

    void *neu = cm_realloc(NULL, 0);
    ASSERT_EQ(NULL, neu);
    ASSERT_EQ(memcmp(mempoolcopy, mempool, MEM_POOL_SIZE), 0);
}

#ifdef MALLOCSPLIT
/*
 * Test cm_realloc() (Vergroessern)
 * 4b, 4
 * @param void
 * @return void
 */
TEST(Speicherverwaltung, test_cm_realloc_enlarge) {
    PRINT_INFO("cm_realloc: Vergroessert sich der Speicher?\n");

    init_heap(); // wichtig, damit malloc() nicht den speziell praeparierten mempool zuruecksetzt!
    TESTSTART

    memblock *a, *b, *c;
    void* nutzbereich, *neu;
    char* str, *strNeu;

    a = (memblock*) mempool;
    a->size = 100 - sizeof(memblock);
    a->next = (memblock*) MAGIC_INT;

    b = (memblock*) (mempool + 100);
    b->size = 1000;
    b->next = NULL;
    freemem = b;
    c = b;

    nutzbereich = (void*) (a + 1);
    str = (char*) nutzbereich;
    *str = 'T';
    *(str + 1) = 'E';
    *(str + 2) = 'S';
    *(str + 3) = 'T';
    *(str + 4) = '\0';
    *(str + 5) = 'A';
    *(str + 6) = 'B';

    strNeu = (char*) (b + 1);
    *strNeu = 'a';
    *(strNeu + 1) = 'b';
    *(strNeu + 2) = 'c';
    *(strNeu + 3) = 'd';
    *(strNeu + 4) = 'e';
    *(strNeu + 5) = 'f';
    *(strNeu + 6) = 'g';

    neu = cm_realloc(nutzbereich, 200);
    strNeu = (char*) neu;

    ASSERT_EQ(freemem, a);
    ASSERT_EQ(a->size, 100 - sizeof(memblock));
    ASSERT_EQ(a->next,
            (memblock* ) ((char* ) (c) + 200 + sizeof(memblock)));
    ASSERT_EQ(a->next->size, 1000 - 200 - sizeof(memblock));
    ASSERT_EQ(NULL, a->next->next);
    ASSERT_EQ(neu, (void* ) (b + 1));
    ASSERT_EQ(b->size, 200);
    ASSERT_EQ(b->next, (memblock*) MAGIC_INT);
    ASSERT_EQ(*strNeu, *str);
    ASSERT_EQ(*(strNeu + 1), *(str + 1));
    ASSERT_EQ(*(strNeu + 2), *(str + 2));
    ASSERT_EQ(*(strNeu + 3), *(str + 3));
    ASSERT_EQ(*(strNeu + 4), *(str + 4));
    ASSERT_EQ(*(strNeu + 5), *(str + 5));
    ASSERT_EQ(*(strNeu + 6), *(str + 6));
}

/*
 * Test cm_realloc() (Verkleinern)
 * 4b, 4
 * @param void
 * @return void
 */
TEST(Speicherverwaltung, test_cm_realloc_shrink) {
    PRINT_INFO("cm_realloc: Verkleinert sich der Speicher?\n");

    init_heap(); // wichtig, damit malloc() nicht den speziell praeparierten mempool zuruecksetzt!
    TESTSTART

    memblock *a, *b, *c;
    void* nutzbereich, *neu;
    char* str, *strNeu;

    a = (memblock*) mempool;
    a->size = 100 - sizeof(memblock);
    a->next = (memblock*) MAGIC_INT;

    b = (memblock*) (mempool + 100);
    b->size = 1000;
    b->next = NULL;
    freemem = b;
    c = b;

    nutzbereich = (void*) (a + 1);
    str = (char*) nutzbereich;
    *str = 'T';
    *(str + 1) = 'E';
    *(str + 2) = 'S';
    *(str + 3) = 'T';
    *(str + 4) = '\0';
    *(str + 5) = 'A';
    *(str + 6) = 'B';
    *(str + 7) = 'C';
    *(str + 8) = 'D';

    strNeu = (char*) (b + 1);
    *strNeu = 'a';
    *(strNeu + 1) = 'b';
    *(strNeu + 2) = 'c';
    *(strNeu + 3) = 'd';
    *(strNeu + 4) = 'e';
    *(strNeu + 5) = 'f';
    *(strNeu + 6) = 'g';
    *(strNeu + 7) = 'h';
    *(strNeu + 8) = 'i';

    neu = cm_realloc(nutzbereich, 7);
    strNeu = (char*) neu;

    ASSERT_EQ(freemem, a);
    ASSERT_EQ(a->size, 100 - sizeof(memblock));
    ASSERT_EQ(a->next,
            (memblock* ) ((char* ) (c) + 7 + sizeof(memblock)));
    ASSERT_EQ(a->next->size, 1000 - 7 - sizeof(memblock));
    ASSERT_EQ(NULL, a->next->next);
    ASSERT_EQ(neu, (void* ) (b + 1));
    ASSERT_EQ(b->size, 7);
    ASSERT_EQ(b->next, (memblock*) MAGIC_INT);
    ASSERT_EQ(*strNeu, *str);
    ASSERT_EQ(*(strNeu + 1), *(str + 1));
    ASSERT_EQ(*(strNeu + 2), *(str + 2));
    ASSERT_EQ(*(strNeu + 3), *(str + 3));
    ASSERT_EQ(*(strNeu + 4), *(str + 4));
    ASSERT_EQ(*(strNeu + 5), *(str + 5));
    ASSERT_EQ(*(strNeu + 6), *(str + 6));
    ASSERT_EQ((void* ) (strNeu + 7), (void* ) a->next);
    ASSERT_EQ('C', *(str + 7));
    ASSERT_EQ('D', *(str + 8));
}

#endif // MALLOCSPLIT

int main(int argc, char *argv[]) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
