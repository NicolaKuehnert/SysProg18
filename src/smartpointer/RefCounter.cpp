#include "smartpointer/RefCounter.h"
#include <iostream>


RefCounter::RefCounter(){
	this->n = 0;
}

void RefCounter::inc(){
	this->n+= 1;
}

void RefCounter::dec(){
	if(RefCounter::n >0){
		this->n-=1;
	}
}


bool RefCounter::isZero() const{
	return this->n == 0;
}

unsigned int RefCounter::getRefCount() const{
	return this->n;
}


void test(){
	RefCounter* t =new  RefCounter;
	t->inc();
	std::cout << std::to_string(t->getRefCount() == 1) + "\n";
	t->inc();
	std::cout << std::to_string(t->getRefCount() == 2) + "\n";
	t->inc();
	std::cout << std::to_string(t->getRefCount() == 3) + "\n";
	t->inc();
}

