#pragma once

#include "Board.h"
#include <algorithm>
#include "Rules.h"

class GamePosition {
private:
    Board board;
    Color sideToMove = Color::White;

public:
    GamePosition() = default;
    Board& getBoard();
    const Board& getBoard() const;
    Color getSideToMove() const;
    void setSideToMove(Color color);
    void nextTurn();
    bool isLegalMove(const Position& from, const Position& to) const;
    void applyMove(const Position& from, const Position& to);
    bool isCheck(Color color) const;
    bool isCheckmate(Color color) const;
    void clear();
};