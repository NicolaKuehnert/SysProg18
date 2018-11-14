#include <string.h>
#include <gtest/gtest.h>

extern "C" {
#include "ledanzeige/segmentanzeige.h"
#include "ledanzeige/TM1637_intern.h"
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

/* ============================TESTS======================================== */

/*
 * Testet ob Datentyp byte richtig definiert wird.
 * @param void
 * @return void
 */
TEST(Segmentanzeige, test_byte_definition) {
    PRINT_INFO("Bereichsgrenzen von byte testen \n");

    byte a, b;  // Wenn der Datentyp nicht definiert ist, kompiliert das nicht.

    a = 255;
    ASSERT_EQ(++a, 0);

    b = 0;
    ASSERT_EQ(--b, 255);
}

/*
 * Enum segment testen
 * @param void
 * @return void
 */
TEST(Segmentanzeige, test_segment_definition) {
    PRINT_INFO("Werte von segment testen \n");

    segment s; // Dummy-Deklaration: Wenn der Datentyp nicht definiert ist, kompiliert das nicht.

    s = SEG1;
    ASSERT_EQ(s, 0);
    s = SEG2;
    ASSERT_EQ(s, 1);
    s = SEG3;
    ASSERT_EQ(s, 2);
    s = SEG4;
    ASSERT_EQ(s, 3);
}

/*
 * Enum dot testen
 * @param void
 * @return void
 */
TEST(Segmentanzeige, test_dot_definition) {
    PRINT_INFO("Werte von dot testen \n");

    dot d; // Dummy-Deklaration: Wenn der Datentyp nicht definiert ist, kompiliert das nicht.

    d = OFF;
    ASSERT_EQ(d, 0);
    d = ON;
    ASSERT_EQ(ON, 1);
}

/*
 * Enum brightness testen
 * @param void
 * @return void
 */
TEST(Segmentanzeige, test_brightness_definition) {
    PRINT_INFO("Werte von brightness testen \n");

    brightness b; // Dummy-Deklaration: Wenn der Datentyp nicht definiert ist, kompiliert das nicht.

    b = DARK;
    ASSERT_EQ(b, 0);
    b = MEDIUM;
    ASSERT_EQ(b, 1);
    b = BRIGHT;
    ASSERT_EQ(b, 7);
}

/*
 * Testet die untere Grenze fuer number in TM1637_calculate_display
 * @param void
 * @return void
 */
TEST(Segmentanzeige, test_calculateDisplay_number_lower_bound) {
    PRINT_INFO("TM1637_calculate_display(-99.91, SEG2) (number too small) \n");

    ASSERT_EQ((int ) TM1637_calculate_display(-99.91, SEG2), 0x00);
}

/*
 * Testet die obere Grenze fuer number in TM1637_calculate_display
 * @param void
 * @return void
 */
TEST(Segmentanzeige, test_calculateDisplay_number_upper_bound) {
    PRINT_INFO("TM1637_calculate_display(999.91, SEG3) (number too large) \n");

    ASSERT_EQ((int ) TM1637_calculate_display(999.91, SEG3), 0x00);
}

/*
 * Testet verschiedene positive Werte
 * @param void
 * @return void
 */
TEST(Segmentanzeige, test_calculateDisplay_different_pos_values) {
    PRINT_INFO(
            "TM1637_calculate_display() mit verschiedenen positiven Werten \n");

    ASSERT_EQ((int ) TM1637_calculate_display(0.0, SEG1), 0x3f);
    ASSERT_EQ((int ) TM1637_calculate_display(0.0, SEG2), 0x3f);
    ASSERT_EQ((int ) TM1637_calculate_display(0.0, SEG3), 0x3f);
    ASSERT_EQ((int ) TM1637_calculate_display(0.0, SEG4), 0x3f);

    ASSERT_EQ((int ) TM1637_calculate_display(0.12, SEG1), 0x3f);
    ASSERT_EQ((int ) TM1637_calculate_display(0.12, SEG2), 0x3f);
    ASSERT_EQ((int ) TM1637_calculate_display(0.12, SEG3), 0x3f);
    ASSERT_EQ((int ) TM1637_calculate_display(0.12, SEG4), 0x06);

    ASSERT_EQ((int ) TM1637_calculate_display(1.23, SEG1), 0x3f);
    ASSERT_EQ((int ) TM1637_calculate_display(1.23, SEG2), 0x3f);
    ASSERT_EQ((int ) TM1637_calculate_display(1.23, SEG3), 0x06);
    ASSERT_EQ((int ) TM1637_calculate_display(1.23, SEG4), 0x5b);

    ASSERT_EQ((int ) TM1637_calculate_display(12.34, SEG1), 0x3f);
    ASSERT_EQ((int ) TM1637_calculate_display(12.34, SEG2), 0x06);
    ASSERT_EQ((int ) TM1637_calculate_display(12.34, SEG3), 0x5b);
    ASSERT_EQ((int ) TM1637_calculate_display(12.34, SEG4), 0x4f);

    ASSERT_EQ((int ) TM1637_calculate_display(123.45, SEG1), 0x06);
    ASSERT_EQ((int ) TM1637_calculate_display(123.45, SEG2), 0x5b);
    ASSERT_EQ((int ) TM1637_calculate_display(123.45, SEG3), 0x4f);
    ASSERT_EQ((int ) TM1637_calculate_display(123.45, SEG4), 0x66);

    ASSERT_EQ((int ) TM1637_calculate_display(999.9, SEG1), 0x6f);
    ASSERT_EQ((int ) TM1637_calculate_display(999.9, SEG2), 0x6f);
    ASSERT_EQ((int ) TM1637_calculate_display(999.9, SEG3), 0x6f);
    ASSERT_EQ((int ) TM1637_calculate_display(999.9, SEG4), 0x6f);
}

/*
 * Testet verschiedene negative Werte
 * @param void
 * @return void
 */
TEST(Segmentanzeige, test_calculateDisplay_different_neg_values) {
    PRINT_INFO(
            "TM1637_calculate_display() mit verschiedenen negative Werten \n");

    ASSERT_EQ((int ) TM1637_calculate_display(-0.12, SEG1), 0x40);
    ASSERT_EQ((int ) TM1637_calculate_display(-0.12, SEG2), 0x3f);
    ASSERT_EQ((int ) TM1637_calculate_display(-0.12, SEG3), 0x3f);
    ASSERT_EQ((int ) TM1637_calculate_display(-0.12, SEG4), 0x06);

    ASSERT_EQ((int ) TM1637_calculate_display(-1.23, SEG1), 0x40);
    ASSERT_EQ((int ) TM1637_calculate_display(-1.23, SEG2), 0x3f);
    ASSERT_EQ((int ) TM1637_calculate_display(-1.23, SEG3), 0x06);
    ASSERT_EQ((int ) TM1637_calculate_display(-1.23, SEG4), 0x5b);

    ASSERT_EQ((int ) TM1637_calculate_display(-12.34, SEG1), 0x40);
    ASSERT_EQ((int ) TM1637_calculate_display(-12.34, SEG2), 0x06);
    ASSERT_EQ((int ) TM1637_calculate_display(-12.34, SEG3), 0x5b);
    ASSERT_EQ((int ) TM1637_calculate_display(-12.34, SEG4), 0x4f);

    ASSERT_EQ((int ) TM1637_calculate_display(-99.9, SEG1), 0x40);
    ASSERT_EQ((int ) TM1637_calculate_display(-99.9, SEG2), 0x6f);
    ASSERT_EQ((int ) TM1637_calculate_display(-99.9, SEG3), 0x6f);
    ASSERT_EQ((int ) TM1637_calculate_display(-99.9, SEG4), 0x6f);

}

/* ============================ABGABE======================================= */
#ifdef ABGABE

#endif  /* ABGABE */
/* ========================================================================= */

int main(int argc, char *argv[]) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
