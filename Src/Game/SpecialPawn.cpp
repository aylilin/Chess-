#include "SpecialPawn.h"
#include "Board.h"

SpecialPawn::SpecialPawn(Color c, Position p) : Piece(c, p), isDisguised(true) {}

PieceType SpecialPawn::getType() const
{
    return PieceType::Pawn;
}

char SpecialPawn::getSymbol() const
{
    return (color == Color::White) ? 'P' : 'p';
}

vector<Position> SpecialPawn::getLegalMoves(const Board &board) const
{
    vector<Position> moves;
    int dir = (color == Color::White) ? -1 : 1;

    Position oneStep{position.row + dir, position.col};
    if (board.isInside(oneStep) && board.isEmpty(oneStep))
    {
        moves.push_back(oneStep);
    }

    if ((color == Color::White && position.row == 6) || (color == Color::Black && position.row == 1))
    {
        Position twoStep{position.row + 2 * dir, position.col};
        if (board.isInside(twoStep) && board.isEmpty(twoStep) && board.isEmpty(oneStep))
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

    Position front{position.row + dir, position.col};
    if (board.isInside(front) && board.getPiece(front) && board.getPiece(front)->getColor() != color)
    {
        moves.push_back(front);
    }

    return moves;
}

bool SpecialPawn::canThreatenFront() const
{
    return true;
}