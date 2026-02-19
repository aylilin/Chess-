#ifndef GAMEMODEMANAGER_H
#define GAMEMODEMANAGER_H

#include "GameMode.h"
#include "SeasonEvent.h"
#include "TimeBomb.h"
#include "MassacreAbility.h"
#include "Mission.h"
#include <memory>
#include <string>

class GameModeManager
{
public:
    GameModeManager();

    void setMode(GameMode mode);
    GameMode getMode() const;

    SeasonEvent *getSeasonEvent();
    const SeasonEvent *getSeasonEvent() const;

    TimeBomb *getTimeBomb();
    const TimeBomb *getTimeBomb() const;
    
    const MassacreAbility *getMassacreAbility() const;

    MissionManager *getMissionManager();
    const MissionManager *getMissionManager() const;

    void updateGame(Board &board, Color currentPlayer, int currentTurn);

    void onMoveMade(const Position &from, const Position &to, Board &board, int currentTurn);
    void onPieceCaptured(PieceType type, Color color, int currentTurn);

    bool canPieceMove(const Piece *piece, const Position &from, const Position &to, int currentTurn) const;

    void generateMissions(Color playerColor, int turnNumber);
    void updateMissions(Board &board, Color playerColor, const Move &lastMove);
   string getMissionDescription(Color playerColor) const;
    bool isMissionCompleted(Color playerColor) const;
    int getRemainingMoves(Color playerColor) const;

    int getEnergyForMove(PieceType pieceType) const;

    bool canUseDoubleMove(Color playerColor) const;
    void useDoubleMove(Color playerColor);

    void resetAll();

private:
    GameMode currentMode;
   unique_ptr<SeasonEvent> seasonEvent;
   unique_ptr<TimeBomb> timeBomb;
   unique_ptr<MassacreAbility> massacreAbility;
   unique_ptr<MissionManager> missionManager;
};

#endif