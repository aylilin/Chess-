#ifndef GAMESTORAGE_H
#define GAMESTORAGE_H

#include "Game.h"
#include "SaveInfo.h"
#include "ArmoredQueen.h"
#include "Joker.h"
#include "Spy.h"
#include "King.h"
#include "Queen.h"
#include "Rook.h"
#include "Bishop.h"
#include "Knight.h"
#include "Pawn.h"
#include "GetPlayer.h"
#include <fstream>
#include <ctime>
#include <string>
#include <iostream>
#include <chrono>
#include <iomanip>
#include <vector>
#include <memory>
#include <filesystem>

using namespace std;

class GameStorage
{
public:
    GameStorage() : watcher(nullptr) {}

    void setWatcher(Watcher *w);
    string currenttime();

    bool SaveGame(const Game &game, const string &filename);
    unique_ptr<Game> LoadGame(const std::string& Saves);
    bool deleteSave(const string &filename);
    vector<string> listSaves(const string &directory = ".");

private:
    Watcher *watcher;
    unique_ptr<GetPlayer> savedWhitePlayer;
    unique_ptr<GetPlayer> savedBlackPlayer;  
};

#endif