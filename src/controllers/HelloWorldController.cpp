#include <iostream>
#include "build-your-own-http/controllers/HelloWorldController.h"
#include "build-your-own-http/http/HttpRequest.h"
#include "build-your-own-http/http/HttpResponse.h"

void HelloWorldController::handle(HttpRequest& httpRequest, HttpResponse& httpResponse) {
    std::cout << "Received request for hello world" << std::endl;
    httpResponse.setStatus(200, "OK");
    httpResponse.setBody("<html><body>Hello from server</body></html>");
}