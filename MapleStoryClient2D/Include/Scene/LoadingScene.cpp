
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

	// �ε� ������ ����
	//m_LoadingThread = CThread::CreateThread<CLoadingThread>("LoadingThread");

	// m_LoadingThread->Start() �̰� ���⼭ �����ְ� �Ʒ� CLoadingScene::Update���� ���ִ� ������
	// m_LoadingThread->Start()���ڸ��� Event��ü�� Set�ż� LoadingThread::Run �����ϰ� 
	// LoadingThread::Run ���� ������ Loading������ �Ҹ��ϰ� MainThread�� CLoadingScene::Update �����Ϸ��ϸ�
	// CThreadQueue<LoadingMessage>* Queue�� Loading�����尡 �̹� �Ҹ��ؼ� CLoadingScene::Update����
	// m_LoadingThread�� �̹� nullptr��
	//m_LoadingThread->Start();
	//CRenderManager::GetInst()->SetStartFadeOut(true);

	return true;
}

// MainThread�� �������ִ� ��
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
