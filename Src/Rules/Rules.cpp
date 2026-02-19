#include "Rules.h"
#include <algorithm>

// Helper: check if a position exists in a vector
static bool containsPosition(const vector<Position> &moves,
                             const Position &target)
{
    return find(moves.begin(), moves.end(), target) != moves.end();
}

// isLegalMove
bool Rules::isLegalMove(const GamePosition &pos,
                        const Position &from,
                        const Position &to)
{
    const Board &board = pos.getBoard();

    if (!board.isInside(from) || !board.isInside(to))
        return false;

    Piece *piece = board.getPiece(from);
    if (!piece)
        return false;

    if (piece->getColor() != pos.getSideToMove())
        return false;

    Piece *target = board.getPiece(to);
    if (target && target->getColor() == piece->getColor())
        return false;

    // Use getLegalMoves from Piece
    auto legalMoves = piece->getLegalMoves(board);
    if (!containsPosition(legalMoves, to))
        return false;

    // Check king safety
    if (wouldBeInCheckAfterMove(pos, from, to))
        return false;

    return true;
}

// applyMove
void Rules::applyMove(GamePosition &pos,
                      const Position &from,
                      const Position &to)
{
    if (!isLegalMove(pos, from, to))
        return;

    pos.getBoard().movePiece(from, to);
    pos.nextTurn();
}

// isCheck
bool Rules::isCheck(const GamePosition &pos, Color color)
{
    const Board &board = pos.getBoard();

    Position kingPos = findKing(board, color);

    Color opponent = (color == Color::White)
                         ? Color::Black
                         : Color::White;

    return squareAttacked(board, kingPos, opponent);
}

// isCheckmate
bool Rules::isCheckmate(const GamePosition &pos, Color color)
{
    if (!isCheck(pos, color))
        return false;

    return generateLegalMoves(pos, color).empty();
}

// isStalemate
bool Rules::isStalemate(const GamePosition &pos, Color color)
{
    if (isCheck(pos, color))
        return false;

    return generateLegalMoves(pos, color).empty();
}

// generateLegalMoves
vector<Move> Rules::generateLegalMoves(const GamePosition &pos, Color color)
{
    vector<Move> moves;
    const Board &board = pos.getBoard();

    for (int r = 0; r < Board::SIZE; ++r)
    {
        for (int c = 0; c < Board::SIZE; ++c)
        {

            Position from{r, c};
            Piece *piece = board.getPiece(from);

            if (!piece || piece->getColor() != color)
                continue;

            auto pieceMoves = piece->getLegalMoves(board);

            for (const Position &to : pieceMoves)
            {

                if (isLegalMove(pos, from, to))
                {
                    moves.push_back({from, to});
                }
            }
        }
    }

    return moves;
}

bool Rules::wouldBeInCheckAfterMove(const GamePosition &pos,
                                    const Position &from,
                                    const Position &to)
{
    GamePosition tempPos;
    pos.getBoard().copyTo(tempPos.getBoard());
    tempPos.setSideToMove(pos.getSideToMove());

    auto piece = tempPos.getBoard().removePiece(from);
    if (piece)
    {
        piece->setPosition(to);
        tempPos.getBoard().placePiece(move(piece));
    }

    return isCheck(tempPos, pos.getSideToMove());
}

// findKing
Position Rules::findKing(const Board &board, Color color)
{
    for (int r = 0; r < Board::SIZE; ++r)
    {
        for (int c = 0; c < Board::SIZE; ++c)
        {

            Position pos{r, c};
            Piece *piece = board.getPiece(pos);

            if (!piece)
                continue;

            if (piece->getColor() == color && piece->getType() == PieceType::King)
            {
                return pos;
            }
        }
    }

    return Position{-1, -1};
}

// squareAttacked
bool Rules::squareAttacked(const Board &board,
                           const Position &square,
                           Color byColor)
{
    for (int r = 0; r < Board::SIZE; ++r)
    {
        for (int c = 0; c < Board::SIZE; ++c)
        {

            Position from{r, c};
            Piece *piece = board.getPiece(from);

            if (!piece || piece->getColor() != byColor)
                continue;

            auto moves = piece->getLegalMoves(board);

            if (containsPosition(moves, square))
                return true;
        }
    }

    return false;
}