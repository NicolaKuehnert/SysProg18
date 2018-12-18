#ifndef SmartPointerImpl_H
#define SmartPointerImpl_H

#include "smartpointer/RefCounter.h"
#include "smartpointer/SmartPointer.h"

template<class T>

SmartPointer<T>::SmartPointer(T * const p) {
	pObj = p;
	rc = new RefCounter;
}
template<class T>
SmartPointer<T>::SmartPointer(const SmartPointer<T>&){
	pObj = SmartPointer<T>::getObject();
	rc = SmartPointer<T>::getRefCounter();
}
template<class T>
SmartPointer<T>::~SmartPointer(){
	SmartPointer::deleteObject();
}
template<class T>
T* SmartPointer<T>::operator->() const { // irgendwie dereferenzieren...
	return *SmartPointer::pObj;
}
template<class T>
T& SmartPointer<T>::operator*() const {
	return &SmartPointer::pObj;
}
template<class T>
const T *SmartPointer<T>::getObject() const {
	return pObj;
}
template<class T>
const RefCounter* SmartPointer<T>::getRefCounter() const{
	return rc;
}
template<class T>
const SmartPointer<T>& SmartPointer<T>::operator= (T* const p) {
	pObj = p;
	rc = new RefCounter;
}
template<class T>
const SmartPointer<T>& SmartPointer<T>::operator= (const SmartPointer&) {
	pObj = SmartPointer::getObject();
	rc = SmartPointer::getRefCounter();
}
template<class T>
bool SmartPointer<T>::operator== (const SmartPointer& sp) const{
	return sp->getObject() ==pObj;
}
template<class T>
bool SmartPointer<T>::operator!=(const SmartPointer& sp) const{
	return sp->getObject() != SmartPointer::pObj;
}
template<class T>
SmartPointer<T>::operator bool () const{
	return pObj != nullptr;
}
template<class T>
void SmartPointer<T>::deleteObject(){
	delete rc;
	delete pObj;
	delete this;
}




#endif
