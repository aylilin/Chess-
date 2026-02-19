#ifndef POSITION_H
#define POSITION_H

struct Position
{
    int row;
    int col;

    Position() : row(0), col(0) {}
    Position(int r, int c) : row(r), col(c) {}

    bool operator==(const Position &other) const
    {
        return row == other.row && col == other.col;
    }

    bool isValid() const
    {
        return row >= 0 && row < 8 && col >= 0 && col < 8;
    }
};

#endif