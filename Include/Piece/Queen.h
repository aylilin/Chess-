#ifndef QUEEN_H
#define QUEEN_H

#include "Piece.h"
#include "Board.h"

using namespace std;

class Queen : public Piece
{
public:
    Queen(Color color, Position position);
    PieceType getType() const override;
    char getSymbol() const override;
    vector<Position> getLegalMoves(const Board &board) const override;
};

#endif