/*
 * SmartPointer.h
 *
 */

#ifndef SMARTPOINTER_H
#define SMARTPOINTER_H

#include "smartpointer/RefCounter.h"


/*
 * ===========================================================================
 * Eigentliche Smartpointer-Klasse (Schnittstelle)
 *
 *                               VORGABEN
 * ===========================================================================
 */

template<class T>
class SmartPointer {
public:
    // Constructs new SmartPointer
    SmartPointer(T * const p = nullptr);
    SmartPointer(const SmartPointer&);

    // Destructs the owned object if no more SmartPointer links to it
    virtual ~SmartPointer();

    // Dereferences pointer to the managed object
    T *operator->() const;
    T &operator*() const;

    // Returns a pointer to the managed object
    const T *getObject() const;

    // Returns the number of SmartPointer objects referring to the same managed object
    const RefCounter *getRefCounter() const;

    // Assigns the SmartPointer
    const SmartPointer &operator=(T * const p);
    const SmartPointer &operator=(const SmartPointer&);

    // Compares with another SmartPointer or with nullptr
    bool operator==(const SmartPointer&) const;
    bool operator!=(const SmartPointer&) const;

    // Checks if there is associated managed object
    operator bool() const;

private:
    // Pointer to the current object
    T* pObj;

    // Pointer to the reference counter (used for the current object)
    RefCounter *rc;

    // Auxiliary function: decrements reference counter and deletes object and
    // reference counter, if necessary
    void deleteObject();
};


/*
 * ===========================================================================
 * Eigentliche Smartpointer-Klasse (Implementierung: Template -> .h)
 * ===========================================================================
 */

#include "smartpointer/SmartPointerImpl.h"


#endif  // SMARTPOINTER_H

