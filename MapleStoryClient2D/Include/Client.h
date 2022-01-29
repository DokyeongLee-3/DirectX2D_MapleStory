#pragma once

#include "GameInfo.h"

struct PlayerInfo
{
    TCHAR Name[256];
    TCHAR Job[32];
    int Level;
    int HPMax;
    int HP;
    int MPMax;
    int MP;
    int STR;
    int INT;
    int DEX;
    int LUK;
    int EXPMax;
    int EXP;

    PlayerInfo() :
        Name(TEXT("가막못의오리")),
        Job(TEXT("루미너스")),
        Level(60),
        HPMax(1000),
        HP(700),
        MPMax(15000),
        MP(15000),
        STR(4),
        INT(338),
        DEX(4),
        LUK(4),
        EXPMax(Level * 1000),
        EXP(10000)
    {

    }
};

struct LoadingMessage
{
	bool	Complete;
	float	Percent;
};
