
#include "DragObject.h"
#include "Component/StaticMeshComponent.h"
#include "Animation/AnimationSequence2DInstance.h"

CDragObject::CDragObject()
{
}

CDragObject::CDragObject(const CDragObject& obj) :
	CGameObject(obj)
{
	m_Mesh = (CStaticMeshComponent*)FindComponent("Mesh");
}

CDragObject::~CDragObject()
{
}

void CDragObject::SetStartPos(const Vector2& Pos)
{
	m_StartPos = Pos;

	m_Mesh->SetWorldPos(m_StartPos.x, m_StartPos.y, 0.f);

	m_Mesh->SetWorldScale(m_EndPos.x - m_StartPos.x, m_EndPos.y - m_StartPos.y, 1.f);
}

void CDragObject::SetEndPos(const Vector2& Pos)
{
	m_EndPos = Pos;

	// 통상적으로 드래그하는 방식(좌상단에서 우하단)으로 드래그하면
	// Scaling의 y성분이 음수가 되게해서 우하단 방향으로 드래그 영역이 커지도록한다
	// y성분이 양수면 우상단 방향으로 드래그 영역이 Scaling될것이다
	m_Mesh->SetWorldScale(m_EndPos.x - m_StartPos.x, m_EndPos.y - m_StartPos.y, 1.f);
}

bool CDragObject::Init()
{
	m_Mesh = CreateComponent<CStaticMeshComponent>("Mesh");

	SetRootComponent(m_Mesh);

	m_Mesh->SetLayerName("DragLayer");
	m_Mesh->SetMesh("FrameRect");
	// Material은 원래 Color의 Material 그대로 쓰는데 
	// Shader만 교체해서 사용. Material은 CStaticMeshComponent::Init에서
	// 이미 Set해줬음
	m_Mesh->GetMaterial()->SetShader("PosMeshShader");
	m_Mesh->SetPivot(0.5f, 0.5f, 0.f);
	m_Mesh->SetBaseColor(1.f, 0.f, 0.f, 1.f);

	return true;
}

void CDragObject::Update(float DeltaTime)
{
	CGameObject::Update(DeltaTime);
}

void CDragObject::PostUpdate(float DeltaTime)
{
	CGameObject::PostUpdate(DeltaTime);
}

CDragObject* CDragObject::Clone()
{
	return new CDragObject(*this);
}
