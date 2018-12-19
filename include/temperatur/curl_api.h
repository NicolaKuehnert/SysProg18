#include <curl/curl.h>
#include <string>

/**
@file curl_api.h Header für die curl-api Klasse

Dies ist eine Klasse, die auf der API von CURL aufbaut und Daten abrufen kann.
*/

class curl_api{
	public:
		curl_api();
		void exec();
		~curl_api();
		std::string  get_data();

	private:
		CURL *handle;
		
		char*  URL = "api.openweathermap.org/data/2.5/weather?q=Minden,de&APPID=345159b05533c2211da268919dd06c57";


};

int test(void);
