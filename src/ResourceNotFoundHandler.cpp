#include "../include/ResourceNotFoundHandler.h"
#include "../include/HttpRequest.h"
#include "../include/HttpResponse.h"

void ResourceNotFoundHandler::handle(const HttpRequest&, HttpResponse& httpResponse) {
    httpResponse.setStatus(404, "Not Found");
    httpResponse.setBody("<html><body>Not Found</body></html>");
}