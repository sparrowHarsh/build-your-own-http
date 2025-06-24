#include <iostream>
#include <string>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <cstring>
#include <cerrno>
#include "../include/ThreadPoolExecuter.h"
#include "../include/HttpServer.h"

HttpServer::HttpServer(int port) : port(port), serverSocket(-1), isRunning(false), threadpool(4) {
    std::cout << "HttpServer created on port " << port << std::endl;
}

bool HttpServer::start() {
    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket < 0) {
        perror("Failed to create socket");
        return false;
    }

    struct sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(port);

    if (bind(serverSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0) {
        perror("Failed to bind socket");
        close(serverSocket);
        return false;
    }

    if (listen(serverSocket, 5) < 0) {
        perror("Failed to listen on socket");
        close(serverSocket);
        return false;
    }

    isRunning = true;

    std::cout << "HttpServer started on port " << port << std::endl; 
    accept_connections();
    return true;
}

void HttpServer::stop() {
    if (isRunning) {
        close(serverSocket);
        isRunning = false;
        std::cout << "HttpServer stopped" << std::endl;
    }
}

void HttpServer::accept_connections() {
    struct sockaddr_in clientAddress;
    socklen_t clientAddressLen = sizeof(clientAddress);

    while(isRunning){
        int clientSocket = accept(serverSocket, (struct sockaddr*)&clientAddress, &clientAddressLen);

        std::cout<<"accepted connection from client"<<std::endl;
        if(clientSocket < 0){
            std::cout << "Error on accept: " << strerror(errno) << std::endl;
            continue;
        }

        // Check if threadpool is running and enqueue is working
        std::cout << "Enqueuing client socket " << clientSocket << " to threadpool" << std::endl;
        
        threadpool.enqueue([this, clientSocket](){
            std::cout << "Thread picked up client socket " << clientSocket << std::endl;
            this->handleConnections(clientSocket);
        });

    }
}

void HttpServer::handleConnections(int clientSocket){
    char buffer[4096] = {0};
    int readBytes = read(clientSocket, buffer, sizeof(buffer)-1);

    if(readBytes < 0){
        std::cerr << "Error while reading from socket: " << strerror(errno) << std::endl;
        close(clientSocket);
        return;
    }

    // Parse the HTTP request
    HttpRequest request;
    if (!request.parse(buffer)) {
        std::cerr << "Failed to parse HTTP request" << std::endl;
        close(clientSocket);
        return;
    }

    // Routing logic
    HttpResponse response;
    response.setHeader("Content-Type", "text/plain");

    if (request.getPath() == "/hello" && request.getMethod() == "GET") {
        response.setStatus(200, "OK");
        response.setBody("Hello, world!");
    } else if (request.getPath() == "/goodbye" && request.getMethod() == "GET") {
        response.setStatus(200, "OK");
        response.setBody("Goodbye!");
    } else {
        response.setStatus(404, "Not Found");
        response.setBody("Route not found");
    }

    // Send the response
    std::string responseStr = response.toString();
    ssize_t sentBytes = write(clientSocket, responseStr.c_str(), responseStr.size());
    if (sentBytes < 0) {
        std::cerr << "Error while writing to socket: " << strerror(errno) << std::endl;
    }

    close(clientSocket);
}