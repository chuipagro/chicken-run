/************************************************************
 * Author    : Pablo Levy
 * Created   : 4/19/25$
 * Project   : chicken-run$
 * File      : Player.cpp$
 * Description : 
 *     $USER_COMMENT$
 ************************************************************/

#include "Player.hpp"
#include <cstdlib>

Player::Player(int id, int mapLength, int baseRisk, int riskIncrement)
    : id(id) {
    generateMap(mapLength, baseRisk, riskIncrement);
}

void Player::generateMap(int length, int baseRisk, int riskIncrement) {
    for (int i = 0; i < length; ++i) {
        map.push_back(baseRisk + i * riskIncrement);
    }
}

int Player::getId() const { return id; }
int Player::getPosition() const { return position; }
bool Player::isAlive() const { return alive; }
bool Player::hasStopped() const { return stopped; }

std::string Player::getStatusSymbol() const {
    if (!alive) return "💥";
    if (stopped) return "🛑 at " + std::to_string(position);
    return "🐔 at " + std::to_string(position);
}

void Player::move() {
    if (!alive || stopped) return;

    position++;
    if (position >= (int)map.size()) {
        stopped = true;
        return;
    }

    int risk = map[position];
    int roll = rand() % 100;
    if (roll < risk) {
        alive = false;
    }
}

void Player::stop() {
    if (alive) stopped = true;
}

void Player::reset() {
    position = 0;
    alive = true;
    stopped = false;
}
