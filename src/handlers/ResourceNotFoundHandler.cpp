#include "build-your-own-http/handlers/ResourceNotFoundHandler.h"
#include "build-your-own-http/http/HttpRequest.h"
#include "build-your-own-http/http/HttpResponse.h"

void ResourceNotFoundHandler::handle(HttpRequest&, HttpResponse& httpResponse) {
    httpResponse.setStatus(404, "Not Found");
    httpResponse.setBody("<html><body>Not Found</body></html>");
}