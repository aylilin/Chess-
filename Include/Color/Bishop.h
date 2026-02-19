#pragma once

#include "Piece.h"
#include "Board.h"


class Bishop : public Piece
{
public:
    Bishop(Color c, Position p);
    PieceType getType() const override;
    std::vector<Position> getLegalMoves(const Board &board) const override;
    char getSymbol() const override;
};