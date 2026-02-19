#include "Rook.h"
#include "Board.h"

Rook::Rook(Color c, Position p) : Piece(c, p) {}

PieceType Rook::getType() const
{
    return PieceType::Rook;
}

char Rook::getSymbol() const
{
    return (color == Color::White) ? 'R' : 'r';
}

std::vector<Position> Rook::getLegalMoves(const Board &board) const
{
    static const int directions[4][2] = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};

    std::vector<Position> moves;

    int maxRange = Board::SIZE;

    for (auto &d : directions)
    {
        int r = position.row + d[0];
        int c = position.col + d[1];
        int steps = 0;

        while (steps < maxRange)
        {
            Position next{r, c};

            if (!board.isInside(next))
            {
                break;
            }

            if (board.isEmpty(next))
            {
                moves.push_back(next);
            }
            else
            {
                Piece *p = board.getPiece(next);
                if (p && p->getColor() != color)
                {
                    moves.push_back(next);
                }
                break;
            }
            r += d[0];
            c += d[1];
            steps++;
        }
    }
    return moves;
}