#include "smartpointer/RefCounter.h"
#include "smartpointer/NullPointerException.h"
#include "smartpointer/SmartPointer.h"
#include "smartpointer/SmartPointerImpl.h"
#include <iostream>

class Test{
	public:
		Test(std::string p = "") {
			this->st = p;
		}
		std::string getString(){
			return this->st;
		}
		void setString(std::string t) {
			this->st = t;
		}
	private:
		std::string st;
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
	std::cout << "weiter3\n\n";
	
	//while(true);
	return 0;
}
