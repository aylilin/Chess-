#include "Joker.h"
#include "Board.h"
#include "King.h"
#include "Queen.h"
#include "Rook.h"
#include "Bishop.h"
#include "Knight.h"
#include "Pawn.h"
#include <iostream>

using namespace std;

Joker::Joker(Color c, Position p) : Piece(c, p), mimickedType(PieceType::None), hasMimickedThisTurn(false)
{
}

PieceType Joker::getType() const
{
    return PieceType::Pawn;
}

char Joker::getSymbol() const
{
    if (isMimicking())
    {
        switch (mimickedType)
        {
        case PieceType::King:
            return (color == Color::White) ? 'K' : 'k';
        case PieceType::Queen:
            return (color == Color::White) ? 'Q' : 'q';
        case PieceType::Rook:
            return (color == Color::White) ? 'R' : 'r';
        case PieceType::Bishop:
            return (color == Color::White) ? 'B' : 'b';
        case PieceType::Knight:
            return (color == Color::White) ? 'N' : 'n';
        case PieceType::Pawn:
            return (color == Color::White) ? 'P' : 'p';
        default:
            break;
        }
    }
    return (color == Color::White) ? 'J' : 'j';
}

bool Joker::mimicPiece(PieceType type, const Board &board)
{
    if (hasMimickedThisTurn)
    {
        cout << "Joker has already mimicked this turn!" << endl;
        return false;
    }

    if (type == PieceType::King)
    {
        cout << "Joker cannot mimic the King!" << endl;
        return false;
    }

    switch (type)
    {
    case PieceType::Queen:
        mimickedPiece = make_unique<Queen>(color, position);
        break;
    case PieceType::Rook:
        mimickedPiece = make_unique<Rook>(color, position);
        break;
    case PieceType::Bishop:
        mimickedPiece = make_unique<Bishop>(color, position);
        break;
    case PieceType::Knight:
        mimickedPiece = make_unique<Knight>(color, position);
        break;
    case PieceType::Pawn:
        mimickedPiece = make_unique<Pawn>(color, position);
        break;
    default:
        return false;
    }

    mimickedType = type;
    hasMimickedThisTurn = true;

    cout << "Joker is now mimicking a ";
    switch (type)
    {
    case PieceType::Queen:
        cout << "Queen";
        break;
    case PieceType::Rook:
        cout << "Rook";
        break;
    case PieceType::Bishop:
        cout << "Bishop";
        break;
    case PieceType::Knight:
        cout << "Knight";
        break;
    case PieceType::Pawn:
        cout << "Pawn";
        break;
    default:
        break;
    }
    cout << "!" << endl;

    return true;
}

void Joker::resetMimic()
{
    mimickedType = PieceType::None;
    mimickedPiece.reset();
    hasMimickedThisTurn = false;
}

void Joker::setMimickedTypeDirect(PieceType type)
{
    mimickedType = type;
    hasMimickedThisTurn = true;
}

vector<Position> Joker::getLegalMoves(const Board &board) const
{
    if (isMimicking() && mimickedPiece)
    {
        return mimickedPiece->getLegalMoves(board);
    }

    vector<Position> moves;
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
        if (target && target->getColor() != color)
        {
            moves.push_back(diag);
        }
    }

    return moves;
}