#include "smartpointer/RefCounter.h"
#include "smartpointer/NullPointerException.h"
#include "smartpointer/SmartPointer.h"
#include "smartpointer/SmartPointerImpl.h"
#include <iostream>

class Test{
	public:
		Test(char* p = "") {
			this->st = p;
		}
		char* getString(){
			return this->st;
		}
		void setString(char* t) {
			this->st = t;
		}
	private:
		char* st;
};

void test(void);

int main(void) {
	test();
	Test t = Test("hallo");
	SmartPointer<Test> s = SmartPointer<Test>(nullptr);
	s.getObject();
	
	
	std::cout << "weiter\n\n";
	s = &t;
	std::cout << "weiter2\n\n";

	if(s == SmartPointer<Test>(nullptr)) {
		std::cout << "null\n";
	} else {
		std::cout << "not null\n";
	}
	
	//while(true);
	return 0;
}
