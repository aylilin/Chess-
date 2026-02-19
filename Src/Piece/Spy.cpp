#include "Spy.h"

using namespace std;

Spy::Spy(Color c, Position p) : Piece(c, p), isDisguised(true), trueColor((c == Color::White) ? Color::Black : Color::White), revealTurn(0), hasBeenRevealed(false)
{
    srand(time(nullptr));
    revealTurn = 3 + (rand() % 5);
}

PieceType Spy::getType() const
{
    return PieceType::Pawn;
}

char Spy::getSymbol() const
{
    if (hasBeenRevealed)
    {
        return (trueColor == Color::White) ? 'S' : 's';
    }
    return (color == Color::White) ? 'P' : 'p';
}

vector<Position> Spy::getLegalMoves(const Board &board) const
{
    vector<Position> moves;

    if (hasBeenRevealed)
    {
        static const int directions[8][2] = {
            {-1, 0}, {1, 0}, {0, -1}, {0, 1}, {-1, -1}, {-1, 1}, {1, -1}, {1, 1}};

        for (const auto &dir : directions)
        {
            Position next{position.row + dir[0], position.col + dir[1]};

            if (!board.isInside(next))
                continue;

            Piece *target = board.getPiece(next);
            if (!target || target->getColor() != color)
            {
                moves.push_back(next);
            }
        }
        return moves;
    }

    int dir = (color == Color::White) ? -1 : 1;

    Position oneStep{position.row + dir, position.col};
    if (board.isInside(oneStep) && board.isEmpty(oneStep))
    {
        moves.push_back(oneStep);
    }

    if ((color == Color::White && position.row == 6) ||
        (color == Color::Black && position.row == 1))
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
            continue;

        Piece *target = board.getPiece(diag);
        if (target)
        {
            moves.push_back(diag);
        }
    }

    return moves;
}

bool Spy::canAttackAsSpy(const Board &board, const Position &target) const
{
    if (!board.isInside(target))
        return false;

    Piece *targetPiece = board.getPiece(target);
    if (!targetPiece)
    {
        return false;
    }

    return true;
}

void Spy::reveal()
{
    if (!hasBeenRevealed)
    {
        hasBeenRevealed = true;
        isDisguised = false;

        cout << endl;
        cout << "SPY REVEALED" << endl;
        cout << "The piece at " << char('a' + position.col) << 8 - position.row << " is actually a " << (trueColor == Color::White ? "WHITE" : "BLACK") << " spy!" << endl;
        cout << "The spy now moves like a queen!" << endl;
    }
}

void Spy::forceReveal()
{
    if (!hasBeenRevealed)
    {
        hasBeenRevealed = true;
        isDisguised = false;
    }
}