#ifndef SmartPointerImpl_H
#define SmartPointerImpl_H

#include "smartpointer/RefCounter.h"
#include <iostream>

#include "smartpointer/SmartPointer.h"

template<class T>

SmartPointer<T>::SmartPointer(T * const p) { // hier muss eigentlich ein default parameter angegeben werden
	std::cout << "create\n";
	this->pObj = p;
	this->rc = new RefCounter;
}
template<class T>
SmartPointer<T>::SmartPointer(const SmartPointer<T>& p){
	std::cout << "create2\n";
	this->pObj= p.pObj;
	this->rc = p.rc;
}
template<class T>
SmartPointer<T>::~SmartPointer(){
	std::cout << "delete\n";
	deleteObject();
}
template<class T>
T* SmartPointer<T>::operator->() const { // irgendwie dereferenzieren...
	return pObj;
}
template<class T>
T& SmartPointer<T>::operator*() const {
	//return &pObj;
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
	std::cout << "assign\n";
	pObj = p;
	rc = new RefCounter;	
}
template<class T>
const SmartPointer<T>& SmartPointer<T>::operator= (const SmartPointer<T>& p) {
	this->pObj = p.getObject();
	this->rc = p.getRefCounter();
}
template<class T>
bool SmartPointer<T>::operator == (const SmartPointer& sp) const{
	return sp.getObject() ==pObj;
}
template<class T>
bool SmartPointer<T>::operator != (const SmartPointer& sp) const{
	return sp.getObject() != pObj;
}
template<class T>
SmartPointer<T>::operator bool () const{
	return pObj != nullptr;
}
template<class T>
void SmartPointer<T>::deleteObject(){
	delete rc;
}






#endif
