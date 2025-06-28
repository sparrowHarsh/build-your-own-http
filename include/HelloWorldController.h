#pragma once

#include "RequestHandler.h"
#include "HttpRequest.h"
#include "HttpResponse.h"

class HelloWorldController: public RequestHandler {
    public:
        void handle(const HttpRequest& httpRequest, HttpResponse& httpResponse) override;
};