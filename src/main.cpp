#include "../include/HttpServer.h"

int main(){
    int port = 8080;
    HttpServer server(port);

    server.registerHandler(HttpMethod::GET, "/hello", new HelloWorldController());
    
    server.start();
    return 0;
}