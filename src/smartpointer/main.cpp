#include "smartpointer/RefCounter.h"
#include "smartpointer/NullPointerException.h"
#include "smartpointer/SmartPointer.h"
#include "smartpointer/SmartPointerImpl.h"
#include <iostream>

class Test{
	public:
		Test(char* p = "") {
			st = p;
		}
		char* getString();
		void setString();
	private:
		char* st;
};

void test(void);

int main(void) {
	test();
	
	SmartPointer<Test> s = SmartPointer<Test>(nullptr);
	return 0;
}
