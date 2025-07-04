#include <iostream>
#include <string>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <cstring>
#include <cerrno>
#include "build-your-own-http/http/ThreadPoolExecuter.h"
#include "build-your-own-http/http/HttpServer.h"
#include "build-your-own-http/http/HttpRequest.h"
#include "build-your-own-http/http/HttpResponse.h"
#include "build-your-own-http/handlers/RequestHandler.h"
#include "build-your-own-http/http/RouteInfo.h"

HttpServer::HttpServer(int port) : port(port), serverSocket(-1), isRunning(false), threadpool(4) {
    std::cout << "HttpServer created on port " << port << std::endl;
}

void HttpServer::registerHandler(std::string& method, const std::string& requestURI, RequestHandler* handler) {
    RouteInfo route;

    std::string pattern = "^";
    size_t start = 0, colon = -1;

    while ((colon = requestURI.find(':', start)) != std::string::npos) {
        pattern += requestURI.substr(start, colon - start);
        
        size_t paramEnd = requestURI.find('/', colon);
        if (paramEnd == std::string::npos) {
            paramEnd = requestURI.length();
        }

        std::string paramName = requestURI.substr(colon + 1, paramEnd - colon -1);
        route.paramNames.push_back(paramName);

        pattern += "([^/]+)";
        start = paramEnd;
    }
    
    pattern += requestURI.substr(start) + "$";

    size_t asterisk;
    while ((asterisk = pattern.find('*')) != std::string::npos) {
        pattern.replace(asterisk, 1, ".*");
    }
    
    route.method = method;
    route.pattern = std::regex(pattern);
    route.handler = std::move(handler);
    routes.push_back(route);
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
   // Use registered handler for routing
    HttpResponse response;
    RequestHandler* handler = findHandler(request);
    if (handler) {
        handler->handle(request, response);
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


RequestHandler* HttpServer::findHandler(HttpRequest& httpRequest) {
    std::string path = httpRequest.getPath();
    
    bool patternMatchingHandlerExists = false;

    for (const auto& route : routes) {
        std::smatch matches;
        if (std::regex_match(path, matches, route.pattern)) {
            patternMatchingHandlerExists = true;

            for (size_t i = 0 ; i < route.paramNames.size() && i + 1 < matches.size(); ++i) {
                httpRequest.setPathParam(route.paramNames[i], matches[i + 1].str());
            }
            
            if (route.method == httpRequest.getMethod()) {
                return route.handler;
            }
        }
    }

    return (patternMatchingHandlerExists) ? methodNotFoundHandler : resourceNotFoundHandler;
}