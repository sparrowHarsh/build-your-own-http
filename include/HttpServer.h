#include <iostream>
#include <string>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <cstring>
#include <cerrno>
#include "../include/ThreadPoolExecuter.h"


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