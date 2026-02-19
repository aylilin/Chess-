#include "Piece.h"

// Constructor
Piece::Piece(Color c, Position p) : color(c), position(p) {}

// Getters
Color Piece::getColor() const
{
    return color;
}

Position Piece::getPosition() const
{
    return position;
}

bool Piece::hasSpecialAbility() const
{
    return false;
}

bool Piece::isKing() const
{
    return false;
}

// Update position
void Piece::setPosition(Position newPos)
{
    position = newPos;
}