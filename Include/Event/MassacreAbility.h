#pragma once
#include "Color.h"

class MassacreAbility
{
public:
    MassacreAbility();
    
    void onQueenCaptured(Color capturedQueenColor);
    bool canUseDoubleMove(Color playerColor) const;
    void useDoubleMove(Color playerColor);
    void reset();
    
private:
    bool whiteHasDoubleMove;
    bool blackHasDoubleMove;
    bool whiteDoubleMoveUsed;
    bool blackDoubleMoveUsed;
};