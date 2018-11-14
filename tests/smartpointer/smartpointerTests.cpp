#include <iostream>
#include <vector>
#include <string>
#include <gtest/gtest.h>

#include "smartpointer/SmartPointer.h"
#include "smartpointer/RefCounter.h"
#include "smartpointer/NullPointerException.h"

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

static std::vector<void*> saveOldAdress(0);

/*
 * Hilfsklassen
 */
class A {
public:
    A() {
    }
    virtual int getNum() {
        return 42;
    }
    virtual ~A() {
        saveOldAdress.push_back(this);
    }
};

class B: public A {
public:
    B() {
    }
    virtual int getNum() {
        return 24;
    }
    virtual ~B() {
    }
};

template<typename T>
int func(const SmartPointer<T> &p) {
    return p->getNum();
}

// ==========================================================================

/*
 * Test ob eine Klasse NullPointerException existiert, die eine echte Exception
 * ist (d.h. letztlich von `exception` erbt).
 */
TEST(Smartpointer, testIfNullPointerExists) {
    PRINT_INFO("Existiert eine Klasse NullPointerException, die eine echte Exception ist (d.h. letztlich von `exception` erbt)?\n");

    NullPointerException np;
    std::string message { np.what() };

    ASSERT_TRUE(message.length() > 0);
}

// ==========================================================================

/*
 * Test ob eine Klasse RefCounter existiert.
 */
TEST(Smartpointer, testIfRefCounterExists) {
    PRINT_INFO("Existiert eine Klasse RefCounter?\n");

    RefCounter rc;

    SUCCEED();
}

/*
 * Ist die getRefCount() Methode implementiert?
 */
TEST(Smartpointer, testGetRefCount) {
    PRINT_INFO("Ist die getRefCount() Methode implementiert?\n");

    RefCounter rc;

    ASSERT_TRUE(rc.getRefCount() == 0);
}

/*
 * Funktioniert die Methode `RefCounter::inc()`?
 */
TEST(Smartpointer, testRefCounterInc) {
    PRINT_INFO("Funktioniert die Methode `RefCounter::inc()`?\n");

    RefCounter rc;

    rc.inc();
    ASSERT_TRUE(rc.getRefCount() == 1);

    rc.inc();
    ASSERT_TRUE(rc.getRefCount() == 2);

    rc.inc();
    ASSERT_TRUE(rc.getRefCount() == 3);
}

/*
 * Funktioniert die Methode `RefCounter::dec()`?
 */
TEST(Smartpointer, testRefCounterDec) {
    PRINT_INFO("Funktioniert die Methode `RefCounter::dec()`?\n");

    RefCounter rc;

    rc.inc();
    rc.inc();
    rc.inc();

    rc.dec();
    ASSERT_TRUE(rc.getRefCount() == 2);

    rc.dec();
    ASSERT_TRUE(rc.getRefCount() == 1);

    rc.dec();
    ASSERT_TRUE(rc.getRefCount() == 0);
}

/*
 * Funktioniert die Methode `RefCounter::isZero()`?
 */
TEST(Smartpointer, testRefCounterZero) {
    PRINT_INFO("Funktionieren die Methoden `RefCounter::dec()` und `RefCounter::inc()` im Zusammenspiel?\n");

    RefCounter rc;

    ASSERT_TRUE(rc.isZero());

    rc.inc();
    ASSERT_TRUE(!rc.isZero());

    rc.dec();
    ASSERT_TRUE(rc.isZero());
}

// ==========================================================================

/*
 * Test ob eine Klasse SmartPointer existiert, diese als Template angelegt
 * wurde und deren Konstruktor implementiert ist.
 */
TEST(Smartpointer, testIfClassExists) {
    PRINT_INFO("Existiert eine Klasse SmartPointer und ist diese als Template angelegt mit einem implementierten Konstruktor?\n");

    SmartPointer<A> smart_A { new A };

    SUCCEED();
}

/*
 * Test ob der Konstruktor richtig implementiert wurde.
 * Parameterloser Konstruktor
 */
TEST(Smartpointer, testConstructorParameterless) {
    PRINT_INFO("Kommt der Konstruktor auch mit ohne Parameter klar?\n");

    SmartPointer<A> smart_A1;

    ASSERT_TRUE(smart_A1.getObject() == nullptr);

    ASSERT_TRUE(smart_A1.getRefCounter() == nullptr);
}

/*
 * Test ob der Konstruktor richtig implementiert wurde.
 * Konstruktor mit nullptr
 */
TEST(Smartpointer, testConstructorNullPtr) {
    PRINT_INFO("Kommt der Konstruktor auch mit einem nullptr als Parameter klar?\n");

    SmartPointer<A> smart_A1 { nullptr };

    ASSERT_TRUE(smart_A1.getObject() == nullptr);

    ASSERT_TRUE(smart_A1.getRefCounter() == nullptr);
}

/*
 * Test ob der Konstruktor richtig implementiert wurde.
 * Konstruktor mit 0
 */
TEST(Smartpointer, testConstructorNullPtr2) {
    PRINT_INFO("Kommt der Konstruktor auch mit einer 0 als Parameter klar?\n");

    SmartPointer<A> smart_A1 { 0 };

    ASSERT_TRUE(smart_A1.getObject() == nullptr);

    ASSERT_TRUE(smart_A1.getRefCounter() == nullptr);
}

/*
 * Test ob der Copykonstruktor implementiert wurde.
 * Konstruktor mit "echtem" Objekt-Pointer
 */
TEST(Smartpointer, testCopyConstructorPtr) {
    PRINT_INFO("Ist Copykonstruktor implementiert und wird RefCounter Objekt richtig angelegt?\n");

    A *a = new A; // kein delete noetig -- SmartPointer should do the trick :)
    SmartPointer<A> smart_A1 { a };

    ASSERT_TRUE(smart_A1.getObject() == a);

    ASSERT_TRUE(smart_A1.getRefCounter() != nullptr);

    ASSERT_TRUE(smart_A1.getRefCounter()->getRefCount() == 1);
}

/*
 * Test ob der Copykonstruktor implementiert wurde.
 * Copy-Konstruktor mit Referenz auf "echten" Smartpointer (mit Objekt-Pointer)
 */
TEST(Smartpointer, testCopyConstructorRef) {
    PRINT_INFO("Werden zwei Smartpointer richtig miteinander referenziert?\n");

    A *a = new A; // kein delete noetig -- SmartPointer should do the trick :)
    SmartPointer<A> smart_A1 { a };
    SmartPointer<A> smart_A2 = smart_A1;

    ASSERT_TRUE(smart_A1.getObject() == a);
    ASSERT_TRUE(smart_A2.getObject() == a);

    ASSERT_TRUE(smart_A1.getRefCounter() != nullptr);
    ASSERT_TRUE(smart_A1.getRefCounter() == smart_A2.getRefCounter());

    ASSERT_TRUE(smart_A1.getRefCounter()->getRefCount() == 2);
}

/*
 * Test ob der Copykonstruktor implementiert wurde.
 * Copy-Konstruktor mit Referenz auf leeren Smartpointer
 */
TEST(Smartpointer, testCopyConstructorRef2) {
    PRINT_INFO("Testet Copy-Konstruktor mit Referenz auf leeren Smartpointer\n");

    SmartPointer<A> smart_A1;
    SmartPointer<A> smart_A2 = smart_A1;

    ASSERT_TRUE(smart_A1.getObject() == nullptr);
    ASSERT_TRUE(smart_A2.getObject() == nullptr);

    ASSERT_TRUE(smart_A1.getRefCounter() == nullptr);
    ASSERT_TRUE(smart_A2.getRefCounter() == nullptr);
}

/*
 * Verhaelt sich der Destruktor vom SP korrekt?
 * Leerer Smartpointer
 */
TEST(Smartpointer, testCallDestructorEmpty) {
    PRINT_INFO("Verhaelt sich der Destruktor vom SP korrekt?\n");

    saveOldAdress.clear();

    {
        SmartPointer<A> p1;
    }

    ASSERT_TRUE(saveOldAdress.size() == 0);
}

/*
 * Wird der Destruktor aufgerufen?
 * RefCount == 0
 */
TEST(Smartpointer, testCallDestructorCountOne) {
    PRINT_INFO("Wird der Destruktor aufgerufen mit RefCount = 0?\n");

    saveOldAdress.clear();
    A *a = new A;
    A *b = new A;
    //void *temp = &(*a);
    SmartPointer<A> p1 { a };

    p1 = b;

    ASSERT_TRUE(p1.getObject() == b);
    ASSERT_TRUE(p1.getRefCounter() != nullptr);
    ASSERT_TRUE(p1.getRefCounter()->getRefCount() == 1);

    ASSERT_TRUE(saveOldAdress.back());
    ASSERT_TRUE((int )saveOldAdress.size());
}

/*
 * Der Destruktor wurde aufgerufen?!
 * RefCount > 0
 */
TEST(Smartpointer, testCallDestructorCountThree) {
    PRINT_INFO("Wird der Destruktor aufgerufen mit RefCount > 0?\n");

    saveOldAdress.clear();
    A *a = new A;
    A *b = new A;
    SmartPointer<A> p1 { a };
    SmartPointer<A> p2(p1);
    SmartPointer<A> p3(p2);

    p1 = b;

    ASSERT_TRUE(p1.getObject() == b);
    ASSERT_TRUE(p1.getRefCounter() != nullptr);
    ASSERT_TRUE(p1.getRefCounter()->getRefCount() == 1);

    ASSERT_TRUE(p2.getObject() == a);
    ASSERT_TRUE(p2.getRefCounter() != nullptr);
    ASSERT_TRUE(p2.getRefCounter()->getRefCount() == 2);

    ASSERT_TRUE(p3.getObject() == a);
    ASSERT_TRUE(p3.getRefCounter() != nullptr);
    ASSERT_TRUE(p3.getRefCounter()->getRefCount() == 2);

    ASSERT_TRUE(p1.getRefCounter() != p2.getRefCounter());
    ASSERT_TRUE(p2.getRefCounter() == p3.getRefCounter());

    ASSERT_EQ(0, (int)saveOldAdress.size());
}

/*
 * Test ob der -> Operator ueberladen wurde.
 * Dereferenzierung eines leeren Smartpointers (NullPointerException)
 */
TEST(Smartpointer, testArrowOperatorNull) {
    PRINT_INFO("Wird eine NullPointerException beim Dereferenzieren mit '->' geworfen, wenn ein SmartPointer ein Nullpointer ist?\n");

    SmartPointer<A> smart_A;

    ASSERT_THROW(smart_A->getNum(), NullPointerException);
}

/*
 * Test ob der -> Operator ueberladen wurde.
 * Dereferenzierung eines nicht-leeren Smartpointers
 */
TEST(Smartpointer, testArrowOperator) {
    PRINT_INFO("Ist der '->' Operator richtig implementiert?\n");

    SmartPointer<A> smart_A { new A };

    ASSERT_EQ(42, smart_A->getNum());
}

/*
 * Test ob der Stern-Operator ueberladen wurde.
 * Dereferenzierung eines leeren Smartpointers (NullPointerException)
 */
TEST(Smartpointer, testDereferenceOperatorNull) {
    PRINT_INFO("Wird eine NullPointerException beim Dereferenzieren geworfen, wenn ein SmartPointer ein Nullpointer ist?\n");

    SmartPointer<B> smart_B;

    ASSERT_THROW(*smart_B, NullPointerException);
}

/*
 * Test ob der Stern-Operator ueberladen wurde.
 * Dereferenzierung eines nicht-leeren Smartpointers
 */
TEST(Smartpointer, testDereferenceOperator) {
    PRINT_INFO("Ist der * Operator richtig implementiert?\n");

    SmartPointer<B> smart_B { new B };

    ASSERT_EQ(24, (*smart_B).getNum());
}

/*
 * Test ob der SmartPointer<T>::getObject() implementiert wurde.
 * leerer Smartpointer
 */
TEST(Smartpointer, testGetObjectNull) {
    PRINT_INFO("Ist SmartPointer<T>::getObject() implementiert? Test mit leerem SMartpointer\n");

    SmartPointer<A> smart_A;

    ASSERT_TRUE(smart_A.getObject() == nullptr);
}

/*
 * Test ob der SmartPointer<T>::getObject() implementiert wurde.
 * Smartpointer mit Objekt
 */
TEST(Smartpointer, testGetObject) {
    PRINT_INFO("Ist SmartPointer<T>::getObject() richtig implementiert?\n");

    A *a = new A;
    SmartPointer<A> smart_A { a };

    ASSERT_TRUE(smart_A.getObject() == a);
}

/*
 * Test ob der SmartPointer<T>::getRefCounter() implementiert wurde.
 * Leerer Smartpointer
 */
TEST(Smartpointer, testGetRefCounterNull) {
    PRINT_INFO("Ist SmartPointer<T>::getRefCounter() implementiert? Test mit leerem Smartpointer\n");

    SmartPointer<A> smart_A;

    ASSERT_TRUE(smart_A.getRefCounter() == nullptr);
}

/*
 * Test ob der SmartPointer<T>::getRefCounter() implementiert wurde.
 * Smartpointer mit Objekt
 */
TEST(Smartpointer, testGetRefCounter) {
    PRINT_INFO("Ist SmartPointer<T>::getRefCounter() richtig implementiert?\n");

    A *a = new A;
    SmartPointer<A> smart_A { a };

    ASSERT_TRUE(smart_A.getRefCounter() != nullptr);
    ASSERT_TRUE(smart_A.getRefCounter()->getRefCount() == 1);
}

/*
 * Test ob der Zuweisungsoperator fuer Pointer korrekt ueberladen wurde.
 * Smartpointer zeigt auf nullptr und nullptr wird zugewiesen ("Selfassignment" mit Sonderwert)
 */
TEST(Smartpointer, testAssignmentPtrNullNullptr) {
    PRINT_INFO("Ueberpruefung des Zuweisungsoperators: Smartpointer zeigt auf nullptr und nullptr wird zugewiesen ('Selfassignment' mit Sonderwert)\n");

    saveOldAdress.clear();
    SmartPointer<A> smart_A;

    smart_A = nullptr;

    ASSERT_TRUE(smart_A.getObject() == nullptr);
    ASSERT_TRUE(smart_A.getRefCounter() == nullptr);

    ASSERT_EQ(0, (int )saveOldAdress.size());
}

/*
 * Test ob der Zuweisungsoperator fuer Pointer korrekt ueberladen wurde.
 * Smartpointer zeigt auf Objekt und nullptr wird zugewiesen (normales Assignment mit Sonderwert)
 */
TEST(Smartpointer, testAssignmentPtrNullptr) {
    PRINT_INFO("Ueberpruefung des Zuweisungsoperators: Smartpointer zeigt auf Objekt und nullptr wird zugewiesen (normales Assignment mit Sonderwert)\n");

    saveOldAdress.clear();
    A *a = new A;
    void *temp = &(*a);
    SmartPointer<A> smart_A { a };

    smart_A = nullptr;

    ASSERT_TRUE(smart_A.getObject() == nullptr);
    ASSERT_TRUE(smart_A.getRefCounter() == nullptr);
    ASSERT_EQ(1, (int )saveOldAdress.size());

    ASSERT_EQ(temp, saveOldAdress.back());
}

/*
 * Test ob der Zuweisungsoperator fuer Pointer korrekt ueberladen wurde.
 * Normales Assignment: Null-Smartpointer, neues Objekt wird zugewiesen
 */
TEST(Smartpointer, testAssignmentPtrNull) {
    PRINT_INFO("Ueberpruefung des Zuweisungsoperators: Normales Assignment: Null-Smartpointer, neues Objekt wird zugewiesen\n");

    saveOldAdress.clear();
    A *a = new A;
    SmartPointer<A> smart_A;

    smart_A = a;

    ASSERT_TRUE(smart_A.getObject() == a);
    ASSERT_TRUE(smart_A.getRefCounter() != nullptr);
    ASSERT_TRUE(smart_A.getRefCounter()->getRefCount() == 1);

    ASSERT_EQ(0, (int )saveOldAdress.size());
}

/*
 * Test ob der Zuweisungsoperator fuer Pointer korrekt ueberladen wurde.
 * Normales Assignment: Smartpointer mit Objekt, neues Objekt wird zugewiesen
 */
TEST(Smartpointer, testAssignmentPtr) {
    PRINT_INFO("Ueberpruefung des Zuweisungsoperators: Normales Assignment: Smartpointer mit Objekt, neues Objekt wird zugewiesen\n");

    saveOldAdress.clear();
    A *a = new A;
    A *b = new A;
    void *temp = &(*a);
    SmartPointer<A> smart_A { a };

    smart_A = b;

    ASSERT_TRUE(smart_A.getObject() == b);
    ASSERT_TRUE(smart_A.getRefCounter() != nullptr);
    ASSERT_TRUE(smart_A.getRefCounter()->getRefCount() == 1);

    ASSERT_EQ(1, (int )saveOldAdress.size());
    ASSERT_EQ(temp, saveOldAdress.back());
}

/*
 * Test ob der Zuweisungsoperator fuer Pointer korrekt ueberladen wurde.
 * Self-Assignment mit Pointern
 */
TEST(Smartpointer, testAssignmentPtrSelfassignment) {
    PRINT_INFO("Ueberpruefung des Zuweisungsoperators: Self-Assignment mit Pointern\n");

    saveOldAdress.clear();
    A *a = new A;
    SmartPointer<A> smart_A { a };

    smart_A = a;

    ASSERT_TRUE(smart_A.getObject() == a);
    ASSERT_TRUE(smart_A.getRefCounter() != nullptr);
    ASSERT_TRUE(smart_A.getRefCounter()->getRefCount() == 1);

    ASSERT_EQ(0, (int )saveOldAdress.size());
}

/*
 * Test ob der Zuweisungsoperator fuer Pointer korrekt ueberladen wurde.
 * Normales Assignment: Smartpointer mit Objekt, neues Objekt wird zugewiesen (komplexeres Szenario)
 */
TEST(Smartpointer, testAssignmentPtrComplex) {
    PRINT_INFO("Ueberpruefung des Zuweisungsoperators: Normales Assignment: Smartpointer mit Objekt, neues Objekt wird zugewiesen (komplexeres Szenario)\n");

    saveOldAdress.clear();
    A *a = new A;
    A *b = new A;
    SmartPointer<A> p1 { a };
    SmartPointer<A> p2(p1);
    SmartPointer<A> p3(p2);

    p1 = b;

    ASSERT_TRUE(p1.getObject() == b);
    ASSERT_TRUE(p1.getRefCounter() != nullptr);
    ASSERT_TRUE(p1.getRefCounter()->getRefCount() == 1);

    ASSERT_TRUE(p2.getObject() == a);
    ASSERT_TRUE(p2.getRefCounter() != nullptr);
    ASSERT_TRUE(p2.getRefCounter()->getRefCount() == 2);

    ASSERT_TRUE(p3.getObject() == a);
    ASSERT_TRUE(p3.getRefCounter() != nullptr);
    ASSERT_TRUE(p3.getRefCounter()->getRefCount() == 2);

    ASSERT_TRUE(p1.getRefCounter() != p2.getRefCounter());
    ASSERT_TRUE(p2.getRefCounter() == p3.getRefCounter());

    ASSERT_EQ(0, (int )saveOldAdress.size());
}

/*
 * Test ob der Zuweisungsoperator fuer Pointer korrekt ueberladen wurde.
 * Self-Assignment mit Pointern (komplexeres Szenario)
 */
TEST(Smartpointer, testAssignmentPtrSelfassignmentComplex) {
    PRINT_INFO("Ueberpruefung des Zuweisungsoperators: Self-Assignment mit Pointern (komplexeres Szenario)\n");

    saveOldAdress.clear();
    A *a = new A;
    SmartPointer<A> p1 { a };
    SmartPointer<A> p2(p1);
    SmartPointer<A> p3(p2);

    p1 = a;

    ASSERT_TRUE(p1.getObject() == a);
    ASSERT_TRUE(p1.getRefCounter() != nullptr);
    ASSERT_TRUE(p1.getRefCounter()->getRefCount() == 3);

    ASSERT_TRUE(p2.getObject() == a);
    ASSERT_TRUE(p2.getRefCounter() != nullptr);
    ASSERT_TRUE(p2.getRefCounter()->getRefCount() == 3);

    ASSERT_TRUE(p3.getObject() == a);
    ASSERT_TRUE(p3.getRefCounter() != nullptr);
    ASSERT_TRUE(p3.getRefCounter()->getRefCount() == 3);

    ASSERT_TRUE(p1.getRefCounter() == p2.getRefCounter());
    ASSERT_TRUE(p2.getRefCounter() == p3.getRefCounter());

    ASSERT_EQ(0, (int )saveOldAdress.size());
}

/*
 * Test ob der Zuweisungsoperator fuer Referenzen korrekt ueberladen wurde.
 * Smartpointer zeigt auf nullptr und nullptr-Smartpointer wird zugewiesen ("Selfassignment" mit Sonderwert)
 */
TEST(Smartpointer, testAssignmentRefNullNullptr) {
    PRINT_INFO("Ueberpruefung des Zuweisungsoperators: Smartpointer zeigt auf nullptr und nullptr-Smartpointer wird zugewiesen ('Selfassignment' mit Sonderwert)\n");

    saveOldAdress.clear();
    SmartPointer<A> smart_A;

    smart_A = smart_A;

    ASSERT_TRUE(smart_A.getObject() == nullptr);
    ASSERT_TRUE(smart_A.getRefCounter() == nullptr);

    ASSERT_EQ(0, (int )saveOldAdress.size());
}

/*
 * Test ob der Zuweisungsoperator fuer Referenzen korrekt ueberladen wurde.
 * Smartpointer zeigt auf nullptr und nullptr-Smartpointer wird zugewiesen ("Selfassignment" mit Sonderwert)
 */
TEST(Smartpointer, testAssignmentRefNullNullptr2) {
    PRINT_INFO("Ueberpruefung des Zuweisungsoperators: Smartpointer zeigt auf nullptr und nullptr-Smartpointer wird zugewiesen ('Selfassignment' mit Sonderwert)\n");

    saveOldAdress.clear();
    SmartPointer<A> smart_A, smart_B;

    smart_A = smart_B;

    ASSERT_TRUE(smart_A.getObject() == nullptr);
    ASSERT_TRUE(smart_A.getRefCounter() == nullptr);

    ASSERT_TRUE(smart_B.getObject() == nullptr);
    ASSERT_TRUE(smart_B.getRefCounter() == nullptr);

    ASSERT_EQ(0, (int )saveOldAdress.size());
}

/*
 * Test ob der Zuweisungsoperator fuer Referenzen korrekt ueberladen wurde.
 * Smartpointer zeigt auf Objekt und nullptr-Smartpointer wird zugewiesen (normales Assignment mit Sonderwert)
 */
TEST(Smartpointer, testAssignmentRefNullptr) {
    PRINT_INFO("Ueberpruefung des Zuweisungsoperators: Smartpointer zeigt auf Objekt und nullptr-Smartpointer wird zugewiesen (normales Assignment mit Sonderwert)\n");

    saveOldAdress.clear();
    A *a = new A;
    void *temp = &(*a);
    SmartPointer<A> smart_A { a }, smart_B;

    smart_A = smart_B;

    ASSERT_TRUE(smart_A.getObject() == nullptr);
    ASSERT_TRUE(smart_A.getRefCounter() == nullptr);

    ASSERT_TRUE(smart_B.getObject() == nullptr);
    ASSERT_TRUE(smart_B.getRefCounter() == nullptr);

    ASSERT_EQ(1, (int )saveOldAdress.size());
    ASSERT_EQ(temp, saveOldAdress.back());
}

/*
 * Test ob der Zuweisungsoperator fuer Referenzen korrekt ueberladen wurde.
 * Normales Assignment: Null-Smartpointer, neues Objekt wird zugewiesen
 */
TEST(Smartpointer, testAssignmentRefNull) {
    PRINT_INFO("Ueberpruefung des Zuweisungsoperators: Normales Assignment: Null-Smartpointer, neues Objekt wird zugewiesen\n");

    saveOldAdress.clear();
    A *a = new A;
    SmartPointer<A> smart_A, smart_B { a };

    smart_A = smart_B;

    ASSERT_TRUE(smart_A.getObject() == a);
    ASSERT_TRUE(smart_A.getRefCounter() != nullptr);
    ASSERT_TRUE(smart_A.getRefCounter()->getRefCount() == 2);

    ASSERT_TRUE(smart_B.getObject() == a);
    ASSERT_TRUE(smart_B.getRefCounter() != nullptr);
    ASSERT_TRUE(smart_B.getRefCounter()->getRefCount() == 2);

    ASSERT_TRUE(smart_A.getRefCounter() == smart_B.getRefCounter());

    ASSERT_EQ(0, (int )saveOldAdress.size());
}

/*
 * Test ob der Zuweisungsoperator fuer Referenzen korrekt ueberladen wurde.
 * Normales Assignment: Smartpointer mit Objekt, neues Objekt wird zugewiesen
 */
TEST(Smartpointer, testAssignmentRef) {
    PRINT_INFO("Ueberpruefung des Zuweisungsoperators: Normales Assignment: Smartpointer mit Objekt, neues Objekt wird zugewiesen\n");

    saveOldAdress.clear();
    A *a = new A;
    A *b = new A;
    void *temp = &(*a);
    SmartPointer<A> smart_A { a }, smart_B { b };

    smart_A = smart_B;

    ASSERT_TRUE(smart_A.getObject() == b);
    ASSERT_TRUE(smart_A.getRefCounter() != nullptr);
    ASSERT_TRUE(smart_A.getRefCounter()->getRefCount() == 2);

    ASSERT_TRUE(smart_B.getObject() == b);
    ASSERT_TRUE(smart_B.getRefCounter() != nullptr);
    ASSERT_TRUE(smart_B.getRefCounter()->getRefCount() == 2);

    ASSERT_TRUE(smart_A.getRefCounter() == smart_B.getRefCounter());

    ASSERT_EQ(1, (int )saveOldAdress.size());
    ASSERT_EQ(temp, saveOldAdress.back());
}

/*
 * Test ob der Zuweisungsoperator fuer Referenzen korrekt ueberladen wurde.
 * Self-Assignment mit Referenzen
 */
TEST(Smartpointer, testAssignmentRefSelfassignment) {
    PRINT_INFO("Ueberpruefung des Zuweisungsoperators: Self-Assignment mit Referenzen\n");

    saveOldAdress.clear();
    A *a = new A;
    SmartPointer<A> smart_A { a };

    smart_A = smart_A;

    ASSERT_TRUE(smart_A.getObject() == a);
    ASSERT_TRUE(smart_A.getRefCounter() != nullptr);
    ASSERT_TRUE(smart_A.getRefCounter()->getRefCount() == 1);

    ASSERT_EQ(0, (int )saveOldAdress.size());
}

/*
 * Test ob der Zuweisungsoperator fuer Referenzen korrekt ueberladen wurde.
 * Normales Assignment: Smartpointer mit Objekt, neues Objekt wird zugewiesen (komplexeres Szenario)
 */
TEST(Smartpointer, testAssignmentRefComplex) {
    PRINT_INFO("Ueberpruefung des Zuweisungsoperators: Normales Assignment: Smartpointer mit Objekt, neues Objekt wird zugewiesen (komplexeres Szenario)\n");

    saveOldAdress.clear();
    A *a = new A;
    A *b = new A;
    SmartPointer<A> p1 { a };
    SmartPointer<A> p2(p1);
    SmartPointer<A> p3(b);

    p1 = p3;

    ASSERT_TRUE(p1.getObject() == b);
    ASSERT_TRUE(p1.getRefCounter() != nullptr);
    ASSERT_TRUE(p1.getRefCounter()->getRefCount() == 2);

    ASSERT_TRUE(p2.getObject() == a);
    ASSERT_TRUE(p2.getRefCounter() != nullptr);
    ASSERT_TRUE(p2.getRefCounter()->getRefCount() == 1);

    ASSERT_TRUE(p3.getObject() == b);
    ASSERT_TRUE(p3.getRefCounter() != nullptr);
    ASSERT_TRUE(p3.getRefCounter()->getRefCount() == 2);

    ASSERT_TRUE(p1.getRefCounter() != p2.getRefCounter());
    ASSERT_TRUE(p1.getRefCounter() == p3.getRefCounter());

    ASSERT_EQ(0, (int )saveOldAdress.size());
}

/*
 * Test ob der Zuweisungsoperator fuer Referenzen korrekt ueberladen wurde.
 * Self-Assignment mit Referenzen (komplexeres Szenario)
 */
TEST(Smartpointer, testAssignmentRefSelfassignmentComplex) {
    PRINT_INFO("Ueberpruefung des Zuweisungsoperators: Self-Assignment mit Referenzen (komplexeres Szenario)\n");

    saveOldAdress.clear();
    A *a = new A;
    SmartPointer<A> p1 { a };
    SmartPointer<A> p2(p1);
    SmartPointer<A> p3(p2);

    p1 = p1;

    ASSERT_TRUE(p1.getObject() == a);
    ASSERT_TRUE(p1.getRefCounter() != nullptr);
    ASSERT_TRUE(p1.getRefCounter()->getRefCount() == 3);

    ASSERT_TRUE(p2.getObject() == a);
    ASSERT_TRUE(p2.getRefCounter() != nullptr);
    ASSERT_TRUE(p2.getRefCounter()->getRefCount() == 3);

    ASSERT_TRUE(p3.getObject() == a);
    ASSERT_TRUE(p3.getRefCounter() != nullptr);
    ASSERT_TRUE(p3.getRefCounter()->getRefCount() == 3);

    ASSERT_TRUE(p1.getRefCounter() == p2.getRefCounter());
    ASSERT_TRUE(p2.getRefCounter() == p3.getRefCounter());

    ASSERT_EQ(0, (int )saveOldAdress.size());
}

/*
 * Test ob der Zuweisungsoperator fuer Referenzen korrekt ueberladen wurde.
 * Self-Assignment mit Referenzen (komplexeres Szenario)
 */
TEST(Smartpointer, testAssignmentRefSelfassignmentComplex2) {
    PRINT_INFO("Ueberpruefung des Zuweisungsoperators: Self-Assignment mit Referenzen (komplexeres Szenario)\n");

    saveOldAdress.clear();
    A *a = new A;
    SmartPointer<A> p1 { a };
    SmartPointer<A> p2(p1);
    SmartPointer<A> p3(p2);

    p1 = p3;

    ASSERT_TRUE(p1.getObject() == a);
    ASSERT_TRUE(p1.getRefCounter() != nullptr);
    ASSERT_TRUE(p1.getRefCounter()->getRefCount() == 3);

    ASSERT_TRUE(p2.getObject() == a);
    ASSERT_TRUE(p2.getRefCounter() != nullptr);
    ASSERT_TRUE(p2.getRefCounter()->getRefCount() == 3);

    ASSERT_TRUE(p3.getObject() == a);
    ASSERT_TRUE(p3.getRefCounter() != nullptr);
    ASSERT_TRUE(p3.getRefCounter()->getRefCount() == 3);

    ASSERT_TRUE(p1.getRefCounter() == p2.getRefCounter());
    ASSERT_TRUE(p2.getRefCounter() == p3.getRefCounter());

    ASSERT_EQ(0, (int )saveOldAdress.size());
}

/*
 * Test ob der Vergleichssoperator korrekt ueberladen wurde.
 * 2x leere Smartpointer
 */
TEST(Smartpointer, testCompareNullNull) {
    PRINT_INFO("Ueberpruefung des Vergleichsoperators: 2x leere Smartpointer\n");

    saveOldAdress.clear();
    SmartPointer<A> smart_A, smart_B;

    ASSERT_TRUE(smart_A == smart_B);

    ASSERT_TRUE(smart_A.getObject() == nullptr);
    ASSERT_TRUE(smart_A.getRefCounter() == nullptr);

    ASSERT_TRUE(smart_B.getObject() == nullptr);
    ASSERT_TRUE(smart_B.getRefCounter() == nullptr);

    ASSERT_EQ(0, (int )saveOldAdress.size());
}

/*
 * Test ob der Vergleichssoperator korrekt ueberladen wurde.
 * 1x leerer Smartpointer, 1x Objekt
 */
TEST(Smartpointer, testCompareNullObject) {
    PRINT_INFO("Ueberpruefung des Vergleichsoperators: 1x leerer Smartpointer, 1x Objekt\n");

    saveOldAdress.clear();
    A *a = new A;
    SmartPointer<A> smart_A, smart_B { a };

    ASSERT_TRUE(!(smart_A == smart_B));

    ASSERT_TRUE(smart_A.getObject() == nullptr);
    ASSERT_TRUE(smart_A.getRefCounter() == nullptr);

    ASSERT_TRUE(smart_B.getObject() == a);
    ASSERT_TRUE(smart_B.getRefCounter() != nullptr);
    ASSERT_TRUE(smart_B.getRefCounter()->getRefCount() == 1);

    ASSERT_TRUE(smart_A.getRefCounter() != smart_B.getRefCounter());

    ASSERT_EQ(0, (int )saveOldAdress.size());
}

/*
 * Test ob der Vergleichssoperator korrekt ueberladen wurde.
 * 1x Objekt, 1x leerer Smartpointer
 */
TEST(Smartpointer, testCompareObjectNull) {
    PRINT_INFO("Ueberpruefung des Vergleichsoperators: 1x Objekt, 1x leerer Smartpointer\n");

    saveOldAdress.clear();
    A *a = new A;
    SmartPointer<A> smart_A { a }, smart_B;

    ASSERT_TRUE(!(smart_A == smart_B));

    ASSERT_TRUE(smart_A.getObject() == a);
    ASSERT_TRUE(smart_A.getRefCounter() != nullptr);
    ASSERT_TRUE(smart_A.getRefCounter()->getRefCount() == 1);

    ASSERT_TRUE(smart_B.getObject() == nullptr);
    ASSERT_TRUE(smart_B.getRefCounter() == nullptr);

    ASSERT_TRUE(smart_A.getRefCounter() != smart_B.getRefCounter());

    ASSERT_EQ(0, (int )saveOldAdress.size());
}

/*
 * Test ob der Vergleichssoperator korrekt ueberladen wurde.
 * 2x Smartpointer mit ungleichen Objekten
 */
TEST(Smartpointer, testCompareUnequalObjects) {
    PRINT_INFO("Ueberpruefung des Vergleichsoperators: 2x Smartpointer mit ungleichen Objekten\n");

    saveOldAdress.clear();
    A *a = new A;
    A *b = new A;
    SmartPointer<A> smart_A { a }, smart_B { b };

    ASSERT_TRUE(!(smart_A == smart_B));

    ASSERT_TRUE(smart_A.getObject() == a);
    ASSERT_TRUE(smart_A.getRefCounter() != nullptr);
    ASSERT_TRUE(smart_A.getRefCounter()->getRefCount() == 1);

    ASSERT_TRUE(smart_B.getObject() == b);
    ASSERT_TRUE(smart_B.getRefCounter() != nullptr);
    ASSERT_TRUE(smart_B.getRefCounter()->getRefCount() == 1);

    ASSERT_TRUE(smart_A.getRefCounter() != smart_B.getRefCounter());

    ASSERT_EQ(0, (int )saveOldAdress.size());
}

/*
 * Test ob der Vergleichssoperator korrekt ueberladen wurde.
 * 2x Smartpointer mit gleichen Objekten
 */
TEST(Smartpointer, testCompareEqualObjects) {
    PRINT_INFO("Ueberpruefung des Vergleichsoperators: 2x Smartpointer mit gleichen Objekten\n");

    saveOldAdress.clear();
    A *a = new A;
    SmartPointer<A> smart_A { a }, smart_B { smart_A };

    ASSERT_TRUE(smart_A == smart_B);

    ASSERT_TRUE(smart_A.getObject() == a);
    ASSERT_TRUE(smart_A.getRefCounter() != nullptr);
    ASSERT_TRUE(smart_A.getRefCounter()->getRefCount() == 2);

    ASSERT_TRUE(smart_B.getObject() == a);
    ASSERT_TRUE(smart_B.getRefCounter() != nullptr);
    ASSERT_TRUE(smart_B.getRefCounter()->getRefCount() == 2);

    ASSERT_TRUE(smart_A.getRefCounter() == smart_B.getRefCounter());

    ASSERT_EQ(0, (int )saveOldAdress.size());
}

/*
 * Test ob der Vergleichssoperator korrekt ueberladen wurde.
 * 2x leere Smartpointer
 */
TEST(Smartpointer, testNotCompareNullNull) {
    PRINT_INFO("Ueberpruefung des Vergleichsoperators: 2x leere Smartpointer\n");

    saveOldAdress.clear();
    SmartPointer<A> smart_A, smart_B;

    ASSERT_TRUE(!(smart_A != smart_B));

    ASSERT_TRUE(smart_A.getObject() == nullptr);
    ASSERT_TRUE(smart_A.getRefCounter() == nullptr);

    ASSERT_TRUE(smart_B.getObject() == nullptr);
    ASSERT_TRUE(smart_B.getRefCounter() == nullptr);

    ASSERT_EQ(0, (int )saveOldAdress.size());
}

/*
 * Test ob der Vergleichssoperator korrekt ueberladen wurde.
 * 1x leerer Smartpointer, 1x Objekt
 */
TEST(Smartpointer, testNotCompareNullObject) {
    PRINT_INFO("Ueberpruefung des Vergleichsoperators: 1x leerer Smartpointer, 1x Objekt\n");

    saveOldAdress.clear();
    A *a = new A;
    SmartPointer<A> smart_A, smart_B { a };

    ASSERT_TRUE(smart_A != smart_B);

    ASSERT_TRUE(smart_A.getObject() == nullptr);
    ASSERT_TRUE(smart_A.getRefCounter() == nullptr);

    ASSERT_TRUE(smart_B.getObject() == a);
    ASSERT_TRUE(smart_B.getRefCounter() != nullptr);
    ASSERT_TRUE(smart_B.getRefCounter()->getRefCount() == 1);

    ASSERT_TRUE(smart_A.getRefCounter() != smart_B.getRefCounter());

    ASSERT_EQ(0, (int )saveOldAdress.size());
}

/*
 * Test ob der Vergleichssoperator korrekt ueberladen wurde.
 * 1x Objekt, 1x leerer Smartpointer
 */
TEST(Smartpointer, testNotCompareObjectNull) {
    PRINT_INFO("Ueberpruefung des Vergleichsoperators: 1x Objekt, 1x leerer Smartpointer\n");

    saveOldAdress.clear();
    A *a = new A;
    SmartPointer<A> smart_A { a }, smart_B;

    ASSERT_TRUE(smart_A != smart_B);

    ASSERT_TRUE(smart_A.getObject() == a);
    ASSERT_TRUE(smart_A.getRefCounter() != nullptr);
    ASSERT_TRUE(smart_A.getRefCounter()->getRefCount() == 1);

    ASSERT_TRUE(smart_B.getObject() == nullptr);
    ASSERT_TRUE(smart_B.getRefCounter() == nullptr);

    ASSERT_TRUE(smart_A.getRefCounter() != smart_B.getRefCounter());

    ASSERT_EQ(0, (int )saveOldAdress.size());
}

/*
 * Test ob der Vergleichssoperator korrekt ueberladen wurde.
 * 2x Smartpointer mit ungleichen Objekten
 */
TEST(Smartpointer, testNotCompareUnequalObjects) {
    PRINT_INFO("Ueberpruefung des Vergleichsoperators: 2x Smartpointer mit ungleichen Objekten\n");

    saveOldAdress.clear();
    A *a = new A;
    A *b = new A;
    SmartPointer<A> smart_A { a }, smart_B { b };

    ASSERT_TRUE(smart_A != smart_B);

    ASSERT_TRUE(smart_A.getObject() == a);
    ASSERT_TRUE(smart_A.getRefCounter() != nullptr);
    ASSERT_TRUE(smart_A.getRefCounter()->getRefCount() == 1);

    ASSERT_TRUE(smart_B.getObject() == b);
    ASSERT_TRUE(smart_B.getRefCounter() != nullptr);
    ASSERT_TRUE(smart_B.getRefCounter()->getRefCount() == 1);

    ASSERT_TRUE(smart_A.getRefCounter() != smart_B.getRefCounter());

    ASSERT_EQ(0, (int )saveOldAdress.size());
}

/*
 * Test ob der Vergleichssoperator korrekt ueberladen wurde.
 * 2x Smartpointer mit gleichen Objekten
 */
TEST(Smartpointer, testNotCompareEqualObjects) {
    PRINT_INFO("Ueberpruefung des Vergleichsoperators: 2x Smartpointer mit gleichen Objekten\n");

    saveOldAdress.clear();
    A *a = new A;
    SmartPointer<A> smart_A { a }, smart_B { smart_A };

    ASSERT_TRUE(!(smart_A != smart_B));

    ASSERT_TRUE(smart_A.getObject() == a);
    ASSERT_TRUE(smart_A.getRefCounter() != nullptr);
    ASSERT_TRUE(smart_A.getRefCounter()->getRefCount() == 2);

    ASSERT_TRUE(smart_B.getObject() == a);
    ASSERT_TRUE(smart_B.getRefCounter() != nullptr);
    ASSERT_TRUE(smart_B.getRefCounter()->getRefCount() == 2);

    ASSERT_TRUE(smart_A.getRefCounter() == smart_B.getRefCounter());

    ASSERT_EQ(0, (int )saveOldAdress.size());
}

/*
 * Test fuer Bool-Operator.
 * leerer Smartpointer
 */
TEST(Smartpointer, testBoolEmptySmartptr) {
    PRINT_INFO("Ueberpruefung des Bool-Operators: leerer Smartpointer\n");

    saveOldAdress.clear();
    SmartPointer<A> smart_A;

    ASSERT_TRUE((!smart_A == true));

    ASSERT_TRUE(smart_A.getObject() == nullptr);
    ASSERT_TRUE(smart_A.getRefCounter() == nullptr);

    ASSERT_EQ(0, (int )saveOldAdress.size());
}

/*
 * Test fuer Bool-Operator.
 * nicht-leerer Smartpointer
 */
TEST(Smartpointer, testBoolNotEmptySmartptr) {
    PRINT_INFO("Ueberpruefung des Bool-Operators: nicht-leerer Smartpointer\n");

    saveOldAdress.clear();
    A *a = new A;
    SmartPointer<A> smart_A { a };

    ASSERT_TRUE((smart_A == true));

    ASSERT_TRUE(smart_A.getObject() == a);
    ASSERT_TRUE(smart_A.getRefCounter() != nullptr);
    ASSERT_TRUE(smart_A.getRefCounter()->getRefCount() == 1);

    ASSERT_EQ(0, (int )saveOldAdress.size());
}

/*
 * Test ob Polymorphie funktioniert.
 */
TEST(Smartpointer, testPolymorphism) {
    PRINT_INFO("Funktioniert die Polymorphie?\n");

    SmartPointer<A> smart_B;

    smart_B = new B;

    ASSERT_EQ(24, smart_B->getNum());
}

/*
 * Smartpointer als Parameter per Referenz uebergeben
 */
TEST(Smartpointer, testSPAsParameterPerReference) {
    PRINT_INFO("Kann man Smartpointer als Parameter per Referenz uebergeben?\n");

    SmartPointer<A> smart_A { new A };

    ASSERT_EQ(42, func(smart_A));
}

int main(int argc, char *argv[]) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

