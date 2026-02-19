#ifndef SPY_H
#define SPY_H

#include "Piece.h"
#include "Board.h"
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <memory>

class Spy : public Piece
{
private:
    bool isDisguised;
    Color trueColor;
    int revealTurn;
    bool hasBeenRevealed;

public:
    Spy(Color c, Position p);

    PieceType getType() const override;
    char getSymbol() const override;
    void forceReveal();
    vector<Position> getLegalMoves(const Board &board) const override;

    void reveal();

    bool isRevealed() const
    {
        return hasBeenRevealed;
    }

    Color getTrueColor() const
    {
        return trueColor;
    }

    bool canAttackAsSpy(const Board &board, const Position &target) const;

    bool hasSpecialAbility() const override
    {
        return true;
    }

    void setRevealed(bool revealed)
    {
        hasBeenRevealed = revealed;
    }

    void setDisguised(bool disguised)
    {
        isDisguised = disguised;
    }
};

#endif