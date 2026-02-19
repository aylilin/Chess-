#pragma once

#include "Piece.h"
#include "Move.h"
#include "Board.h"

class Board;

class Pawn : public Piece
{
public:
    Pawn(Color c, Position p);
    PieceType getType() const override;
    vector<Position> getLegalMoves(const Board &board) const override;
    char getSymbol() const override;
    bool canEnPassant(const Board &board, const Position &target) const;
    vector<Position> getEnPassantMoves(const Board &board, const Move &lastMove) const;
    void setJustDoubleMoved(bool value);
    bool getJustDoubleMoved() const;
    Position getEnPassantTarget() const;
    void setEnPassantTarget(const Position &target);

private:
    bool isFirstMove() const;
    Position enPassantTarget;
    bool justDoubleMoved;
};