#include "Mission.h"

//constructor
MissionManager::MissionManager()
    : whiteMovesUsed(0), blackMovesUsed(0), lastMove(nullptr),
      whiteWasInCheck(false), blackWasInCheck(false)
{
    whiteMission.isCompleted = false;
    blackMission.isCompleted = false;
    whiteMission.currentProgress = 0;
    blackMission.currentProgress = 0;
}

void MissionManager::generateMission(Color playerColor, int turnNumber)
{
    srand(time(nullptr) + turnNumber);
    int missionType = rand() % 5;

    if (playerColor == Color::White)
    {
        whiteMission.currentProgress = 0;
        whiteMission.isCompleted = false;
        whiteMovesUsed = 0;

        switch (missionType)
        {
        case 0:
            createCaptureQueenMission(whiteMission, playerColor);
            break;
        case 1:
            createReachPositionMission(whiteMission, playerColor);
            break;
        case 2:
            createEscapeCheckMission(whiteMission, playerColor);
            break;
        case 3:
            createCheckOpponentMission(whiteMission, playerColor);
            break;
        case 4:
            createSurviveMission(whiteMission, playerColor);
            break;
        }
    }
    else
    {
        blackMission.currentProgress = 0;
        blackMission.isCompleted = false;
        blackMovesUsed = 0;

        switch (missionType)
        {
        case 0:
            createCaptureQueenMission(blackMission, playerColor);
            break;
        case 1:
            createReachPositionMission(blackMission, playerColor);
            break;
        case 2:
            createEscapeCheckMission(blackMission, playerColor);
            break;
        case 3:
            createCheckOpponentMission(blackMission, playerColor);
            break;
        case 4:
            createSurviveMission(blackMission, playerColor);
            break;
        }
    }
}

void MissionManager::createCaptureQueenMission(Mission &mission, Color playerColor)
{
    mission.type = MissionType::CaptureQueen;
    mission.maxMoves = 10;
    mission.targetPiece = PieceType::Queen;
    mission.targetColor = (playerColor == Color::White) ? Color::Black : Color::White;
    mission.description = "Capture opponent's queen within 10 moves!";
}

void MissionManager::createReachPositionMission(Mission &mission, Color playerColor)
{
    mission.type = MissionType::ReachPosition;
    mission.maxMoves = 15;

    int targetRow = rand() % 8;
    int targetCol = rand() % 8;
    mission.targetPosition = Position(targetRow, targetCol);

    mission.description = "Move your king to " + string(1, 'a' + targetCol) + to_string(8 - targetRow) + " within 15 moves!";
}

void MissionManager::createEscapeCheckMission(Mission &mission, Color playerColor)
{
    mission.type = MissionType::EscapeCheck;
    mission.maxMoves = 3;
    mission.description = "Get out of check within 3 moves!";
}

void MissionManager::createCheckOpponentMission(Mission &mission, Color playerColor)
{
    mission.type = MissionType::CheckOpponent;
    mission.maxMoves = 5;
    mission.targetColor = (playerColor == Color::White) ? Color::Black : Color::White;
    mission.description = "Put opponent in check within 5 moves!";
}

void MissionManager::createSurviveMission(Mission &mission, Color playerColor)
{
    mission.type = MissionType::SurviveMoves;
    mission.maxMoves = 8;
    mission.description = "Survive for 8 moves without being checkmated!";
}

void MissionManager::updateMission(Board &board, Color playerColor, const Move &lastMove)
{
    this->lastMove = &lastMove;

    if (playerColor == Color::White)
    {
        if (whiteMission.isCompleted)
        {
            return;
        }

        GamePosition tempPos;
        board.copyTo(tempPos.getBoard());
        tempPos.setSideToMove(Color::White);
        whiteWasInCheck = Rules::isCheck(tempPos, Color::White);

        whiteMovesUsed++;

        bool completed = false;

        switch (whiteMission.type)
        {
        case MissionType::CaptureQueen:
            completed = checkCaptureQueenMission(whiteMission, board, playerColor, lastMove);
            break;

        case MissionType::ReachPosition:
            completed = checkReachPositionMission(whiteMission, board, playerColor);
            break;

        case MissionType::EscapeCheck:
            completed = checkEscapeCheckMission(whiteMission, board, playerColor);
            break;

        case MissionType::CheckOpponent:
            completed = checkCheckOpponentMission(whiteMission, board, playerColor);
            break;

        case MissionType::SurviveMoves:
            completed = (whiteMovesUsed >= whiteMission.maxMoves);
            break;
        }

        if (completed)
        {
            whiteMission.isCompleted = true;
            cout << "*** WHITE MISSION COMPLETED! ***" << endl;
        }

        if (whiteMovesUsed > whiteMission.maxMoves && !whiteMission.isCompleted)
        {
            cout << "White mission failed! " << whiteMission.description << endl;
            generateMission(Color::White, whiteMovesUsed);
        }
    }
    else
    {
        if (blackMission.isCompleted)
        {
            return;
        }

        GamePosition tempPos;
        board.copyTo(tempPos.getBoard());
        tempPos.setSideToMove(Color::Black);
        blackWasInCheck = Rules::isCheck(tempPos, Color::Black);

        blackMovesUsed++;

        bool completed = false;

        switch (blackMission.type)
        {
        case MissionType::CaptureQueen:
            completed = checkCaptureQueenMission(blackMission, board, playerColor, lastMove);
            break;

        case MissionType::ReachPosition:
            completed = checkReachPositionMission(blackMission, board, playerColor);
            break;

        case MissionType::EscapeCheck:
            completed = checkEscapeCheckMission(blackMission, board, playerColor);
            break;

        case MissionType::CheckOpponent:
            completed = checkCheckOpponentMission(blackMission, board, playerColor);
            break;

        case MissionType::SurviveMoves:
            completed = (blackMovesUsed >= blackMission.maxMoves);
            break;
        }

        if (completed)
        {
            blackMission.isCompleted = true;
            cout << "*** BLACK MISSION COMPLETED! ***" << endl;
        }

        if (blackMovesUsed > blackMission.maxMoves && !blackMission.isCompleted)
        {
            cout << "Black mission failed! " << blackMission.description << endl;
            generateMission(Color::Black, blackMovesUsed);
        }
    }
}

bool MissionManager::checkCaptureQueenMission(const Mission &mission, Board &board, Color playerColor, const Move &lastMove)
{
    Piece *captured = board.getPiece(lastMove.to);
    if (captured && captured->getType() == PieceType::Queen &&
        captured->getColor() != playerColor)
    {
        return true;
    }
    return false;
}

bool MissionManager::checkReachPositionMission(const Mission &mission, Board &board, Color playerColor)
{
    for (int r = 0; r < 8; r++)
    {
        for (int c = 0; c < 8; c++)
        {
            Piece *piece = board.getPiece(Position(r, c));
            if (piece && piece->getType() == PieceType::King &&
                piece->getColor() == playerColor)
            {
                return (r == mission.targetPosition.row && c == mission.targetPosition.col);
            }
        }
    }
    return false;
}

bool MissionManager::checkEscapeCheckMission(const Mission &mission, Board &board, Color playerColor)
{
    if (!lastMove)
    {
        return false;
    }

    bool wasInCheck;
    if (playerColor == Color::White)
    {
        wasInCheck = whiteWasInCheck;
    }
    else
    {
        wasInCheck = blackWasInCheck;
    }

    GamePosition tempPos;
    board.copyTo(tempPos.getBoard());
    tempPos.setSideToMove(playerColor);
    bool isInCheckNow = Rules::isCheck(tempPos, playerColor);

    if (wasInCheck && !isInCheckNow)
    {
        return true;
    }

    return false;
}

bool MissionManager::checkCheckOpponentMission(const Mission &mission, Board &board, Color playerColor)
{
    if (!lastMove)
    {
        return false;
    }

    Color opponentColor = (playerColor == Color::White) ? Color::Black : Color::White;

    GamePosition tempPos;
    board.copyTo(tempPos.getBoard());
    tempPos.setSideToMove(opponentColor);

    return Rules::isCheck(tempPos, opponentColor);
}

//Description
string MissionManager::getMissionDescription(Color playerColor) const
{
    if (playerColor == Color::White)
    {
        if (whiteMission.isCompleted)
        {
            return "Mission COMPLETED! ";
        }
        return whiteMission.description + " (Moves left: " + to_string(whiteMission.maxMoves - whiteMovesUsed) + ")";
    }
    else
    {
        if (blackMission.isCompleted)
        {
            return "Mission COMPLETED! ";
        }

        return blackMission.description + " (Moves left: " + to_string(blackMission.maxMoves - blackMovesUsed) + ")";
    }
}

int MissionManager::getRemainingMoves(Color playerColor) const
{
    if (playerColor == Color::White)
    {
        return whiteMission.maxMoves - whiteMovesUsed;
    }
    else
    {
        return blackMission.maxMoves - blackMovesUsed;
    }
}

bool MissionManager::isMissionCompleted(Color playerColor) const
{
    return (playerColor == Color::White) ? whiteMission.isCompleted : blackMission.isCompleted;
}

void MissionManager::reset()
{
    whiteMission.isCompleted = false;
    blackMission.isCompleted = false;
    whiteMovesUsed = 0;
    blackMovesUsed = 0;
    whiteMission.currentProgress = 0;
    blackMission.currentProgress = 0;
    whiteWasInCheck = false;
    blackWasInCheck = false;
    lastMove = nullptr;
}