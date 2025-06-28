
class MethodNotFoundHandler: public RequestHandler {
    public:
        void handle(const HttpRequest& httpRequest, HttpResponse& httpResponse) override;
        ~MethodNotFoundHandler() override = default;
};