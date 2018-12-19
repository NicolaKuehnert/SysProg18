#ifndef SmartPointerImpl_H
#define SmartPointerImpl_H

#include "smartpointer/RefCounter.h"
#include "smartpointer/NullPointerException.h"
#include <iostream>

#include "smartpointer/SmartPointer.h"

template<class T>

SmartPointer<T>::SmartPointer(T * const p) {
	std::cout << "create\n";
	if(p != nullptr){
		std::cout << "test2\n";
		this->rc = new RefCounter;
		this->pObj = p;
	} else {
		std::cout << "test3\n";
		this->rc = nullptr;
		this->pObj = nullptr;
	}		
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
T* SmartPointer<T>::operator->() const { 
	if(pObj == nullptr){
		throw NullPointerException();
	}
	if(this->rc != nullptr)
	{
		this->rc->inc();
	}	
	return pObj;
}
template<class T>
T& SmartPointer<T>::operator*() const {
	if(pObj == nullptr){
		throw NullPointerException();
	}
	if(this->rc != nullptr)
	{
		this->rc->inc();
	}
	return *pObj;
}
template<class T>
const T *SmartPointer<T>::getObject() const {
	std::cout << "getObject\n";
	if(this->rc != nullptr)
	{
		this->rc->inc();
	}
	return pObj;
}
template<class T>
const RefCounter* SmartPointer<T>::getRefCounter() const{
	return rc;
}
template<class T>
const SmartPointer<T>& SmartPointer<T>::operator= (T* const p) {
	std::cout << "assign1\n";
	pObj = p;
	rc = new RefCounter;	 
	return *this;
}
template<class T>
const SmartPointer<T>& SmartPointer<T>::operator= (const SmartPointer<T>& p) {
	std::cout << "assign2\n";
	this->pObj = p.pObj;
	this->rc = p.rc;
	return *this;
}
template<class T>
bool SmartPointer<T>::operator == (const SmartPointer<T>& sp) const{
	std::cout << "vergleich\n";
	return sp.getObject() ==this->pObj;
}
template<class T>
bool SmartPointer<T>::operator != (const SmartPointer<T>& sp) const{
	return sp.getObject() != this->pObj;
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
