
#include "LoadingThread.h"
#include "LobbyScene.h"
#include "OnionScene.h"
#include "WayToZakumScene.h"
#include "Library2ndScene.h"
#include "RadishScene.h"
#include "Scene/SceneManager.h"

CLoadingThread::CLoadingThread()
{
}

CLoadingThread::~CLoadingThread()
{
}

bool CLoadingThread::Init()
{
	if (!CThread::Init())
		return false;

	return true;
}

void CLoadingThread::Run()
{
	// ·Îµù
	if (m_LoadingScene == ThreadLoadingScene::Lobby)
	{
		CLobbyScene* LobbyScene = (CLobbyScene*)(CSceneManager::GetInst()->GetNextScene()->GetSceneMode());

		LobbyScene->SetLoadingFunction<CLoadingThread>(this, &CLoadingThread::AddMessage);

		LobbyScene->Init();

		AddMessage(true, 1.f);
	}

	else if (m_LoadingScene == ThreadLoadingScene::Onion)
	{
		COnionScene* OnionScene = (COnionScene*)(CSceneManager::GetInst()->GetNextScene()->GetSceneMode());

		OnionScene->SetLoadingFunction<CLoadingThread>(this, &CLoadingThread::AddMessage);

		OnionScene->Init();

		AddMessage(true, 1.f);
	}

	else if (m_LoadingScene == ThreadLoadingScene::WayToZakum)
	{
		CWayToZakumScene* ZakumScene = (CWayToZakumScene*)(CSceneManager::GetInst()->GetNextScene()->GetSceneMode());

		ZakumScene->SetLoadingFunction<CLoadingThread>(this, &CLoadingThread::AddMessage);

		ZakumScene->Init();

		AddMessage(true, 1.f);
	}

	else if (m_LoadingScene == ThreadLoadingScene::Library2nd)
	{
		CLibrary2ndScene* LibraryScene = (CLibrary2ndScene*)(CSceneManager::GetInst()->GetNextScene()->GetSceneMode());

		LibraryScene->SetLoadingFunction<CLoadingThread>(this, &CLoadingThread::AddMessage);

		LibraryScene->Init();

		AddMessage(true, 1.f);
	}

	else if (m_LoadingScene == ThreadLoadingScene::Radish)
	{
		CRadishScene* RadishScene = (CRadishScene*)(CSceneManager::GetInst()->GetNextScene()->GetSceneMode());

		RadishScene->SetLoadingFunction<CLoadingThread>(this, &CLoadingThread::AddMessage);

		RadishScene->Init();

		AddMessage(true, 1.f);
	}
}
