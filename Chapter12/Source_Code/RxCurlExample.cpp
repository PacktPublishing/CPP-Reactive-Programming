//g++  -ICurl_Lib\include 
//       -IRxcpp-master\Rx\v2\src RxCurlExample.cpp 
//        -LCurl_Lib\lib  -lcurl
//////////////////////////////////////////
// A Simple program to pull HTTP conent 
// using a Rx wrapper on top of the Libcurl
//
//
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <map>
#include <chrono>
using namespace std;
using namespace std::chrono;
////////////////////////
// include Curl Library and 
// Rxcpp library 
//
#include <curl/curl.h>
#include <rxcpp/rx.hpp>
using namespace rxcpp;
using namespace rxcpp::rxo;
using namespace rxcpp::rxs;
//////////////////////////
// include the modified rxcurl library from 
// Kirk Shoop's Twitter Analysis app
//
#include "rxcurl.h"
using namespace rxcurl;

int main() {
     /////////////////////////////////////
     //
     // Create a factory object to create 
     // HTTP request.  The http_request structure
     // is defined in rxcurl.h
     string url = "http://example.com";
     auto factory = create_rxcurl();
     auto request  = factory.create(http_request{url, "GET",{}, {}}) |
            rxo::map([](http_response r){
                return r.body.complete;
            });     
     
     ////////////////////////////////////////
     //
     // make a blocking call to the url..
     observable<string>   response_message;
     request.as_blocking().subscribe([&] (observable<string> s) {
               response_message = s.sum();
     } ,[] () {});
 
     ///////////////////////////////
     //
     // retrieve the html content form the site 
     string html;
     response_message.as_blocking().subscribe( [&html] ( string temp ) {         
                   html = temp;
     }, [&html] () { } );
     //------------ Print to the Console...
     cout << html << endl;
}