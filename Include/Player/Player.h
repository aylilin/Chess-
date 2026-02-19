#ifndef PLAYER_H
#define PLAYER_H

#include <string>
#include "Color.h"
#include "PlayerType.h"
#include "Move.h"
#include "Board.h"

using namespace std;

class Player
{
public:
    Player();
    Player(string name, Color color);
    virtual ~Player();

    string getName() const;
    void setName(string name);
    Color getColor() const;
    void setColor(Color color);
    int getScore() const;
    void addScore(int points);
    bool isActive() const;
    void setActive(bool active);

    virtual Move getMove(const Board &board) = 0;

protected:
    string name;
    Color color;
    int score;
    bool active;
};

#endif