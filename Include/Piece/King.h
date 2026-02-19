#ifndef KING_H
#define KING_H

#include "Piece.h"
#include "Board.h"
#include "Rook.h"

class King : public Piece
{
public:
    King(Color c, Position p);

    PieceType getType() const override;
    char getSymbol() const override;
    std::vector<Position> getLegalMoves(const Board &board) const override;
    bool isFirstMove() const;
    bool canCastle(const Board &board, bool kingSide) const;

    bool performCastling(const Board &board, bool kingSide, Position &rookFrom, Position &rookTo) const;
};

#endif