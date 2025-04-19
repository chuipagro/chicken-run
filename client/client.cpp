#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>
#include <iostream>
#include <thread>
#include <mutex>
#include <sstream>

constexpr int WINDOW_WIDTH = 800;
constexpr int WINDOW_HEIGHT = 600;

sf::Font loadFont() {
    sf::Font font;
    if (!font.loadFromFile("arial.ttf")) {
        std::cerr << "Impossible de charger la police arial.ttf\n";
        exit(1);
    }
    return font;
}

std::vector<std::string> parseGameState(const std::string& raw) {
    std::vector<std::string> lines;
    std::istringstream iss(raw);
    std::string line;
    while (std::getline(iss, line)) {
        lines.push_back(line);
    }
    return lines;
}

int main() {
    sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Push Your Luck - Menu");
    sf::Font font = loadFont();

    enum class State { Menu, Game };
    State state = State::Menu;

    std::string ipInput = "";
    std::string portInput = "";
    bool typingIp = true;

    sf::TcpSocket socket;
    std::string gameState;
    std::mutex mutex;

    std::thread receiver;

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();

            if (state == State::Menu) {
                if (event.type == sf::Event::TextEntered) {
                    char c = static_cast<char>(event.text.unicode);
                    if (c == '\b') { // Backspace
                        if (typingIp && !ipInput.empty()) ipInput.pop_back();
                        else if (!typingIp && !portInput.empty()) portInput.pop_back();
                    } else if (c == '\t') {
                        typingIp = !typingIp;
                    } else if (c == '\r') { // Enter
                        unsigned short port = static_cast<unsigned short>(std::stoi(portInput));
                        if (socket.connect(ipInput, port) != sf::Socket::Done) {
                            std::cerr << "Connexion échouée\n";
                        } else {
                            std::cout << "Connecté à " << ipInput << ":" << port << "\n";
                            state = State::Game;

                            receiver = std::thread([&]() {
                                char buffer[1024];
                                while (true) {
                                    std::size_t received;
                                    if (socket.receive(buffer, sizeof(buffer), received) == sf::Socket::Done) {
                                        std::lock_guard<std::mutex> lock(mutex);
                                        gameState = std::string(buffer, received);
                                    }
                                }
                            });
                        }
                    } else if (isprint(c)) {
                        if (typingIp) ipInput += c;
                        else portInput += c;
                    }
                }
            }
        }

        window.clear(sf::Color::Black);

        if (state == State::Menu) {
            sf::Text title("Entrez IP et PORT (Tab pour changer, Entrée pour valider)", font, 24);
            title.setPosition(40, 30);
            window.draw(title);

            sf::Text ip("IP: " + ipInput, font, 28);
            ip.setPosition(100, 100);
            ip.setFillColor(typingIp ? sf::Color::Green : sf::Color::White);
            window.draw(ip);

            sf::Text port("PORT: " + portInput, font, 28);
            port.setPosition(100, 150);
            port.setFillColor(!typingIp ? sf::Color::Green : sf::Color::White);
            window.draw(port);
        }

        else if (state == State::Game) {
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::M)) {
                socket.send("move", 4);
                sf::sleep(sf::milliseconds(200));
            } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
                socket.send("stop", 4);
                sf::sleep(sf::milliseconds(200));
            }

            std::lock_guard<std::mutex> lock(mutex);
            auto lines = parseGameState(gameState);

            for (std::size_t i = 0; i < lines.size(); ++i) {
                sf::Text text(lines[i], font, 24);
                text.setPosition(50, 50 + i * 40);
                text.setFillColor(sf::Color::White);
                window.draw(text);
            }
        }

        window.display();
    }

    if (receiver.joinable()) receiver.detach();
    return 0;
}
