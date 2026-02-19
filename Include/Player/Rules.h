#pragma once

#include <vector>
#include "GamePosition.h"
#include "Move.h"

// struct Move {
//     Position from;
//     Position to;
// };

class Rules {
public:
    // Core rule checking
    static bool isLegalMove(const GamePosition& pos,
                            const Position& from,
                            const Position& to);

    static void applyMove(GamePosition& pos,
                          const Position& from,
                          const Position& to);

    // State checks
    static bool isCheck(const GamePosition& pos, Color color);
    static bool isCheckmate(const GamePosition& pos, Color color);
    static bool isStalemate(const GamePosition& pos, Color color);

    // Move generation
    static std::vector<Move> generateLegalMoves(const GamePosition& pos,
                                                Color color);

private:
    // Helpers
    static Position findKing(const Board& board, Color color);
    static bool squareAttacked(const Board& board,
                               const Position& square,
                               Color byColor);

    static bool wouldBeInCheckAfterMove(const GamePosition& pos,
                                        const Position& from,
                                        const Position& to);
};
