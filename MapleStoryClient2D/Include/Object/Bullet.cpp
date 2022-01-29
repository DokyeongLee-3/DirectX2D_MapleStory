#include "Bullet.h"
#include "Component/SpriteComponent.h"
#include "Animation/AnimationSequence2DInstance.h"

CBullet::CBullet() :
	m_Distance(0.f),
	m_Speed(300.f),
	m_Body(nullptr)
{
	SetTypeID<CBullet>();
}

CBullet::CBullet(const CBullet& obj) :
	CGameObject(obj)
{
	m_Sprite = (CSpriteComponent*)FindComponent("BulletSprite");
	m_Body = (CColliderBox2D*)FindComponent("Body");
	m_Distance = obj.m_Distance;
}

CBullet::~CBullet()
{
}

bool CBullet::Init()
{
	m_Sprite = CreateComponent<CSpriteComponent>("BulletSprite");

	SetRootComponent(m_Sprite);

	m_Sprite->SetRelativeScale(50.f, 50.f, 1.f);
	m_Sprite->SetPivot(0.5f, 0.5f, 0.f);
	m_Sprite->SetTransparency(true);

	m_Body = CreateComponent<CColliderBox2D>("Body");
	//m_Body->SetCollisionProfile("PlayerAttack");
	m_Body->SetExtent(40.f, 20.f);
	m_Body->AddCollisionCallback<CBullet>(Collision_State::Begin, this,
		&CBullet::CollisionBeginCallback);

	m_Sprite->AddChild(m_Body);

	m_Sprite->CreateAnimationInstance<CAnimationSequence2DInstance>();

	CAnimationSequence2DInstance* Anim = m_Sprite->GetAnimationInstance();

	Anim->AddAnimation(TEXT("SylphideLancerArrowLeft.sqc"), ANIMATION_PATH, "SylphideLancerArrowLeft", true, 0.3f);

	return true;
}

void CBullet::Update(float DeltaTime)
{
	CGameObject::Update(DeltaTime);

	Vector3 Axis = GetWorldAxis(AXIS::AXIS_X);

	if (m_Sprite->IsFlip())
		AddRelativePos(Axis * m_Speed * DeltaTime);
	
	else
		AddRelativePos(Vector3(-Axis.x, Axis.y, Axis.z) * m_Speed * DeltaTime);

	m_Distance += m_Speed * DeltaTime;

	if (m_Distance >= 500.f)
	{
		m_Distance = 0.f;
		Destroy();
	}
}

void CBullet::PostUpdate(float DeltaTime)
{
	CGameObject::PostUpdate(DeltaTime);
}

CBullet* CBullet::Clone()
{
	return new CBullet(*this);
}

void CBullet::SetCollisionProfile(const std::string& Name)
{
	m_Body->SetCollisionProfile(Name);
}

void CBullet::FlipAll(float DeltaTime)
{
	m_Sprite->Flip();
}

void CBullet::CollisionBeginCallback(const CollisionResult& result)
{
	Destroy();
}

void CBullet::CollisionEndCallback(const CollisionResult& result)
{
}
