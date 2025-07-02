#include "build-your-own-http/handlers/MethodNotFoundHandler.h"
#include "build-your-own-http/http/HttpRequest.h"
#include "build-your-own-http/http/HttpResponse.h"

void MethodNotFoundHandler::handle(const HttpRequest&, HttpResponse& httpResponse) {
    httpResponse.setStatus(405, "Method Not Found");
    httpResponse.setBody("<html><body>Method Not Found</body></html>");
}