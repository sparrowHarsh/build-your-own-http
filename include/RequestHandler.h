

class RequestHandler {
    public:
        virtual void handle(const HttpRequest& httpRequest, HttpResponse& httpResponse) = 0;
        virtual ~RequestHandler() = default;
};