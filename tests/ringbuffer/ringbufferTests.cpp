#include <string.h>
#include <gtest/gtest.h>

extern "C" {
#include "ringbuffer/ringbuffer.h"
}

namespace testing {
    namespace internal {
        enum GTestColor {
            COLOR_DEFAULT,
            COLOR_RED,
            COLOR_GREEN,
            COLOR_YELLOW
        };
    extern void ColoredPrintf(GTestColor color, const char* fmt, ...);
    }
}

#define PRINT_INFO(x)   testing::internal::ColoredPrintf(testing::internal::COLOR_GREEN, "[          ] "); testing::internal::ColoredPrintf(testing::internal::COLOR_YELLOW, x)

#define FREE_RINGBUFFER(BUFFER) free(BUFFER->elems); free(BUFFER);

#define INIT_FREELISTBUFFER(SIZE) freelist_buffer = (void**) malloc(sizeof(void*)*(SIZE));\
        memset(freelist_buffer, 0, (sizeof(void*)*(SIZE)));\
        freelist_buffer_size = (SIZE);

#define DEL_FREELISTBUFFER free(freelist_buffer)

/* ========================================================================= */
void *ptr = 0;
int free_cb_count = 0;
void free_cb(void *p) {
    ptr = p;
    free_cb_count++;
}

void** freelist_buffer = 0;
int freelist_buffer_position = 0;
int freelist_buffer_size = 0;

void free_check(void *p) {
    if (freelist_buffer_position >= freelist_buffer_size) {
        PRINT_INFO("Es wird für mehr Elemente das Callback aufgerufen als vorhanden.\n");
    }
    freelist_buffer[freelist_buffer_position] = p;
    freelist_buffer_position++;
}

/* ============================TESTS======================================== */

/*
 * Testet ob es den Typ ringbuffer gibt
 * @param void
 * @return void
 */
TEST(Ringpuffer, test_type_ringbuffer_defined) {
    PRINT_INFO("Testet ob es den Typ ringbuffer gibt\n");

    ring_buffer p;

    p.size = 0;
    p.count = 0;
    p.head = 0;
    p.free_callback = NULL;

    ASSERT_EQ(p.count, 0);
}

/*************************************************************************/

/*
 * Testet ob man einen ringbuffer der Groesse 0 anlegen kann und ohne Callback
 * @param void
 * @return void
 */
TEST(Ringpuffer, test_init_buffer_null) {
    PRINT_INFO("Testet ob man einen ringbuffer der Groesse 0 anlegen kann\n");

    ring_buffer *p = init_buffer(0, 0);

    ASSERT_EQ(NULL, p);
}

/*
 * Testet ob man einen ringbuffer ohne Callback anlegen kann
 * @param void
 * @return void
 */
TEST(Ringpuffer, test_init_buffer_wo_callback) {
    PRINT_INFO("Testet ob man einen ringbuffer ohne Callback anlegen kann\n");

    ring_buffer *p = init_buffer(10, 0);

    ASSERT_EQ(NULL, p);
}

/*
 * Testet ob man einen ringbuffer anlegen kann
 * @param void
 * @return void
 */
TEST(Ringpuffer, test_init_buffer_normal) {
    PRINT_INFO("Testet ob man einen ringbuffer anlegen kann\n");

    ring_buffer *p = init_buffer(10, free_cb);

    ASSERT_TRUE(NULL != p);
    ASSERT_EQ(p->size, 10);
    ASSERT_EQ(p->count, 0);
    ASSERT_EQ(p->head, 0);
    ASSERT_EQ(p->free_callback, free_cb);
    ASSERT_TRUE(NULL != p->elems);

    FREE_RINGBUFFER(p)
}

/*
 * Testet ob man einen ringbuffer der Groesse 0 anlegen kann mit Callback
 * @param void
 * @return void
 */
TEST(Ringpuffer, test_init_buffer_wo_callback_size_is_null) {
    PRINT_INFO("Testet ob man einen ringbuffer der Groesse 0 anlegen kann mit Callback\n");

    ring_buffer *p = init_buffer(0, free_cb);

    ASSERT_EQ(NULL, p);
}

/*************************************************************************/

/*
 * Testet ob read_buffer Null-Pointer faengt
 * @param void
 * @return void
 */
TEST(Ringpuffer, test_read_buffer_no_buffer) {
    PRINT_INFO("Testet ob read_buffer Null-Pointer faengt\n");

    void *p = read_buffer(NULL);

    ASSERT_EQ(NULL, p);
}

/*
 * Testet ob read_buffer mit einem leeren Puffer klarkommt
 * @param void
 * @return void
 */
TEST(Ringpuffer, test_read_buffer_empty) {
    PRINT_INFO("Testet ob read_buffer mit einem leeren Puffer klarkommt\n");

    ptr = NULL;
    ring_buffer *p = init_buffer(10, free_cb);
    ring_buffer *pold = p;

    void *erg = read_buffer(p);

    ASSERT_EQ(p, pold);
    ASSERT_EQ(p->size, 10);
    ASSERT_EQ(p->count, 0);
    ASSERT_EQ(p->head, 0);
    ASSERT_EQ(NULL, ptr);
    ASSERT_EQ(NULL, erg);

    FREE_RINGBUFFER(p)
}

/*
 * Testet ob read_buffer Puffer mit einem Element lesen kann
 * @param void
 * @return void
 */
TEST(Ringpuffer, test_read_buffer_single_elem) {
    PRINT_INFO("Testet ob read_buffer Puffer mit einem Element lesen kann\n");

    int x = 42;
    ptr = NULL;
    ring_buffer *p = init_buffer(10, free_cb);
    ring_buffer *pold = p;

    p->elems[0] = &x;
    p->count = 1;

    void *erg = read_buffer(p);

    ASSERT_EQ(p, pold);
    ASSERT_EQ(p->size, 10);
    ASSERT_EQ(p->count, 0);
    ASSERT_EQ(p->head, 1);
    ASSERT_EQ(NULL, ptr);
    ASSERT_EQ(erg, &x);

    FREE_RINGBUFFER(p)
}

/*
 * Testet ob read_buffer mit mehreren Elementen funktioniert
 * @param void
 * @return void
 */
TEST(Ringpuffer, test_read_buffer) {
    PRINT_INFO("Testet ob read_buffer mit mehreren Elementen funktioniert\n");

    int x = 42;
    int y = 99;
    int z = -1;
    ptr = NULL;

    ring_buffer *p = init_buffer(10, free_cb);
    ring_buffer *pold = p;

    p->head = 2;
    p->elems[2] = &x;
    p->elems[3] = &y;
    p->elems[4] = &z;
    p->count = 3;

    void *erg = read_buffer(p);

    ASSERT_EQ(p, pold);
    ASSERT_EQ(p->size, 10);
    ASSERT_EQ(p->count, 2);
    ASSERT_EQ(p->head, 3);
    ASSERT_EQ(NULL, ptr);
    ASSERT_EQ(erg, &x);
    ASSERT_EQ(p->elems[3], &y);
    ASSERT_EQ(p->elems[4], &z);

    FREE_RINGBUFFER(p)
}

/*
 * Testet ob read_buffer ein Element aus vollem Puffer lesen kann (mit Überschlag)
 * @param void
 * @return void
 */
TEST(Ringpuffer, test_read_buffer_full) {
    PRINT_INFO("Testet ob read_buffer ein Element aus vollem Puffer lesen kann (mit Überschlag)\n");

    int x = 42;
    int y = 99;
    ptr = NULL;

    ring_buffer *p = init_buffer(10, free_cb);
    ring_buffer *pold = p;

    p->head = 9;
    p->count = 10;
    p->elems[9] = &x;
    p->elems[0] = &y;

    void *erg = read_buffer(p);

    ASSERT_EQ(p, pold);
    ASSERT_EQ(p->size, 10);
    ASSERT_EQ(p->count, 9);
    ASSERT_EQ(p->head, 0);
    ASSERT_EQ(p->elems[0], &y);
    ASSERT_EQ(NULL, ptr);
    ASSERT_EQ(erg, &x);

    FREE_RINGBUFFER(p)
}

/*
 * Testet ob es auch mit einem Ringbuffer mit einer anderen Größe funktioniert
 * @param void
 * @return void
 */
TEST(Ringpuffer, test_init_buffer_different_size) {
PRINT_INFO("Testet ob es auch mit einem Ringbuffer mit einer anderen Größe funktioniert.\n");

    int x = 42;
    int y = 99;
    ptr = NULL;

    ring_buffer *p = init_buffer(14, free_cb);
    ring_buffer *pold = p;

    p->head = 13;
    p->count = 14;
    p->elems[13] = &x;
    p->elems[0] = &y;

    void *erg = read_buffer(p);

    ASSERT_EQ(p, pold);
    ASSERT_EQ(p->size, 14);
    ASSERT_EQ(p->count, 13);
    ASSERT_EQ(p->head, 0);
    ASSERT_EQ(p->elems[0], &y);
    ASSERT_EQ(NULL, ptr);
    ASSERT_EQ(erg, &x);

    FREE_RINGBUFFER(p)
}

/*
 * Test von read_buffer mit head == count != 0
 *
 * @param void
 * @return void
 */
TEST(Ringpuffer, test_read_buffer_head_equals_count_not_null) {
    PRINT_INFO("Test von read_buffer mit head == count != 0)\n");

    int x = 42;
    int y = 99;
    ptr = NULL;
    ring_buffer *p;
    ring_buffer *pold;
    void *erg;

    p = init_buffer(10, free_cb);
    pold = p;
    p->head = 3;
    p->count = 3;
    p->elems[3] = &x;
    p->elems[4] = &y;

    erg = read_buffer(p);

    ASSERT_EQ(p, pold);
    ASSERT_EQ(p->size, 10);
    ASSERT_EQ(p->count, 2);
    ASSERT_EQ(p->head, 4);
    ASSERT_EQ(p->elems[4], &y);
    ASSERT_EQ(erg, &x);
    ASSERT_EQ(NULL, ptr);

    FREE_RINGBUFFER(p)
}

/*
 * Test von read_buffer mit count == 0 und head != 0
 *
 * @param void
 * @return void
 */
TEST(Ringpuffer, test_read_buffer_empty_head_not_null) {
    PRINT_INFO("Test von read_buffer mit count == 0 und head != 0)\n");

    ptr = NULL;
    ring_buffer *p;
    ring_buffer *pold;
    void *erg;

    p = init_buffer(10, free_cb);
    pold = p;
    p->head = 3;
    p->count = 0;

    erg = read_buffer(p);

    ASSERT_EQ(p, pold);
    ASSERT_EQ(p->size, 10);
    ASSERT_EQ(p->count, 0);
    ASSERT_EQ(p->head, 3);
    ASSERT_EQ(NULL, erg);
    ASSERT_EQ(NULL, ptr);

    FREE_RINGBUFFER(p)
}

/*************************************************************************/

/*
 * Testet ob write_buffer Null-Pointer faengt
 * @param void
 * @return void
 */
TEST(Ringpuffer, test_write_buffer_no_buffer_no_data) {
    PRINT_INFO("Testet ob write_buffer Null-Pointer faengt\n");

    write_buffer(NULL, NULL);

    SUCCEED();
}

/*
 * Testet ob write_buffer Null-Pointer faengt
 * @param void
 * @return void
 */
TEST(Ringpuffer, test_write_buffer_no_buffer) {
    PRINT_INFO("Testet ob write_buffer Null-Pointer faengt\n");

    int x = 42;

    write_buffer(NULL, &x);

    SUCCEED();
}

/*
 * Testet ob write_buffer Null-Pointer faengt
 * @param void
 * @return void
 */
TEST(Ringpuffer, test_write_buffer_no_data) {
    PRINT_INFO("Testet ob write_buffer Null-Pointer faengt\n");

    ring_buffer *p = init_buffer(10, free_cb);
    ptr = NULL;

    write_buffer(p, NULL);

    ASSERT_TRUE(NULL != p);
    ASSERT_EQ(p->size, 10);
    ASSERT_EQ(p->count, 0);
    ASSERT_EQ(p->head, 0);
    ASSERT_EQ(p->free_callback, free_cb);
    ASSERT_EQ(NULL, ptr);

    FREE_RINGBUFFER(p)
}

/*
 * Testet ob write_buffer ein Element in leeren Puffer einfuegen kann
 * @param void
 * @return void
 */
TEST(Ringpuffer, test_write_buffer_insert_empty) {
    PRINT_INFO("Testet ob write_buffer ein Element in leeren Puffer einfuegen kann\n");

    int x = 42;
    ptr = NULL;

    ring_buffer *p = init_buffer(10, free_cb);

    write_buffer(p, &x);

    ASSERT_TRUE(NULL != p);
    ASSERT_EQ(p->size, 10);
    ASSERT_EQ(p->count, 1);
    ASSERT_EQ(p->head, 0);
    ASSERT_EQ(p->elems[0], &x);
    ASSERT_EQ(p->free_callback, free_cb);
    ASSERT_EQ(NULL, ptr);

    FREE_RINGBUFFER(p)
}

/*
 * Testet ob write_buffer ein Element einfuegen kann
 * @param void
 * @return void
 */
TEST(Ringpuffer, test_write_buffer_insert) {
    PRINT_INFO("Testet ob write_buffer ein Element einfuegen kann\n");

    int x = 42;
    ptr = NULL;

    ring_buffer *p = init_buffer(10, free_cb);
    p->count = 3;

    write_buffer(p, &x);

    ASSERT_TRUE(NULL != p);
    ASSERT_EQ(p->size, 10);
    ASSERT_EQ(p->count, 4);
    ASSERT_EQ(p->head, 0);
    ASSERT_EQ(p->elems[3], &x);
    ASSERT_EQ(p->free_callback, free_cb);
    ASSERT_EQ(NULL, ptr);

    FREE_RINGBUFFER(p)
}

/*
 * Testet ob write_buffer ein Element am Ende einfuegen kann
 * @param void
 * @return void
 */
TEST(Ringpuffer, test_write_buffer_insert_end) {
    PRINT_INFO("Testet ob write_buffer ein Element am Ende einfuegen kann\n");

    int x = 42;
    ptr = NULL;

    ring_buffer *p = init_buffer(10, free_cb);
    p->count = 9;

    write_buffer(p, &x);

    ASSERT_TRUE(NULL != p);
    ASSERT_EQ(p->size, 10);
    ASSERT_EQ(p->count, 10);
    ASSERT_EQ(p->head, 0);
    ASSERT_EQ(p->elems[9], &x);
    ASSERT_EQ(p->free_callback, free_cb);
    ASSERT_EQ(NULL, ptr);

    FREE_RINGBUFFER(p)
}

/*
 * Testet ob write_buffer ein Element am Ende einfuegen kann (mit Ueberschlag)
 * @param void
 * @return void
 */
TEST(Ringpuffer, test_write_buffer_insert_full) {
    PRINT_INFO("Testet ob write_buffer ein Element am Ende einfuegen kann (mit Ueberschlag)\n");

    int x = 42;
    int y = 99;
    ptr = NULL;

    ring_buffer *p = init_buffer(10, free_cb);
    p->head = 0;
    p->count = 10;
    p->elems[0] = &y;

    write_buffer(p, &x);

    ASSERT_TRUE(NULL != p);
    ASSERT_EQ(p->size, 10);
    ASSERT_EQ(p->count, 10);
    ASSERT_EQ(p->head, 1);
    ASSERT_EQ(p->elems[0], &x);
    ASSERT_EQ(p->free_callback, free_cb);
    ASSERT_EQ(ptr, &y);

    FREE_RINGBUFFER(p)
}

/*
 * Testet ob write_buffer ein Element am Ende einfuegen kann (mit Ueberschlag)
 * @param void
 * @return void
 */
TEST(Ringpuffer, test_write_buffer_insert_full2) {
    PRINT_INFO("Testet ob write_buffer ein Element am Ende einfuegen kann (mit Ueberschlag)\n");

    int x = 42;
    int y = 99;
    int z = 99;
    ptr = NULL;

    ring_buffer *p = init_buffer(10, free_cb);
    p->head = 3;
    p->count = 10;
    p->elems[2] = &z;
    p->elems[3] = &y;
    p->elems[4] = &z;

    write_buffer(p, &x);

    ASSERT_TRUE(NULL != p);
    ASSERT_EQ(p->size, 10);
    ASSERT_EQ(p->count, 10);
    ASSERT_EQ(p->head, 4);
    ASSERT_EQ(p->elems[2], &z);
    ASSERT_EQ(p->elems[3], &x);
    ASSERT_EQ(p->elems[4], &z);
    ASSERT_EQ(p->free_callback, free_cb);
    ASSERT_EQ(ptr, &y);

    FREE_RINGBUFFER(p)
}

/*
 * Test von write_buffer mit vollem Puffer und
 * Ueberlauf (count == size, head == size-1)
 *
 * @param void
 * @return void
 */
TEST(Ringpuffer, test_write_buffer_with_full_buffer_with_overflow) {
    PRINT_INFO("Test von write_buffer mit vollem Puffer und Ueberlauf\n");

    int x = 42;
    int y = 99;
    int z = 99;
    ring_buffer *p;
    ptr = NULL;

    p = init_buffer(10, free_cb);
    p->head = 9;
    p->count = 10;
    p->elems[8] = &z;
    p->elems[9] = &y;
    p->elems[0] = &z;

    write_buffer(p, &x);

    ASSERT_TRUE(NULL != p);
    ASSERT_EQ(p->size, 10);
    ASSERT_EQ(p->count, 10);
    ASSERT_EQ(p->head, 0);
    ASSERT_EQ(p->elems[8], &z);
    ASSERT_EQ(p->elems[9], &x);
    ASSERT_EQ(p->elems[0], &z);
    ASSERT_EQ(p->free_callback, free_cb);
    ASSERT_EQ(ptr, &y);

    FREE_RINGBUFFER(p)
}

/*
 * Test von write_buffer mit nicht-vollem Puffer und
 * Ueberlauf (head + count > size (aber count < size))
 *
 * @param void
 * @return void
 */
TEST(Ringpuffer, test_write_buffer_not_full_with_overflow) {
    PRINT_INFO("Test von write_buffer mit nicht-vollem Puffer und Ueberlauf\n");

    int x = 42;
    int y = 99;
    int z = 99;
    ring_buffer *p;
    ptr = NULL;

    p = init_buffer(10, free_cb);
    p->head = 8;
    p->count = 3;
    p->elems[8] = &z;
    p->elems[9] = &y;
    p->elems[0] = &z;

    write_buffer(p, &x);

    ASSERT_TRUE(NULL != p);
    ASSERT_EQ(p->size, 10);
    ASSERT_EQ(p->count, 4);
    ASSERT_EQ(p->head, 8);
    ASSERT_EQ(p->elems[8], &z);
    ASSERT_EQ(p->elems[9], &y);
    ASSERT_EQ(p->elems[0], &z);
    ASSERT_EQ(p->elems[1], &x);
    ASSERT_EQ(p->free_callback, free_cb);
    ASSERT_EQ(NULL, ptr);

    FREE_RINGBUFFER(p)
}

/*
 * Testet ob write_buffer ein Element am Ende des
 * Ringpuffers einfuegen kann
 *
 * @param void
 * @return void
 */
TEST(Ringpuffer, test_write_buffer_insert_at_end_of_buffer) {
    PRINT_INFO("Testet ob write_buffer ein Element am Ende des Puffers einfuegen kann\n");

    int x = 42;
    int y = 99;
    ring_buffer *p;
    ptr = NULL;

    p = init_buffer(10, free_cb);
    p->head = 8;
    p->count = 1;
    p->elems[8] = &x;

    write_buffer(p, &y);

    ASSERT_TRUE(NULL != p);
    ASSERT_EQ(p->size, 10);
    ASSERT_EQ(p->count, 2);
    ASSERT_EQ(p->head, 8);
    ASSERT_EQ(p->elems[8], &x);
    ASSERT_EQ(p->elems[9], &y);
    ASSERT_EQ(p->free_callback, free_cb);
    ASSERT_EQ(NULL, ptr);

    FREE_RINGBUFFER(p)
}

/*************************************************************************/

/*
 * Testet ob free_buffer Null-Pointer faengt
 * @param void
 * @return void
 */
TEST(Ringpuffer, test_free_buffer_no_buffer) {
    PRINT_INFO("Testet ob free_buffer Null-Pointer faengt\n");

    int x = free_buffer(NULL);

    ASSERT_EQ(x, -1);
}

/*
 * Testet ob free_buffer alles freigibt
 * @param void
 * @return void
 */
TEST(Ringpuffer, test_free_buffer) {
    PRINT_INFO("Testet ob free_buffer alles freigibt\n");

    int x = 42;
    ptr = NULL;

    ring_buffer *p = init_buffer(10, free_cb);
    p->head = 3;
    p->count = 1;
    p->elems[3] = &x;

    int erg = free_buffer(p);

    ASSERT_EQ(ptr, &x);
    ASSERT_EQ(erg, 1);
}

/*
 * Test von free_buffer mit mehr als nur einen Eintrag
 *
 * TODO: Überprüfung ob die einzelnen Elemente freigegeben werden.
 *
 * @param void
 * @return void
 */
TEST(Ringpuffer, test_free_buffer_with_more_elements) {
    PRINT_INFO("Test von free_buffer mit mehr als nur einem Eintrag\n");

    int x = 42, y = 43, z = 44, w = 45;
    ptr = NULL;
    free_cb_count = 0;
    ring_buffer *p;

    p = init_buffer(10, free_cb);
    p->head = 3;
    p->count = 4;
    p->elems[3] = &x;
    p->elems[4] = &y;
    p->elems[5] = &z;
    p->elems[6] = &w;

    int erg = free_buffer(p);

    ASSERT_EQ(free_cb_count, 4);
    ASSERT_EQ(erg, 4);
}


TEST(Ringpuffer, test_free_buffer_with_more_elements_own_memory) {
    PRINT_INFO("Test von free_buffer mit mehr als nur einem Eintrag mit eigenem Speicher\n");

    int x = 42, y = 43, z = 44, w = 45;
    void **el = (void**) malloc(4 * sizeof(void*));
    el[0] = &x;
    el[1] = &y;
    el[2] = &z;
    el[3] = &w;
    ring_buffer *p = (ring_buffer*) malloc(sizeof(ring_buffer));
    p->size = 4;
    p->head = 3;
    p->count = 4;
    p->elems = el;
    p->free_callback = free_check;

    INIT_FREELISTBUFFER(4);

    int erg = free_buffer(p);

    ASSERT_EQ(freelist_buffer_position, 4);
    ASSERT_EQ(erg, 4);
    ASSERT_EQ(&w, freelist_buffer[0]);
    ASSERT_EQ(&x, freelist_buffer[1]);
    ASSERT_EQ(&y, freelist_buffer[2]);
    ASSERT_EQ(&z, freelist_buffer[3]);

    DEL_FREELISTBUFFER;
}

/*
 * Test von free_buffer mit einem Überlauf
 *
 * TODO: Überprüfung ob die einzelnen Elemente freigegeben werden.
 *
 * @param void
 * @return void
 */
TEST(Ringpuffer, test_free_buffer_with_overflow) {
    PRINT_INFO("Test von free_buffer mit einem Ueberlauf\n");

    int x = 42, y = 43, z = 44, w = 45;
    ptr = NULL;
    free_cb_count = 0;
    ring_buffer *p;

    p = init_buffer(10, free_cb);
    p->head = 8;
    p->count = 4;
    p->elems[8] = &x;
    p->elems[9] = &y;
    p->elems[0] = &z;
    p->elems[1] = &w;

    int erg = free_buffer(p);

    ASSERT_EQ(free_cb_count, 4);
    ASSERT_EQ(erg, 4);
}
/*************************************************************************/

/*
 * Testet ob count_elements NULL-Ptr abfaengt
 * @param void
 * @return void
 */
TEST(Ringpuffer, test_count_elements_null) {
    PRINT_INFO("Testet ob count_elements Null-Pointer abfaengt\n");

    int x = count_elements(NULL);

    ASSERT_EQ(x, -1);
}

/*
 * Testet ob count_elements den Count liefert
 * @param void
 * @return void
 */
TEST(Ringpuffer, test_count_elements) {
    PRINT_INFO("Testet ob count_elements den Count liefert\n");

    ring_buffer *p = init_buffer(10, free_cb);
    p->count = 99;

    int x = count_elements(p);

    ASSERT_EQ(x, 99);

    FREE_RINGBUFFER(p)
}

/* ============================ABGABE======================================= */
#ifdef ABGABE

/**
 * Hinweis fuer die Abgabe: korrektes malloc ueberpruefen
 *
 * @param void
 * @return void
 */
TEST(Ringpuffer, test_acceptance_note_init_buffer) {
    PRINT_INFO("Ist die Speicherallokation in init_buffer() korrekt? Manuell ueberpruefen...\n");

    FAIL();
}

/**
 * Hinweis fuer die Abgabe: korrektes free ueberpruefen
 *
 * @param void
 * @return void
 */
TEST(Ringpuffer, test_acceptance_note_free_buffer) {
    PRINT_INFO("Ist das free'n in free_buffer() korrekt? Manuell ueberpruefen...\n");

    FAIL();
}

#endif  /* ABGABE */
/* ========================================================================= */

int main(int argc, char *argv[]) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
