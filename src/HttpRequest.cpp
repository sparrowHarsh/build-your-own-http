#include "../include/HttpRequest.h"
#include <sstream>

HttpRequest :: HttpRequest (){

}

std::string HttpRequest::getMethod(){
    return method;
}

std::string HttpRequest::getVersion(){
    return version;
}

std::string HttpRequest::getPath(){
    return path;
}

std::string HttpRequest::getBody(){
    return body;
}

std::string HttpRequest::getHeader(const std::string& name){
    return headers[name];
}

std::string HttpRequest::getPathParam(const std::string& name){
    return pathParams[name];
}

void HttpRequest::parseHeaders(std::istringstream& stream){
    std::string line;
    while (std::getline(stream, line) && line != "\r" && !line.empty()) {
        if (!line.empty() && line.back() == '\r') {
            line.pop_back(); // Remove trailing '\r'
        }

        auto colonPos = line.find(": ");
        if (colonPos != std::string::npos) {
            std::string key = line.substr(0, colonPos);
            std::string value = line.substr(colonPos + 2);
            headers[key] = value;
        }
    }
}

void HttpRequest::parseBody(std::istringstream& requestLine){
    if (method == "POST" || method == "PUT") {
        // Read the entire remaining content as body
        std::stringstream body_stream;
        body_stream << requestLine.rdbuf();
        body = body_stream.str();
    }
}

void HttpRequest::parseMethod(const std::string& methodStr) {
    if (methodStr == "GET") {
        method = "GET";
    } else if (methodStr == "POST") {
        method ="POST";
    } else if (methodStr == "PUT") {
        method = "PUT";
    } else if (methodStr == "DELETE") {
        method = "DELETE";
    } else {
        method = "UNKNOWN";
    }
}

void HttpRequest::setPathParam(const std::string& name, const std::string& value){
    pathParams[name] = value;
}

