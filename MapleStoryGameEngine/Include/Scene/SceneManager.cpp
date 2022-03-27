
#include "SceneManager.h"
#include "../Render/RenderManager.h"
#include "../Sync.h"


DEFINITION_SINGLE(CSceneManager)

CSceneManager::CSceneManager() :
	m_Scene(nullptr),
	m_NextScene(nullptr),
	m_FadeInEndCallback(nullptr),
	m_FadeOutEndCallback(nullptr)
{
	InitializeCriticalSection(&m_Crt);
}

CSceneManager::~CSceneManager()
{
	DeleteCriticalSection(&m_Crt);

	SAFE_DELETE(m_Scene);

	SAFE_DELETE(m_NextScene);
}

void CSceneManager::Start()
{
	m_Scene->Start();
}

bool CSceneManager::Init()
{
	m_Scene = new CScene;

	CRenderManager::GetInst()->SetObjectList(&m_Scene->m_ObjList);

	return true;
}

bool CSceneManager::Update(float DeltaTime)
{
	m_Scene->Update(DeltaTime);

	return ChangeScene();
}

bool CSceneManager::PostUpdate(float DeltaTime)
{
	m_Scene->PostUpdate(DeltaTime);

	return ChangeScene();
}

bool CSceneManager::ChangeScene()
{
	// Loading Thread가 CreateNextScnee에서 동적할당만 하고 MainThread로 context switching이 돼서
	// CSceneManager::ChangeScene을 호출해도, CSceneManager의 멤버인 m_Crt로 Sync를 걸어놔서 ChangeScene의 시작부분에서는 CreateNextScene에서 Sync걸때 사용한
	// CSceneManager 멤버 변수 m_Crt로 Sync를 걸려고하면 Loading Thread에서 그 멤버 변수 m_Crt로 Sync를 이미 걸어놔서 Loading Thread의 작업이 끝날때 까지는
	// MainThread가 ChangeScene에 첫줄인 CSync sync(&m_Crt); 에 Blocking상태가 되어서 대기한다
	CSync	sync(&m_Crt);

	if (m_NextScene)
	{
		if (m_NextScene->m_Change)
		{
			SAFE_DELETE(m_Scene);
			m_Scene = m_NextScene;
			m_NextScene = nullptr;

			CRenderManager::GetInst()->SetObjectList(&m_Scene->m_ObjList);

			m_Scene->Start();

			return true;
		}
	}

	return false;
}

void CSceneManager::CreateNextScene(bool AutoChange)
{
	// CSync를 걸어둔 이유는 이 함수를 실행하는 쓰레드는 Loading Thread가 될텐데, 아래 코드에서 동적할당만 하고 MainThread로 context switching이 돼서
	// CSceneManager::ChangeScene을 호출해도, CSceneManager의 멤버인 m_Crt로 Sync를 걸어놔서 ChangeScene의 시작부분에 같은 멤버인 m_Crt로 Sync를 걸려고하면
	// Loading Thread에서 같은 멤버 m_Crt로 Sync를 이미 걸어놔서 Loading Thread의 작업이 끝날때 까지는 MainThread가 ChangeScene에 첫줄인 CSync sync(&m_Crt);에
	// Blocking상태가 되어서 대기한다
	CSync	sync(&m_Crt);

	SAFE_DELETE(m_NextScene);

	m_NextScene = new CScene;

	m_NextScene->SetAutoChange(AutoChange);
}

void CSceneManager::ChangeNextScene()
{
	CSync	sync(&m_Crt);

	m_NextScene->SetAutoChange(true);
}
