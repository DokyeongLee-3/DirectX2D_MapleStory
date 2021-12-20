#pragma once

#include "GameInfo.h"

class CClientManager
{
public:
	bool Init(HINSTANCE hInst);
	void CreateDefaultSceneMode();
	int Run();

	void CreateSceneMode(class CScene* Scene, size_t Type);
	void CreateObject(class CScene* Scene, size_t Type);

	DECLARE_SINGLE(CClientManager)
};

