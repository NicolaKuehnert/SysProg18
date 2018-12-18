#ifndef SmartPointerImpl_H
#define SmartPointerImpl_H

#include "smartpointer/RefCounter.h"
#include "smartpointer/SmartPointer.h"

template<class T>
SmartPointer<T>::SmartPointer(T * const p) {
	SmartPointer::pObj = p;
	SmartPointer::rc = new RefCounter;
}

SmartPointer<T>::SmartPointer(const SmartPointer&){
	SmartPointer::pObj = SmartPointer<T>::getObject();
	SmartPointer::rc = SmartPointer<T>::getRefCounter();
}/*

SmartPointer::~SmartPointer(){
	SmartPointer::deleteObject();
}

T* SmartPointer::operator->() const { // irgendwie dereferenzieren...
	return *SmartPointer::pObj;
}

T& SmartPointer::operator*() const {
	return &SmartPointer::pObj;
}

const T *SmartPointer::getObject() const {
	return SmartPointer::pObj;
}

const RefCounter* SmartPointer::getRefCounter(){
	return SmartPointer::rc;
}

const SmartPointer& SmartPointer::operator= (T* const p) {
	SmartPointer::pObj = T;
	SmartPointer::rc = new RefCounter;
}

const SmartPointer& SmartPointer::operator= (const SmartPointer&) {
	SmartPointer::pObj = SmartPointer::getIbject();
	SmartPointer::rc = SmartPointer::getRefCounter();
}

bool SmartPointer::operator==(const SmartPointer& sp){
	return sp->getObject() == SmartPointer::pObj;
}

bool SmartPointer::operator!=(const SmartPointer& sp){
	return sp->getObject() != SmartPointer::pObj;
}

bool SmartPointer::operator bool () {
	return SmartPointer::pObj != nullptr;
}

void SmartPointer::deleteObject(){
	~SmartPointer::rc;
	~SmartPointer::pObj;
}
*/



#endif
