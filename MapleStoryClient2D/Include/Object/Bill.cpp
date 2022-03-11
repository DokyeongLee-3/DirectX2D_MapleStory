
#include "Bill.h"
#include "Animation/AnimationSequence2DInstance.h"
#include "TileObject.h"

CBill::CBill()	:
	m_JumpForce(350.f),
	m_EatByPlayer(false)
{
	SetTypeID<CBill>();

	m_Gravity = true;
	m_GravityFactor = 900.f;
	m_ItemCategory = Item_Category::Money;

	m_Money = rand() % 100 + 100;

	m_JumpForce = 320.f;
}

CBill::CBill(const CBill& obj)	:
	CGameObject(obj)
{
}

CBill::~CBill()
{
}

void CBill::Start()
{
	CGameObject::Start();
}

bool CBill::Init()
{
	m_Sprite = CreateComponent<CSpriteComponent>("BillSprite");
	m_Body = CreateComponent<CColliderBox2D>("Body");

	m_Body->SetCollisionProfile("Object");

	SetRootComponent(m_Sprite);
	m_Sprite->AddChild(m_Body);

	m_Sprite->SetTransparency(true);

	m_Sprite->CreateAnimationInstance<CAnimationSequence2DInstance>();

	m_Sprite->SetWorldScale(100.f, 100.f, 1.f);
	m_Sprite->SetPivot(0.5f, 0.5f, 0.f);
	m_Body->SetExtent(15.f, 15.f);

	m_Sprite->SetLayerName("MovingObjFront");
	m_Body->SetLayerName("MovingObjFront");
	m_Body->AddCollisionCallback<CBill>(Collision_State::Begin, this, &CBill::CollisionBeginCallback);


	CAnimationSequence2DInstance* Anim = m_Sprite->GetAnimationInstance();

	Anim->AddAnimation(TEXT("FloatingBill.sqc"), ANIMATION_PATH, "FloatingBill", true, 0.7f);

	m_Gravity = true;

	return true;
}

void CBill::Update(float DeltaTime)
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
}

void CBill::PostUpdate(float DeltaTime)
{
	CGameObject::PostUpdate(DeltaTime);
}

CBill* CBill::Clone()
{
	return new CBill(*this);
}

void CBill::CollisionBeginCallback(const CollisionResult& Result)
{
	CGameObject* Tile = Result.Dest->GetGameObject();

	if (Tile->GetTypeID() == typeid(CTileObject).hash_code())
	{
		CColliderBox2D* DestCollider = (CColliderBox2D*)Result.Dest;

		if (DestCollider->GetInfo().Center.y >= m_Body->GetInfo().Center.y)
			return;

		//m_GravityAccTime = 0.f;
		m_Gravity = false;
	}
}

void CBill::GetByPlayer()
{
	AddWorldPos(0.f, 10.f, 0.f);
	m_Gravity = true;
	m_GravityAccTime = 0.f;
	m_EatByPlayer = true;
}
