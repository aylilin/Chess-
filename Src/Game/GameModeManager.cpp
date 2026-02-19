#include "GameModeManager.h"

using namespace std;

GameModeManager::GameModeManager() : currentMode(GameMode::Standard), seasonEvent(make_unique<SeasonEvent>()), timeBomb(make_unique<TimeBomb>(5)), massacreAbility(make_unique<MassacreAbility>()), missionManager(make_unique<MissionManager>())
{
}

// setters and getters for GameModeManager
void GameModeManager::setMode(GameMode mode)
{
    currentMode = mode;
}

GameMode GameModeManager::getMode() const
{
    return currentMode;
}

SeasonEvent *GameModeManager::getSeasonEvent()
{
    return seasonEvent.get();
}

const SeasonEvent *GameModeManager::getSeasonEvent() const
{
    return seasonEvent.get();
}

TimeBomb *GameModeManager::getTimeBomb()
{
    return timeBomb.get();
}

const TimeBomb *GameModeManager::getTimeBomb() const
{
    return timeBomb.get();
}

const MassacreAbility *GameModeManager::getMassacreAbility() const
{
    return massacreAbility.get();
}

MissionManager *GameModeManager::getMissionManager()
{
    return missionManager.get();
}

const MissionManager *GameModeManager::getMissionManager() const
{
    return missionManager.get();
}

//update the Game
void GameModeManager::updateGame(Board &board, Color currentPlayer, int currentTurn)
{
    switch (currentMode)
    {
    case GameMode::Energy:
        timeBomb->update(board);
        break;

    case GameMode::Mission:
        seasonEvent->activateNextSeason(currentTurn);
        seasonEvent->applySeasonEffects(board);
        break;

    default:
        break;
    }
}

// checking move status for GameModes
void GameModeManager::onMoveMade(const Position &from, const Position &to, Board &board, int currentTurn)
{
    switch (currentMode)
    {
    case GameMode::Energy:
        timeBomb->pieceMoved(from, to);

        if (Piece *piece = board.getPiece(to))
        {
            timeBomb->registerPiece(piece);
        }
        break;

    case GameMode::Mission:
        if (Piece *piece = board.getPiece(to))
        {
            seasonEvent->registerPieceMove(piece, currentTurn);
        }
        break;

    default:
        break;
    }
}

void GameModeManager::onPieceCaptured(PieceType type, Color color, int currentTurn)
{
    if (type == PieceType::Queen && currentMode == GameMode::Mission)
    {
        massacreAbility->onQueenCaptured(color);
    }
}

bool GameModeManager::canPieceMove(const Piece *piece, const Position &from, const Position &to, int currentTurn) const
{
    if (!piece)
        return true;

    if (currentMode == GameMode::Mission)
    {
        return seasonEvent->canPieceMoveInSeason(piece, from, to, currentTurn);
    }

    return true;
}

void GameModeManager::generateMissions(Color playerColor, int turnNumber)
{
    if (currentMode == GameMode::Mission)
    {
        missionManager->generateMission(playerColor, turnNumber);
    }
}

void GameModeManager::updateMissions(Board &board, Color playerColor, const Move &lastMove)
{
    if (currentMode == GameMode::Mission)
    {
        missionManager->updateMission(board, playerColor, lastMove);
    }
}

string GameModeManager::getMissionDescription(Color playerColor) const
{
    if (currentMode == GameMode::Mission)
    {
        return missionManager->getMissionDescription(playerColor);
    }
    return "";
}

bool GameModeManager::isMissionCompleted(Color playerColor) const
{
    if (currentMode == GameMode::Mission)
    {
        return missionManager->isMissionCompleted(playerColor);
    }
    return false;
}

int GameModeManager::getRemainingMoves(Color playerColor) const
{
    if (currentMode == GameMode::Mission)
    {
        return missionManager->getRemainingMoves(playerColor);
    }
    return 0;
}

int GameModeManager::getEnergyForMove(PieceType pieceType) const
{
    if (currentMode != GameMode::Energy)
    {
        return 0;
    }

    switch (pieceType)
    {
    case PieceType::Pawn:
        return 1;
    case PieceType::Knight:
        return 3;
    case PieceType::Bishop:
        return 3;
    case PieceType::Rook:
        return 5;
    case PieceType::Queen:
        return 8;
    case PieceType::King:
        return 2;
    default:
        return 0;
    }
}

bool GameModeManager::canUseDoubleMove(Color playerColor) const
{
    if (currentMode == GameMode::Mission)
    {
        return massacreAbility->canUseDoubleMove(playerColor);
    }
    return false;
}

void GameModeManager::useDoubleMove(Color playerColor)
{
    if (currentMode == GameMode::Mission)
    {
        massacreAbility->useDoubleMove(playerColor);
    }
}

// reset every GameMode status
void GameModeManager::resetAll()
{
    seasonEvent->resetForNewSeason();
    massacreAbility->reset();
    missionManager->reset();
}