///////////////////////////////////
// A Simple Program to demonstrate 
// the usage of libcurl library
//
#include <stdio.h>
#include <curl/curl.h>
///////////////////////
// Entrypoint for the program
// 
int main(void)
{
  CURL *curl;
  CURLcode res;
  ///////////////////////////
  // Initialize the library
  //
  curl = curl_easy_init();
  if(curl) {
    //----------- Set the URL 
    curl_easy_setopt(curl, CURLOPT_URL, 
				"http://example.com");
    //////////////////////////////////////////
    // To support URL re-direction, we need to configure
    // the lib curl library with CURLOPT_FOLLOWLOCATION
    // 
    curl_easy_setopt(curl, 
			CURLOPT_FOLLOWLOCATION, 1L);
 
    ///////////////////////////////////////////////////
    // Now that, we have setup the options necessary,
    // invoke the operation to pull data 
    //
    res = curl_easy_perform(curl);
    
    if(res != CURLE_OK) {
      //----- if error, print the error on console
      cout << "curl_easy_perform() failed: "
              << curl_easy_strerror(res) << endl;
    }
    curl_easy_cleanup(curl);
  }
  return 0;
}