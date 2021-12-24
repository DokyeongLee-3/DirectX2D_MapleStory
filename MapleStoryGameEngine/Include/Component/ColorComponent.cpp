
#include "ColorComponent.h"
#include "../Scene/Scene.h"
#include "../Scene/SceneResource.h"
#include "../Resource/Mesh/ColorMesh.h"

CColorComponent::CColorComponent()
{
	SetTypeID<CColorComponent>();
	m_Render = true;
}

CColorComponent::CColorComponent(const CColorComponent& com) :
	CSceneComponent(com)
{
	m_Mesh = com.m_Mesh;
}

CColorComponent::~CColorComponent()
{
}

void CColorComponent::Start()
{
	CSceneComponent::Start();
}

bool CColorComponent::Init()
{
	m_Mesh = (CColorMesh*)m_Scene->GetResource()->FindMesh("ColorMesh");
	m_Material = m_Scene->GetResource()->FindMaterial("Color");

	SetMeshSize(1.f, 1.f, 0.f);

	return true;
}

void CColorComponent::Update(float DeltaTime)
{
	CSceneComponent::Update(DeltaTime);
}

void CColorComponent::PostUpdate(float DeltaTime)
{
	CSceneComponent::PostUpdate(DeltaTime);
}

void CColorComponent::PrevRender()
{
	CSceneComponent::PrevRender();
}

void CColorComponent::Render()
{
	CSceneComponent::Render();

	m_Material->Render();

	m_Mesh->Render();
}

void CColorComponent::PostRender()
{
	CSceneComponent::PostRender();
}

CColorComponent* CColorComponent::Clone()
{
	return new CColorComponent(*this);
}
