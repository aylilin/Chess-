#ifndef MISSION_H
#define MISSION_H

#include "Board.h"
#include "Color.h"
#include "Move.h"
#include "Rules.h"
#include "GamePosition.h"
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <string>
#include <vector>
#include <memory>

using namespace std;

enum class MissionType
{
    CaptureQueen,
    ReachPosition,
    EscapeCheck,
    CheckOpponent,
    SurviveMoves
};

struct Mission
{
    MissionType type;
    string description;
    int maxMoves;
    int currentProgress;
    bool isCompleted;
    Position targetPosition;
    PieceType targetPiece;
    Color targetColor;

    Mission() : type(MissionType::SurviveMoves), description(""), maxMoves(0), currentProgress(0), isCompleted(false), targetPiece(PieceType::None), targetColor(Color::None) {}
};

class MissionManager
{
public:
    MissionManager();

    void generateMission(Color playerColor, int turnNumber);
    void updateMission(Board &board, Color playerColor, const Move &lastMove);
    bool isMissionCompleted(Color playerColor) const;
    string getMissionDescription(Color playerColor) const;
    int getRemainingMoves(Color playerColor) const;
    void reset();

private:
    Mission whiteMission;
    Mission blackMission;
    int whiteMovesUsed;
    int blackMovesUsed;
    const Move *lastMove;

    bool whiteWasInCheck;
    bool blackWasInCheck;

    void createCaptureQueenMission(Mission &mission, Color playerColor);
    void createReachPositionMission(Mission &mission, Color playerColor);
    void createEscapeCheckMission(Mission &mission, Color playerColor);
    void createCheckOpponentMission(Mission &mission, Color playerColor);
    void createSurviveMission(Mission &mission, Color playerColor);

    bool checkCaptureQueenMission(const Mission &mission, Board &board, Color playerColor, const Move &lastMove);
    bool checkReachPositionMission(const Mission &mission, Board &board, Color playerColor);
    bool checkEscapeCheckMission(const Mission &mission, Board &board, Color playerColor);
    bool checkCheckOpponentMission(const Mission &mission, Board &board, Color playerColor);
};

#endif