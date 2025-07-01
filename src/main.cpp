#include "../include/HttpServer.h"
#include "../include/HelloWorldController.h"

int main(){
    int port = 8080;
    HttpServer server(port);
    std::string method = "GET";
    server.registerHandler(method, "/hello", new HelloWorldController());
    
    server.start();
    return 0;
}

