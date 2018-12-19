#ifndef NULLPTR_EXCEPTION_H
#define NULLPTR_EXCEPTION_H
#include <exception>
#include <stdexcept>
class NullPointerException: public std::runtime_error{
	public:
		NullPointerException():std::runtime_error("Das ist ein NullPointer\n"){}
		std::string const message = "Das ist ein NullPointer\n";
	
		
};


#endif /* NULLPTR_EXCEPTION_H */
