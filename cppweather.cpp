#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <algorithm>
#include <curl/curl.h>
#include <sstream>
#include <fstream>

using namespace std;

int main(void)
{
cout <<"AAA\n";
CURL * crl = curl_easy_init();
if(crl)
{
curl_easy_setopt(crl, CURLOPT_URL,"api.openweathermap.org/data/2.5/weather?zip=94040,us&appid=b5df5912a869b5cf1cfa4899b10da754");
  /* use a GET to fetch this */
  curl_easy_setopt(crl, CURLOPT_HTTPGET, 1L);
 
  /* Perform the request */
  curl_easy_perform(crl);
  curl_easy_reset(crl);

  cout << "BBBBBBBBB\n\n\n\n\n";
	curl_easy_setopt(crl, CURLOPT_URL, "https://query.yahooapis.com/v1/public/yql?q=select%20item.condition%20from%20weather.forecast%20where%20woeid%20%3D%202487889&format=json&env=store%3A%2F%2Fdatatables.org%2Falltableswithkeys");
  curl_easy_setopt(crl, CURLOPT_HTTPGET, 1L);
  curl_easy_perform(crl);
  curl_easy_cleanup(crl);
}


return 0;
}

