#ifndef PLAYER_H
#define PLAYER_H

#include <string>
#include "PlayerType.h"

using namespace std;

class Move;
class Board;
class Position;
class Piece;

class Player
{
public:
    Player(string name, Color color);
    virtual ~Player();
    string getname() const;
    string getcolr() const;
    int getscore() const;
    void addscore();
    bool Isactive() const;
    void setactive();
    virtual Move getmove(Board &board) = 0;

protected:
    string name;
    string color;
    int score;
    bool active;
};

#endif