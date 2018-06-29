#include <iostream>
#include <stdio.h>
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


rxcurl::rxcurl factory;

string HttpCall( string url , 
               string method,
               std::map<string,string> headers,
               string  body  )
{        
  // auto factory = create_rxcurl();
     auto request  = factory.create(http_request{url,method,headers,body}) |
            rxo::map([](http_response r){
                return r.body.complete;
            });     
     
     ////////////////////////////////////////
     //
     // make a blocking call to the url..
     observable<string>   response_message;
     request.as_blocking().subscribe([&] (observable<string> s) {
               response_message = s.sum();
     } ,[] () {printf("");});
 
     ///////////////////////////////
     //
     // retrieve the html content form the site 
     string html;
     response_message.as_blocking().subscribe( [&html] ( string temp ) {         
                   html = temp;
     }, [] () { printf(""); } );
     return html;
}
/////////////////////////
// The EntryPoint...
//
int main() {

     ///////////////////////////////////
     // set the url and create the rxcurl object
     string url = "http://localhost:34567/DBDEMO/";
     factory = create_rxcurl();
     /////////////////////////////////
     // default header values
     std::map<string,string> headers;
     headers["Content-Type"] = "application/json";
     headers["Cache-Control"] = "no-cache";

     //------- invoke GET to retrieve the contents
     string html = HttpCall( url,"GET",headers, "" );
     cout << html << endl;

     //------- Retrieve values for the following 
     string body = string("[\"Praseed\"]\r\n");
     html = HttpCall( url,"POST", headers,body);
     cout << html << endl;
     //--------- Add new Values using PUT
     body = string("\r\n{\"Praveen\": \"29\",\"Rajesh\" :\"41\"}\r\n");
     html = HttpCall( url,"PUT", headers,body);
     cout << html << endl;
     //-------- See whether values has been added
     html = HttpCall( url,"GET",headers, "" );
     cout << "-------------------------current database state" << endl;
     cout << html << endl;

     //--------------- DELETE a particular record
     body = string("[\"Praseed\"]\r\n");
     html = HttpCall( url,"DELETE", headers,body);
     cout << "Delleted..." << html << endl;
     html = HttpCall( url,"GET",headers, "" );
     cout << "-------------------------current database state" << endl;
     cout << html << endl;

}