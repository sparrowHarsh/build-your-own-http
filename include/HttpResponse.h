#pragma once

#include <string>
#include <map>

class HttpResponse {
    private:
        std::string httpVersion;
        int statusCode;
        std::string statusMessage;
        std::map<std::string, std::string> headers;
        std::string body;

    public:
        HttpResponse();
        void setStatus(int code, const std::string& message);
        void setHeader(const std::string& key, const std::string& value);
        void setBody(const std::string& content);
        std::string toString() const;
};
