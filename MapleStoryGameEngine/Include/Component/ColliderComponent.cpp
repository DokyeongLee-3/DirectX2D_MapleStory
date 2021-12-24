
#include "ColliderComponent.h"

CColliderComponent::CColliderComponent()
{
	SetTypeID<CColliderComponent>();
	m_ComponentType = Component_Type::SceneComponent;
	m_Render = true;
}

CColliderComponent::CColliderComponent(const CColliderComponent& com) :
	CSceneComponent(com)
{
}

CColliderComponent::~CColliderComponent()
{
}

void CColliderComponent::Start()
{
	CSceneComponent::Start();
}

bool CColliderComponent::Init()
{
	if (!CSceneComponent::Init())
		return false;

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
	return new CColliderComponent(*this);
}

void CColliderComponent::Save(FILE* File)
{
	CSceneComponent::Save(File);
}

void CColliderComponent::Load(FILE* File)
{
	CSceneComponent::Load(File);
}
