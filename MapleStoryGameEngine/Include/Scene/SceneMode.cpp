
#include "SceneMode.h"
#include "../GameObject/GameObject.h"
#include "../Engine.h"

CSceneMode::CSceneMode()
{
	SetTypeID<CSceneMode>();
}

CSceneMode::~CSceneMode()
{
}

void CSceneMode::SetPlayerObject(CGameObject* Obj)
{

	m_PlayerObject = Obj;

}

void CSceneMode::Start()
{
}

bool CSceneMode::Init()
{
	return true;
}

void CSceneMode::Update(float DeltaTime)
{

#ifdef _DEBUG
	char FPSStr[256] = {};

	float FPS = CEngine::GetInst()->GetFPS();

	sprintf_s(FPSStr, "FPS : %f\n", FPS);

	OutputDebugStringA(FPSStr);
#else

#endif	// _DEBUG

}

void CSceneMode::PostUpdate(float DeltaTime)
{
}

void CSceneMode::AddObjectList(const char* ObjName)
{
}

void CSceneMode::AddComponentList(const char* ComponentName)
{
}
