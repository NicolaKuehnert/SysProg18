#include "webserver/gui.h"
#include "webserver/client.h"
#include <string.h>
#include <iostream>


int main() {
	init_client();
	while(true)
	{
		send_to_server("l");
		std::cout << "send l" << std::endl;
	}
}
