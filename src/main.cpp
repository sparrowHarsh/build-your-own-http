#include "../include/HttpServer.h"
#include "../include/HelloWorldController.h"
#include "../include/StudentController.h"

int main(){
    int port = 8080;
    HttpServer server(port);
    std::string method = "GET";
    std::string method1 = "POST";
    server.registerHandler(method, "/hello", new HelloWorldController());
    server.registerHandler(method1, "/student", new StudentController());
    server.registerHandler(method,"/student", new StudentController());
    
    server.start();
    return 0;
}

