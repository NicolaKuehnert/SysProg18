#include <string.h>
#include <gtest/gtest.h>

#include "studiverwaltung/studiverwaltung.h"

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
 * Testet ob das enum Studiengang korrekt definiert wurde
 * @param void
 * @return void
 */
TEST(Studiverwaltung, test_enum) {
    PRINT_INFO("Wurde das enum degree_program korrekt definiert?\n");

degree_program test;

    test = IFM;
    test = ELM;
    test = PFLEGE;

    ASSERT_EQ(test, PFLEGE); /* suppress warning: unused variables */
    SUCCEED();
}

/*
 * Testet ob NULL returned wird, wenn NULL uebergeben wurde.
 *
 * @param void
 * @return void
 */
TEST(Studiverwaltung, test_get_list_origin_with_null) {
    PRINT_INFO(
        "get_list_origin: Wird NULL returned, wenn NULL uebergeben wurde?\n");

node *begin = NULL;

    ASSERT_EQ(NULL, get_list_origin(begin));
}

/*
 * Testet ob die Wurzel einer Liste gefunden wird
 *      - bei nur einer node.
 *
 * @param void
 * @return void
 */
TEST(Studiverwaltung, test_get_list_origin_with_one) {
    PRINT_INFO(
        "get_list_origin: Wird die Wurzel der Liste returned - bei nur einer node in der Liste.\n");

node *begin = NULL;
    student studiTest;

    begin = append_student(NULL, &studiTest);

    ASSERT_EQ(get_list_origin(begin), begin);

    free(begin);
}

/*
 * Testet ob die Wurzel einer Liste gefunden wird
 *      - bei uebergabe einer mittleren node.
 *
 * @param void
 * @return void
 */
TEST(Studiverwaltung, test_get_list_origin_middle_node) {
    PRINT_INFO(
        "get_list_origin: Wird die Wurzel der Liste returned, wenn ein mittlerer node uebergeben wurde?\n");

node *begin = NULL;
    node *end = NULL;

    student studiTest;
    student studiTest1;
    student studiTest2;
    student studiTest3;
    student studiTest4;
    student studiTest5;

    begin = append_student(NULL, &studiTest);
    begin = append_student(begin, &studiTest1);
    begin = append_student(begin, &studiTest2);
    begin = append_student(begin, &studiTest3);
    begin = append_student(begin, &studiTest4);
    begin = append_student(begin, &studiTest5);

    end = begin->next->next->next->next->next;

    ASSERT_EQ(get_list_origin(end->prev->prev), begin);

    free(end->prev->prev->prev->prev->prev);
    free(end->prev->prev->prev->prev);
    free(end->prev->prev->prev);
    free(end->prev->prev);
    free(end->prev);
    free(end);
}

/*
 * Testet ob die Wurzel einer Liste gefunden wird.
 *
 * @param void
 * @return void
 */
TEST(Studiverwaltung, test_get_list_origin) {
    PRINT_INFO("get_list_origin: Wird die Wurzel der Liste returned?\n");

node *begin = NULL;
    node *end = NULL;

    student studiTest;
    student studiTest1;
    student studiTest2;
    student studiTest3;
    student studiTest4;
    student studiTest5;

    begin = append_student(NULL, &studiTest);
    begin = append_student(begin, &studiTest1);
    begin = append_student(begin, &studiTest2);
    begin = append_student(begin, &studiTest3);
    begin = append_student(begin, &studiTest4);
    begin = append_student(begin, &studiTest5);

    end = begin->next->next->next->next->next;

    ASSERT_EQ(get_list_origin(end), begin);

    free(end->prev->prev->prev->prev->prev);
    free(end->prev->prev->prev->prev);
    free(end->prev->prev->prev);
    free(end->prev->prev);
    free(end->prev);
    free(end);
}

/*
 * Testet ob das Feld existiert.
 * Compiliert nicht, wenn NAME_LENGTH nicht existiert.
 * @param void
 * @return void
 */
TEST(Studiverwaltung, test_name_length_exists) {
    PRINT_INFO("Wurde das Feld NAME_LENGTH definiert?\n");

int x = NAME_LENGTH;

    ASSERT_GT(x, 0);
}

/*
 * Testet ob die student Struct korrekt definiert wurde
 * Kompiliert nicht, wenn student anders definiert wurde!
 * @param void
 * @return void
 */
TEST(Studiverwaltung, test_student) {
    PRINT_INFO("Wurde die struct student korrekt definiert?\n");

student stdnt;

    strcpy(stdnt.name, "Horst");
    stdnt.cps = 42;
    stdnt.enrolled = IFM;

    SUCCEED();
}

/*
 * Testet ob die Node Struct korrekt definiert wurde
 * Compiliert nicht, wenn Node anders definiert wurde!
 * @param void
 * @return void
 */
TEST(Studiverwaltung, test_node) {
    PRINT_INFO("Wurde die struct Node korrekt definiert?\n");

student studiTest;
    node n;

    n.stdnt = &studiTest;
    n.next = &n;
    n.prev = &n;

    SUCCEED();
}

/*
 * Testet ob cps nicht negativ werden koennen.
 * @param void
 * @return void
 */
TEST(Studiverwaltung, test_studi_cps_with_negativ_cps) {
    PRINT_INFO("Koennen student cps neagtiv sein?\n");

student s;

    strcpy(s.name, "Horst");
    s.cps = -42;
    s.enrolled = IFM;

    ASSERT_GT(s.cps, 0);
}

/*
 * Testet ob bei einem NULL student NULL returned wird
 * @param void
 * @return void
 */
TEST(Studiverwaltung, test_append_student_with_null_student) {
    PRINT_INFO(
        "append_student: Wird bei einem NULL Studi auch wirklich NULL returned?\n");

node *erg = append_student(NULL, NULL);

    ASSERT_EQ(NULL, erg);
}

/*
 * Testet ob bei einem NULL Node und einem student eine neue Liste
 * returned wird
 * @param void
 * @return void
 */
TEST(Studiverwaltung, test_append_student_with_student) {
    PRINT_INFO(
        "append_student: Wird eine neue Liste returned, wenn wir einen NULL Node uebergeben?\n");

student studiTest;

    node *erg = append_student(NULL, &studiTest);

    ASSERT_TRUE(erg);
    ASSERT_EQ(erg->stdnt, &studiTest);
    ASSERT_EQ(NULL, erg->next);
    ASSERT_EQ(NULL, erg->prev);

    free(erg);
}

/*
 * Testet ob bei einer nicht-leeren Liste und NULL Studi die Liste nicht
 * geaendert wird
 * @param void
 * @return void
 */
TEST(Studiverwaltung, test_append_student_list_with_null_student) {
    PRINT_INFO(
        "append_student: Wird eine nicht leere Liste bei einem NULL Studi veraendert?\n");

student studiTest;

    node *erg1 = append_student(NULL, &studiTest);
    node *erg2 = append_student(erg1, NULL);

    ASSERT_EQ(erg2, erg1);
    ASSERT_EQ(erg2->stdnt, &studiTest);
    ASSERT_EQ(NULL, erg2->next);
    ASSERT_EQ(NULL, erg2->prev);

    free(erg1);
}

/*
 * Testet ob bei append_studi neue students richtig eingefuegt werden
 * @param void
 * @return void
 */
TEST(Studiverwaltung, test_append_student_with_more_students) {
    PRINT_INFO(
        "append_student: Werden neue Studis korrekt in die Liste eingepflegt?\n");

node *listTest = NULL;
    student studiTest;
    student studiTest1;
    student studiTest2;

    listTest = append_student(NULL, &studiTest);
    listTest = append_student(listTest, &studiTest1);
    listTest = append_student(listTest, &studiTest2);

    ASSERT_TRUE(listTest);
    ASSERT_EQ(listTest->stdnt, &studiTest);
    ASSERT_EQ(listTest->next->stdnt, &studiTest1);
    ASSERT_EQ(NULL, listTest->prev);

    listTest = listTest->next;
    ASSERT_EQ(listTest->stdnt, &studiTest1);
    ASSERT_EQ(listTest->next->stdnt, &studiTest2);
    ASSERT_EQ(listTest->prev->stdnt, &studiTest);

    listTest = listTest->next;
    ASSERT_EQ(listTest->stdnt, &studiTest2);
    ASSERT_EQ(NULL, listTest->next);
    ASSERT_EQ(listTest->prev->stdnt, &studiTest1);

    free(listTest->prev->prev);
    free(listTest->prev);
    free(listTest);
}

/*
 * Testet ob bei append_studi neue students richtig eingefuegt werden
 *      - wenn nicht der Listenanfang uebergeben wird
 * @param void
 * @return void
 */
TEST(Studiverwaltung, test_append_student_with_more_students_glo) {
    PRINT_INFO(
        "append_student: Werden neue Studis korrekt in die Liste eingepflegt?\n");

node *listTest = NULL;
    student studiTest;
    student studiTest1;
    student studiTest2;
    student studiTest3;

    listTest = append_student(NULL, &studiTest);
    listTest = append_student(listTest, &studiTest1);
    listTest = append_student(listTest->next, &studiTest2);
    listTest = append_student(listTest->next->next, &studiTest3);

    ASSERT_TRUE(listTest);
    ASSERT_EQ(listTest->stdnt, &studiTest);
    ASSERT_EQ(listTest->next->stdnt, &studiTest1);
    ASSERT_EQ(NULL, listTest->prev);

    listTest = listTest->next;
    ASSERT_EQ(listTest->stdnt, &studiTest1);
    ASSERT_EQ(listTest->next->stdnt, &studiTest2);
    ASSERT_EQ(listTest->prev->stdnt, &studiTest);

    listTest = listTest->next;
    ASSERT_EQ(listTest->stdnt, &studiTest2);
    ASSERT_EQ(listTest->next->stdnt, &studiTest3);
    ASSERT_EQ(listTest->prev->stdnt, &studiTest1);

    listTest = listTest->next;
    ASSERT_EQ(listTest->stdnt, &studiTest3);
    ASSERT_EQ(NULL, listTest->next);
    ASSERT_EQ(listTest->prev->stdnt, &studiTest2);

    free(listTest->prev->prev->prev);
    free(listTest->prev->prev);
    free(listTest->prev);
    free(listTest);
}

/*
 * Test ob die NULL-Liste geloescht wird
 * @param void
 * @return void
 */
TEST(Studiverwaltung, test_delete_node_null_list_nodeonly) {
    PRINT_INFO("delete_node: Wird eine NULL Liste geloescht?\n");

    node *erg = delete_node(NULL, NODE_ONLY);

    ASSERT_EQ(NULL, erg);
}

/*
 * Test ob die Liste geloescht wird wenn nur eine Node in der Liste ist.
 * @param void
 * @return void
 */
TEST(Studiverwaltung, test_delete_node_only_node_nodeonly) {
    PRINT_INFO("delete_node: Wird eine Liste mit nur einem Node geloescht?\n");

    node *listTest = NULL;
    student *studiTest;

    studiTest = (student*) malloc(sizeof(student));
    CHECK_MALLOC(studiTest);
    strcpy(studiTest->name, "Horst");
    studiTest->cps = 42;
    studiTest->enrolled = IFM;

    listTest = append_student(NULL, studiTest);

    node *erg = delete_node(listTest, NODE_ONLY);

    ASSERT_EQ(NULL, erg);

    free(studiTest);
}

/*
 * Testet ob der erste Knoten in der Liste geloescht wird
 * @param void
 * @return void
 */
TEST(Studiverwaltung, test_delete_node_first_node_nodeonly) {
    PRINT_INFO("delete_node: Wird die erste Node in der Liste geloescht?\n");

    node *listTest = NULL;
    student *studiTest;
    student *studiTest1;

    studiTest = (student*) malloc(sizeof(student));
    CHECK_MALLOC(studiTest);strcpy(studiTest->name, "Horst");
    studiTest->cps = 42;
    studiTest->enrolled = IFM;

    studiTest1 = (student*) malloc(sizeof(student));
    CHECK_MALLOC(studiTest1);
    strcpy(studiTest1->name, "Huhn");
    studiTest1->cps = 39;
    studiTest1->enrolled = ELM;

    listTest = append_student(NULL, studiTest);
    listTest = append_student(listTest, studiTest1);

    node *erg = delete_node(listTest, NODE_ONLY);

    ASSERT_NE(erg, listTest);
    ASSERT_EQ(erg->stdnt, studiTest1);
    ASSERT_EQ(NULL, erg->next);
    ASSERT_EQ(NULL, erg->prev);

    free(studiTest);
    free(studiTest1);
    free(erg);
}

/*
 * Testet ob die letzte Node in der Liste geloescht wird
 *
 * @param void
 * @return void
 */
TEST(Studiverwaltung, test_delete_node_last_node_nodeonly) {
    PRINT_INFO("delete_node: Wird die letzte Node in der Liste geloescht?\n");

    node *listTest = NULL;
    student *studiTest;
    student *studiTest1;

    studiTest = (student*) malloc(sizeof(student));
    CHECK_MALLOC(studiTest);strcpy(studiTest->name, "Horst");
    studiTest->cps = 42;
    studiTest->enrolled = IFM;

    studiTest1 = (student*) malloc(sizeof(student));
    CHECK_MALLOC(studiTest1);
    strcpy(studiTest1->name, "Huhn");
    studiTest1->cps = 39;
    studiTest1->enrolled = ELM;

    listTest = append_student(NULL, studiTest);
    listTest = append_student(listTest, studiTest1);

    node *erg = delete_node(listTest->next, NODE_ONLY);

    ASSERT_EQ(erg, listTest);
    ASSERT_EQ(erg->stdnt, studiTest);
    ASSERT_EQ(NULL, erg->next);
    ASSERT_EQ(NULL,erg->prev);

    free(studiTest);
    free(studiTest1);
    free(erg);
}

/*
 * Testet ob eine mittlere Node in der Liste geloescht wird
 *
 * @param void
 * @return void
 */
TEST(Studiverwaltung, test_delete_node_middle_node_nodeonly) {
    PRINT_INFO("delete_node: Wird eine mittlere Node in der Liste geloescht?\n");

    node *listTest = NULL;
    student *studiTest;
    student *studiTest1;
    student *studiTest2;

    studiTest = (student*) malloc(sizeof(student));
    CHECK_MALLOC(studiTest);strcpy(studiTest->name, "Horst");
    studiTest->cps = 42;
    studiTest->enrolled = IFM;

    studiTest1 = (student*) malloc(sizeof(student));
    CHECK_MALLOC(studiTest1);strcpy(studiTest1->name, "Horst");
    studiTest1->cps = 42;
    studiTest1->enrolled = IFM;

    studiTest2 = (student*) malloc(sizeof(student));
    CHECK_MALLOC(studiTest2);
    strcpy(studiTest->name, "Horst");
    studiTest2->cps = 42;
    studiTest2->enrolled = IFM;

    listTest = append_student(NULL, studiTest);
    listTest = append_student(listTest, studiTest1);
    listTest = append_student(listTest, studiTest2);

    node *erg = delete_node(listTest->next, NODE_ONLY);

    ASSERT_EQ(erg, listTest);
    ASSERT_EQ(listTest->stdnt, studiTest);
    ASSERT_EQ(listTest->next->stdnt, studiTest2);
    ASSERT_EQ(NULL, listTest->prev);

    listTest = listTest->next;
    ASSERT_EQ(listTest->stdnt, studiTest2);
    ASSERT_EQ(NULL, listTest->next);
    ASSERT_EQ(listTest->prev->stdnt, studiTest);

    free(studiTest);
    free(studiTest1);
    free(studiTest2);
    free(erg->next);
    free(erg);
}

/*
 * Test ob die NULL-Liste geloescht wird
 * @param void
 * @return void
 */
TEST(Studiverwaltung, test_delete_node_null_list_nodeandstudent) {
    PRINT_INFO("delete_node: Wird eine NULL Liste geloescht?\n");

    node *erg = delete_node(NULL, NODE_AND_STUDENT);

    ASSERT_EQ(NULL, erg);
}

/*
 * Test ob die Liste geloescht wird wenn nur eine Node in der Liste ist.
 * @param void
 * @return void
 */
TEST(Studiverwaltung, test_delete_node_only_node_nodeandstudent) {
    PRINT_INFO("delete_node: Wird eine Liste mit nur einem Node geloescht?\n");

    node *listTest = NULL;
    student *studiTest;

    studiTest = (student*) malloc(sizeof(student));
    CHECK_MALLOC(studiTest);
    strcpy(studiTest->name, "Horst");
    studiTest->cps = 42;
    studiTest->enrolled = IFM;

    listTest = append_student(NULL, studiTest);

    node *erg = delete_node(listTest, NODE_AND_STUDENT);

    ASSERT_EQ(NULL, erg);
}

/*
 * Testet ob der erste Knoten in der Liste geloescht wird
 * @param void
 * @return void
 */
TEST(Studiverwaltung, test_delete_node_first_node_nodeandstudent) {
    PRINT_INFO("delete_node: Wird die erste Node in der Liste geloescht?\n");

    node *listTest = NULL;
    student *studiTest;
    student *studiTest1;

    studiTest = (student*) malloc(sizeof(student));
    CHECK_MALLOC(studiTest);strcpy(studiTest->name, "Horst");
    studiTest->cps = 42;
    studiTest->enrolled = IFM;

    studiTest1 = (student*) malloc(sizeof(student));
    CHECK_MALLOC(studiTest1);
    strcpy(studiTest1->name, "Huhn");
    studiTest1->cps = 39;
    studiTest1->enrolled = ELM;

    listTest = append_student(NULL, studiTest);
    listTest = append_student(listTest, studiTest1);

    node *erg = delete_node(listTest, NODE_AND_STUDENT);

    ASSERT_NE(erg, listTest);
    ASSERT_EQ(erg->stdnt, studiTest1);
    ASSERT_EQ(NULL, erg->next);
    ASSERT_EQ(NULL, erg->prev);

    free(studiTest1);
    free(erg);
}

/*
 * Testet ob die letzte Node in der Liste geloescht wird
 *
 * @param void
 * @return void
 */
TEST(Studiverwaltung, test_delete_node_last_node_nodeandstudent) {
    PRINT_INFO("delete_node: Wird die letzte Node in der Liste geloescht?\n");

    node *listTest = NULL;
    student *studiTest;
    student *studiTest1;

    studiTest = (student*) malloc(sizeof(student));
    CHECK_MALLOC(studiTest);strcpy(studiTest->name, "Horst");
    studiTest->cps = 42;
    studiTest->enrolled = IFM;

    studiTest1 = (student*) malloc(sizeof(student));
    CHECK_MALLOC(studiTest1);
    strcpy(studiTest1->name, "Huhn");
    studiTest1->cps = 39;
    studiTest1->enrolled = ELM;

    listTest = append_student(NULL, studiTest);
    listTest = append_student(listTest, studiTest1);

    node *erg = delete_node(listTest->next, NODE_AND_STUDENT);

    ASSERT_EQ(erg, listTest);
    ASSERT_EQ(erg->stdnt, studiTest);
    ASSERT_EQ(NULL, erg->next);
    ASSERT_EQ(NULL,erg->prev);

    free(studiTest);
    free(erg);
}

/*
 * Testet ob eine mittlere Node in der Liste geloescht wird
 *
 * @param void
 * @return void
 */
TEST(Studiverwaltung, test_delete_node_middle_node_nodeandstudent) {
    PRINT_INFO("delete_node: Wird eine mittlere Node in der Liste geloescht?\n");

    node *listTest = NULL;
    student *studiTest;
    student *studiTest1;
    student *studiTest2;

    studiTest = (student*) malloc(sizeof(student));
    CHECK_MALLOC(studiTest);strcpy(studiTest->name, "Horst");
    studiTest->cps = 42;
    studiTest->enrolled = IFM;

    studiTest1 = (student*) malloc(sizeof(student));
    CHECK_MALLOC(studiTest1);strcpy(studiTest1->name, "Horst");
    studiTest1->cps = 42;
    studiTest1->enrolled = IFM;

    studiTest2 = (student*) malloc(sizeof(student));
    CHECK_MALLOC(studiTest2);
    strcpy(studiTest->name, "Horst");
    studiTest2->cps = 42;
    studiTest2->enrolled = IFM;

    listTest = append_student(NULL, studiTest);
    listTest = append_student(listTest, studiTest1);
    listTest = append_student(listTest, studiTest2);

    node *erg = delete_node(listTest->next, NODE_AND_STUDENT);

    ASSERT_EQ(erg, listTest);
    ASSERT_EQ(listTest->stdnt, studiTest);
    ASSERT_EQ(listTest->next->stdnt, studiTest2);
    ASSERT_EQ(NULL, listTest->prev);

    listTest = listTest->next;
    ASSERT_EQ(listTest->stdnt, studiTest2);
    ASSERT_EQ(NULL, listTest->next);
    ASSERT_EQ(listTest->prev->stdnt, studiTest);

    free(studiTest);
    free(studiTest2);
    free(erg->next);
    free(erg);
}


/*
 * Testet ob deleteStudiList 0 returned wenn die Liste NULL ist
 *
 * @param void
 * @return void
 */
TEST(Studiverwaltung, test_delete_studi_list_with_null) {
    PRINT_INFO("delete_list: Wird 0 returned, wenn NULL uebergeben wird?\n");

int erg = delete_list(NULL);

    ASSERT_TRUE(!erg);
}

/*
 * Testet ob die deleteStudiList wirklich 3 Nodes loescht
 *
 * @param void
 * @return void
 */
TEST(Studiverwaltung, test_delete_studi_list) {
    PRINT_INFO("delete_list: Werden alle Studis in der Liste geloescht?\n");

node *listTest = NULL;
    student *studiTest;
    student *studiTest1;
    student *studiTest2;

    studiTest = (student*) malloc(sizeof(student));
    CHECK_MALLOC(studiTest);strcpy(studiTest->name, "Horst");
    studiTest->cps = 42;
    studiTest->enrolled = IFM;

    studiTest1 = (student*) malloc(sizeof(student));
    CHECK_MALLOC(studiTest1);strcpy(studiTest1->name, "Horst");
    studiTest1->cps = 42;
    studiTest1->enrolled = IFM;

    studiTest2 = (student*) malloc(sizeof(student));
    CHECK_MALLOC(studiTest2);
strcpy(studiTest->name, "Horst");
    studiTest2->cps = 42;
    studiTest2->enrolled = IFM;

    listTest = append_student(NULL, studiTest);
    append_student(listTest, studiTest1);
    append_student(listTest, studiTest2);

    int erg = delete_list(listTest);

    ASSERT_TRUE(erg == 3);
}

/*
 * Testet ob die deleteStudiList wirklich 3 Nodes loescht
 *  - wenn nicht der Listenanfang uebergeben wird
 *
 * @param void
 * @return void
 */
TEST(Studiverwaltung, test_delete_studi_list_glo) {
    PRINT_INFO(
        "delete_list: Werden alle Studis in der Liste geloescht, wenn nicht der Listenanfang uebergeben wurde?\n");

node *listTest = NULL;
    student *studiTest;
    student *studiTest1;
    student *studiTest2;

    studiTest = (student*) malloc(sizeof(student));
    CHECK_MALLOC(studiTest);strcpy(studiTest->name, "Horst");
    studiTest->cps = 42;
    studiTest->enrolled = IFM;

    studiTest1 = (student*) malloc(sizeof(student));
    CHECK_MALLOC(studiTest1);strcpy(studiTest1->name, "Horst");
    studiTest1->cps = 42;
    studiTest1->enrolled = IFM;

    studiTest2 = (student*) malloc(sizeof(student));
    CHECK_MALLOC(studiTest2);
strcpy(studiTest->name, "Horst");
    studiTest2->cps = 42;
    studiTest2->enrolled = IFM;

    listTest = append_student(NULL, studiTest);
    append_student(listTest, studiTest1);
    append_student(listTest, studiTest2);

    int erg = delete_list(listTest->next->next);

    ASSERT_TRUE(erg == 3);
}

/*
 * testet ob NULL returned wird wenn die ubergebene Liste NULL ist
 *
 * @param void
 * @return void
 */
TEST(Studiverwaltung, test_get_ifm_students_with_null) {
    PRINT_INFO(
        "get_ifm_students: Wird NULL returned, wenn NULL uebergeben wurde?\n");

node *erg = get_ifm_students(NULL);

    ASSERT_EQ(NULL, erg);
}

/*
 * testet ob die richtige Anzahl an students zurueck kommt
 *
 * @param void
 * @return void
 */
TEST(Studiverwaltung, test_get_ifm_students) {
    PRINT_INFO(
        "get_ifm_students: Wird die richtige Anzahl an Informatik Studis returned?\n");

node *listTest = NULL;
    node *helpTest = NULL;
    student studiTest;
    student studiTest1;
    student studiTest2;

    strcpy(studiTest.name, "Horst");
    studiTest.cps = 42;
    studiTest.enrolled = IFM;

    strcpy(studiTest1.name, "Horst");
    studiTest1.cps = 42;
    studiTest1.enrolled = ELM;

    strcpy(studiTest2.name, "Horst");
    studiTest2.cps = 42;
    studiTest2.enrolled = IFM;

    listTest = append_student(NULL, &studiTest);
    listTest = append_student(listTest, &studiTest1);
    listTest = append_student(listTest, &studiTest2);

    helpTest = get_ifm_students(listTest);

    ASSERT_TRUE(helpTest);
    ASSERT_EQ(helpTest->stdnt, &studiTest);
    ASSERT_EQ(helpTest->next->stdnt, &studiTest2);
    ASSERT_EQ(NULL, helpTest->prev);

    helpTest = helpTest->next;
    ASSERT_EQ(helpTest->stdnt, &studiTest2);
    ASSERT_EQ(NULL, helpTest->next);
    ASSERT_EQ(helpTest->prev->stdnt, &studiTest);

    ASSERT_EQ(listTest->stdnt, &studiTest);
    ASSERT_EQ(listTest->next->stdnt, &studiTest1);
    ASSERT_EQ(NULL, listTest->prev);

    listTest = listTest->next;
    ASSERT_EQ(listTest->stdnt, &studiTest1);
    ASSERT_EQ(listTest->next->stdnt, &studiTest2);
    ASSERT_EQ(listTest->prev->stdnt, &studiTest);

    listTest = listTest->next;
    ASSERT_EQ(listTest->stdnt, &studiTest2);
    ASSERT_EQ(NULL, listTest->next);
    ASSERT_EQ(listTest->prev->stdnt, &studiTest1);

    free(listTest->prev->prev);
    free(listTest->prev);
    free(listTest);
    free(helpTest->prev);
    free(helpTest);
}

/*
 * testet ob die richtige Anzahl an students zurueck kommt
 *      - wenn nicht der Listenanfang uebergeben wird
 *
 * @param void
 * @return void
 */
TEST(Studiverwaltung, test_get_ifm_students_glo) {
    PRINT_INFO(
        "get_ifm_students: Wird die richtige Anzahl an Informatik Studis returned, wenn nicht der Listenanfang uebergeben wurde?\n");

node *listTest = NULL;
    node *helpTest = NULL;
    student studiTest;
    student studiTest1;
    student studiTest2;

    strcpy(studiTest.name, "Horst");
    studiTest.cps = 42;
    studiTest.enrolled = IFM;

    strcpy(studiTest1.name, "Horst");
    studiTest1.cps = 42;
    studiTest1.enrolled = ELM;

    strcpy(studiTest2.name, "Horst");
    studiTest2.cps = 42;
    studiTest2.enrolled = IFM;

    listTest = append_student(NULL, &studiTest);
    listTest = append_student(listTest, &studiTest1);
    listTest = append_student(listTest, &studiTest2);

    helpTest = get_ifm_students(listTest->next->next);

    ASSERT_TRUE(helpTest);
    ASSERT_EQ(helpTest->stdnt, &studiTest);
    ASSERT_EQ(helpTest->next->stdnt, &studiTest2);
    ASSERT_EQ(NULL, helpTest->prev);

    helpTest = helpTest->next;
    ASSERT_EQ(helpTest->stdnt, &studiTest2);
    ASSERT_EQ(NULL, helpTest->next);
    ASSERT_EQ(helpTest->prev->stdnt, &studiTest);

    ASSERT_EQ(listTest->stdnt, &studiTest);
    ASSERT_EQ(listTest->next->stdnt, &studiTest1);
    ASSERT_EQ(NULL, listTest->prev);

    listTest = listTest->next;
    ASSERT_EQ(listTest->stdnt, &studiTest1);
    ASSERT_EQ(listTest->next->stdnt, &studiTest2);
    ASSERT_EQ(listTest->prev->stdnt, &studiTest);

    listTest = listTest->next;
    ASSERT_EQ(listTest->stdnt, &studiTest2);
    ASSERT_EQ(NULL, listTest->next);
    ASSERT_EQ(listTest->prev->stdnt, &studiTest1);

    free(listTest->prev->prev);
    free(listTest->prev);
    free(listTest);
    free(helpTest->prev);
    free(helpTest);
}

/*
 * testet ob die richtige Anzahl an Studis zurueck kommt
 *
 * @param void
 * @return void
 */
TEST(Studiverwaltung, test_get_ifm_students_no_ifm) {
    PRINT_INFO(
        "get_ifm_students: Wird die richtige Anzahl an Informatik Studis returned, wenn niemand Informatik studiert?\n");

node *listTest = NULL;
    node *helpTest = NULL;
    student studiTest;
    student studiTest1;
    student studiTest2;

    strcpy(studiTest.name, "Horst");
    studiTest.cps = 42;
    studiTest.enrolled = ELM;

    strcpy(studiTest1.name, "Horst");
    studiTest1.cps = 42;
    studiTest1.enrolled = ELM;

    strcpy(studiTest2.name, "Horst");
    studiTest2.cps = 42;
    studiTest2.enrolled = ELM;

    listTest = append_student(NULL, &studiTest);
    listTest = append_student(listTest, &studiTest1);
    listTest = append_student(listTest, &studiTest2);

    helpTest = get_ifm_students(listTest);

    ASSERT_EQ(NULL, helpTest);
    ASSERT_EQ(listTest->stdnt, &studiTest);
    ASSERT_EQ(listTest->next->stdnt, &studiTest1);
    ASSERT_EQ(NULL, listTest->prev);

    listTest = listTest->next;
    ASSERT_EQ(listTest->stdnt, &studiTest1);
    ASSERT_EQ(listTest->next->stdnt, &studiTest2);
    ASSERT_EQ(listTest->prev->stdnt, &studiTest);

    listTest = listTest->next;
    ASSERT_EQ(listTest->stdnt, &studiTest2);
    ASSERT_EQ(NULL, listTest->next);
    ASSERT_EQ(listTest->prev->stdnt, &studiTest1);

    free(listTest->prev->prev);
    free(listTest->prev);
    free(listTest);
}

/*
 * testet ob 0 returned wird wenn NULL uebergeben wird
 *
 * @param void
 * @return void
 */
TEST(Studiverwaltung, test_delete_list_partial_with_null) {
    PRINT_INFO("delete_list_partial: Wird 0 returned, wenn NULL uebergeben wird\n");

int erg = delete_list_partial(NULL);

    ASSERT_TRUE(erg == 0);
}

/*
 * testet delete_list_partial mit nur einem Knoten
 *
 * @param void
 * @return void
 */
TEST(Studiverwaltung, test_delete_list_partial_with_one_node) {
    PRINT_INFO(
        "delete_list_partial: Wird 1 returned, wenn nur eine Node uebergeben wird?\n");

node *listTest = NULL;
    node *helpTest = NULL;
    student *studiTest;
    student *studiTest1;
    student *studiTest2;

    studiTest = (student*) malloc(sizeof(student));
    CHECK_MALLOC(studiTest);strcpy(studiTest->name, "Horst");
    studiTest->cps = 42;
    studiTest->enrolled = ELM;

    studiTest1 = (student*) malloc(sizeof(student));
    CHECK_MALLOC(studiTest1);strcpy(studiTest1->name, "Horst");
    studiTest1->cps = 42;
    studiTest1->enrolled = IFM;

    studiTest2 = (student*) malloc(sizeof(student));
    CHECK_MALLOC(studiTest2);
strcpy(studiTest->name, "Horst");
    studiTest2->cps = 42;
    studiTest2->enrolled = ELM;

    listTest = append_student(NULL, studiTest);
    listTest = append_student(listTest, studiTest1);
    listTest = append_student(listTest, studiTest2);

    helpTest = get_ifm_students(listTest);

    int erg = delete_list_partial(helpTest);

    ASSERT_TRUE(erg == 1);
    ASSERT_EQ(listTest->stdnt, studiTest);
    ASSERT_EQ(listTest->next->stdnt, studiTest1);
    ASSERT_EQ(NULL, listTest->prev);

    listTest = listTest->next;
    ASSERT_EQ(listTest->stdnt, studiTest1);
    ASSERT_EQ(listTest->next->stdnt, studiTest2);
    ASSERT_EQ(listTest->prev->stdnt, studiTest);

    listTest = listTest->next;
    ASSERT_EQ(listTest->stdnt, studiTest2);
    ASSERT_EQ(NULL, listTest->next);
    ASSERT_EQ(listTest->prev->stdnt, studiTest1);

    free(studiTest);
    free(studiTest1);
    free(studiTest2);
    free(listTest->prev->prev);
    free(listTest->prev);
    free(listTest);
}

/*
 * testet ob die richtige Anzahl an Knoten geloescht wird
 *
 * @param void
 * @return void
 */
TEST(Studiverwaltung, test_delete_list_partial) {
    PRINT_INFO(
        "delete_list_partial: Wird die korrekte Anzahl an Nodes geloescht?\n");

node *listTest = NULL;
    node *helpTest = NULL;
    student *studiTest;
    student *studiTest1;
    student *studiTest2;
    student *studiTest3;
    student *studiTest4;
    student *studiTest5;

    studiTest = (student*) malloc(sizeof(student));
    CHECK_MALLOC(studiTest);strcpy(studiTest->name, "Horst");
    studiTest->cps = 42;
    studiTest->enrolled = IFM;

    studiTest1 = (student*) malloc(sizeof(student));
    CHECK_MALLOC(studiTest1);strcpy(studiTest1->name, "Horst");
    studiTest1->cps = 42;
    studiTest1->enrolled = ELM;

    studiTest2 = (student*) malloc(sizeof(student));
    CHECK_MALLOC(studiTest2);strcpy(studiTest2->name, "Horst");
    studiTest2->cps = 42;
    studiTest2->enrolled = PFLEGE;

    studiTest3 = (student*) malloc(sizeof(student));
    CHECK_MALLOC(
        studiTest3);strcpy(studiTest3->name, "Horst");
    studiTest3->cps = 42;
    studiTest3->enrolled = IFM;

    studiTest4 = (student*) malloc(sizeof(student));
    CHECK_MALLOC(studiTest4);strcpy(studiTest4->name, "Horst");
    studiTest4->cps = 42;
    studiTest4->enrolled = PFLEGE;

    studiTest5 = (student*) malloc(sizeof(student));
    CHECK_MALLOC(studiTest5);
strcpy(studiTest5->name, "Horst");
    studiTest5->cps = 42;
    studiTest5->enrolled = IFM;

    listTest = append_student(NULL, studiTest);
    listTest = append_student(listTest, studiTest1);
    listTest = append_student(listTest, studiTest2);
    listTest = append_student(listTest, studiTest3);
    listTest = append_student(listTest, studiTest4);
    listTest = append_student(listTest, studiTest5);

    helpTest = get_ifm_students(listTest);

    int erg = delete_list_partial(helpTest);

    ASSERT_TRUE(erg == 3);
    ASSERT_EQ(listTest->stdnt, studiTest);
    ASSERT_EQ(listTest->next->stdnt, studiTest1);
    ASSERT_EQ(NULL, listTest->prev);

    listTest = listTest->next;
    ASSERT_EQ(listTest->stdnt, studiTest1);
    ASSERT_EQ(listTest->next->stdnt, studiTest2);
    ASSERT_EQ(listTest->prev->stdnt, studiTest);

    listTest = listTest->next;
    ASSERT_EQ(listTest->stdnt, studiTest2);
    ASSERT_EQ(listTest->next->stdnt, studiTest3);
    ASSERT_EQ(listTest->prev->stdnt, studiTest1);

    listTest = listTest->next;
    ASSERT_EQ(listTest->stdnt, studiTest3);
    ASSERT_EQ(listTest->next->stdnt, studiTest4);
    ASSERT_EQ(listTest->prev->stdnt, studiTest2);

    listTest = listTest->next;
    ASSERT_EQ(listTest->stdnt, studiTest4);
    ASSERT_EQ(listTest->next->stdnt, studiTest5);
    ASSERT_EQ(listTest->prev->stdnt, studiTest3);

    listTest = listTest->next;
    ASSERT_EQ(listTest->stdnt, studiTest5);
    ASSERT_EQ(NULL, listTest->next);
    ASSERT_EQ(listTest->prev->stdnt, studiTest4);

    free(studiTest);
    free(studiTest1);
    free(studiTest2);
    free(studiTest3);
    free(studiTest4);
    free(studiTest5);
    free(listTest->prev->prev->prev->prev->prev);
    free(listTest->prev->prev->prev->prev);
    free(listTest->prev->prev->prev);
    free(listTest->prev->prev);
    free(listTest->prev);
    free(listTest);
}

/*
 * testet ob die richtige Anzahl an Knoten geloescht wird
 *      - wobei nicht der Startknoten der Liste uebergeben wird.
 *
 * @param void
 * @return void
 */
TEST(Studiverwaltung, test_delete_list_partial_glo) {
    PRINT_INFO(
        "delete_list_partial: Wird die korrekte Anzahl an Nodes geloescht, wenn nicht der Listenanfang uebergeben wurde?\n");

node *listTest = NULL;
    node *helpTest = NULL;
    student *studiTest;
    student *studiTest1;
    student *studiTest2;
    student *studiTest3;
    student *studiTest4;
    student *studiTest5;

    studiTest = (student*) malloc(sizeof(student));
    CHECK_MALLOC(studiTest);strcpy(studiTest->name, "Horst");
    studiTest->cps = 42;
    studiTest->enrolled = IFM;

    studiTest1 = (student*) malloc(sizeof(student));
    CHECK_MALLOC(studiTest1);strcpy(studiTest1->name, "Horst");
    studiTest1->cps = 42;
    studiTest1->enrolled = ELM;

    studiTest2 = (student*) malloc(sizeof(student));
    CHECK_MALLOC(studiTest2);strcpy(studiTest2->name, "Horst");
    studiTest2->cps = 42;
    studiTest2->enrolled = PFLEGE;

    studiTest3 = (student*) malloc(sizeof(student));
    CHECK_MALLOC(
        studiTest3);strcpy(studiTest3->name, "Horst");
    studiTest3->cps = 42;
    studiTest3->enrolled = IFM;

    studiTest4 = (student*) malloc(sizeof(student));
    CHECK_MALLOC(studiTest4);strcpy(studiTest4->name, "Horst");
    studiTest4->cps = 42;
    studiTest4->enrolled = PFLEGE;

    studiTest5 = (student*) malloc(sizeof(student));
    CHECK_MALLOC(studiTest5);
strcpy(studiTest5->name, "Horst");
    studiTest5->cps = 42;
    studiTest5->enrolled = IFM;

    listTest = append_student(NULL, studiTest);
    listTest = append_student(listTest, studiTest1);
    listTest = append_student(listTest, studiTest2);
    listTest = append_student(listTest, studiTest3);
    listTest = append_student(listTest, studiTest4);
    listTest = append_student(listTest, studiTest5);

    helpTest = get_ifm_students(listTest);

    int erg = delete_list_partial(helpTest->next->next);

    ASSERT_TRUE(erg == 3);
    ASSERT_EQ(listTest->stdnt, studiTest);
    ASSERT_EQ(listTest->next->stdnt, studiTest1);
    ASSERT_EQ(NULL, listTest->prev);

    listTest = listTest->next;
    ASSERT_EQ(listTest->stdnt, studiTest1);
    ASSERT_EQ(listTest->next->stdnt, studiTest2);
    ASSERT_EQ(listTest->prev->stdnt, studiTest);

    listTest = listTest->next;
    ASSERT_EQ(listTest->stdnt, studiTest2);
    ASSERT_EQ(listTest->next->stdnt, studiTest3);
    ASSERT_EQ(listTest->prev->stdnt, studiTest1);

    listTest = listTest->next;
    ASSERT_EQ(listTest->stdnt, studiTest3);
    ASSERT_EQ(listTest->next->stdnt, studiTest4);
    ASSERT_EQ(listTest->prev->stdnt, studiTest2);

    listTest = listTest->next;
    ASSERT_EQ(listTest->stdnt, studiTest4);
    ASSERT_EQ(listTest->next->stdnt, studiTest5);
    ASSERT_EQ(listTest->prev->stdnt, studiTest3);

    listTest = listTest->next;
    ASSERT_EQ(listTest->stdnt, studiTest5);
    ASSERT_EQ(NULL, listTest->next);
    ASSERT_EQ(listTest->prev->stdnt, studiTest4);

    free(studiTest);
    free(studiTest1);
    free(studiTest2);
    free(studiTest3);
    free(studiTest4);
    free(studiTest5);
    free(listTest->prev->prev->prev->prev->prev);
    free(listTest->prev->prev->prev->prev);
    free(listTest->prev->prev->prev);
    free(listTest->prev->prev);
    free(listTest->prev);
    free(listTest);
}


/* ============================ABGABE======================================= */
#ifdef ABGABE

/*
 * Dummy Test, um in der Abgabe an die manuelle Pruefung des Types
 * von cps im student Struct zu errinnern.
 * @param void
 * @return void
 */
TEST(Studiverwaltung, check_datatype_studentcredits_MANUALLY) {
PRINT_INFO("Bitte korrekten Datentyp von struct student.credits manuell pruefen!\n");

EXPECT_FALSE(true);
}

/*
 * Ruft show_all mit NULL auf
 *
 * @param void
 * @return void
 */
TEST(Studiverwaltung, test_show_all_with_null) {
PRINT_INFO("show_all: Wie ist die Ausgabe wenn NULL uebergeben wird?\n");

printf("\n");
show_all(NULL);

SUCCEED();
}

/*
 * Ruft show_all mit einer Node aber keinem student auf
 *
 * @param void
 * @return void
 */
TEST(Studiverwaltung, test_show_all_with_no_studi) {
PRINT_INFO("show_all: Wie ist die Ausgabe wenn Node keinen studi hat?\n");

node listTest;

listTest.prev = NULL;
listTest.next = NULL;
listTest.stdnt = NULL;

printf("\n");
show_all(&listTest);

ASSERT_TRUE(&listTest);
ASSERT_EQ(NULL, listTest.prev);
ASSERT_EQ(NULL, listTest.next);
ASSERT_EQ(NULL, listTest.stdnt);
}

/*
 * Ruft show_all mit students auf
 *
 * @param void
 * @return void
 */
TEST(Studiverwaltung, test_show_all) {
PRINT_INFO("show_all: Wie ist die Ausgabe mit einer Liste mit Studis?\n");

node *listTest = NULL;
student studiTest;
student studiTest1;

strcpy(studiTest.name, "Horst");
studiTest.cps = 42;
studiTest.enrolled = IFM;

strcpy(studiTest1.name, "Horst");
studiTest1.cps = 42;
studiTest1.enrolled = ELM;

listTest = append_student(NULL, &studiTest);
listTest = append_student(listTest, &studiTest1);

ASSERT_TRUE(&listTest);
ASSERT_EQ(listTest->stdnt, &studiTest);
ASSERT_EQ(listTest->next->stdnt, &studiTest1);
ASSERT_EQ(NULL, listTest->prev);
ASSERT_EQ(NULL, listTest->next->next);

printf("\n");
show_all(listTest);

ASSERT_TRUE(&listTest);
ASSERT_EQ(listTest->stdnt, &studiTest);
ASSERT_EQ(listTest->next->stdnt, &studiTest1);
ASSERT_EQ(NULL, listTest->prev);
ASSERT_EQ(NULL, listTest->next->next);

free(listTest->next);
free(listTest);
}

/*
 * Ruft show_all mit students auf
 *      - uebergibt aber nicht den Listenanfang
 *
 * @param void
 * @return void
 */
TEST(Studiverwaltung, test_show_all_glo) {
PRINT_INFO("show_all: Wie ist die Ausgabe mit einer Liste mit Studis, wenn nicht der Listenanfang uebergeben wurde?\n");

node *listTest = NULL;
student studiTest;
student studiTest1;
student studiTest2;
student studiTest3;

strcpy(studiTest.name, "Horst");
studiTest.cps = 42;
studiTest.enrolled = IFM;

strcpy(studiTest1.name, "Horst");
studiTest1.cps = 42;
studiTest1.enrolled = ELM;

strcpy(studiTest2.name, "Peter");
studiTest2.cps = 42;
studiTest2.enrolled = ELM;

strcpy(studiTest3.name, "Michael");
studiTest3.cps = 42;
studiTest3.enrolled = IFM;

listTest = append_student(NULL, &studiTest);
listTest = append_student(listTest, &studiTest1);
listTest = append_student(listTest, &studiTest2);
listTest = append_student(listTest, &studiTest3);

ASSERT_TRUE(&listTest);
ASSERT_EQ(listTest->stdnt, &studiTest);
ASSERT_EQ(listTest->next->stdnt, &studiTest1);
ASSERT_EQ(listTest->next->next->stdnt, &studiTest2);
ASSERT_EQ(listTest->next->next->next->stdnt, &studiTest3);
ASSERT_EQ(NULL, listTest->prev);
ASSERT_EQ(NULL, listTest->next->next->next->next);

printf("\n");
show_all(listTest->next->next);

ASSERT_TRUE(&listTest);
ASSERT_EQ(listTest->stdnt, &studiTest);
ASSERT_EQ(listTest->next->stdnt, &studiTest1);
ASSERT_EQ(listTest->next->next->stdnt, &studiTest2);
ASSERT_EQ(listTest->next->next->next->stdnt, &studiTest3);
ASSERT_EQ(NULL, listTest->prev);
ASSERT_EQ(NULL, listTest->next->next->next->next);

free(listTest->next->next->next);
free(listTest->next->next);
free(listTest->next);
free(listTest);
}

TEST(Studiverwaltung, test_various_use_cases) {
PRINT_INFO("Aufruf der Funktionen zum Aufbau einer Personalverwaltung.\n");

node *start = NULL;
node *helpStart = NULL;
int helpPartial = 0;
int helpDelete = 0;

student *studi1 = (student*) malloc(sizeof(student));
student *studi2 = (student*) malloc(sizeof(student));
student *studi3 = (student*) malloc(sizeof(student));
if (!studi1 || !studi2 || !studi3) {
    printf("Es gibt keinen Speicher fuer noch einen Studi!\n");
    exit(-1);
}

strcpy(studi1->name, "Horst");
studi1->cps = 42;
studi1->enrolled = IFM;

strcpy(studi2->name, "Klaus");
studi2->cps = 18;
studi2->enrolled = ELM;

strcpy(studi3->name, "Peter");
studi3->cps = 10;
studi3->enrolled = IFM;

start = append_student(NULL, studi1);
start = append_student(start, studi2);
start = append_student(start, studi3);

/* ***************************************************** */
/* Tests von append_student() */

ASSERT_TRUE(start);
ASSERT_EQ(start->stdnt, studi1);
ASSERT_EQ(start->next->stdnt, studi2);
ASSERT_EQ(start->next->next->stdnt, studi3);
ASSERT_EQ(NULL, start->prev);

start = start->next;
ASSERT_EQ(start->stdnt, studi2);
ASSERT_EQ(start->next->stdnt, studi3);
ASSERT_EQ(start->prev->stdnt, studi1);

start = start->next;
ASSERT_EQ(start->stdnt, studi3);
ASSERT_EQ(NULL, start->next);
ASSERT_EQ(start->prev->stdnt, studi2);
start = start->prev->prev;

/* Tests von append_student() */
/* ***************************************************** */

printf("Hauptliste:\n");
show_all(start);

helpStart = get_ifm_students(start);

/* ***************************************************** */
/* Tests von get_ifm_students() */

ASSERT_TRUE(helpStart);
ASSERT_EQ(helpStart->stdnt, studi1);
ASSERT_EQ(helpStart->next->stdnt, studi3);
ASSERT_EQ(NULL, helpStart->prev);

helpStart = helpStart->next;
ASSERT_EQ(helpStart->stdnt, studi3);
ASSERT_EQ(NULL, helpStart->next);
ASSERT_EQ(helpStart->prev->stdnt, studi1);
helpStart = helpStart->prev;

ASSERT_EQ(start->stdnt, studi1);
ASSERT_EQ(start->next->stdnt, studi2);
ASSERT_EQ(NULL, start->prev);

start = start->next;
ASSERT_EQ(start->stdnt, studi2);
ASSERT_EQ(start->next->stdnt, studi3);
ASSERT_EQ(start->prev->stdnt, studi1);

start = start->next;
ASSERT_EQ(start->stdnt, studi3);
ASSERT_EQ(NULL, start->next);
ASSERT_EQ(start->prev->stdnt, studi2);
start = start->prev->prev;

/* Tests von get_ifm_students() */
/* ***************************************************** */

printf("IFM Studenten von der Hauptliste:\n");
show_all(helpStart);
printf("IFM Liste loeschen (partial)...\n");

helpPartial = delete_list_partial(helpStart);

/* ***************************************************** */
/* Tests von delete_list_partial() */

ASSERT_TRUE(helpPartial == 2);

ASSERT_EQ(start->stdnt, studi1);
ASSERT_EQ(start->next->stdnt, studi2);
ASSERT_EQ(NULL, start->prev);

start = start->next;
ASSERT_EQ(start->stdnt, studi2);
ASSERT_EQ(start->next->stdnt, studi3);
ASSERT_EQ(start->prev->stdnt, studi1);

start = start->next;
ASSERT_EQ(start->stdnt, studi3);
ASSERT_EQ(NULL, start->next);
ASSERT_EQ(start->prev->stdnt, studi2);
start = start->prev->prev;

/* Tests von delete_list_partial() */
/* ***************************************************** */

printf("Anzahl geloeschter Listenelemente: %d\n", helpPartial);
helpStart = NULL;
show_all(helpStart);

printf("Hauptliste loeschen...\n");

helpDelete = delete_list(start);

/* ***************************************************** */
/* Tests von delete_list() */

ASSERT_TRUE(helpDelete == 3);

/* Tests von delete_list() */
/* ***************************************************** */

printf("Anzahl geloeschter Listenelemente: %d\n", helpDelete);
start = NULL;
show_all(start);
}

#endif  /* ABGABE */
/* ========================================================================= */

int main(int argc, char *argv[]) {
testing::InitGoogleTest(&argc, argv);
return RUN_ALL_TESTS();
}
