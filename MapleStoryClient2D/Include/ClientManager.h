#pragma once

#include "GameInfo.h"

class CClientManager
{
public:
	bool Init(HINSTANCE hInst);
	void CreateDefaultSceneMode();
	int Run();

public:
	void CreateSceneMode(class CScene* Scene, size_t Type);
	class CGameObject* CreateObject(class CScene* Scene, size_t Type);
	class CComponent* CreateComponent(class CGameObject* Obj, size_t Type);
	void CreateAnimInstance(class CSpriteComponent* Sprite, size_t Type);

	DECLARE_SINGLE(CClientManager)
};

