// client.cpp
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include <iostream>
#include <string>
#include <thread>
#include <cstring>

constexpr int PORT = 12345;

void listenServer(int sock) {
    char buffer[1024];
    while (true) {
        std::memset(buffer, 0, 1024);
        int bytes = recv(sock, buffer, 1024, 0);
        if (bytes <= 0) break;
        std::cout << "\n" << buffer << std::endl;
    }
}

int main() {
    std::string server_ip;
    std::cout << "IP du serveur : ";
    std::cin >> server_ip;

    int sock = socket(AF_INET, SOCK_STREAM, 0);
    sockaddr_in server_addr{};
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    inet_pton(AF_INET, server_ip.c_str(), &server_addr.sin_addr);

    if (connect(sock, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        std::cerr << "Connexion échouée\n";
        return 1;
    }

    std::thread(listenServer, sock).detach();

    std::string input;
    while (true) {
        std::cout << "[a] avancer, [s] stop : ";
        std::cin >> input;
        std::string command = (input == "a") ? "move" : (input == "s") ? "stop" : "";
        if (!command.empty()) {
            send(sock, command.c_str(), command.size(), 0);
        }
    }

    close(sock);
    return 0;
}
