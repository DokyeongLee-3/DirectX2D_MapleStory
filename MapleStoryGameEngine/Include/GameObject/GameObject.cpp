
#include "GameObject.h"
#include "../Component/SpriteComponent.h"
#include "../Scene/SceneManager.h"
#include "../PathManager.h"
#include "../Component/TileMapComponent.h"
#include "../Component/ColliderBox2D.h"
#include "../Component/ColliderCircle.h"
#include "../Component/ColliderPixel.h"

CGameObject::CGameObject()	:
	m_Scene(nullptr),
	m_Parent(nullptr),
	m_LifeSpan(0.f),
	m_Gravity(false),
	m_GravityFactor(3700.f),
	m_GravityAccTime(0.f),
	m_TileCollisionEnable(false)
{
	SetTypeID<CGameObject>();
}

CGameObject::CGameObject(const CGameObject& obj)
{
	*this = obj;

	m_RefCount = 0;

	if (obj.m_RootComponent)
	{
		m_RootComponent = obj.m_RootComponent->Clone();

		m_RootComponent->SetGameObject(this);

		m_RootComponent->SetSceneComponent(this);
	}

	m_vecObjectComponent.clear();

	size_t	Size = obj.m_vecObjectComponent.size();

	for (size_t i = 0; i < Size; ++i)
	{
		m_vecObjectComponent.push_back(obj.m_vecObjectComponent[i]->Clone());

		m_vecObjectComponent[i]->SetGameObject(this);
	}
}

CGameObject::~CGameObject()
{
	auto iter = m_SceneComponentList.begin();
	auto iterEnd = m_SceneComponentList.end();

	for (; iter != iterEnd; ++iter)
	{
		if ((*iter)->GetTypeID() == typeid(CColliderComponent).hash_code()
			|| (*iter)->GetTypeID() == typeid(CColliderBox2D).hash_code()
			|| (*iter)->GetTypeID() == typeid(CColliderCircle).hash_code()
			|| (*iter)->GetTypeID() == typeid(CColliderPixel).hash_code())
		{
			if (m_Scene)
			{
				CSceneCollision* SceneColl = m_Scene->GetCollision();

				if (SceneColl)
				{
					SceneColl->DeleteCollider((CColliderComponent*)(*iter));
				}
			}
		}
	}
}

void CGameObject::SetScene(CScene* Scene)
{
	m_Scene = Scene;
}

void CGameObject::Destroy()
{
	CRef::Destroy();

	if (m_RootComponent)
	{
		m_RootComponent->Destroy();
	}

	size_t	Size = m_vecObjectComponent.size();

	for (size_t i = 0; i < Size; ++i)
	{
		m_vecObjectComponent[i]->Destroy();
	}
}


CComponent* CGameObject::FindComponent(const std::string& Name)
{
	{
		auto	iter = m_SceneComponentList.begin();
		auto	iterEnd = m_SceneComponentList.end();

		for (; iter != iterEnd; ++iter)
		{
			if ((*iter)->GetName() == Name)
				return *iter;
		}
	}

	{
		auto	iter = m_vecObjectComponent.begin();
		auto	iterEnd = m_vecObjectComponent.end();

		for (; iter != iterEnd; ++iter)
		{
			if ((*iter)->GetName() == Name)
				return *iter;
		}
	}

	return nullptr;
}

void CGameObject::DeleteSceneComponent(CSceneComponent* Component)
{
	auto	iter = m_SceneComponentList.begin();
	auto	iterEnd = m_SceneComponentList.end();

	for (; iter != iterEnd; ++iter)
	{
		if ((*iter) == Component)
		{
			(*iter)->Destroy();
			iter = m_SceneComponentList.erase(iter);
			iterEnd = m_SceneComponentList.end();
			break;
		}
	}
}

void CGameObject::GetAllSceneComponentsName(std::vector<FindComponentName>& vecNames)
{
	if (!m_RootComponent)
		return;

	m_RootComponent->GetAllSceneComponentsName(vecNames);

	//auto iter = m_SceneComponentList.begin();
	//auto iterEnd = m_SceneComponentList.end();

	//for (; iter != iterEnd; ++iter)
	//{
	//	(*iter)->GetAllSceneComponentsName(vecNames);
	//}
}

void CGameObject::SetAllSceneComponentsLayer(const std::string& Name)
{
	m_RootComponent->SetAllSceneComponentsLayer(Name);
}

void CGameObject::ClearSceneComponents()
{
	if (m_RootComponent)
	{
		m_RootComponent = nullptr;
	}

	m_SceneComponentList.clear();
}

void CGameObject::SetDamage(float Damage, bool Critical)
{
}

void CGameObject::Start()
{
	if (m_RootComponent)
		m_RootComponent->Start();

	size_t Size = m_vecObjectComponent.size();

	for (size_t i = 0; i < Size; ++i)
	{
		m_vecObjectComponent[i]->Start();
	}
}

bool CGameObject::Init()
{
	return true;
}

void CGameObject::Update(float DeltaTime)
{
	// 처음 m_LiftSpan의 초기값이 0.f라는건 정해진 LiftSpan이 무한이라는 의미
	if (m_LifeSpan > 0.f)
	{
		m_LifeSpan -= DeltaTime;

		if (m_LifeSpan <= 0.f)
		{
			Destroy();
			return;
		}
	}

	if (m_Gravity)
	{
		m_GravityAccTime += DeltaTime;
		AddWorldPos(0.f, -m_GravityAccTime * m_GravityFactor * DeltaTime, 0.f);
	}

	size_t Size = m_vecObjectComponent.size();

	for (size_t i = 0; i < Size; ++i)
	{
		m_vecObjectComponent[i]->Update(DeltaTime);
	}

	if (m_RootComponent)
		m_RootComponent->Update(DeltaTime);
}

void CGameObject::PostUpdate(float DeltaTime)
{
	size_t Size = m_vecObjectComponent.size();

	for (size_t i = 0; i < Size; ++i)
	{
		m_vecObjectComponent[i]->PostUpdate(DeltaTime);
	}

	if (m_RootComponent)
		m_RootComponent->PostUpdate(DeltaTime);
}

void CGameObject::AddCollision()
{
	if (m_RootComponent)
		m_RootComponent->CheckCollision();
}

void CGameObject::PrevRender()
{
	if (!m_Enable)
		return;

	size_t Size = m_vecObjectComponent.size();

	for (size_t i = 0; i < Size; ++i)
	{
		m_vecObjectComponent[i]->PrevRender();
	}

	if (m_RootComponent)
		m_RootComponent->PrevRender();
}

void CGameObject::Render()
{
	size_t Size = m_vecObjectComponent.size();

	for (size_t i = 0; i < Size; ++i)
	{
		m_vecObjectComponent[i]->Render();
	}

	if (m_RootComponent)
		m_RootComponent->Render();
}

void CGameObject::PostRender()
{
	size_t Size = m_vecObjectComponent.size();

	for (size_t i = 0; i < Size; ++i)
	{
		m_vecObjectComponent[i]->PostRender();
	}

	if (m_RootComponent)
		m_RootComponent->PostRender();
}

CGameObject* CGameObject::Clone()
{
	return new CGameObject(*this);
}

void CGameObject::Save(FILE* File)
{
	CRef::Save(File);

	if (m_RootComponent)
	{
		bool	Root = true;
		fwrite(&Root, sizeof(bool), 1, File);

		size_t	TypeID = m_RootComponent->GetTypeID();
		fwrite(&TypeID, sizeof(size_t), 1, File);

		m_RootComponent->Save(File);
	}

	else
	{
		bool	Root = false;
		fwrite(&Root, sizeof(bool), 1, File);
	}

	int	ObjComponentCount = (int)m_vecObjectComponent.size();

	fwrite(&ObjComponentCount, sizeof(int), 1, File);

	for (int i = 0; i < ObjComponentCount; ++i)
	{
		size_t	TypeID = m_vecObjectComponent[i]->GetTypeID();
		fwrite(&TypeID, sizeof(size_t), 1, File);

		m_vecObjectComponent[i]->Save(File);
	}
}

void CGameObject::Load(FILE* File)
{
	CRef::Load(File);

	bool	Root = false;
	fread(&Root, sizeof(bool), 1, File);

	if (Root)
	{
		size_t	TypeID = 0;
		fread(&TypeID, sizeof(size_t), 1, File);

		if (CSceneManager::GetInst()->CallCreateComponent(this, TypeID))
		{
			m_RootComponent->Load(File);
			
			CScene* NextScene = CSceneManager::GetInst()->GetNextScene();

			if (NextScene)
				m_RootComponent->SetScene(NextScene);
			else
				m_RootComponent->SetScene(m_Scene);
		}
	}

	int	ObjComponentCount = 0;

	fread(&ObjComponentCount, sizeof(int), 1, File);

	for (int i = 0; i < ObjComponentCount; ++i)
	{
		size_t	TypeID = 0;
		fread(&TypeID, sizeof(size_t), 1, File);

		CComponent* Component = CSceneManager::GetInst()->CallCreateComponent(this, TypeID);

		Component->Load(File);

		m_vecObjectComponent.push_back((CObjectComponent*)Component);

		CSceneManager::GetInst()->GetScene()->GetSceneMode()->AddComponentList(Component->GetName().c_str());
	}
}

void CGameObject::Save(const char* FullPath)
{
	FILE* File = nullptr;

	fopen_s(&File, FullPath, "wb");

	if (!File)
		return;

	Save(File);

	fclose(File);
}

void CGameObject::Load(const char* FullPath)
{
	FILE* File = nullptr;

	fopen_s(&File, FullPath, "rb");

	if (!File)
		return;

	Load(File);

	fclose(File);
}

void CGameObject::Save(const char* FileName, const std::string& PathName)
{
	char	FullPath[MAX_PATH] = {};

	const PathInfo* Info = CPathManager::GetInst()->FindPath(PathName);

	if (Info)
		strcpy_s(FullPath, Info->PathMultibyte);

	strcat_s(FullPath, FileName);

	Save(FullPath);
}

void CGameObject::Load(const char* FileName, const std::string& PathName)
{
	char	FullPath[MAX_PATH] = {};

	const PathInfo* Info = CPathManager::GetInst()->FindPath(PathName);

	if (Info)
		strcpy_s(FullPath, Info->PathMultibyte);

	strcat_s(FullPath, FileName);

	Load(FullPath);
}

bool CGameObject::EdgeTileCheck(CTileMapComponent* TileComponent, const Vector3& WorldPos, const Vector3& ColliderScale)
{
	CTile* Tile = TileComponent->GetTile(Vector3(WorldPos.x, WorldPos.y - ColliderScale.y, WorldPos.x));

	if (Tile && Tile->GetTileType() == Tile_Type::Edge)
		return true;

	return false;
}

//void CGameObject::Move(const Vector3& EndPos)
//{
//	size_t	Size = m_vecObjectComponent.size();
//
//	for (size_t i = 0; i < Size; ++i)
//	{
//		if (m_vecObjectComponent[i]->CheckType<CNavAgent>())
//		{
//			((CNavAgent*)m_vecObjectComponent[i].Get())->Move(EndPos);
//			break;
//		}
//	}
//}
