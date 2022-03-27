
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
	// Loading Thread�� CreateNextScnee���� �����Ҵ縸 �ϰ� MainThread�� context switching�� �ż�
	// CSceneManager::ChangeScene�� ȣ���ص�, CSceneManager�� ����� m_Crt�� Sync�� �ɾ���� ChangeScene�� ���ۺκп����� CreateNextScene���� Sync�ɶ� �����
	// CSceneManager ��� ���� m_Crt�� Sync�� �ɷ����ϸ� Loading Thread���� �� ��� ���� m_Crt�� Sync�� �̹� �ɾ���� Loading Thread�� �۾��� ������ ������
	// MainThread�� ChangeScene�� ù���� CSync sync(&m_Crt); �� Blocking���°� �Ǿ ����Ѵ�
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
	// CSync�� �ɾ�� ������ �� �Լ��� �����ϴ� ������� Loading Thread�� ���ٵ�, �Ʒ� �ڵ忡�� �����Ҵ縸 �ϰ� MainThread�� context switching�� �ż�
	// CSceneManager::ChangeScene�� ȣ���ص�, CSceneManager�� ����� m_Crt�� Sync�� �ɾ���� ChangeScene�� ���ۺκп� ���� ����� m_Crt�� Sync�� �ɷ����ϸ�
	// Loading Thread���� ���� ��� m_Crt�� Sync�� �̹� �ɾ���� Loading Thread�� �۾��� ������ ������ MainThread�� ChangeScene�� ù���� CSync sync(&m_Crt);��
	// Blocking���°� �Ǿ ����Ѵ�
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
