/************************************************************
 * Author    : Pablo Levy
 * Created   : 4/19/25$
 * Project   : chicken-run$
 * File      : Player.hpp$
 * Description : 
 *     $USER_COMMENT$
 ************************************************************/

#pragma once
#include <vector>
#include <string>

class Player {
public:
    Player(int id, int mapLength = 20, int baseRisk = 5, int riskIncrement = 5);

    int getId() const;
    int getPosition() const;
    bool isAlive() const;
    bool hasStopped() const;
    std::string getStatusSymbol() const;
    void move();
    void stop();
    void reset();

private:
    int id;
    int position = 0;
    bool alive = true;
    bool stopped = false;
    std::vector<int> map;

    void generateMap(int length, int baseRisk, int riskIncrement);
};
