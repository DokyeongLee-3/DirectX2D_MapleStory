
#include "SceneComponent.h"
#include "../Render/RenderManager.h"
#include "../GameObject/GameObject.h"
#include "../Resource/Shader/Standard2DConstantBuffer.h"
#include "../Scene/SceneManager.h"
#include "../Device.h"

CSceneComponent::CSceneComponent()	:
	m_ZOrder(0)
{
	SetTypeID<CSceneComponent>();

	m_ComponentType = Component_Type::SceneComponent;
	m_Render = false;

	m_Transform = new CTransform;

	m_Transform->m_Owner = this;
	m_Transform->Init();

	m_Parent = nullptr;

	m_LayerName = "Default";
}

CSceneComponent::CSceneComponent(const CSceneComponent& com) :
	CComponent(com)
{
	*this = com;
	m_Transform = com.m_Transform->Clone();

	m_Transform->m_Parent = nullptr;
	m_Transform->m_vecChild.clear();

	m_Transform->m_Owner = this;

	m_Parent = nullptr;

	m_vecChild.clear();

	size_t Size = com.m_vecChild.size();

	for (size_t i = 0; i < Size; ++i)
	{
		CSceneComponent* CloneComponent = com.m_vecChild[i]->Clone();

		CloneComponent->m_Parent = this;

		m_vecChild.push_back(CloneComponent);

		CloneComponent->m_Transform->m_Parent = m_Transform;

		m_Transform->m_vecChild.push_back(CloneComponent->m_Transform);
	}

}

CSceneComponent::~CSceneComponent()
{
	SAFE_DELETE(m_Transform);
}

void CSceneComponent::SetAllChildComponentScene(CScene* Scene)
{
	size_t Count = m_vecChild.size();

	for (size_t i = 0; i < Count; ++i)
	{
		m_vecChild[i]->SetAllChildComponentScene(Scene);
	}

	SetScene(Scene);
}

void CSceneComponent::SetSceneComponent(CGameObject* Object)
{
	Object->AddSceneComponent(this);

	size_t	Size = m_vecChild.size();

	for (size_t i = 0; i < Size; ++i)
	{
		m_vecChild[i]->SetSceneComponent(Object);
	}
}

void CSceneComponent::GetAllSceneComponentsName(std::vector<FindComponentName>& vecNames)
{
	FindComponentName Name;

	Name.Name = m_Name;

	if (m_Parent)
		Name.ParentName = m_Parent->GetName();

	vecNames.push_back(Name);

	size_t Size = m_vecChild.size();

	for (size_t i = 0; i < Size; ++i)
	{
		m_vecChild[i]->GetAllSceneComponentsName(vecNames);
	}
}

void CSceneComponent::Destroy()
{
	CComponent::Destroy();

	size_t	Size = m_vecChild.size();

	for (size_t i = 0; i < Size; ++i)
	{
		m_vecChild[i]->Destroy();
	}
}

void CSceneComponent::SetScene(CScene* Scene)
{
	CComponent::SetScene(Scene);

	m_Transform->m_Scene = Scene;

	size_t	Size = m_vecChild.size();

	for (size_t i = 0; i < Size; ++i)
	{
		m_vecChild[i]->SetScene(Scene);
	}
}

void CSceneComponent::SetGameObject(CGameObject* Object)
{
	CComponent::SetGameObject(Object);

	m_Transform->m_Object = Object;

	size_t	Size = m_vecChild.size();

	for (size_t i = 0; i < Size; ++i)
	{
		m_vecChild[i]->SetGameObject(Object);
	}
}

void CSceneComponent::AddChild(CSceneComponent* Child)
{
	if (!Child)
		nullptr;

	Child->m_Parent = this;

	m_vecChild.push_back(Child);

	Child->m_Transform->m_Parent = m_Transform;

	m_Transform->m_vecChild.push_back(Child->m_Transform);
}

bool CSceneComponent::DeleteChild(CSceneComponent* Child)
{
	size_t	Size = m_vecChild.size();

	for (size_t i = 0; i < Size; ++i)
	{
		if (m_vecChild[i] == Child)
		{
			size_t ChildSizeofChild = Child->m_vecChild.size();

			for (size_t i = 0; i < ChildSizeofChild; ++i)
			{
				Child->ClearChild();
			}

			auto	iter = m_vecChild.begin() + i;

			m_vecChild.erase(iter);

			auto	iterTr = m_Transform->m_vecChild.begin() + i;

			m_Transform->m_vecChild.erase(iterTr);
			return true;
		}
	}

	return false;
}

bool CSceneComponent::DeleteChild(const std::string& Name)
{
	size_t	Size = m_vecChild.size();

	for (size_t i = 0; i < Size; ++i)
	{
		if (m_vecChild[i]->GetName() == Name)
		{
			size_t OriginSize = m_vecChild.size();

			auto	iter = m_vecChild.begin() + i;
			(*iter)->Destroy();

			m_vecChild.erase(iter);

			auto	iterTr = m_Transform->m_vecChild.begin() + i;

			m_Transform->m_vecChild.erase(iterTr);

			Size = m_vecChild.size();

			return true;
		}

		if (m_vecChild[i]->DeleteChild(Name))
			return true;
	}

	return false;
}

void CSceneComponent::ClearChild()
{
	int ChildCount = (int)m_vecChild.size();
	
	for (int i = 0; i < ChildCount; ++i)
	{
		DeleteChild(m_vecChild[i]);
		ChildCount = (int)m_vecChild.size();
	}

	Destroy();
}

CSceneComponent* CSceneComponent::FindComponent(const std::string& Name)
{
	if (m_Name == Name)
		return this;

	size_t	Size = m_vecChild.size();

	for (size_t i = 0; i < Size; ++i)
	{
		CSceneComponent* Find = m_vecChild[i]->FindComponent(Name);

		if (Find)
			return Find;
	}

	return nullptr;
}

void CSceneComponent::SetAllSceneComponentsLayer(const std::string& Name)
{
	SetLayerName(Name);

	size_t Count = m_vecChild.size();

	for (size_t i = 0; i < Count; ++i)
	{
		m_vecChild[i]->SetAllSceneComponentsLayer(Name);
	}
}

void CSceneComponent::Start()
{
	CComponent::Start();
	size_t	Size = m_vecChild.size();

	for (size_t i = 0; i < Size; ++i)
	{
		m_vecChild[i]->Start();
	}


	if (!IsActive())
	{
		ClearChild();
		return;
	}


	Size = m_vecChild.size();

	for (size_t i = 0; i < Size; ++i)
	{
		if (!m_vecChild[i]->IsActive())
		{
			DeleteChild(m_vecChild[i]);
			Size = m_vecChild.size();
			continue;
		}
	}
}

bool CSceneComponent::Init()
{
	return true;
}

void CSceneComponent::Update(float DeltaTime)
{
	if (!IsActive())
	{
		ClearChild();
	}

	m_Transform->Update(DeltaTime);

	size_t	Size = m_vecChild.size();

	for (size_t i = 0; i < Size; ++i)
	{
		if (!m_vecChild[i]->IsActive())
		{
			DeleteChild(m_vecChild[i]);
			Size = m_vecChild.size();
			continue;
		}

		m_vecChild[i]->Update(DeltaTime);
	}

}

void CSceneComponent::PostUpdate(float DeltaTime)
{
	//if (!IsActive())
	//{
	//	ClearChild();
	//}

	m_Transform->PostUpdate(DeltaTime);

	size_t	Size = m_vecChild.size();

	for (size_t i = 0; i < Size; ++i)
	{
		m_vecChild[i]->PostUpdate(DeltaTime);
	}

}

void CSceneComponent::CheckCollision()
{
	size_t	Size = m_vecChild.size();

	for (size_t i = 0; i < Size; ++i)
	{
		m_vecChild[i]->CheckCollision();
	}
}

void CSceneComponent::PrevRender()
{
	CCameraComponent* Camera = m_Scene->GetCameraManager()->GetCurrentCamera();

	// Culling으로 최적화하기
	if (Camera)
	{
		Vector3 WorldPos = GetWorldPos();
		Vector3 Pivot = GetPivot();
		Vector3 WorldScale = GetWorldScale();
		Resolution RS = CDevice::GetInst()->GetResolution();
		Vector3 CamPos = Camera->GetWorldPos();


		Vector2 LB = Vector2(WorldPos.x - WorldScale.x * Pivot.x, WorldPos.y - WorldScale.y * Pivot.y);
		Vector2 RT = Vector2(WorldPos.x + WorldScale.x, WorldPos.y + WorldScale.y);

		if (LB.x > CamPos.x + (float)RS.Width || LB.y > CamPos.y + (float)RS.Height)
		{
			std::string Name = GetGameObject()->GetName();
			int a = 3;
			return;
		}

		if (RT.x < CamPos.x || RT.y < CamPos.y)
		{
			std::string Name = GetGameObject()->GetName();
			int a = 3;
			return;
		}
	}

	if (m_Render)
	{
		CRenderManager::GetInst()->AddRenderList(this);
	}

	size_t	Size = m_vecChild.size();

	for (size_t i = 0; i < Size; ++i)
	{
		m_vecChild[i]->PrevRender();
	}
}

// Transform정보를 담고 있는 상수버퍼를 
// 렌더 직전에 셰이더로 넘겨준다
void CSceneComponent::Render()
{
	if (!m_Enable)
		return;

	m_Transform->SetTransform();

	CRenderManager::GetInst()->GetStandard2DCBuffer()->SetAnimation2DEnable(false);
	CRenderManager::GetInst()->GetStandard2DCBuffer()->UpdateCBuffer();
}

void CSceneComponent::PostRender()
{
}

CSceneComponent* CSceneComponent::Clone()
{
	return new CSceneComponent(*this);
}

void CSceneComponent::Save(FILE* File)
{
	CComponent::Save(File);

	fwrite(&m_Render, sizeof(bool), 1, File);

	int	Length = (int)m_LayerName.length();
	fwrite(&Length, sizeof(int), 1, File);
	fwrite(m_LayerName.c_str(), sizeof(char), Length, File);
	fwrite(&m_ZOrder, sizeof(int), 1, File);

	m_Transform->Save(File);

	int	ChildCount = (int)m_vecChild.size();

	fwrite(&ChildCount, sizeof(int), 1, File);

	for (int i = 0; i < ChildCount; ++i)
	{
		size_t	TypeID = m_vecChild[i]->GetTypeID();
		fwrite(&TypeID, sizeof(size_t), 1, File);

		m_vecChild[i]->Save(File);
	}
}

void CSceneComponent::Load(FILE* File)
{
	CComponent::Load(File);

	fread(&m_Render, sizeof(bool), 1, File);

	int	Length = 0;
	char	LayerName[256] = {};
	fread(&Length, sizeof(int), 1, File);
	fread(LayerName, sizeof(char), Length, File);
	fread(&m_ZOrder, sizeof(int), 1, File);

	m_LayerName = LayerName;

	m_Transform->Load(File);

	int	ChildCount = 0;

	fread(&ChildCount, sizeof(int), 1, File);

	for (int i = 0; i < ChildCount; ++i)
	{
		size_t	TypeID = 0;
		fread(&TypeID, sizeof(size_t), 1, File);

		CComponent* Component = CSceneManager::GetInst()->CallCreateComponent(m_Object, TypeID);

		Component->Load(File);

		AddChild((CSceneComponent*)Component);

		CScene* NextScene = CSceneManager::GetInst()->GetNextScene();

		if (NextScene)
			Component->SetScene(NextScene);
		else
			Component->SetScene(m_Scene);


		//CSceneManager::GetInst()->GetScene()->GetSceneMode()->AddComponentList(Component->GetName().c_str());
	}

}

