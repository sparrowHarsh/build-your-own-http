#pragma once
#include "RequestHandler.h"
#include "HttpRequest.h"
#include "HttpResponse.h"

class StudentController : public RequestHandler {
public:
    void handle(HttpRequest& httpRequest, HttpResponse& httpResponse) override;
};