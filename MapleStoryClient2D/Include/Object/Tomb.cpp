
#include "Tomb.h"
#include "TileObject.h"
#include "Scene/Scene.h"
#include "StaticMapObj.h"

CTomb::CTomb()
{
	m_GravityFactor = 1000.f;
	m_Gravity = true;
}

CTomb::CTomb(const CTomb& obj)	:
	CGameObject(obj)
{
}

CTomb::~CTomb()
{
}

void CTomb::Start()
{
	CGameObject::Start();
}

bool CTomb::Init()
{
	m_Sprite = CreateComponent<CSpriteComponent>("TombSprite");
	m_Body = CreateComponent<CColliderBox2D>("Body");

	m_Body->SetCollisionProfile("Object");

	SetRootComponent(m_Sprite);
	m_Sprite->AddChild(m_Body);

	m_Sprite->SetTransparency(true);

	m_Sprite->SetPivot(0.5f, 0.5f, 0.f);
	m_Body->SetExtent(15.f, 15.f);

	m_Sprite->SetLayerName("MovingObjFront");
	m_Body->SetLayerName("MovingObjFront");

	m_Sprite->SetTexture(0, 0, (int)Buffer_Shader_Type::Pixel, "Tomb", TEXT("UI/Die/fall.1.png"));
	m_Sprite->SetWorldScale(40.f, 43.f, 1.f);

	m_Body->AddCollisionCallback<CTomb>(Collision_State::Begin, this, &CTomb::CollisionBeginCallback);

	return true;
}

void CTomb::Update(float DeltaTime)
{
	CGameObject::Update(DeltaTime);
}

void CTomb::PostUpdate(float DeltaTime)
{
	CGameObject::PostUpdate(DeltaTime);
}

CTomb* CTomb::Clone()
{
	return new CTomb(*this);
}

void CTomb::CollisionBeginCallback(const CollisionResult& Result)
{
	CGameObject* DestObj = Result.Dest->GetGameObject();

	if (DestObj->GetTypeID() == typeid(CTileObject).hash_code())
	{
		Vector3 DestPos = DestObj->GetWorldPos();
		Vector3 PlayerPos = m_Scene->GetPlayerObject()->GetWorldPos();

		if (DestPos.y <= PlayerPos.y)
		{
			m_Gravity = false;
			m_GravityAccTime = 0.f;
		}
	}

	else if (DestObj->GetTypeID() == typeid(CStaticMapObj).hash_code())
	{
		Vector3 DestPos = DestObj->GetWorldPos();
		Vector3 PlayerPos = m_Scene->GetPlayerObject()->GetWorldPos();

		if (DestPos.y <= PlayerPos.y)
		{
			m_Gravity = false;
			m_GravityAccTime = 0.f;
		}
	}
}
