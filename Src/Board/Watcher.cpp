#include "Watcher.h"

using namespace std;

Watcher::Watcher() : WhitePlayer(nullptr), BlackPlayer(nullptr), CurrnetPlayer(nullptr)
{
}

void Watcher::setwhitePlayer(Player *player)
{
    if (!player)
    {
        cout << "Error: White player cannot be null!" << endl;
        return;
    }
    WhitePlayer = player;
    cout << "White player set to: " << WhitePlayer->getName() << endl;
}

void Watcher::setBlackPlayer(Player *player)
{
    if (!player)
    {
        cout << "Error: Black player cannot be null!" << endl;
        return;
    }
    BlackPlayer = player;
    cout << "Black player set to: " << BlackPlayer->getName() << endl;
}

Player *Watcher::getWhitePlayer() const
{
    return WhitePlayer;
}

Player *Watcher::getBlackPlayer() const
{
    return BlackPlayer;
}

Player *Watcher::getCurrentPlayer() const
{
    return CurrnetPlayer;
}

Player *Watcher::getOpponenet() const
{
    if (!CurrnetPlayer)
        return nullptr;

    if (CurrnetPlayer == WhitePlayer)
        return BlackPlayer;
    else if (CurrnetPlayer == BlackPlayer)
        return WhitePlayer;

    return nullptr;
}

void Watcher::switchturn()
{
    if (!WhitePlayer || !BlackPlayer)
    {
        cout << "Error: Players not set!" << endl;
        return;
    }

    if (!CurrnetPlayer)
    {
        CurrnetPlayer = WhitePlayer;
    }
    else if (CurrnetPlayer == WhitePlayer)
    {
        CurrnetPlayer = BlackPlayer;
    }
    else
    {
        CurrnetPlayer = WhitePlayer;
    }

    cout << "Now it's " << CurrnetPlayer->getName() << " turn." << endl;
}

void Watcher::reset()
{
    CurrnetPlayer = WhitePlayer;
    cout << "Game reset. It's " << CurrnetPlayer->getName() << " turn." << endl;
}