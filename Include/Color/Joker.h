#ifndef JOKER_H
#define JOKER_H

#include "Piece.h"
#include "Joker.h"
#include "Board.h"
#include "King.h"
#include "Queen.h"
#include "Rook.h"
#include "Bishop.h"
#include "Knight.h"
#include "Pawn.h"
#include <iostream>
#include <memory>

class Joker : public Piece
{
private:
    PieceType mimickedType;
    bool hasMimickedThisTurn;
    unique_ptr<Piece> mimickedPiece;

public:
    Joker(Color c, Position p);

    PieceType getType() const override;
    char getSymbol() const override;
    vector<Position> getLegalMoves(const Board &board) const override;

    bool mimicPiece(PieceType type, const Board &board);
    void setMimickedTypeDirect(PieceType type);
    void resetMimic();

    bool canMimicThisTurn() const
    {
        return !hasMimickedThisTurn;
    }

    void setMimickedThisTurn(bool mimicked)
    {
        hasMimickedThisTurn = mimicked;
    }

    PieceType getMimickedType() const
    {
        return mimickedType;
    }

    bool isMimicking() const
    {
        return mimickedType != PieceType::None;
    }

    bool hasSpecialAbility() const override
    {
        return true;
    }

    bool gethasMimickedThisTurn() const
    {
        return hasMimickedThisTurn;
    }

    void resetMimicForNewTurn()
    {
        hasMimickedThisTurn = false;
    }
};

#endif