#pragma once

#include "PieceType.h"
#include "Position.h"
#include "Color.h"
#include <vector>
#include <memory>

class Board;

// Abstract base class for all pieces
class Piece
{
protected:
    Color color;
    Position position;
    bool hasMoved;

public:
    // Constructor / Destructor
    Piece(Color color, Position position);
    virtual ~Piece() = default;

    // Getters
    Color getColor() const;
    Position getPosition() const;
    bool getHasMoved() const
    {
        return hasMoved;
    }

    // Update position after a valid move
    void setPosition(Position newPos);
    void setHasMoved(bool moved)
    {
        hasMoved = moved;
    }

    // For printing (CLI, save/load)
    virtual char getSymbol() const = 0;
    virtual PieceType getType() const = 0;

    // Core rule: legal moves for this piece
    virtual std::vector<Position> getLegalMoves(const Board &board) const = 0;

    // Override only if a piece has special behavior
    virtual bool hasSpecialAbility() const;

    // Helper methods
    virtual bool isKing() const;
    
    virtual bool isPawn() const
    {
        return false;
    }

    virtual void onTurnStart() {}
    virtual void onTurnEnd() {}
};