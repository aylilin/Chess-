#pragma once

#include <vector>
#include <memory>
#include <functional>
#include <iostream>
#include <algorithm>
#include "Position.h"
#include "Color.h"
#include "PieceType.h"
#include"Piece.h"
#include "King.h"
#include "Queen.h"
#include "Rook.h"
#include "Bishop.h"
#include "Knight.h"
#include "Pawn.h"
#include "ArmoredQueen.h"
#include "SpecialPawn.h"
#include "Spy.h"
#include "Joker.h"

class Board
{
public:
    static constexpr int SIZE = 8;

private:
    std::vector<std::vector<std::unique_ptr<Piece>>> grid;

public:
    Board();

    // Queries
    bool isInside(const Position &pos) const;
    bool isEmpty(const Position &pos) const;
    Piece *getPiece(const Position &pos) const;

    // Modifiers
    void placePiece(std::unique_ptr<Piece> piece);
    std::unique_ptr<Piece> removePiece(const Position &pos);
    void movePiece(const Position &from, const Position &to);

    // Utilities
    void clear();
    void forEachPiece(const std::function<void(const Piece &)> &func) const;
    void print() const;
    void copyTo(Board &other) const;

    // Setup
    void setupStandardPosition();
    void setupMissionModePosition();

    // Count pieces
    int countPieces(Color color) const;
    bool hasPieceAt(const Position &pos, Color color, PieceType type) const;

    // Find king position
    Position findKing(Color color) const;
};