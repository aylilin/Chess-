#include "MassacreAbility.h"
#include <iostream>

using namespace std;

//constructor
MassacreAbility::MassacreAbility()
    : whiteHasDoubleMove(false), blackHasDoubleMove(false),
      whiteDoubleMoveUsed(false), blackDoubleMoveUsed(false) {}

//special abilities
void MassacreAbility::onQueenCaptured(Color capturedQueenColor)
{
    if (capturedQueenColor == Color::White)
    {
        blackHasDoubleMove = true;
        blackDoubleMoveUsed = false;
        cout << "Black queen can move twice this turn!" << endl;
    }
    else if (capturedQueenColor == Color::Black)
    {
        whiteHasDoubleMove = true;
        whiteDoubleMoveUsed = false;
        cout << "White queen can move twice this turn!" << endl;
    }
}

bool MassacreAbility::canUseDoubleMove(Color playerColor) const
{
    if (playerColor == Color::White)
    {
        return whiteHasDoubleMove && !whiteDoubleMoveUsed;
    }
    else if (playerColor == Color::Black)
    {
        return blackHasDoubleMove && !blackDoubleMoveUsed;
    }
    return false;
}

void MassacreAbility::useDoubleMove(Color playerColor)
{
    if (playerColor == Color::White)
    {
        whiteDoubleMoveUsed = true;
    }
    else if (playerColor == Color::Black)
    {
        blackDoubleMoveUsed = true;
    }
}

void MassacreAbility::reset()
{
    whiteHasDoubleMove = false;
    blackHasDoubleMove = false;
    whiteDoubleMoveUsed = false;
    blackDoubleMoveUsed = false;
}