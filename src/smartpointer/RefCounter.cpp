#include "smartpointer/RefCounter.h"
#include <iostream>


RefCounter::RefCounter(){
	
}

void RefCounter::inc(){
	RefCounter::n++;
}

void RefCounter::dec(){
	if(RefCounter::n >0){
		RefCounter::n--;
	}
}


bool RefCounter::isZero() const{
	return RefCounter::n == 0;
}

unsigned int RefCounter::getRefCount() const{
	return RefCounter::n;
}


void test(){
	RefCounter* t =new  RefCounter;
	std::cout << std::to_string(t->getRefCount()) + "\n";
	
	std::cout << std::to_string(t->getRefCount()) + "\n";
	t->dec();
}

