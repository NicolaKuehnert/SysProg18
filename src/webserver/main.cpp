#include "webserver/gui.h"
#include "webserver/client.h"
#include <string.h>
#include <iostream>


int main() {
	move();
	init_client();
	send_to_server("Hallo");
	std::cout << receive_from_server();
}
