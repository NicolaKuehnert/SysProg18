#include <mutex>
#include <iostream>

/**
This is an example class for RAII
*/

int count[2] ={0,1};
int index = 0;
std::mutex m;

void increment()
{
	if(index > 1){
		throw std::out_of_range( "Index out of bounds");		//generate exception so the program crashes
	}
	
	std::cout << count[index] << " : in increment\n";
	
	index++;
}

int main(){
	std::lock_guard<std::mutex> lock(m);	//Mutex quisition is initialization
	
	increment();
	increment();
	//increment();  hier würde er eine exception werfen. m würde trotzdem freigegeben werden
	
	return 0; // kommt die Funktion bis hier, wird m auch automatisch freigegeben
	

}
