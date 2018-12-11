#include <curl/curl.h>
#include <iostream>

static size_t curl_api::f(char *data, size_t size, size_t nmemb, void *userdata){
	//save the userdata
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
		cout << "An Error occured";
	}
}

~curl_api(){
	curl_easy_cleanup(handle);
}
