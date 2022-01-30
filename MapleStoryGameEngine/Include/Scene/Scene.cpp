
#include "Scene.h"
#include "../PathManager.h"
#include "SceneManager.h"

CScene::CScene()
{
	m_Mode = new CSceneMode;
	m_Resource = new CSceneResource;
	m_Collision = new CSceneCollision;
	m_CameraManager = new CCameraManager;
	m_Viewport = new CViewport;

	m_Mode->m_Scene = this;
	m_Resource->m_Scene = this;
	m_Collision->m_Scene = this;
	m_CameraManager->m_Scene = this;
	m_Viewport->m_Scene = this;

	m_Start = false;

	m_Collision->Init();
	m_CameraManager->Init();
	m_Viewport->Init();

	m_Change = true;
}

CScene::~CScene()
{
	SAFE_DELETE(m_Viewport);
	SAFE_DELETE(m_CameraManager);
	SAFE_DELETE(m_Collision);
	SAFE_DELETE(m_Resource);
}

void CScene::Start()
{
	m_Mode->Start();

	m_CameraManager->Start();
	m_Collision->Start();
	m_Viewport->Start();

	auto	iter = m_ObjList.begin();
	auto	iterEnd = m_ObjList.end();

	for (; iter != iterEnd; ++iter)
	{
		(*iter)->Start();
	}

	m_Start = true;

	if (m_Mode->GetPlayerObject())
	{
		CCameraComponent* Camera = m_Mode->GetPlayerObject()->FindComponentFromType<CCameraComponent>();

		if (Camera)
		{
			m_CameraManager->SetCurrentCamera(Camera);
		}
	}
}

void CScene::Update(float DeltaTime)
{
	m_Mode->Update(DeltaTime);

	// ���� ����
	m_CameraManager->Update(DeltaTime);

	auto	iter = m_ObjList.begin();
	auto	iterEnd = m_ObjList.end();

	for (; iter != iterEnd;)
	{
		if (!(*iter)->IsActive())
		{
			iter = m_ObjList.erase(iter);
			iterEnd = m_ObjList.end();
			continue;
		}

		else if (!(*iter)->IsEnable())
		{
			++iter;
			continue;
		}

		(*iter)->Update(DeltaTime);
		++iter;
	}

	m_Viewport->Update(DeltaTime);
}

void CScene::PostUpdate(float DeltaTime)
{
	m_Mode->PostUpdate(DeltaTime);

	// ���� ����
	m_CameraManager->PostUpdate(DeltaTime);

	auto	iter = m_ObjList.begin();
	auto	iterEnd = m_ObjList.end();

	for (; iter != iterEnd;)
	{
		if (!(*iter)->IsActive())
		{
			iter = m_ObjList.erase(iter);
			iterEnd = m_ObjList.end();
			continue;
		}

		else if (!(*iter)->IsEnable())
		{
			++iter;
			continue;
		}

		(*iter)->PostUpdate(DeltaTime);
		++iter;
	}

	m_Viewport->PostUpdate(DeltaTime);

	// �浹ü���� �浹 ������ ���Խ�Ų��.
	iter = m_ObjList.begin();
	iterEnd = m_ObjList.end();

	for (; iter != iterEnd; ++iter)
	{
		if (!(*iter)->IsEnable())
			continue;

		(*iter)->AddCollision();
	}

	// ���Ե� �浹ü���� �̿��ؼ� �浹ó���� �����Ѵ�.
	m_Collision->Collision(DeltaTime);
}

void CScene::Save(const char* FileName, const std::string& PathName)
{
	const PathInfo* Info = CPathManager::GetInst()->FindPath(PathName);

	char	FullPath[MAX_PATH] = {};

	if (Info)
		strcpy_s(FullPath, Info->PathMultibyte);

	strcat_s(FullPath, FileName);

	SaveFullPath(FullPath);
}

void CScene::SaveFullPath(const char* FullPath)
{
	FILE* File = nullptr;

	fopen_s(&File, FullPath, "wb");

	if (!File)
		return;

	size_t	SceneModeType = m_Mode->GetTypeID();

	fwrite(&SceneModeType, sizeof(size_t), 1, File);

	// DefaultScene���� �� ������Ʈ�� ī�޶� ������Ʈ �޾Ƴ��� ������Ʈ�� ���� ����
	size_t	ObjCount = m_ObjList.size() - 1;

	fwrite(&ObjCount, sizeof(size_t), 1, File);

	auto	iter = m_ObjList.begin();
	auto	iterEnd = m_ObjList.end();

	for (; iter != iterEnd; ++iter)
	{
		if ((*iter)->GetName() == "EditorCamera")
			continue;

		size_t	ObjType = (*iter)->GetTypeID();

		fwrite(&ObjType, sizeof(size_t), 1, File);

		(*iter)->Save(File);
	}


	fclose(File);
}

void CScene::Load(const char* FileName, const std::string& PathName)
{
	const PathInfo* Info = CPathManager::GetInst()->FindPath(PathName);

	char	FullPath[MAX_PATH] = {};

	if (Info)
		strcpy_s(FullPath, Info->PathMultibyte);

	strcat_s(FullPath, FileName);

	LoadFullPath(FullPath);
}

void CScene::LoadFullPath(const char* FullPath)
{
	FILE* File = nullptr;

	fopen_s(&File, FullPath, "rb");

	if (!File)
		return;
	
	// Editor���� ����� Scene�� Load�ϸ鼭 ���� DefaultScene�� CameraObject�� �������Ƿ� CameraObject���� clear�Ѵ�
	// ���������� Editor���� ������ Scene�� Client���� Load�Ҷ� Client���� Scene�� �̸� �������� Player�� ������ �ʵ����Ѵ� 
	// m_ObjList.clear();

	auto iter = m_ObjList.begin();
	auto iterEnd = m_ObjList.end();

	for (; iter != iterEnd; )
	{
		if ((*iter)->GetName() == "EditorCamera" || 
			(*iter)->GetName() == "Player")
		{	
			++iter;
			continue;
		}

		m_ObjList.erase(iter);
		iter = m_ObjList.begin();
		iterEnd = m_ObjList.end();

	}

	size_t	SceneModeType = 0;

	fread(&SceneModeType, sizeof(size_t), 1, File);

	// SceneMode ����
	CSceneManager::GetInst()->CallCreateSceneMode(this, SceneModeType);

	size_t	ObjCount = m_ObjList.size();

	fread(&ObjCount, sizeof(size_t), 1, File);

	for (size_t i = 0; i < ObjCount; ++i)
	{
		size_t	ObjType = 0;
		fread(&ObjType, sizeof(size_t), 1, File);

		CGameObject* Obj = CSceneManager::GetInst()->CallCreateObject(this, ObjType);

		Obj->Load(File);

		// virtual�� �����ؼ� �����Ϳ����� HierarchyWindow�� ObjectList�� �߰��ϴ� ������ �Ѵ�. Hierarachy�� Load�� Scene�� ����ִ� Object�߰�
		m_Mode->AddObjectList(Obj->GetName().c_str());
	}

	fclose(File);
}
