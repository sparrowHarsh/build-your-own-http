#include "../include/HttpServer.h"

int main(){
    int port = 8080;
    HttpServer server(port);
    server.start();
    return 0;
}