
#include "LoadingScene.h"
#include "Scene/Scene.h"
#include "Scene/Viewport.h"
#include "Scene/SceneResource.h"
#include "LoadingThread.h"
#include "Scene/SceneManager.h"
#include "../Object/Player2D.h"

CLoadingScene::CLoadingScene()
{
	SetTypeID<CLoadingScene>();
}

CLoadingScene::~CLoadingScene()
{
	SAFE_DELETE(m_LoadingThread);
}

bool CLoadingScene::Init()
{
	//m_LoadingWidget = m_Scene->GetViewport()->CreateWidgetWindow<CLoadingWidget>("LoadingWidget");

	// 로딩 스레드 생성
	//m_LoadingThread = CThread::CreateThread<CLoadingThread>("LoadingThread");

	// m_LoadingThread->Start() 이걸 여기서 안해주고 아래 CLoadingScene::Update에서 해주는 이유는
	// m_LoadingThread->Start()하자마자 Event객체가 Set돼서 LoadingThread::Run 실행하고 
	// LoadingThread::Run 실행 끝나고 Loading쓰레드 소멸하고 MainThread가 CLoadingScene::Update 실행하려하면
	// CThreadQueue<LoadingMessage>* Queue는 Loading쓰레드가 이미 소멸해서 CLoadingScene::Update에서
	// m_LoadingThread가 이미 nullptr임
	//m_LoadingThread->Start();
	//CRenderManager::GetInst()->SetStartFadeOut(true);

	return true;
}

// MainThread가 실행해주는 중
void CLoadingScene::Update(float DeltaTime)
{
	CSceneMode::Update(DeltaTime);

	CThreadQueue<LoadingMessage>* Queue = m_LoadingThread->GetLoadingQueue();

	m_LoadingThread->Start();

	if (!m_LoadingThread)
	{
		CSceneManager::GetInst()->ChangeNextScene();
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
		}
	}
}
