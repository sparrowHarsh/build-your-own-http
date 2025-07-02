#include <iostream>
#include "../include/HelloWorldController.h"
#include "../include/HttpRequest.h"
#include "../include/HttpResponse.h"

void HelloWorldController::handle(HttpRequest& httpRequest, HttpResponse& httpResponse) {
    std::cout << "Received request for hello world" << std::endl;
    httpResponse.setStatus(200, "OK");
    httpResponse.setBody("<html><body>Hello from server</body></html>");
}