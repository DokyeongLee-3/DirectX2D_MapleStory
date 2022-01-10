
#include "Player2D.h"
#include "BulletTornaido.h"
#include "Scene/Scene.h"
#include "Input.h"
#include "PlayerAnimation2D.h"
#include "SylphideLancerEffectAnimation2D.h"
#include "PlayerSkillBodyEffect.h"
#include "BulletCamera.h"
#include "Bullet.h"

CPlayer2D::CPlayer2D()	:
	m_BodySprite(nullptr),
	m_SylphideLancerMuzzle(nullptr),
	m_SylphideLancerMirror(nullptr),
	m_SkillBodyEffect(nullptr),
	m_ScaleFactor(1.f),
	m_Opacity(1.f)
{
	SetTypeID<CPlayer2D>();
}

CPlayer2D::CPlayer2D(const CPlayer2D& obj) :
	CGameObject(obj)
{
	m_BodySprite = (CSpriteComponent*)FindComponent("PlayerSprite");
	m_SylphideLancerMuzzle = (CSceneComponent*)FindComponent("PlayerSylphideLancerMuzzle");
	m_SylphideLancerMirror = (CSpriteComponent*)FindComponent("PlayerSylphideLancerMirror");
	m_SkillBodyEffect = (CSpriteComponent*)FindComponent("PlayerSkillBodyEffect");
	m_VoidPressureAttackSphere = (CSpriteComponent*)FindComponent("PlayerVoidPressureAttackSphere");
	m_VoidPressureSphere = (CSpriteComponent*)FindComponent("PlayerVoidPressureSphere");

	m_Body = (CColliderBox2D*)FindComponent("Body");

	m_Opacity = obj.m_Opacity;
}

CPlayer2D::~CPlayer2D()
{
}

bool CPlayer2D::Init()
{
	m_BodySprite = CreateComponent<CSpriteComponent>("PlayerSprite");
	m_SylphideLancerMuzzle = CreateComponent<CSceneComponent>("PlayerSylphideLancerMuzzle");
	/*
	m_SylphideLancerMirror = CreateComponent<CSpriteComponent>("PlayerSylphideLancerMirror");
	m_SkillBodyEffect = CreateComponent<CSpriteComponent>("PlayerSkillBodyEffect");
	m_VoidPressureAttackSphere = CreateComponent<CSpriteComponent>("PlayerVoidPressureAttackSphere");
	m_VoidPressureSphere = CreateComponent<CSpriteComponent>("PlayerVoidPressureSphere");
	*/
	
	m_Body = CreateComponent<CColliderBox2D>("Body");

	m_Camera = CreateComponent<CCameraComponent>("Camera");

	SetRootComponent(m_BodySprite);

	m_Body->SetCollisionProfile("Player");

	m_Camera->OnViewportCenter();

	m_BodySprite->AddChild(m_SylphideLancerMuzzle);
	//m_BodySprite->AddChild(m_SkillBodyEffect);
	//m_BodySprite->AddChild(m_VoidPressureAttackSphere);
	//m_BodySprite->AddChild(m_VoidPressureSphere);
	
	m_BodySprite->AddChild(m_Body);

	m_BodySprite->AddChild(m_Camera);

	//m_SylphideLancerMuzzle->AddChild(m_SylphideLancerMirror);


	m_BodySprite->SetTransparency(true);
	//m_SylphideLancerMirror->SetTransparency(true);
	//m_SkillBodyEffect->SetTransparency(true);

	m_BodySprite->CreateAnimationInstance<CPlayerAnimation2D>();
	//m_SylphideLancerMirror->CreateAnimationInstance<CSylphideLancerEffectAnimation2D>();
	//m_SkillBodyEffect->CreateAnimationInstance<CPlayerSkillBodyEffect>();

	// Stand상태 기준의 Scale
	m_BodySprite->SetRelativeScale(Vector3(91.f, 82.f, 1.f) * m_ScaleFactor);
	m_BodySprite->SetRelativePos(100.f, 100.f, 0.f);
	m_BodySprite->SetPivot(0.5f, 0.5f, 0.f);

	m_SylphideLancerMuzzle->SetInheritRotZ(true);
	m_SylphideLancerMuzzle->SetPivot(0.5f, 0.5f, 0.f);

	//m_SylphideLancerMirror->SetRelativeScale(120.f, 200.f, 1.f);
	//m_SylphideLancerMirror->SetOpacity(0.f);
	//m_SylphideLancerMirror->SetInheritRotZ(true);
	//m_SylphideLancerMirror->SetPivot(0.5f, 0.5f, 0.f);

	//m_SkillBodyEffect->SetRelativePos(0.f, 0.f, 0.f);
	//m_SkillBodyEffect->SetWorldScale(100.f, 100.f, 1.f);
	//m_SkillBodyEffect->SetPivot(0.5f, 0.5f, 0.f);


	CInput::GetInst()->SetKeyCallback<CPlayer2D>("MoveUp", KeyState_Push, this, &CPlayer2D::MoveUp);
	CInput::GetInst()->SetKeyCallback<CPlayer2D>("MoveDown", KeyState_Push, this, &CPlayer2D::MoveDown);
	CInput::GetInst()->SetKeyCallback<CPlayer2D>("RotationZInv", KeyState_Push, this, &CPlayer2D::RotationZInv);
	CInput::GetInst()->SetKeyCallback<CPlayer2D>("RotationZ", KeyState_Push, this, &CPlayer2D::RotationZ);
	CInput::GetInst()->SetKeyCallback<CPlayer2D>("Skill1", KeyState_Down, this, &CPlayer2D::SylphideLancer);

	CInput::GetInst()->SetKeyCallback<CPlayer2D>("Skill2", KeyState_Down, this, &CPlayer2D::Skill2);
	CInput::GetInst()->SetKeyCallback<CPlayer2D>("Flip", KeyState_Down, this, &CPlayer2D::FlipAll);

	//CInput::GetInst()->SetKeyCallback("MoveLeft", KeyState_Up, this, &CPlayer2D::Stand);
	//CInput::GetInst()->SetKeyCallback("MoveRight", KeyState_Up, this, &CPlayer2D::Stand);

	return true;
}

void CPlayer2D::Update(float DeltaTime)
{
	CGameObject::Update(DeltaTime);

	
}

void CPlayer2D::PostUpdate(float DeltaTime)
{
	CGameObject::PostUpdate(DeltaTime);
}

CPlayer2D* CPlayer2D::Clone()
{
	return new CPlayer2D(*this);
}

void CPlayer2D::Stand()
{
	std::string CurAnim = m_BodySprite->GetCurrentAnimationName();

	if (CurAnim.find("Left") != std::string::npos)
	{
		m_BodySprite->ChangeAnimation("PlayerStandLeft");
	}

	else
	{
		m_BodySprite->ChangeAnimation("PlayerStandRight");
	}

	CAnimationSequence2DData* CurrentAnimation = m_BodySprite->GetCurrentAnimation();
	AnimationFrameData FrameData = CurrentAnimation->GetFrameData(0);

	Vector3 ScalingFactor = Vector3(FrameData.Size.x, FrameData.Size.y, 1.f);

	m_BodySprite->SetRelativeScale(ScalingFactor * m_ScaleFactor);
}

void CPlayer2D::Stand(float DeltaTime)
{
	std::string CurAnim = m_BodySprite->GetCurrentAnimationName();

	if (CurAnim.find("Left") != std::string::npos)
	{
		m_BodySprite->ChangeAnimation("PlayerStandLeft");
	}

	else
	{
		m_BodySprite->ChangeAnimation("PlayerStandRight");
	}

	CAnimationSequence2DData* CurrentAnimation = m_BodySprite->GetCurrentAnimation();
	AnimationFrameData FrameData = CurrentAnimation->GetFrameData(0);

	Vector3 ScalingFactor = Vector3(FrameData.Size.x, FrameData.Size.y, 1.f);

	m_BodySprite->SetRelativeScale(ScalingFactor * m_ScaleFactor);
}

void CPlayer2D::MoveUp(float DeltaTime)
{
	m_BodySprite->AddRelativePos(m_BodySprite->GetWorldAxis(AXIS_Y) * 300.f * DeltaTime);
}

void CPlayer2D::MoveDown(float DeltaTime)
{
	m_BodySprite->AddRelativePos(m_BodySprite->GetWorldAxis(AXIS_Y) * -300.f * DeltaTime);
}

//void CPlayer2D::MoveLeft(float DeltaTime)
//{
//	m_BodySprite->AddRelativePos(m_BodySprite->GetWorldAxis(AXIS_X) * -300.f * DeltaTime);
//
//	m_BodySprite->ChangeAnimation("PlayerWalkLeft");
//
//	CAnimationSequence2DData* CurrentAnimation = m_BodySprite->GetCurrentAnimation();
//	AnimationFrameData FrameData = CurrentAnimation->GetFrameData(0);
//
//	Vector3 ScalingFactor = Vector3(FrameData.Size.x, FrameData.Size.y, 1.f);
//
//	m_BodySprite->SetRelativeScale(ScalingFactor * m_ScaleFactor);
//}
//
//void CPlayer2D::MoveRight(float DeltaTime)
//{
//	m_BodySprite->AddRelativePos(m_BodySprite->GetWorldAxis(AXIS_X) * 300.f * DeltaTime);
//
//	m_BodySprite->ChangeAnimation("PlayerWalkRight");
//
//		CAnimationSequence2DData* CurrentAnimation = m_BodySprite->GetCurrentAnimation();
//	AnimationFrameData FrameData = CurrentAnimation->GetFrameData(0);
//
//	Vector3 ScalingFactor = Vector3(FrameData.Size.x, FrameData.Size.y, 1.f);
//
//	m_BodySprite->SetRelativeScale(ScalingFactor * m_ScaleFactor);
//}

void CPlayer2D::RotationZInv(float DeltaTime)
{
	m_BodySprite->AddRelativeRotationZ(180.f * DeltaTime);
}

void CPlayer2D::RotationZ(float DeltaTime)
{
	m_BodySprite->AddRelativeRotationZ(-180.f * DeltaTime);
}

void CPlayer2D::SylphideLancer(float DeltaTime)
{
	/*std::string CurAnim = m_BodySprite->GetCurrentAnimationName();
	m_SylphideLancerMirror->SetOpacity(1.f);


	m_SylphideLancerMirror->ChangeAnimation("Blank");

	if (CurAnim.find("Left") != std::string::npos)
	{
		m_BodySprite->ChangeAnimation("PlayerSkillActionLeft");
		m_SylphideLancerMirror->ChangeAnimation("SylphideLancerMirrorLeft");
		m_SkillBodyEffect->ChangeAnimation("SylphideLancerBodyEffectLeft");
		m_SylphideLancerMuzzle->SetRelativePos(60.f, 70.f, 0.f);
	}

	else
	{
		m_BodySprite->ChangeAnimation("PlayerSkillActionRight");
		m_SylphideLancerMirror->ChangeAnimation("SylphideLancerMirrorRight");
		m_SkillBodyEffect->ChangeAnimation("SylphideLancerBodyEffectRight");
		m_SylphideLancerMuzzle->SetRelativePos(-60.f, 70.f, 0.f);
	}

	CAnimationSequence2DData* CurrentAnimation = m_BodySprite->GetCurrentAnimation();
	AnimationFrameData FrameData = CurrentAnimation->GetFrameData(0);

	Vector3 ScalingFactor = Vector3(FrameData.Size.x, FrameData.Size.y, 1.f);

	m_BodySprite->SetRelativeScale(ScalingFactor * m_ScaleFactor);

	CurrentAnimation = m_SkillBodyEffect->GetCurrentAnimation();
	FrameData = CurrentAnimation->GetFrameData(0);

	ScalingFactor = Vector3(FrameData.Size.x, FrameData.Size.y, 1.f);

	m_SkillBodyEffect->SetRelativeScale(ScalingFactor * m_ScaleFactor);*/


	///////

	CBullet* Bullet = m_Scene->CreateGameObject<CBullet>("Bullet");

	//Bullet->SetWorldPos(GetWorldPos() + GetWorldAxis(AXIS_Y) * 75.f);
	Bullet->SetWorldPos(GetWorldPos());
	Bullet->SetWorldRotation(GetWorldRot());

	if (m_BodySprite->IsFlip())
	{
		Bullet->FlipAll(DeltaTime);
	}

}

void CPlayer2D::VoidPressure(float DeltaTime)
{
}

void CPlayer2D::Skill2(float DeltaTime)
{
	CBulletCamera* Bullet = m_Scene->CreateGameObject<CBulletCamera>("Bullet");

	//Bullet->SetWorldPos(GetWorldPos() + GetWorldAxis(AXIS_Y) * 75.f);
	Bullet->SetWorldPos(m_SylphideLancerMuzzle->GetWorldPos());
	Bullet->SetWorldRotation(GetWorldRot());
	
}

void CPlayer2D::FlipAll(float DeltaTime)
{
	m_BodySprite->Flip();

	//float RelativeOrbXPos = m_OrbSprite->GetRelativePos().x;

	//// 플레이어와 간격(절대값)을 유지하면서 플레이어 기준으로 위치만 반대로 이동 
	//m_OrbSprite->SetRelativePos(-RelativeOrbXPos, m_OrbSprite->GetRelativePos().y, m_OrbSprite->GetRelativePos().z);
	//// OrbSprite의 중심 기준으로 이미지 Flip
	//m_OrbSprite->Flip();
}



