#include <curl/curl.h>
#include <iostream>
#include "temperatur/curl_api.h"
using namespace std;

static size_t f(char *data, size_t size, size_t nmemb, void *userdata){
	//save the userdata
	return 0;
}

curl_api::curl_api(){
	handle = curl_easy_init();
	curl_easy_setopt(handle, CURLOPT_WRITEFUNCTION, f);
	curl_easy_setopt(handle, CURLOPT_WRITEDATA, &data);
	curl_easy_setopt(handle, CURLOPT_URL, &URL);
	
}

void curl_api::exec(){
	CURLcode res = curl_easy_perform(handle);

	if(res){
		std::cout << "An Error occured";
	}
}

curl_api::~curl_api(){
	curl_easy_cleanup(handle);
}
