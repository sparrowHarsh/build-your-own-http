#pragma once

// Forward declarations to avoid circular dependencies
class HttpRequest;
class HttpResponse;

class RequestHandler {
    public:
        virtual void handle(const HttpRequest& httpRequest, HttpResponse& httpResponse) = 0;
        virtual ~RequestHandler() = default;
};