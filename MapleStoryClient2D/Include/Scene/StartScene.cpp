
#include "StartScene.h"
#include "Scene/Scene.h"
#include "Scene/Viewport.h"
#include "../Object/Player2D.h"
#include "Scene/SceneResource.h"
#include "Scene/SceneManager.h"
#include "Resource/Sound/SoundManager.h"
#include "LoadingThread.h"
#include "Render/RenderManager.h"
#include "LobbyScene.h"

CStartScene::CStartScene()	:
	m_LoadingThread(nullptr)
{
	SetTypeID<CStartScene>();
}

CStartScene::~CStartScene()
{
	//m_Scene->GetResource()->SoundStop("CharacterSelectBGM");

	SAFE_DELETE(m_LoadingThread);
}

void CStartScene::Start()
{
	CSceneMode::Start();
}

bool CStartScene::Init()
{
	/*CPlayer2D* Player = m_Scene->CreateGameObject<CPlayer2D>("Player");

	SetPlayerObject(Player);

	Player->SetAllSceneComponentsLayer("MovingObjFront");*/


	m_StartWidget = m_Scene->GetViewport()->CreateWidgetWindow<CStartWidget>("StartWidget");

	m_CharacterSelectInfoWidget = m_Scene->GetViewport()->CreateWidgetWindow<CCharacterSelectInfoWidget>("CharacterSelectInfoWidget");
	m_CharacterSelectInfoWidget->SetZOrder(1);



	return true;
}

void CStartScene::Update(float DeltaTime)
{
	CSceneMode::Update(DeltaTime);

	// CStartScene::CreateNextScene에서 로딩쓰레드 만들어지면 그때 Queue로 로딩진행률 확인
	if (m_LoadingThread)
	{
		CThreadQueue<LoadingMessage>* Queue = m_LoadingThread->GetLoadingQueue();

		if (!m_LoadingThread)
		{
			CSceneManager::GetInst()->ChangeNextScene();
			CRenderManager::GetInst()->SetStartFadeOut(true);
			return;
		}

		else if (!Queue->empty())
		{
			LoadingMessage	Msg = Queue->front();

			Queue->pop();

			//m_LoadingWidget->SetLoadingPercent(Msg.Percent);

			if (Msg.Complete)
			{
				CSceneManager::GetInst()->ChangeNextScene();
				CRenderManager::GetInst()->SetStartFadeOut(true);
			}
		}
	}
}

void CStartScene::CreateNextScene()
{
	m_Scene->GetResource()->SoundStop("CharacterSelectBGM");

	CSceneManager::GetInst()->CreateNextScene(false);
	CSceneManager::GetInst()->CreateSceneModeEmpty<CLobbyScene>(false);


	m_LoadingThread = CThread::CreateThread<CLoadingThread>("LobbyLoadingThread");
	m_LoadingThread->SetLoadingScene(ThreadLoadingScene::Lobby);
	//m_LoadingThread->SetLoadingScene(ThreadLoadingScene::Main);

	m_LoadingThread->Start();
}
