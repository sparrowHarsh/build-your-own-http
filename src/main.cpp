#include "build-your-own-http/http/HttpServer.h"
#include "build-your-own-http/controllers/HelloWorldController.h"
#include "build-your-own-http/controllers/StudentController.h"

int main(){
    int port = 8080;
    HttpServer server(port);
    std::string method = "GET";
    std::string method1 = "POST";
    server.registerHandler(method, "/hello", new HelloWorldController());
    server.registerHandler(method1, "/student", new StudentController());
    server.registerHandler(method,"/student", new StudentController());
    method = "DELETE";
    server.registerHandler(method, "/student", new StudentController());
    
    server.start();
    return 0;
}

