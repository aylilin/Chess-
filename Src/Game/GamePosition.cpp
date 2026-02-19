#include "GamePosition.h"

Board& GamePosition::getBoard() {
    return board;
}

const Board& GamePosition::getBoard() const {
    return board;
}

Color GamePosition::getSideToMove() const {
    return sideToMove;
}

void GamePosition::setSideToMove(Color color) {
    sideToMove = color;
}

void GamePosition::nextTurn() {
    sideToMove = (sideToMove == Color::White) ? Color::Black : Color::White;
}

bool GamePosition::isLegalMove(const Position& from, const Position& to) const {
    return Rules::isLegalMove(*this, from, to);
}

void GamePosition::applyMove(const Position& from, const Position& to) {
    if (!isLegalMove(from, to)) return;
    board.movePiece(from, to);
    nextTurn();
}

bool GamePosition::isCheck(Color color) const {
    return Rules::isCheck(*this, color);
}

bool GamePosition::isCheckmate(Color color) const {
    return Rules::isCheckmate(*this, color);
}

void GamePosition::clear() {
    board.clear();
    sideToMove = Color::White;
}