#pragma once

#include "GameInfo.h"

enum class CreateObject_Type
{
	GameObject,
	Player,
	RadishMonster,
	OnionMonster,
	LowerClassBook,
	PlayerSkillSet,
	Portal,
	LampLight,
	LobbyBigLamp,
	LobbySmallLamp,
	BlinkTree,
	Butterfly,
	DoubleHelixBlinkTree,
	SingleHelixBlinkTree,
	CharacterSelectBackLight,
	StaticMapObj,
	TileMapObj,
	Stage,
	VerticalLampLight,
	LopeTileMapObj,
	NPCAdobis,
	ZakumBody,
	NPCAmon,
	Puko,
	Punko,
	Cuzco,
	ParticleObject
};

enum class TileEdit_Mode
{
	Type,
	Frame
};

enum class Select_ParticleMaterial
{
	None,
	Rain,
	Dust,
	Smoke
};