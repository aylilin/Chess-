#pragma once

#include "Position.h"
#include "Board.h"
#include <map>

struct BombInfo
{
    Position pos;
    int movesWithoutMoving;
    PieceType type;
    Color color;
};

class TimeBomb
{
public:
    TimeBomb(int threshold = 5);
    
    void update(Board &board);
    void registerPiece(const Piece *piece);
    void pieceMoved(const Position &from, const Position &to);
    
private:
    std::map<std::string, BombInfo> bombMap;
    int explosionThreshold;
    
    void explode(Board &board, const Position &pos);
    std::string posToString(const Position &pos) const;
    void destroySurrounding(Board &board, const Position &center);
};