#include <curl/curl.h>


class curl_api{
	public:
		curl_api();
		void exec();
		~curl_api();

	private:
		CURL *handle;
		std::string data;
		const std::string URL = "http://api.openweathermap.org/data/2.5/weather?q=Minden,de&APPID=345159b05533c2211da268919dd06c57";


};
