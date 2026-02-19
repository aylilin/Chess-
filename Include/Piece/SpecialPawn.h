#pragma once

#include "Piece.h"
#include "Board.h"

using namespace std;

class SpecialPawn : public Piece
{
private:
    bool isDisguised;

public:
    SpecialPawn(Color c, Position p);
    PieceType getType() const override;
    char getSymbol() const override;
    vector<Position> getLegalMoves(const Board &board) const override;
    bool canThreatenFront() const;
};