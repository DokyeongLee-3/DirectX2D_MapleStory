
#include "ColliderComponent.h"
#include "../Scene/Scene.h"
#include "../Scene/SceneCollision.h"
#include "../Collision/CollisionManager.h"
#include "../Resource/Shader/ColliderConstantBuffer.h"

CColliderComponent::CColliderComponent()
{
	SetTypeID<CColliderComponent>();
	m_ComponentType = Component_Type::SceneComponent;
	m_Render = true;

	m_CurrentSectionCheck = false;
	m_Profile = nullptr;
	m_MouseCollision = false;
	m_Mesh = nullptr;
	m_CBuffer = nullptr;
}

CColliderComponent::CColliderComponent(const CColliderComponent& com) :
	CSceneComponent(com)
{
	m_MouseCollision = false;
	m_CurrentSectionCheck = false;
	m_Render = com.m_Render;
	m_Profile = com.m_Profile;
	m_CBuffer = com.m_CBuffer->Clone();
	m_Mesh = com.m_Mesh;
	m_Shader = com.m_Shader;
}

CColliderComponent::~CColliderComponent()
{
	SAFE_DELETE(m_CBuffer);

	auto	iter = m_PrevCollisionList.begin();
	auto	iterEnd = m_PrevCollisionList.end();

	for (; iter != iterEnd; ++iter)
	{
		(*iter)->CallCollisionCallback(Collision_State::End);
		CallCollisionCallback(Collision_State::End);
		(*iter)->DeletePrevCollision(this);


		// ���� ���� ���ŵǴ� ���� �浹�ƴ� ��󿡰� m_Result�� ���� �����س��� ������Ѵ�
		(*iter)->m_Result.Dest = nullptr;
		
	}
}

void CColliderComponent::SetCollisionProfile(const std::string& Name)
{
	m_Profile = CCollisionManager::GetInst()->FindProfile(Name);
}

void CColliderComponent::CheckPrevColliderSection()
{
	auto	iter = m_PrevCollisionList.begin();
	auto	iterEnd = m_PrevCollisionList.end();

	// ���������ӿ� �浹�Ǿ��� �浹ü���� �ݺ��ϸ� �ϳ��� ��ġ�� Section�� �ִ��� Ȯ���Ѵ�
	// ���� ���������ӿ� �浹�� �Ǿ��µ� ���� ��ġ�� Section�� �ϳ��� ���ٸ� �ش� �浹ü�ʹ� �浹 ���ɼ���
	// �����Ƿ� �浹�Ǿ��ٰ� ���������̴�
	for (; iter != iterEnd;)
	{
		bool	Check = false;

		// �浹������ üũ�Ѵ�.
		size_t	Size = m_vecSectionIndex.size();

		for (size_t i = 0; i < Size; ++i)
		{
			size_t	DestSize = (*iter)->m_vecSectionIndex.size();

			for (size_t j = 0; j < DestSize; ++j)
			{
				if (m_vecSectionIndex[i] == (*iter)->m_vecSectionIndex[j])
				{
					Check = true;
					break;
				}
			}

			if (Check)
				break;
		}

		// ���� ��ġ�� �浹������ �����Ƿ� �پ��ִٰ� �������� ���̴�.
		if (!Check)
		{
			CColliderComponent* SrcResultDestComp = m_Result.Dest;
			CColliderComponent* DestResultDestComp = (*iter)->GetCollisionResult().Dest;

			// ���� iteration���� A�� B�� �浹�ؼ� ������ m_Result�� ���θ� �����س��� ���� iteration���� A�� C�� �浹 �˻縦 �غ���
			// ���� �� ������ ���� ���� else if�� �������� A�� m_Result.Dest�� B�� �����Ǿ������ǵ� ���⼭ �׳�
			// CollisionEnd�� �ݹ��� ȣ���ϸ� A�� m_Result.Dest���� ������ B�� ���־ CollisionEnd���ڷε� Result.Dest�� B�� �Ѱ��ٰ��̹Ƿ�
			// �װ� �߸��� ���̹Ƿ� CollisionEnd ȣ��ÿ� ���� �� �������� ����� ���ڷ� �־��ְ� CollisionEnd �ݹ��� ������ ���󺹱� �����ش�
			if (m_Result.Dest != (*iter))
			{
				m_Result.Dest = *iter;
			}

			if ((*iter)->GetCollisionResult().Dest != this)
			{
				(*iter)->SetDestCollisionResult(this);
			}

			CallCollisionCallback(Collision_State::End);
			(*iter)->CallCollisionCallback(Collision_State::End);

			// ���󺹱�
			m_Result.Dest = SrcResultDestComp;
			(*iter)->SetDestCollisionResult(DestResultDestComp);

			// ���� ���� �浹��Ͽ��� �������ش�.
			(*iter)->DeletePrevCollision(this);

			iter = m_PrevCollisionList.erase(iter);
			iterEnd = m_PrevCollisionList.end();
			continue;
		}

		++iter;
	}
}

void CColliderComponent::AddPrevCollision(CColliderComponent* Collider)
{
	m_PrevCollisionList.push_back(Collider);
}

void CColliderComponent::DeletePrevCollision(CColliderComponent* Collider)
{
	auto	iter = m_PrevCollisionList.begin();
	auto	iterEnd = m_PrevCollisionList.end();

	for (; iter != iterEnd; ++iter)
	{
		if (*iter == Collider)
		{
			m_PrevCollisionList.erase(iter);
			return;
		}
	}
}

bool CColliderComponent::EmptyPrevCollision()
{
	return m_PrevCollisionList.empty();
}

bool CColliderComponent::CheckPrevCollision(CColliderComponent* Collider)
{
	auto	iter = m_PrevCollisionList.begin();
	auto	iterEnd = m_PrevCollisionList.end();

	for (; iter != iterEnd; ++iter)
	{
		if (*iter == Collider)
			return true;
	}

	return false;
}

bool CColliderComponent::CheckPrevCollisionGameObjectType(size_t TypeID)
{
	auto	iter = m_PrevCollisionList.begin();
	auto	iterEnd = m_PrevCollisionList.end();

	for (; iter != iterEnd; ++iter)
	{
		if ((*iter)->GetGameObject()->GetTypeID() == TypeID)
			return true;
	}

	return false;
}

bool CColliderComponent::CheckCurrentFrameCollision(CColliderComponent* Collider)
{
	auto	iter = m_CurrentCollisionList.begin();
	auto	iterEnd = m_CurrentCollisionList.end();

	for (; iter != iterEnd; ++iter)
	{
		if (*iter == Collider)
			return true;
	}

	return false;
}

void CColliderComponent::AddCurrentFrameCollision(CColliderComponent* Collider)
{
	if (!CheckCurrentFrameCollision(Collider))
		m_CurrentCollisionList.push_back(Collider);
}

void CColliderComponent::CallCollisionCallback(Collision_State State)
{
	auto	iter = m_CollisionCallback[(int)State].begin();
	auto	iterEnd = m_CollisionCallback[(int)State].end();

	for (; iter != iterEnd; ++iter)
	{
		(*iter)(m_Result);
	}
}

void CColliderComponent::CallCollisionMouseCallback(Collision_State State)
{
	if (State == Collision_State::End)
		m_MouseCollision = false;

	auto	iter = m_CollisionMouseCallback[(int)State].begin();
	auto	iterEnd = m_CollisionMouseCallback[(int)State].end();

	for (; iter != iterEnd; ++iter)
	{
		(*iter)(m_MouseResult);
	}
}

void CColliderComponent::ClearFrame()
{
	m_vecSectionIndex.clear();
	m_CurrentCollisionList.clear();
	m_CurrentSectionCheck = false;
}

void CColliderComponent::Start()
{
	CSceneComponent::Start();

	// CColliderComponent::Start�� �ι� ȣ��ǰ� ����
	m_Scene->GetCollision()->AddCollider(this);
}

bool CColliderComponent::Init()
{
	if (!CSceneComponent::Init())
		return false;

	SetCollisionProfile("Object");

	m_CBuffer = new CColliderConstantBuffer;

	m_CBuffer->Init();

	m_CBuffer->SetColliderColor(Vector4(0.f, 1.f, 0.f, 1.f));

	m_Shader = CResourceManager::GetInst()->FindShader("ColliderShader");

	return true;
}

void CColliderComponent::Update(float DeltaTime)
{
	CSceneComponent::Update(DeltaTime);
}

void CColliderComponent::PostUpdate(float DeltaTime)
{
	CSceneComponent::PostUpdate(DeltaTime);
}

void CColliderComponent::CheckCollision()
{
	CSceneComponent::CheckCollision();
}

void CColliderComponent::PrevRender()
{
	CSceneComponent::PrevRender();
}

void CColliderComponent::Render()
{
	CSceneComponent::Render();
}

void CColliderComponent::PostRender()
{
	CSceneComponent::PostRender();
}

CColliderComponent* CColliderComponent::Clone()
{
	return nullptr;
}

void CColliderComponent::Save(FILE* File)
{
	CSceneComponent::Save(File);

	std::string	MeshName = m_Mesh->GetName();

	int	Length = (int)MeshName.length();

	fwrite(&Length, sizeof(int), 1, File);
	fwrite(MeshName.c_str(), sizeof(char), Length, File);

	std::string	ShaderName = m_Shader->GetName();

	Length = (int)ShaderName.length();

	fwrite(&Length, sizeof(int), 1, File);
	fwrite(ShaderName.c_str(), sizeof(char), Length, File);

	fwrite(&m_ColliderType, sizeof(Collider_Type), 1, File);
	fwrite(&m_Offset, sizeof(Vector3), 1, File);
	fwrite(&m_Min, sizeof(Vector3), 1, File);
	fwrite(&m_Max, sizeof(Vector3), 1, File);

	// �߰��� �κ�
	std::string ProfileName = m_Profile->Name;
	Length = (int)ProfileName.length();

	fwrite(&Length, sizeof(int), 1, File);
	fwrite(ProfileName.c_str(), sizeof(char), Length, File);
}

void CColliderComponent::Load(FILE* File)
{
	CSceneComponent::Load(File);

	char	MeshName[256] = {};

	int	Length = 0;

	fread(&Length, sizeof(int), 1, File);
	fread(MeshName, sizeof(char), Length, File);

	m_Mesh = (CMesh*)m_Scene->GetResource()->FindMesh(MeshName);

	char	ShaderName[256] = {};

	Length = 0;

	fread(&Length, sizeof(int), 1, File);
	fread(ShaderName, sizeof(char), Length, File);

	m_Shader = m_Scene->GetResource()->FindShader(ShaderName);

	m_Mesh = (CMesh*)m_Scene->GetResource()->FindMesh(MeshName);

	m_CBuffer = CreateEmptyBuffer<CColliderConstantBuffer>();

	fread(&m_ColliderType, sizeof(Collider_Type), 1, File);
	fread(&m_Offset, sizeof(Vector3), 1, File);
	fread(&m_Min, sizeof(Vector3), 1, File);
	fread(&m_Max, sizeof(Vector3), 1, File);

	// �߰��� �κ�
	char ProfileName[256] = {};

	fread(&Length, sizeof(int), 1, File);
	fread(ProfileName, sizeof(char), Length, File);

	SetCollisionProfile(ProfileName);
}
