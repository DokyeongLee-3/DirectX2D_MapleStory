
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
	m_Render = false;

	m_ColliderType = Collider_Type::Box2D;
}

CDragCollider::CDragCollider(const CDragCollider& com) :
	CColliderBox2D(com)
{
}

CDragCollider::~CDragCollider()
{
}

void CDragCollider::Start()
{
	SAFE_DELETE(m_CBuffer);

	Destroy();
}

CDragCollider* CDragCollider::Clone()
{
	return new CDragCollider(*this);
}
