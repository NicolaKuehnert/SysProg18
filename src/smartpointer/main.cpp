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
	
	return 0;
}
