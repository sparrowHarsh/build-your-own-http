#pragma once

#include <string>
#include <vector>
#include <map>
#include "build-your-own-http/http/ThreadPoolExecuter.h"
#include "build-your-own-http/http/HttpRequest.h"
#include "build-your-own-http/http/HttpResponse.h"
#include "build-your-own-http/handlers/RequestHandler.h"
#include "build-your-own-http/http/RouteInfo.h"


class HttpServer {
    private:
        int port;
        int serverSocket;
        bool isRunning;
        ThreadPoolExecuter threadpool;

        std::vector<RouteInfo> routes;
        RequestHandler* badRequestHandler;
        RequestHandler* resourceNotFoundHandler;
        RequestHandler* methodNotFoundHandler;

        RequestHandler* findHandler(HttpRequest& httpRequest);

    public:
        HttpServer(int port);
        // ~HttpServer();
        bool start();
        void stop();
        int getPort(); 

        void accept_connections();
        void handleConnections(int clientSocket);
        void sendResponse(int clientSocket, const std::string& response);
        void registerHandler(std::string& method, const std::string& path, RequestHandler* handler);
};