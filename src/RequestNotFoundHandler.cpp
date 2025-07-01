#include "../include/RequestNotFoundHandler.h"
#include "../include/HttpRequest.h"
#include "../include/HttpResponse.h"

void RequestNotFoundHandler::handle(const HttpRequest&, HttpResponse& httpResponse) {
    httpResponse.setStatus(404, "Not Found");
    httpResponse.setBody("<html><body>Not Found</body></html>");
}