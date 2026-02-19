#include "Queen.h"
#include "Board.h"

Queen::Queen(Color c, Position p) : Piece(c, p)
{
}

PieceType Queen::getType() const
{
    return PieceType::Queen;
}

char Queen::getSymbol() const
{
    return (color == Color::White) ? 'Q' : 'q';
}

vector<Position> Queen::getLegalMoves(const Board &board) const
{
    static const int directions[8][2] = {
        {-1, 0}, {1, 0}, {0, -1}, {0, 1}, {-1, -1}, {-1, 1}, {1, -1}, {1, 1}};

    vector<Position> moves;
    int maxRange = Board::SIZE;

    for (const auto &dir : directions)
    {
        int r = position.row + dir[0];
        int c = position.col + dir[1];
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
                Piece *target = board.getPiece(next);
                if (target && target->getColor() != color)
                {
                    moves.push_back(next);
                }
                break;
            }

            r += dir[0];
            c += dir[1];
            steps++;
        }
    }

    return moves;
}