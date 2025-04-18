// server.cpp
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include <iostream>
#include <vector>
#include <string>
#include <cstring>
#include <thread>

constexpr int PORT = 12345;
constexpr int MAX_CLIENTS = 4;

struct Player {
    int id;
    int position = 0;
    bool alive = true;
    bool stopped = false;
};

std::vector<int> clients;
std::vector<Player> players;

void broadcastState() {
    std::string state;
    for (const auto& p : players) {
        state += "P" + std::to_string(p.id) + ": ";
        if (!p.alive) state += "💥\n";
        else if (p.stopped) state += "🛑 at " + std::to_string(p.position) + "\n";
        else state += "🐔 at " + std::to_string(p.position) + "\n";
    }

    for (int client : clients) {
        send(client, state.c_str(), state.size(), 0);
    }
}

void handleClient(int client_fd, int id) {
    char buffer[1024];
    while (true) {
        std::memset(buffer, 0, 1024);
        int bytes = recv(client_fd, buffer, 1024, 0);
        if (bytes <= 0) break;

        std::string command(buffer);
        Player& p = players[id];

        if (command == "move" && p.alive && !p.stopped) {
            p.position++;
            float risk = p.position * 0.05f;
            if ((float)rand() / RAND_MAX < risk) {
                p.alive = false;
            }
        } else if (command == "stop" && p.alive && !p.stopped) {
            p.stopped = true;
        }

        broadcastState();
    }

    close(client_fd);
}

int main() {
    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    sockaddr_in address{};
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    bind(server_fd, (struct sockaddr*)&address, sizeof(address));
    listen(server_fd, MAX_CLIENTS);

    std::cout << "Serveur en écoute sur le port " << PORT << "\n";

    while (clients.size() < MAX_CLIENTS) {
        sockaddr_in client_addr{};
        socklen_t client_len = sizeof(client_addr);
        int client_fd = accept(server_fd, (struct sockaddr*)&client_addr, &client_len);
        if (client_fd < 0) continue;

        int id = clients.size();
        clients.push_back(client_fd);
        players.push_back({id});

        std::thread(handleClient, client_fd, id).detach();
        broadcastState();
    }

    close(server_fd);
    return 0;
}
