#include <curl/curl.h>


class curl_api{
	public:
		static size_t f(char *data, size_t size, size_t nmemb, void *userdata);
		curl_api();
		void exec();
		~curl_api();

	private:
		CURL *handle;
		std::string data;
		const std::string URL = "http://api.openweathermap.org/data/2.5/weather?q=Minden,de&APPID=345159b05533c2211da268919dd06c57";


}
