#ifndef WATCHER_H
#define WATCHER_H

#include "PlayerManager.h"

class Watcher
{
public:
    void setwhitePlayer(PlayerManager *player);
    void setBlackPlayer(PlayerManager *player);
    PlayerManager *getWhitePlayer() const;
    PlayerManager *getBlackPlayer() const;
    PlayerManager *getCurrentPlayer() const;
    PlayerManager *gerOpponenet() const;
    void switchtuen();
    void reset();

private:
    PlayerManager *WhitePlayer;
    PlayerManager *BlackPlayer;
    Color color;
};

#endif