
#include "Bill.h"
#include "Animation/AnimationSequence2DInstance.h"

CBill::CBill()	:
	m_JumpForce(350.f)
{
	m_Gravity = true;
	m_GravityFactor = 600.f;
	m_ItemCategory = Item_Category::Money;

	srand(0);
	m_Money = rand() % 100 + 100;
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

	m_Body->SetCollisionProfile("Monster");

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

	return true;
}

void CBill::Update(float DeltaTime)
{
	CGameObject::Update(DeltaTime);

	if (m_Gravity)
	{
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

void CBill::Save(FILE* File)
{
	CGameObject::Save(File);
}

void CBill::Load(FILE* File)
{
	CGameObject::Load(File);
}

void CBill::CollisionBeginCallback(const CollisionResult& Result)
{
	m_JumpForce = 0.f;
	m_GravityAccTime = 0.f;
	m_Gravity = false;
}
