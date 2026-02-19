#include "Move.h"

using namespace std;

Move::Move() : from(0, 0), to(0, 0) {}

Move::Move(const Position &f, const Position &t) : from(f), to(t) {}

Move::Move(int fromRow, int fromCol, int toRow, int toCol) : from(fromRow, fromCol), to(toRow, toCol) {}

bool Move::operator==(const Move &other) const
{
    return from == other.from && to == other.to;
}

bool Move::operator!=(const Move &other) const
{
    return !(*this == other);
}

bool Move::isValid() const
{
    return from.isValid() && to.isValid();
}

string Move::toString() const
{
    if (!isValid())
    {
        return "invalid";
    }

    char fromCol = 'a' + from.col;
    char toCol = 'a' + to.col;
    int fromRow = 8 - from.row;
    int toRow = 8 - to.row;

    return string() + fromCol + to_string(fromRow) + "-" + toCol + to_string(toRow);
}

string Move::toShortString() const
{
    if (!isValid())
    {
        return "invalid";
    }

    char fromCol = 'a' + from.col;
    char toCol = 'a' + to.col;
    int fromRow = 8 - from.row;
    int toRow = 8 - to.row;

    return string() + fromCol + to_string(fromRow) + toCol + to_string(toRow);
}

string Move::toUpperCaseString() const
{
    if (!isValid())
    {

        return "INVALID";
    }

    char fromCol = 'A' + from.col;
    char toCol = 'A' + to.col;
    int fromRow = 8 - from.row;
    int toRow = 8 - to.row;

    return string() + fromCol + to_string(fromRow) + "-" + toCol + to_string(toRow);
}

Move Move::fromString(const string &str)
{
    if (str.empty())
    {
        return Move();
    }

    string clean;
    for (char c : str)
    {
        if (c != ' ' && c != '\t')
        {
            clean += tolower(c);
        }
    }

    if (clean.length() < 4)
    {
        return Move();
    }

    char fromFile, fromRank, toFile, toRank;

    if (clean.length() >= 5 && clean[2] == '-')
    {
        fromFile = clean[0];
        fromRank = clean[1];
        toFile = clean[3];
        toRank = clean[4];
    }
    else if (clean.length() == 4)
    {
        fromFile = clean[0];
        fromRank = clean[1];
        toFile = clean[2];
        toRank = clean[3];
    }
    else if (clean.length() == 5 && (clean[2] == ':' || clean[2] == ' '))
    {
        fromFile = clean[0];
        fromRank = clean[1];
        toFile = clean[3];
        toRank = clean[4];
    }
    else
    {
        return Move();
    }

    if (fromFile < 'a' || fromFile > 'h' ||
        toFile < 'a' || toFile > 'h' ||
        fromRank < '1' || fromRank > '8' ||
        toRank < '1' || toRank > '8')
    {
        return Move();
    }

    int fromCol = fromFile - 'a';
    int fromRow = 8 - (fromRank - '0');
    int toCol = toFile - 'a';
    int toRow = 8 - (toRank - '0');

    return Move(fromRow, fromCol, toRow, toCol);
}