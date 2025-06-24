#pragma once

#include <string>
#include <map>
#include <sstream>

class HttpRequest{
    private:
        std::string method;
        std::string path;
        std::string version;
        std::map<std::string,std::string> headers;
        std::map<std::string,std::string> pathParams;
        std::string body;
        void parseHeaders(std::istringstream& requestStream);
        void parseMethod(const std::string& methodStr);
        void parseBody(std::istringstream& requestStream);

    public:
        HttpRequest();
        bool parse(const std::string& rawRequest);
        std::string getMethod();
        std::string getPath();
        std::string getPathParam(const std::string& name);
        std::string getVersion();
        std::string getHeader(const std::string& name);
        std::string getBody();
        void setPathParam(const std::string& name, const std::string& value);
};

