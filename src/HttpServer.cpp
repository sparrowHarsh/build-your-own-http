class HttpServer {
    private:
        int port;
        int serverSocket;
        bool isRunning;

    public:
        HttpServer(int port);
        ~HttpServer();
        bool start();
        void stop();
        bool isRunning();
        int getPort(); 

        void accept_connections();
        void handleRequest(int clientSocket);
        void sendResponse(int clientSocket, const std::string& response);
        void registerHandler(const HttpMethod method, const std::string& path, RequestHandler* handler);
};


HttpServer :: HttpServer(int port) : port(port), serverSocket(-1), isRunning(false) {
    std::cout << "HttpServer created on port " << port << std::endl;
}

HttpServer :: start() {
    /*Creating socket to establish a end to end connection between client and server*/
    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket < 0) {
        perror("Failed to create socket");
        return false;
    }

    struct sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(port);

    if (bind(serverSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0) {
        perror("Failed to bind socket");
        close(serverSocket);
        return false;
    }

    if (listen(serverSocket, 5) < 0) {
        perror("Failed to listen on socket");
        close(serverSocket);
        return false;
    }

    isRunning = true;

    std::cout << "HttpServer started on port " << port << std::endl; 
    // accepting connections in a separate thread would be ideal here 
    accept_connections();
    return true;
}

void HttpServer :: stop() {
    if (isRunning) {
        close(serverSocket);
        isRunning = false;
        std::cout << "HttpServer stopped" << std::endl;
    }
}

void HttpServer :: accept_connections() {
    
}