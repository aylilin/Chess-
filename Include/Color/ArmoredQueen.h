#pragma once

#include "Piece.h"
#include "Board.h"

class ArmoredQueen : public Piece
{
private:
    int armor;

public:
    ArmoredQueen(Color c, Position p);
    PieceType getType() const override;
    char getSymbol() const override;
    vector<Position> getLegalMoves(const Board &board) const override;
    void takeHit();
    int getArmor() const { return armor; }
    bool hasArmor() const { return armor > 0; }
};