#ifndef GETPLAYER_H
#define GETPLAYER_H

#include "Player.h"
#include <iostream>
#include <limits>

using namespace std;

class GetPlayer : public Player
{
public:
    GetPlayer(string name, Color color);
    Move getMove(const Board &board) override;
};

#endif