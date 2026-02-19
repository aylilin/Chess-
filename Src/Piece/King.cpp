#include "King.h"
#include "Board.h"
#include "Rook.h"

King::King(Color c, Position p) : Piece(c, p)
{
}

PieceType King::getType() const
{
    return PieceType::King;
}

char King::getSymbol() const
{
    return (color == Color::White) ? 'K' : 'k';
}

bool King::isFirstMove() const
{
    return !hasMoved;
}

vector<Position> King::getLegalMoves(const Board &board) const
{
    static const int directions[8][2] = {{-1, -1}, {-1, 0}, {-1, 1}, {0, -1}, {0, 1}, {1, -1}, {1, 0}, {1, 1}};

    vector<Position> moves;

    for (const auto &dir : directions)
    {
        Position next{position.row + dir[0], position.col + dir[1]};

        if (!board.isInside(next))
        {
            continue;
        }

        Piece *target = board.getPiece(next);

        if (!target || target->getColor() != color)
        {
            moves.push_back(next);
        }
    }

    if (!hasMoved)
    {
        if (canCastle(board, true))
        {
            Position castlePos{position.row, position.col + 2};
            moves.push_back(castlePos);
        }

        if (canCastle(board, false))
        {
            Position castlePos{position.row, position.col - 2};
            moves.push_back(castlePos);
        }
    }

    return moves;
}

bool King::canCastle(const Board &board, bool kingSide) const
{
    if (hasMoved)
    {
        return false;
    }

    int rookCol = kingSide ? 7 : 0;
    Position rookPos(position.row, rookCol);
    Piece *rook = board.getPiece(rookPos);

    if (!rook || rook->getType() != PieceType::Rook || rook->getHasMoved())
    {
        return false;
    }

    int step = kingSide ? 1 : -1;
    for (int c = position.col + step; c != rookCol; c += step)
    {
        if (!board.isEmpty(Position(position.row, c)))
        {
            return false;
        }
    }

    return true;
}

bool King::performCastling(const Board &board, bool kingSide, Position &rookFrom, Position &rookTo) const
{
    if (!canCastle(board, kingSide))
    {
        return false;
    }

    rookFrom = Position(position.row, kingSide ? 7 : 0);
    rookTo = Position(position.row, kingSide ? position.col + 1 : position.col - 1);

    return true;
}