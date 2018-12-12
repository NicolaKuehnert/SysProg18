#include <curl/curl.h>


class curl_api{
	public:
		curl_api();
		void exec();
		~curl_api();
		char*  get_data();

	private:
		CURL *handle;
		
		char*  URL = "api.openweathermap.org/data/2.5/weather?q=Minden,de&APPID=345159b05533c2211da268919dd06c57";


};

int test(void);
