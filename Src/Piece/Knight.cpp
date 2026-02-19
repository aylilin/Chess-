#include "Knight.h"
#include "Board.h"

Knight::Knight(Color c, Position p) : Piece(c, p) {}

PieceType Knight::getType() const
{
    return PieceType::Knight;
}


char Knight::getSymbol() const {
    return (color == Color::White) ? 'N' : 'n';
}

std::vector<Position> Knight::getLegalMoves(const Board& board) const {
    static const int offsets[8][2] = {{-2, -1}, {-2, 1}, {-1, -2}, {-1, 2}, {1, -2}, {1, 2}, {2, -1}, {2, 1}};

    std::vector<Position> moves;

    for (const auto& off : offsets) {
        Position next {
            position.row + off[0],
            position.col + off[1]
        };

        if (!board.isInside(next)){
            continue;
        }

        Piece* target = board.getPiece(next);

        if (!target || target->getColor() != color) {
            moves.push_back(next);
        }
    }
    return moves;
}