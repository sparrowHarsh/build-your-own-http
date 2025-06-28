#include "../include/MethodNotFoundHandler.h"
#include "../include/HttpRequest.h"
#include "../include/HttpResponse.h"

void MethodNotFoundHandler::handle(const HttpRequest&, HttpResponse& httpResponse) {
    httpResponse.setStatus(405, "Method Not Found");
    httpResponse.setBody("<html><body>Method Not Found</body></html>");
}