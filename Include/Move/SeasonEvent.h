#pragma once
#include "Board.h"
#include "Piece.h"
#include <string>
#include <map>

enum class Season
{
    Spring,
    Summer,
    Autumn,
    Winter
};

struct PieceMoveInfo
{
    int lastMoveTurn;
    bool canMoveThisTurn;
};

class SeasonEvent
{
public:
    SeasonEvent();
    
    void activateNextSeason(int currentTurn);
    Season getCurrentSeason() const { return currentSeason; }
    std::string getSeasonName() const;
    
    void applySeasonEffects(Board &board);
    void removeSeasonEffects(Board &board);
    
    bool canPieceMoveInSeason(const Piece *piece, const Position &from, const Position &to, int currentTurn) const;
    void registerPieceMove(const Piece *piece, int currentTurn);
    void resetForNewSeason();

private:
    Season currentSeason;
    int turnCounter;
    const int SEASON_CYCLE = 10;
    
    mutable std::map<std::string, PieceMoveInfo> pieceMoveInfo;
    
    bool isPawnRestricted() const;
    bool isBishopRestricted() const;
    bool isKnightRestricted() const;
    bool isRookRestricted() const;
    
    void nextSeason();
    std::string getPieceKey(const Piece *piece) const;
};