#include "Bishop.h"
#include "Board.h"

PieceType Bishop::getType() const
{
    return PieceType::Bishop;
}

Bishop::Bishop(Color c, Position p) : Piece(c, p) {}

char Bishop::getSymbol() const
{
    return (color == Color::White) ? 'B' : 'b';
}

std::vector<Position> Bishop::getLegalMoves(const Board &board) const
{
    static const int directions[4][2] = {{-1, -1}, {-1, 1}, {1, -1}, {1, 1}};

    std::vector<Position> moves;

    for (auto &d : directions)
    {
        int r = position.row + d[0];
        int c = position.col + d[1];

        while (true)
        {
            Position next{r, c};

            if (!board.isInside(next))
                break;

            if (board.isEmpty(next))
            {
                moves.push_back(next);
            }
            else
            {
                Piece *p = board.getPiece(next);
                if (p && p->getColor() != color)
                    moves.push_back(next);
                break;
            }

            r += d[0];
            c += d[1];
        }
    }
    return moves;
}