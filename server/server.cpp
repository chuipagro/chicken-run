/************************************************************
* Author    : Pablo Levy
 * Created   : 4/19/25$
 * Project   : chicken-run$
 * File      : Server.cpp$
 * Description :
 *     $USER_COMMENT$
 ************************************************************/

#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

#include <iostream>
#include <vector>
#include <string>
#include <thread>
#include <cstring>
#include <unordered_map>

#include "env.hpp"
#include "Player.hpp"

constexpr int MAX_CLIENTS = 4;

std::vector<int> clients;
std::vector<Player> players;

void broadcastState() {
    std::string state;
    for (const auto& player : players) {
        state += "P" + std::to_string(player.getId()) + ": " + player.getStatusSymbol() + "\n";
    }

    for (int client : clients) {
        send(client, state.c_str(), state.size(), 0);
    }
}

// Gère les commandes reçues d'un client
void handleClient(int client_fd, int playerId) {
    char buffer[1024];
    Player& p = players[playerId];

    while (true) {
        std::memset(buffer, 0, 1024);
        int bytes = recv(client_fd, buffer, 1024, 0);
        if (bytes <= 0) break;

        std::string command(buffer);

        if (command == "move") {
            p.move();
        } else if (command == "stop") {
            p.stop();
        }

        broadcastState();
    }

    close(client_fd);
}

int main() {
    auto env = loadEnv();
    std::string ipStr = env["SERVER_IP"];
    int port = std::stoi(env["SERVER_PORT"]);

    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == -1) {
        perror("socket creation failed");
        return 1;
    }

    sockaddr_in address{};
    address.sin_family = AF_INET;
    inet_pton(AF_INET, ipStr.c_str(), &address.sin_addr);
    address.sin_port = htons(port);

    if (bind(server_fd, (struct sockaddr*)&address, sizeof(address)) < 0) {
        perror("bind failed");
        return 1;
    }

    if (listen(server_fd, MAX_CLIENTS) < 0) {
        perror("listen failed");
        return 1;
    }

    std::cout << "Serveur en écoute sur " << ipStr << ":" << port << "\n";

    while (clients.size() < MAX_CLIENTS) {
        sockaddr_in client_addr{};
        socklen_t client_len = sizeof(client_addr);
        int client_fd = accept(server_fd, (struct sockaddr*)&client_addr, &client_len);
        if (client_fd < 0) continue;

        int id = clients.size();
        clients.push_back(client_fd);
        players.emplace_back(id);
        std::thread(handleClient, client_fd, id).detach();
        broadcastState();
    }

    close(server_fd);
    return 0;
}
