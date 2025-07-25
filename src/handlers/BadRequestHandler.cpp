#include "build-your-own-http/handlers/BadRequestHandler.h"
#include "build-your-own-http/http/HttpRequest.h"
#include "build-your-own-http/http/HttpResponse.h"

void BadRequestHandler::handle(HttpRequest&, HttpResponse& httpResponse) {
    httpResponse.setStatus(400, "Bad Request");
    httpResponse.setBody("<html><body>Bad Request</body></html>");
}