
#include "DragCollider.h"
#include "Component/ColliderBox2D.h"
#include "Collision/Collision.h"
#include "Scene/Scene.h"
#include "Scene/SceneResource.h"
#include "Resource/Shader/ColliderConstantBuffer.h"
#include "Scene/CameraManager.h"


CDragCollider::CDragCollider()
{
	SetTypeID<CDragCollider>();
	m_ComponentType = Component_Type::SceneComponent;
	m_Render = true;

	m_ColliderType = Collider_Type::Box2D;
}

CDragCollider::CDragCollider(const CDragCollider& com) :
	CColliderBox2D(com)
{
}

CDragCollider::~CDragCollider()
{
}

void CDragCollider::SetColliderColor(const Vector4& Color)
{
	if(m_CBuffer)
		m_CBuffer->SetColliderColor(Color);
}

void CDragCollider::Start()
{
	CColliderBox2D::Start();
}

void CDragCollider::Render()
{
	CColliderComponent::Render();

	CCameraComponent* Camera = m_Scene->GetCameraManager()->GetCurrentCamera();

	Matrix matWorld, matView, matProj, matWVP;

	matView = Camera->GetViewMatrix();
	matProj = Camera->GetProjMatrix();

	Matrix matScale, matRot, matTrans;

	matScale.Scaling(m_Info.Length.x * 2.f, m_Info.Length.y * 2.f, 1.f);
	matRot.Rotation(GetWorldRot());
	matTrans.Translation(m_Info.Center);

	matWorld = matScale * matRot * matTrans;

	matWVP = matWorld * matView * matProj;

	matWVP.Transpose();

	m_CBuffer->SetWVP(matWVP);

	if (m_MouseCollision)
		m_CBuffer->SetColliderColor(Vector4(1.f, 0.f, 0.f, 1.f));

	else
		m_CBuffer->SetColliderColor(Vector4(0.f, 0.f, 1.f, 1.f));

	m_CBuffer->UpdateCBuffer();

	m_Shader->SetShader();

	m_Mesh->Render();
}



CDragCollider* CDragCollider::Clone()
{
	return new CDragCollider(*this);
}
