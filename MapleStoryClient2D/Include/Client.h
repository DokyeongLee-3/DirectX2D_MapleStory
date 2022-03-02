#pragma once

//#include "GameInfo.h"
#include "Object/Stage.h"
#include "Object/StaticMapObj.h"
#include "Object/Portal.h"
#include "Object/BlinkTree.h"
#include "Object/LampLight.h"
#include "Object/DoubleHelixBlinkTree.h"
#include "Object/Butterfly.h"
#include "Object/LobbyBigLamp.h"
#include "Object/LobbySmallLamp.h"
#include "Object/TileObject.h"
#include "Object/LopeTileObject.h"
#include "Object/VerticalLampLight.h"
#include "Object/OnionMonster.h"
#include "Object/LowerClassBook.h"

//struct MonsterInfo
//{
//    int Level;
//    int HPMax;
//    int HP;
//    int Attack;
//
//    MonsterInfo() :
//        Level(0),
//        HPMax(100),
//        HP(100),
//        Attack(10)
//    {
//    }
//};

struct LoadingMessage
{
	bool	Complete;
	float	Percent;
};
