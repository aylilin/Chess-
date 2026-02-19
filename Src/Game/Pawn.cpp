#include "Pawn.h"
#include "Board.h"
#include <iostream>

using namespace std;

Pawn::Pawn(Color c, Position p) : Piece(c, p), justDoubleMoved(false), enPassantTarget(-1, -1) {}

char Pawn::getSymbol() const
{
    return (color == Color::White) ? 'P' : 'p';
}

PieceType Pawn::getType() const
{
    return PieceType::Pawn;
}

bool Pawn::isFirstMove() const
{
    return !hasMoved;
}

vector<Position> Pawn::getLegalMoves(const Board &board) const
{
    vector<Position> moves;
    int dir = (color == Color::White) ? -1 : 1;

    Position oneStep{position.row + dir, position.col};
    if (board.isInside(oneStep) && board.isEmpty(oneStep))
    {
        moves.push_back(oneStep);

        Position twoStep{position.row + 2 * dir, position.col};
        if (isFirstMove() && board.isInside(twoStep) && board.isEmpty(twoStep))
        {
            moves.push_back(twoStep);
        }
    }

    for (int dc : {-1, 1})
    {
        Position diag{position.row + dir, position.col + dc};

        if (!board.isInside(diag))
        {
            continue;
        }

        Piece *target = board.getPiece(diag);
        if (target && target->getColor() != color)
        {
            moves.push_back(diag);
        }
    }

    for (int dc : {-1, 1})
    {
        Position enemyPos{position.row, position.col + dc};

        if (!board.isInside(enemyPos))
        {
            continue;
        }

        Piece *enemyPawn = board.getPiece(enemyPos);

        if (enemyPawn && enemyPawn->getType() == PieceType::Pawn && enemyPawn->getColor() != color)
        {
            Pawn *pawn = dynamic_cast<Pawn *>(enemyPawn);
            if (pawn && pawn->getJustDoubleMoved())
            {
                Position enPassantCapture{position.row + dir, position.col + dc};

                Position enPassantMove{position.row + dir, position.col + dc};

                if (board.isInside(enPassantMove) && board.isEmpty(enPassantMove))
                {
                    Pawn *thisPawn = const_cast<Pawn *>(this);
                    thisPawn->setEnPassantTarget(enPassantMove);
                    moves.push_back(enPassantMove);
                }
            }
        }
    }

    return moves;
}

void Pawn::setJustDoubleMoved(bool value)
{
    justDoubleMoved = value;
}

bool Pawn::getJustDoubleMoved() const
{
    return justDoubleMoved;
}

Position Pawn::getEnPassantTarget() const
{
    return enPassantTarget;
}

void Pawn::setEnPassantTarget(const Position &target)
{
    enPassantTarget = target;
}