#pragma once

#include "RequestHandler.h"
#include "HttpRequest.h"
#include "HttpResponse.h"

class MethodNotFoundHandler: public RequestHandler {
    public:
        void handle(const HttpRequest& httpRequest, HttpResponse& httpResponse) override;
        ~MethodNotFoundHandler() override = default;
};