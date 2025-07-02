#pragma once

#include "build-your-own-http/handlers/RequestHandler.h"
#include "build-your-own-http/http/HttpRequest.h"
#include "build-your-own-http/http/HttpResponse.h"

class HelloWorldController: public RequestHandler {
    public:
        void handle(HttpRequest& httpRequest, HttpResponse& httpResponse) override;
};