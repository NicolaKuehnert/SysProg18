#include <string.h>
#include <gtest/gtest.h>
#include <stdlib.h>
#include <math.h>

extern "C" {
#include "pruefungen/pruefungen.h"
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
 * Testet ob ein einzelner Student richtig registriert wird.
 * @param void
 * @return void
 */
TEST(Pruefungen, test_register_single_student) {
    PRINT_INFO("Einzelnen Studenten registrieren. \n");

    extern student* exams[10];
    /* Anmeldeliste in pruefungen.c MUSS "exams" heissen! */

    memset(exams, 0, sizeof(student*) * 10);

    student std = { "Klaus", 20, IFM };
    int result = register_student(&std, 7);

    ASSERT_EQ(7, result);
    ASSERT_EQ(exams[7], &std);
}

/*
 * Testet wenn zwei Studenten auf die gleiche Pruefung registriert werden, ob
 * der Zweite an die naechste Position geschrieben wird.
 * @param void
 * @return void
 */
TEST(Pruefungen, test_register_two_students_with_same_nr) {
    PRINT_INFO(
            "Zwei Studenten auf den gleichen Pruefungsplatz registrieren. \n");

    extern student* exams[10];
    /* Anmeldeliste in pruefungen.c MUSS "exams" heissen! */

    memset(exams, 0, sizeof(student*) * 10);

    student std1 = { "Klaus", 20, IFM };
    student std2 = { "Peter", 30, IFM };

    int result1 = register_student(&std1, 1);
    int result2 = register_student(&std2, 1);

    ASSERT_EQ(1, result1);
    ASSERT_EQ(exams[1], &std1);

    ASSERT_EQ(2, result2);
    ASSERT_EQ(exams[2], &std2);
}

/*
 * Testet wenn zwei Studenten auf die letzte Pruefung registriert werden, ob
 * der Zweite an die erste Position geschrieben wird.
 * @param void
 * @return void
 */
TEST(Pruefungen, test_register_two_students_with_same_nr_overlimits) {
    PRINT_INFO("Zwei Studenten auf den letzten Pruefungsplatz registrieren. \n");

    extern student* exams[10];
    /* Anmeldeliste in pruefungen.c MUSS "exams" heissen! */

    memset(exams, 0, sizeof(student*) * 10);

    student std1 = { "Klaus", 20, IFM };
    student std2 = { "Peter", 30, IFM };

    int result1 = register_student(&std1, 9);
    int result2 = register_student(&std2, 9);

    ASSERT_EQ(9, result1);
    ASSERT_EQ(exams[9], &std1);

    ASSERT_EQ(0, result2);
    ASSERT_EQ(exams[0], &std2);
}

/*
 * Testet, ob abgefangen wird, wenn der gleiche Student zweimal eingetragen wird.
 * @param void
 * @return void
 */
TEST(Pruefungen, test_register_same_student_twice) {
    PRINT_INFO("Eienen Studenten zweimal registrieren. \n");

    extern student* exams[10];
    /* Anmeldeliste in pruefungen.c MUSS "exams" heissen! */

    memset(exams, 0, sizeof(student*) * 10);

    student std1 = { "Klaus", 20, IFM };

    int result1 = register_student(&std1, 1);
    int result2 = register_student(&std1, 2);

    ASSERT_EQ(1, result1);
    ASSERT_EQ(exams[1], &std1);

    ASSERT_EQ(-1, result2);
    ASSERT_EQ(NULL, exams[2]);
}

/*
 * Testet, ob abgefangen wird, wenn versucht wird einen Studenten ausserhalb
 * der moeglichen Pruefungen einzutragen und, ob er dann nicht irgendwo eigetragen wird.
 * @param void
 * @return void
 */
TEST(Pruefungen, test_register_student_wrong_number) {
    PRINT_INFO("Studenten mit ungueltiger Pruefungsnummer registrieren. \n");

    extern student* exams[10];
    /* Anmeldeliste in pruefungen.c MUSS "exams" heissen! */

    memset(exams, 0, sizeof(student*) * 10);

    student std1 = { "Klaus", 20, IFM };

    int result1 = register_student(&std1, -1);
    int result2 = register_student(&std1, 10);

    ASSERT_EQ(-1, result1);
    ASSERT_EQ(-1, result2);
    for (int i = 0; i < 10; i++) {
        ASSERT_EQ(NULL, exams[i]);
    }
}

/*
 * Testet, ob das Eintragen in eine volle Liste abgefangen wird.
 * @param void
 * @return void
 */
TEST(Pruefungen, test_register_student_full) {
    PRINT_INFO("Studenten registrieren wenn keine Pruefung frei. \n");

    extern student* exams[10];
    /* Anmeldeliste in pruefungen.c MUSS "exams" heissen! */

    memset(exams, 1, sizeof(student*) * 10);

    student std2 = { "Klaus", 20, IFM };

    int result1 = register_student(&std2, 1);

    ASSERT_EQ(-1, result1);
}

/*
 * Testet, ob ein Student eingetragen wird wenn der letzte Platz vor seinem gewaehlten liegt.
 * @param void
 * @return void
 */
TEST(Pruefungen, test_register_student_one_bevor) {
    PRINT_INFO("Studenten registrieren wenn ein Pruefung davor frei. \n");

    extern student* exams[10];
    /* Anmeldeliste in pruefungen.c MUSS "exams" heissen! */

    memset(exams, 0, sizeof(student*) * 10);

    for (int i = 1; i < 10; i++) {
        exams[i] = (student*) 1;
    }

    student std2 = { "Klaus", 20, IFM };

    int result1 = register_student(&std2, 3);

    ASSERT_EQ(0, result1);
    ASSERT_EQ(&std2, exams[0]);
}

/*
 * Testet, ob ein Nullpointer abgefangen wird.
 * @param void
 * @return void
 */
TEST(Pruefungen, test_register_student_null) {
    PRINT_INFO("kein Nullpointer wird registriert. \n");

    extern student* exams[10];
    /* Anmeldeliste in pruefungen.c MUSS "exams" heissen! */

    memset(exams, 0, sizeof(student*) * 10);

    int result1 = register_student(exams[0], 1);

    ASSERT_EQ(-1, result1);
}

/*
 * Testet, ob Student wieder richtig entfernt wird.
 * @param void
 * @return void
 */
TEST(Pruefungen, test_remove_student) {
    PRINT_INFO("Studenten aus Liste entfernen. \n");

    extern student* exams[10];
    /* Anmeldeliste in pruefungen.c MUSS "exams" heissen! */

    memset(exams, 0, sizeof(student*) * 10);

    student std2 = { "Klaus", 20, IFM };
    exams[5] = (&std2);

    int result1 = remove_student(&std2);

    ASSERT_EQ(5, result1);
    ASSERT_EQ(NULL, exams[5]);
}

/*
 * Testet, ob -1 - bei dem Versuch einen unbekannten Studenten
 * zu entfernen - zurueckgeliefert wird.
 * @param void
 * @return void
 */
TEST(Pruefungen, test_remove_student_unknown) {
    PRINT_INFO("Unbekannten Studenten aus Liste entfernen. \n");

    extern student* exams[10];
    /* Anmeldeliste in pruefungen.c MUSS "exams" heissen! */

    memset(exams, 0, sizeof(student*) * 10);

    student std2 = { "Klaus", 20, IFM };
    exams[5] = (&std2);
    student std1 = { "Peter", 40, IFM };
    int result1 = remove_student(&std1);

    ASSERT_EQ(-1, result1);
}

/*
 * Testet, ob ein Nullpointer abgefangen wird.
 * @param void
 * @return void
 */
TEST(Pruefungen, test_remove_student_null) {
    PRINT_INFO("NULL Studenten aus Liste entfernen. \n");

    extern student* exams[10];
    /* Anmeldeliste in pruefungen.c MUSS "exams" heissen! */

    memset(exams, 0, sizeof(student*) * 10);

    student std2 = { "Klaus", 20, IFM };
    exams[5] = (&std2);
    int result1 = remove_student((student*) 0);

    ASSERT_EQ(-1, result1);
}

/*
 * Testet, ob der errechnete Prozentwert richtig ist und darauf geachtet wird,
 * ob leere Eintraege vorhanden sind.
 * @param void
 * @return void
 */
TEST(Pruefungen, test_calculate_average) {
    PRINT_INFO(
            "Prueft, ob calculate_average das gewuenschte Ergebnis liefert. \n");

    extern student* exams[10];
    /* Anmeldeliste in pruefungen.c MUSS "exams" heissen! */

    memset(exams, 0, sizeof(student*) * 10);

    student std1 = { "Klaus", 20, IFM };
    student std2 = { "Peter", 26, IFM };
    student std3 = { "Eugen", 90, IFM };
    student std4 = { "Pimpelhuber", 268, IFM };
    student std5 = { "Dosenkohl", 305, IFM };

    exams[0] = &std1;
    exams[4] = &std2;
    exams[3] = &std3;
    exams[7] = &std4;
    exams[9] = &std5;

    /* expected result depends on MAXIMUM_POINTS, which should be defined as 320 */
    ASSERT_TRUE(fabs(44.3125 - calculate_average()) <= 0.0001);
}

/*
 * Testet, ob die Rueckgabe bei einer leeren Liste gleich 0 ist.
 * @param void
 * @return void
 */
TEST(Pruefungen, test_calculate_average_empty) {
    PRINT_INFO(
            "Prueft, ob calculate_average das gewuenschte Ergebnis liefert. \n");

    extern student* exams[10];
    /* Anmeldeliste in pruefungen.c MUSS "exams" heissen! */

    memset(exams, 0, sizeof(student*) * 10);

    ASSERT_TRUE(fabs(0.0 - calculate_average()) <= 0.0001);
}

/* ============================ABGABE======================================= */
#ifdef ABGABE

#endif  /* ABGABE */
/* ========================================================================= */

int main(int argc, char *argv[]) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
