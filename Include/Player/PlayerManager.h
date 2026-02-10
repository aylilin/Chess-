#ifndef PLAYERMANAGER_H
#define PLAYERMANAGER_H

#include "Player.h"

class PlayerManager final : public Player
{
public:
    PlayerManager(string name, Color color);
    virtual Move getmove(Board &board) override;
    void displayAvailabeMove();
    bool confirmMove();

private:
    Position readposition();
    bool validatePosition(const Position &pos, const Board &board);
    Piece *selectPiece(const Board &board);
};

#endif