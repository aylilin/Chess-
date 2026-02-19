#pragma once

#include "Position.h"
#include <string>
#include <sstream>
#include <cctype>
#include <cstdlib>

using namespace std;

struct Move
{
    Position from;
    Position to;

    Move();
    Move(const Position &f, const Position &t);
    Move(int fromRow, int fromCol, int toRow, int toCol);

    bool operator==(const Move &other) const;
    bool operator!=(const Move &other) const;
    bool isValid() const;

    string toString() const;
    string toShortString() const;
    string toUpperCaseString() const;
    
    static Move fromString(const string &str);
};