
#include "SceneMode.h"
#include "../GameObject/GameObject.h"
#include "../Engine.h"
#include "Scene.h"
#include "../PathManager.h"

#include <sstream>

CSceneMode::CSceneMode()
{
	SetTypeID<CSceneMode>();
}

CSceneMode::~CSceneMode()
{
}

void CSceneMode::PushDeadPos(const Vector3& Pos)
{
	m_DeadPos.push_back(Pos);
}

void CSceneMode::SetPlayerObject(CGameObject* Obj)
{
	m_PlayerObject = Obj;
}

CScene* CSceneMode::GetScene() const
{
	return m_Scene;
}

void CSceneMode::Start()
{
	if (m_PlayerObject)
	{
		CGameObject* Player = m_Scene->FindObject(m_PlayerObject->GetName());

		if (!Player)
		{
			m_Scene->AddObject(m_PlayerObject);
			m_PlayerObject->SetScene(m_Scene);

			CSceneComponent* Component = m_PlayerObject->GetRootComponent();
			Component->SetScene(m_Scene);
			Component->SetAllChildComponentScene(m_Scene);
		}
	}
}

bool CSceneMode::Init()
{
	return true;
}

void CSceneMode::Update(float DeltaTime)
{

//#ifdef _DEBUG
	char FPSStr[256] = {};

	float FPS = CEngine::GetInst()->GetFPS();

	sprintf_s(FPSStr, "%f\n", FPS);

	OutputDebugStringA(FPSStr);


//#else
//
//#endif	// _DEBUG

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
