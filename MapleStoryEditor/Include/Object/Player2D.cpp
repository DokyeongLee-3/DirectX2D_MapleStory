
#include "Player2D.h"
#include "Scene/Scene.h"
#include "Input.h"
#include "Resource/Material/Material.h"
#include "Animation/AnimationSequence2DInstance.h"
#include "Collision/Collision.h"

CPlayer2D::CPlayer2D()	:
	m_Sprite(nullptr),
	m_OrbSprite(nullptr),
	m_BodyEffectSprite(nullptr),
	m_Body(nullptr),
	m_TransparencyTime(2.f),
	m_IsTransParent(false)
{
	SetTypeID<CPlayer2D>();
}

CPlayer2D::CPlayer2D(const CPlayer2D& obj) :
	CGameObject(obj)
{
	m_Sprite = (CSpriteComponent*)FindComponent("PlayerSprite");
	m_OrbSprite = (CSpriteComponent*)FindComponent("PlayerOrbSprite");
	m_BodyEffectSprite = (CSpriteComponent*)FindComponent("PlayerBodyEffectSprite");

	m_Body = (CColliderBox2D*)FindComponent("Body");
}

CPlayer2D::~CPlayer2D()
{
}

void CPlayer2D::Start()
{
	CGameObject::Start();
}

bool CPlayer2D::Init()
{
	m_Sprite = CreateComponent<CSpriteComponent>("PlayerSprite");
	m_OrbSprite = CreateComponent<CSpriteComponent>("PlayerOrbSprite");

	m_Body = CreateComponent<CColliderBox2D>("Body");
	m_Body->SetCollisionProfile("Player");
	m_BodyEffectSprite = CreateComponent<CSpriteComponent>("PlayerBodyEffectSprite");

	SetRootComponent(m_Sprite);

	m_Sprite->SetTransparency(true);
	m_OrbSprite->SetTransparency(true);

	m_BodyEffectSprite->SetTransparency(true);

	m_Sprite->CreateAnimationInstance<CAnimationSequence2DInstance>();
	m_OrbSprite->CreateAnimationInstance<CAnimationSequence2DInstance>();
	m_BodyEffectSprite->CreateAnimationInstance<CAnimationSequence2DInstance>();

	m_Sprite->AddChild(m_OrbSprite);
	m_Sprite->AddChild(m_Body);
	m_Sprite->AddChild(m_BodyEffectSprite);

	AddMotionAnimation();
	AddSkillAnimation();

	// Transform 조정 //

	m_Sprite->SetWorldScale(100.f, 100.f, 1.f);
	m_Sprite->SetWorldPos(200.f, 200.f, 0.f);
	m_Sprite->SetPivot(0.5f, 0.5f, 0.f);

	m_OrbSprite->SetWorldScale(40.f, 40.f, 1.f);
	m_OrbSprite->AddRelativePos(70.f, 10.f, 0.f);
	m_OrbSprite->SetPivot(0.5f, 0.5f, 0.f);

	m_BodyEffectSprite->AddRelativePos(3.f, 3.f, 0.f);
	m_BodyEffectSprite->SetWorldScale(40.f, 40.f, 1.f);
	m_BodyEffectSprite->SetPivot(0.5f, 0.5f, 0.f);

	

	CInput::GetInst()->CreateKey("Flip", 'F');
	CInput::GetInst()->SetKeyCallback<CPlayer2D>("Flip", KeyState_Down, this, &CPlayer2D::FlipAll);
	CInput::GetInst()->CreateKey("NextAnimation", 'N');
	CInput::GetInst()->SetKeyCallback<CPlayer2D>("NextAnimation", KeyState_Down, this, &CPlayer2D::NextAnimation);
	CInput::GetInst()->CreateKey("Transparent", 'T');
	CInput::GetInst()->SetKeyCallback<CPlayer2D>("Transparent", KeyState_Down, this, &CPlayer2D::TransformingLight);
	CInput::GetInst()->CreateKey("Rotate", VK_RIGHT);
	CInput::GetInst()->SetKeyCallback<CPlayer2D>("Rotate", KeyState_Push, this, &CPlayer2D::RotationZ);
	CInput::GetInst()->CreateKey("RotateInv", VK_LEFT);
	CInput::GetInst()->SetKeyCallback<CPlayer2D>("RotateInv", KeyState_Push, this, &CPlayer2D::RotationZInv);


	return true;
}

void CPlayer2D::Update(float DeltaTime)
{
	CGameObject::Update(DeltaTime);

	if (m_IsTransParent)
	{
		m_AccTransparencyTime += DeltaTime;

		if (m_AccTransparencyTime >= m_TransparencyTime)
		{
			m_AccTransparencyTime = 0.f;
			m_Sprite->SetOpacity(1.f);
			m_IsTransParent = false;
		}
	}
}

void CPlayer2D::PostUpdate(float DeltaTime)
{
	CGameObject::PostUpdate(DeltaTime);
}

CPlayer2D* CPlayer2D::Clone()
{
	return new CPlayer2D(*this);
}

void CPlayer2D::Save(FILE* File)
{
	CGameObject::Save(File);
}

void CPlayer2D::Load(FILE* File)
{
	CGameObject::Load(File);

	m_Sprite = (CSpriteComponent*)FindComponent("PlayerSprite");
	m_OrbSprite = (CSpriteComponent*)FindComponent("PlayerOrbSprite");
	m_BodyEffectSprite = (CSpriteComponent*)FindComponent("PlayerBodyEffectSprite");
}

void CPlayer2D::MoveUp(float DeltaTime)
{
	m_Sprite->AddRelativePos(m_Sprite->GetWorldAxis(AXIS_Y) * 300.f * DeltaTime);
}

void CPlayer2D::MoveDown(float DeltaTime)
{
	m_Sprite->AddRelativePos(m_Sprite->GetWorldAxis(AXIS_Y) * -300.f * DeltaTime);
}

void CPlayer2D::RotationZInv(float DeltaTime)
{
	m_Sprite->AddRelativeRotationZ(100.f * DeltaTime);
}

void CPlayer2D::RotationZ(float DeltaTime)
{
	m_Sprite->AddRelativeRotationZ(-100.f * DeltaTime);
}

void CPlayer2D::AddMotionAnimation()
{
	CAnimationSequence2DInstance* Anim = m_Sprite->GetAnimationInstance();

	Anim->AddAnimation(TEXT("PlayerIdleLeft.sqc"), ANIMATION_PATH, "IdleLeft", true);
	Anim->AddAnimation(TEXT("PlayerWalkLeft.sqc"), ANIMATION_PATH, "WalkLeft", true);
	Anim->AddAnimation(TEXT("PlayerHealLeft.sqc"), ANIMATION_PATH, "HealLeft", true);
	Anim->AddAnimation(TEXT("PlayerRope.sqc"), ANIMATION_PATH, "Rope", true);
	Anim->AddAnimation(TEXT("PlayerJumpLeft.sqc"), ANIMATION_PATH, "JumpLeft", true);
	Anim->AddAnimation(TEXT("PlayerDead.sqc"), ANIMATION_PATH, "PlayerDead", true);

	Anim = m_BodyEffectSprite->GetAnimationInstance();
	
	Anim->AddAnimation(TEXT("Blank.sqc"), ANIMATION_PATH, "Blank", false, 10.f);
	Anim->AddAnimation(TEXT("LightTransformingLeft.sqc"), ANIMATION_PATH, "LightTransformingLeft", false, 0.5f);

	Anim->FindAnimation("LightTransformingLeft")->SetEndFunction<CPlayer2D>(this, &CPlayer2D::ReturnToBlank);
}

void CPlayer2D::AddSkillAnimation()
{
	CAnimationSequence2DInstance* OrbAnim = m_OrbSprite->GetAnimationInstance();

	OrbAnim->AddAnimation(TEXT("PlayerOrb.sqc"), ANIMATION_PATH, "Orb", true, 1.f, 1.f, false);

	//CAnimationSequence2DInstance* BodyEffectAnim = m_BodyEffectSprite->GetAnimationInstance();

	//BodyEffectAnim->AddAnimation(TEXT("SylphideLancerBodyEffectLeft.sqc"), ANIMATION_PATH, "SylphideLancerBodyEffectLeft", true, 0.5f, 1.f, false);
}

void CPlayer2D::FlipAll(float DeltaTime)
{
	m_Sprite->Flip();

	float RelativeOrbXPos = m_OrbSprite->GetRelativePos().x;

	// 플레이어와 간격(절대값)을 유지하면서 플레이어 기준으로 위치만 반대로 이동 
	m_OrbSprite->SetRelativePos(-RelativeOrbXPos, m_OrbSprite->GetRelativePos().y, m_OrbSprite->GetRelativePos().z);
	// OrbSprite의 중심 기준으로 이미지 Flip
	m_OrbSprite->Flip();
}

void CPlayer2D::NextAnimation(float DeltaTime)
{
	CAnimationSequence2DInstance* Anim = m_Sprite->GetAnimationInstance();

	Anim->NextAnimation();
}

void CPlayer2D::TransformingLight(float DeltaTime)
{
	m_Sprite->SetOpacity(0.5f);

	m_IsTransParent = true;

	m_BodyEffectSprite->ChangeAnimation("LightTransformingLeft");
}

bool CPlayer2D::IsMouseCollision()
{
	CollisionResult SrcResult;
	CollisionResult DestResult;
	Vector2 MousePos = CInput::GetInst()->GetMouseWorld2DPos();

	if (CCollision::CollisionBox2DToPoint(SrcResult, DestResult, m_Body->GetInfo(), MousePos))
		return true;

	return false;
}

void CPlayer2D::ReturnToBlank()
{
	m_BodyEffectSprite->ChangeAnimation("Blank");
}

//void CPlayer2D::Fire(float DeltaTime)
//{
//	CSylphideLancerArrow* Bullet = m_Scene->CreateGameObject<CSylphideLancerArrow>("Bullet");
//
//	//Bullet->SetWorldPos(GetWorldPos() + GetWorldAxis(AXIS_Y) * 75.f);
//	Bullet->SetWorldPos(m_Sprite->GetWorldPos());
//	Bullet->SetWorldRotation(GetWorldRot());
//
//	if (m_Sprite->IsFlip())
//	{
//		Bullet->FlipAll(DeltaTime);
//	}
//}
