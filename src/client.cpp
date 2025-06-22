#include <iostream>
#include <string>
#include <cstring>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <thread>
#include <vector>

void run_client(int client_id) {
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        std::cerr << "[Client " << client_id << "] Socket creation error\n";
        return;
    }

    struct sockaddr_in serv_addr;
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(8080);

    if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) {
        std::cerr << "[Client " << client_id << "] Invalid address/ Address not supported\n";
        close(sock);
        return;
    }

    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        std::cerr << "[Client " << client_id << "] Connection Failed\n";
        close(sock);
        return;
    }

    std::string message = "Hello from client " + std::to_string(client_id) + "!";
    send(sock, message.c_str(), message.size(), 0);

    std::cout << "[Client " << client_id << "] Message sent to server: " << message << std::endl;

    char buffer[4096] = {0};
    int bytesReceived = recv(sock, buffer, sizeof(buffer) - 1, 0);
    if (bytesReceived > 0) {
        std::cout << "[Client " << client_id << "] Received from server: " << buffer << std::endl;
    } else {
        std::cout << "[Client " << client_id << "] No response received from server." << std::endl;
    }

    close(sock);
}

int main() {
    int num_clients = 5; // Number of clients to simulate
    std::vector<std::thread> threads;
    for (int i = 1; i <= num_clients; ++i) {
        threads.emplace_back(run_client, i);
    }
    for (auto& t : threads) {
        t.join();
    }
    return 0;
}