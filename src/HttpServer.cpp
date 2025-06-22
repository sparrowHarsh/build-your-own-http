#include <iostream>
#include <string>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <cstring>
#include <cerrno>
#include "ThreadPoolExecuter.cpp"


class HttpServer {
    private:
        int port;
        int serverSocket;
        bool isRunning;
        ThreadPoolExecuter threadpool;


    public:
        HttpServer(int port);
        ~HttpServer();
        bool start();
        void stop();
        int getPort(); 

        void accept_connections();
        void handleConnections(int clientSocket);
        void sendResponse(int clientSocket, const std::string& response);
        // void registerHandler(const HttpMethod method, const std::string& path, RequestHandler* handler);
};


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
        if(clientSocket < 0){
            std::cout << "Error on accept: " << strerror(errno) << std::endl;
            continue;
        }

        // Assuming you have a threadpool instance named threadpool
        // and enqueue is the correct method name
        threadpool.enqueue([this, clientSocket](){
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
    // Handle the request here...

    std::cout<<"Received message from clinet : "<<buffer<<std::endl;

    close(clientSocket);
}