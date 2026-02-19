#ifndef SAVEINFO_H
#define SAVEINFO_H

#include <string>
#include <ctime>
#include "GameMode.h"
#include "Watcher.h"

using namespace std;

struct SaveInfo
{
    string gamename;
    string whiteplayer;
    string blackplayer;
    GameMode gamemode;
    string gamestatus;
    
    time_t starttime;
    time_t savetime;
    time_t lastchange;
};

#endif