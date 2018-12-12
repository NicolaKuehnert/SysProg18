#include <curl/curl.h>
#include <iostream>
#include "temperatur/curl_api.h"
#include <string>
using namespace std;
std::string  sdata;

static size_t f(char *data, size_t size, size_t nmemb, void *userdata){
	sdata = data;

	return sizeof(sdata);
}

curl_api::curl_api(){
	char*  ptr = URL;
	handle = curl_easy_init();
	curl_easy_setopt(handle, CURLOPT_WRITEFUNCTION, f);
	curl_easy_setopt(handle, CURLOPT_WRITEDATA, &sdata);
	curl_easy_setopt(handle, CURLOPT_URL, ptr);
	
}

void curl_api::exec(){
	CURLcode res = curl_easy_perform(handle);

	if(res){
		std::cout << curl_easy_strerror(res) << "\n";
	}
}

curl_api::~curl_api(){
	curl_easy_cleanup(handle);
}

std::string  curl_api::get_data(){
	return sdata;
}

int test(void){

	curl_api handler = curl_api();
	handler.exec();
	std::string  ret = handler.get_data();
	std::cout << ret << "\n";
	return 0;
}
