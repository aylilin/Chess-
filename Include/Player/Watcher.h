#ifndef WATCHER_H
#define WATCHER_H

#include "Player.h"

class Watcher
{
friend class GameStorage;
public:
    Watcher();
    void setwhitePlayer(Player *player);
    void setBlackPlayer(Player *player);
    Player *getWhitePlayer() const;
    Player *getBlackPlayer() const;
    Player *getCurrentPlayer() const;
    Player *getOpponenet() const;
    void switchturn();
    void reset();

private:
    Player *WhitePlayer;
    Player *BlackPlayer;
    Player *CurrnetPlayer;
};

#endif