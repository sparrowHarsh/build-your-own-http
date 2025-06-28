#pragma once

#include "RequestHandler.h"
#include "HttpRequest.h"
#include "HttpResponse.h"

class ResourceNotFoundHandler: public RequestHandler {
    public:
        void handle(const HttpRequest& httpRequest, HttpResponse& httpResponse) override ;
        ~ResourceNotFoundHandler() override = default;
};