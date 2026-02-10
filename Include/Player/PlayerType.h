#ifndef PLAYERTYPE_H
#define PLAYERTYPE_H

enum class Color
{
    White = 1,
    Black = 2,
    None = 0
};

enum class Playerstatus
{
    Thinking = 0,
    Waiting = 1,
    Moved = 2
};

#endif