#include "SeasonEvent.h"
#include <iostream>
#include <cmath>

using namespace std;

SeasonEvent::SeasonEvent() : currentSeason(Season::Spring), turnCounter(0) {}

string SeasonEvent::getPieceKey(const Piece *piece) const
{
    if (!piece)
    {
        return "";
    }
    Position pos = piece->getPosition();
    return to_string(pos.row) + "," + to_string(pos.col) + "," + to_string(static_cast<int>(piece->getColor())) + "," + to_string(static_cast<int>(piece->getType()));
}

void SeasonEvent::activateNextSeason(int currentTurn)
{
    turnCounter++;
    if (turnCounter >= SEASON_CYCLE)
    {
        turnCounter = 0;
        nextSeason();
        resetForNewSeason();
        cout << endl;
        cout << "Season Changed to " << getSeasonName() << endl;
        cout << endl;
    }
}

void SeasonEvent::nextSeason()
{
    switch (currentSeason)
    {
    case Season::Spring:
        currentSeason = Season::Summer;
        break;
    case Season::Summer:
        currentSeason = Season::Autumn;
        break;
    case Season::Autumn:
        currentSeason = Season::Winter;
        break;
    case Season::Winter:
        currentSeason = Season::Spring;
        break;
    }
}

void SeasonEvent::resetForNewSeason()
{
    pieceMoveInfo.clear();
}

string SeasonEvent::getSeasonName() const
{
    switch (currentSeason)
    {
    case Season::Spring:
        return "Spring - Heavy Rain (Pawns move every 2 turns)";
    case Season::Summer:
        return "Summer - Burning Sun (Bishops max 3 squares)";
    case Season::Autumn:
        return "Autumn - Food Shortage (Knights move every 2 turns)";
    case Season::Winter:
        return "Winter - Ice Age (Rooks max 4 squares)";
    }
    return "Unknown";
}

bool SeasonEvent::isPawnRestricted() const
{
    return currentSeason == Season::Spring;
}

bool SeasonEvent::isBishopRestricted() const
{
    return currentSeason == Season::Summer;
}

bool SeasonEvent::isKnightRestricted() const
{
    return currentSeason == Season::Autumn;
}

bool SeasonEvent::isRookRestricted() const
{
    return currentSeason == Season::Winter;
}

void SeasonEvent::registerPieceMove(const Piece *piece, int currentTurn)
{
    if (!piece)
    {
        return;
    }

    string key = getPieceKey(piece);
    PieceType type = piece->getType();

    if ((type == PieceType::Pawn && isPawnRestricted()) || (type == PieceType::Knight && isKnightRestricted()))
    {
        auto it = pieceMoveInfo.find(key);
        if (it != pieceMoveInfo.end())
        {
            it->second.lastMoveTurn = currentTurn;
            it->second.canMoveThisTurn = false;
        }
        else
        {
            PieceMoveInfo info;
            info.lastMoveTurn = currentTurn;
            info.canMoveThisTurn = false;
            pieceMoveInfo[key] = info;
        }
    }
}

bool SeasonEvent::canPieceMoveInSeason(const Piece *piece, const Position &from, const Position &to, int currentTurn) const
{
    if (!piece)
    {
        return true;
    }

    PieceType type = piece->getType();

    if (type == PieceType::Pawn && isPawnRestricted())
    {
        string key = getPieceKey(piece);
        auto it = pieceMoveInfo.find(key);

        if (it == pieceMoveInfo.end())
        {
            return true;
        }

        if (it->second.lastMoveTurn == currentTurn)
        {
            cout << "Pawn can only move every 2 turns in Spring!" << endl;
            return false;
        }

        if (currentTurn - it->second.lastMoveTurn < 2)
        {
            cout << "Pawn must wait one more turn before moving again in Spring!" << endl;
            return false;
        }
    }

    if (type == PieceType::Bishop && isBishopRestricted())
    {
        int rowDiff = abs(to.row - from.row);
        int colDiff = abs(to.col - from.col);
        if (rowDiff > 3 || colDiff > 3)
        {
            cout << "Bishop can only move up to 3 squares in Summer!" << endl;
            return false;
        }
    }

    if (type == PieceType::Knight && isKnightRestricted())
    {
        string key = getPieceKey(piece);
        auto it = pieceMoveInfo.find(key);

        if (it == pieceMoveInfo.end())
        {
            return true;
        }

        if (it->second.lastMoveTurn == currentTurn)
        {
            cout << "Knight can only move every 2 turns in Autumn!" << endl;
            return false;
        }

        if (currentTurn - it->second.lastMoveTurn < 2)
        {
            cout << "Knight must wait one more turn before moving again in Autumn!" << endl;
            return false;
        }
    }

    if (type == PieceType::Rook && isRookRestricted())
    {
        int rowDiff = abs(to.row - from.row);
        int colDiff = abs(to.col - from.col);

        if (from.row == to.row)
        {
            if (colDiff > 4)
            {
                cout << "Rook can only move up to 4 squares horizontally in Winter!" << endl;
                return false;
            }
        }
        else if (from.col == to.col)
        {
            if (rowDiff > 4)
            {
                cout << "Rook can only move up to 4 squares vertically in Winter!" << endl;
                return false;
            }
        }
    }

    return true;
}

void SeasonEvent::applySeasonEffects(Board &board)
{
    cout << endl;
    cout << "Season Effects Active: " << getSeasonName() << endl;

    switch (currentSeason)
    {
    case Season::Spring:
        cout << "- Pawns are tired by rain: can move every 2 turns" << endl;
        break;
    case Season::Summer:
        cout << "- Bishops are blinded by sun: max 3 squares move" << endl;
        break;
    case Season::Autumn:
        cout << "- Knights are hungry: can move every 2 turns" << endl;
        break;
    case Season::Winter:
        cout << "- Rooks struggle with ice: max 4 squares move" << endl;
        break;
    }
}

void SeasonEvent::removeSeasonEffects(Board &board)
{
    pieceMoveInfo.clear();
}