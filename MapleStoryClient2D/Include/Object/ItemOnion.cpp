
#include "ItemOnion.h"
#include "TileObject.h"
#include "StaticMapObj.h"

CItemOnion::CItemOnion()	:
	m_JumpForce(350.f),
	m_EatByPlayer(false),
	m_AccTime(0.f),
	m_FloatingDir(1.f)
{
	SetTypeID<CItemOnion>();

	m_Gravity = true;
	m_GravityFactor = 900.f;
	m_ItemCategory = Item_Category::Etc;

	m_JumpForce = 320.f;
}

CItemOnion::CItemOnion(const CItemOnion& obj)	:
	CGameObject(obj)
{
}

CItemOnion::~CItemOnion()
{
}

void CItemOnion::Start()
{
	CGameObject::Start();
}

bool CItemOnion::Init()
{
	m_Sprite = CreateComponent<CSpriteComponent>("ItemOnionSprite");
	m_Body = CreateComponent<CColliderBox2D>("Body");

	m_Body->SetCollisionProfile("Object");

	SetRootComponent(m_Sprite);
	m_Sprite->AddChild(m_Body);

	m_Sprite->SetTransparency(true);

	m_Sprite->SetWorldScale(100.f, 100.f, 1.f);
	m_Sprite->SetPivot(0.5f, 0.5f, 0.f);
	m_Body->SetExtent(15.f, 15.f);

	m_Sprite->SetLayerName("MovingObjFront");
	m_Body->SetLayerName("MovingObjFront");
	m_Body->AddCollisionCallback<CItemOnion>(Collision_State::Begin, this, &CItemOnion::CollisionBeginCallback);

	SetSpriteTexture("ItemOnion", TEXT("Item/04000996.info.icon.png"));

	m_Gravity = true;

	return true;
}

void CItemOnion::Update(float DeltaTime)
{
	CGameObject::Update(DeltaTime);

	if (m_Gravity && !m_EatByPlayer)
	{
		AddWorldPos(0.f, m_JumpForce * DeltaTime, 0.f);
	}

	else if (m_EatByPlayer)
	{
		if (m_JumpForce * DeltaTime < m_GravityAccTime * m_GravityFactor * DeltaTime)
		{
			m_Sprite->AddOpacity(-0.007f);

			if (m_Sprite->GetOpacity() <= 0.f)
				Destroy();
		}


		AddWorldPos(0.f, m_JumpForce * DeltaTime, 0.f);
	}

	else if (!m_Gravity && !m_EatByPlayer)
	{
		m_AccTime += DeltaTime;

		if (m_AccTime >= 0.7f)
		{
			m_AccTime = 0.f;
			m_FloatingDir *= -1.f;
		}

		AddWorldPos(0.f, m_FloatingDir * 5.f * DeltaTime, 0.f);
	}
}

void CItemOnion::PostUpdate(float DeltaTime)
{
	CGameObject::PostUpdate(DeltaTime);
}

CItemOnion* CItemOnion::Clone()
{
	return new CItemOnion(*this);
}

void CItemOnion::CollisionBeginCallback(const CollisionResult& Result)
{
	CGameObject* Tile = Result.Dest->GetGameObject();

	if (Tile->GetTypeID() == typeid(CTileObject).hash_code()
		|| Tile->GetTypeID() == typeid(CStaticMapObj).hash_code())
	{
		CColliderBox2D* DestCollider = (CColliderBox2D*)Result.Dest;

		if (DestCollider->GetInfo().Center.y >= m_Body->GetInfo().Center.y)
			return;

		//m_GravityAccTime = 0.f;
		m_Gravity = false;
	}
}

void CItemOnion::GetByPlayer()
{
	AddWorldPos(0.f, 10.f, 0.f);
	m_Gravity = true;
	m_GravityAccTime = 0.f;
	m_EatByPlayer = true;
}

void CItemOnion::SetSpriteTexture(const std::string& Name, const TCHAR* FileName)
{
	m_Sprite->SetTexture(0, 0, (int)Buffer_Shader_Type::Pixel, Name, FileName);

	float Width = (float)m_Sprite->GetMaterial()->GetTextureWidth();
	float Height = (float)m_Sprite->GetMaterial()->GetTextureHeight();

	m_Sprite->SetWorldScale(Width, Height, 1.f);
}
