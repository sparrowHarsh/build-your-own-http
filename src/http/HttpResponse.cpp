#include "build-your-own-http/http/HttpResponse.h"
#include <string>
#include <map>


HttpResponse::HttpResponse() : httpVersion("HTTP/1.1"), statusCode(200), statusMessage("OK") {

}

void HttpResponse::setStatus(int code, const std::string& message) {
    statusCode = code;
    statusMessage = message;
}

void HttpResponse::setHeader(const std::string& key, const std::string& value) {
    headers[key] = value;
}

void HttpResponse::setBody(const std::string& content) {
    body = content;
    headers["Content-Length"] = std::to_string(body.size());
}

std::string HttpResponse::toString() const {
    std::string response;
    response += httpVersion + " " + std::to_string(statusCode) + " " + statusMessage + "\r\n";
    for (const auto& [key, value] : headers) {
        response += key + ": " + value + "\r\n";
    }
    response += "\r\n";  // blank line after headers
    response += body;
    return response;
}