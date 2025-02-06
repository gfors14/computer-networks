#include "Socket.h"
#include "request.h"
#include "response.h"
#include <iostream>

int main() {
    // Create a socket and connect
    Socket* socket = new Socket("httpbin.org", 80);
    socket->connect();

    // Create a request object
    request* req = new request("GET", "/get");
    req->add_header("Host", "httpbin.org");
    req->add_header("User-agent", "main-http");
    req->add_header("Connection", "close");

    // Send request
    socket->send(req->to_string());

    // Receive response
    std::string responseString = socket->recv();

    // Create a Response object
    response* res = new response();

    // Display the response string
    std::cout << "Response from server:\n" << responseString << std::endl;

     //Clean up
    delete socket;
    delete req;
    delete res;
   

    return 0;
}
