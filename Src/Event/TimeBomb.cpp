#include "TimeBomb.h"
#include <iostream>

using namespace std;

TimeBomb::TimeBomb(int threshold) : explosionThreshold(threshold) {}

string TimeBomb::posToString(const Position &pos) const
{
    return to_string(pos.row) + "," + to_string(pos.col);
}

void TimeBomb::registerPiece(const Piece *piece)
{
    if (!piece)
        return;

    string key = posToString(piece->getPosition());

    if (bombMap.find(key) == bombMap.end())
    {
        BombInfo info;
        info.pos = piece->getPosition();
        info.movesWithoutMoving = 0;
        info.type = piece->getType();
        info.color = piece->getColor();
        bombMap[key] = info;
    }
}

void TimeBomb::pieceMoved(const Position &from, const Position &to)
{
    string fromKey = posToString(from);
    auto it = bombMap.find(fromKey);
    if (it != bombMap.end())
    {
        bombMap.erase(it);
    }
}

void TimeBomb::update(Board &board)
{
    vector<string> toExplode;

    for (auto &entry : bombMap)
    {
        entry.second.movesWithoutMoving++;

        if (entry.second.movesWithoutMoving >= explosionThreshold)
        {
            toExplode.push_back(entry.first);
        }
    }

    for (const auto &key : toExplode)
    {
        auto it = bombMap.find(key);
        if (it != bombMap.end())
        {
            explode(board, it->second.pos);
            bombMap.erase(it);
        }
    }
}

void TimeBomb::destroySurrounding(Board &board, const Position &center)
{
    for (int dr = -1; dr <= 1; dr++)
    {
        for (int dc = -1; dc <= 1; dc++)
        {
            if (dr == 0 && dc == 0)
                continue;

            Position neighbor(center.row + dr, center.col + dc);
            if (board.isInside(neighbor))
            {
                board.removePiece(neighbor);
            }
        }
    }
}

void TimeBomb::explode(Board &board, const Position &pos)
{
    board.removePiece(pos);
    destroySurrounding(board, pos);

    cout << "!!! BOOM !!! Piece exploded at " << char('a' + pos.col) << 8 - pos.row << endl;
}