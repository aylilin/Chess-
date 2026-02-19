#include "ArmoredQueen.h"
#include "Board.h"

ArmoredQueen::ArmoredQueen(Color c, Position p) : Piece(c, p), armor(2) {}

PieceType ArmoredQueen::getType() const
{
    return PieceType::Queen;
}

char ArmoredQueen::getSymbol() const
{
    if (armor == 2)
    {
        return (color == Color::White) ? 'Q' : 'q';
    }
    if (armor == 1)
    {
        return (color == Color::White) ? 'Q' : 'q';
    }
    return (color == Color::White) ? 'Q' : 'q';
}

std::vector<Position> ArmoredQueen::getLegalMoves(const Board &board) const
{
    // مثل وزیر عادی
    static const int directions[8][2] = {
        {-1, 0}, {1, 0}, {0, -1}, {0, 1}, {-1, -1}, {-1, 1}, {1, -1}, {1, 1}};

    std::vector<Position> moves;

    for (const auto &dir : directions)
    {
        int r = position.row + dir[0];
        int c = position.col + dir[1];

        while (board.isInside(Position(r, c)))
        {
            Position next{r, c};

            if (board.isEmpty(next))
            {
                moves.push_back(next);
            }
            else
            {
                Piece *target = board.getPiece(next);
                if (target && target->getColor() != color)
                {
                    moves.push_back(next);
                }
                break;
            }

            r += dir[0];
            c += dir[1];
        }
    }

    return moves;
}

void ArmoredQueen::takeHit()
{
    if (armor > 0)
    {
        armor--;
    }
}