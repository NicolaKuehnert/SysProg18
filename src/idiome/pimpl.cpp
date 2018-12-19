#include <iostream>
#include <memory>
#include <experimental/propagate_const>

//this would usually be in a header file
class widget{
	public:
		widget(int);
		~widget();
	
		void out() const;
		void out();

	private:
		class impl;
		std::experimental::propagate_const<std::unique_ptr<impl>> pImpl;		// pointer to impl class
};

class widget::impl{
	private:
		int n;
	public:
		impl(int n) : n(n) {}
		
		//called functions thanks to pImpl
		void out() const {
			std::cout << "Constant out\n";
		}
		
		int out(){
			std::cout << "Non-constant out\n";
		}
};

//definitions of widget functions
widget::widget(int n) : pImpl{std::make_unique<impl>(n)}{}	// call the constructor for impl and make an object pointer to it
widget::~widget() = default;								// deconstruct needed for complete implementation
void widget::out() const { pImpl->out(); } 					// call the const out function in impl
void widget::out(){ pImpl->out(); }							// call the out function in impl

int main(){

	widget w(1);											// using a non-constant object results in using the non-constant out function
	const widget c_w(2);									// using a constant object results in using the constant out function
	
	w.out();
	std::cout << " after first function call\n";
	
	c_w.out();
	std::cout << " after second function call\n";
	
	return 0;

}
