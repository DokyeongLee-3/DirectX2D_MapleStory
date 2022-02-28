
#include "Player2D.h"
#include "Stage.h"
#include "Device.h"
#include "Portal.h"
#include "Scene/Scene.h"
#include "../Scene/LobbyScene.h"
#include "../Scene/OnionScene.h"
#include "../Scene/WayToZakumScene.h"
#include "../Scene/Library2ndScene.h"
#include "Input.h"
#include "PlayerAnimation2D.h"
#include "../Animation/SylphideLancerMirrorAnimation.h"
#include "../Animation/PlayerSkillBodyEffect.h"
#include "../Animation/VoidPressureAttackSphere.h"
#include "../Animation/PlayerOrb.h"
#include "BulletCamera.h"
#include "SylphideLancer.h"
#include "Scene/SceneManager.h"
#include "Render/RenderManager.h"
#include "VoidPressure.h"
#include "VoidPressureOrb.h"
#include "PlayerSkillInfo.h"
#include "../Widget/PlayerDamageFont.h"
#include "Engine.h"
#include "../Client.h"

CPlayer2D::CPlayer2D() :
	m_BodySprite(nullptr),
	m_SylphideLancerMirror(nullptr),
	m_SkillBodyEffect(nullptr),
	m_VoidPressure(nullptr),
	m_VoidPressureOrb(nullptr),
	m_Opacity(1.f),
	m_IsFlip(false),
	m_Dir(PlayerDir::None),
	m_OnLightTransforming(false),
	m_OnJump(false),
	m_OnKnockBack(false),
	m_OnKnockBackLeft(false),
	m_OnKnockBackTime(2.f),
	m_OnKnockBackAccTime(0.f),
	m_HitOpacity(0.7f),
	m_OnHitTime(1.f),
	m_OnHitAccTime(0.f)
{
	SetTypeID<CPlayer2D>();
	m_PlayerSkillInfo = new CPlayerSkillInfo;
	m_Gravity = true;

	m_JumpForce = 370.f;
}

CPlayer2D::CPlayer2D(const CPlayer2D& obj) :
	CGameObject(obj)
{
	m_BodySprite = (CSpriteComponent*)FindComponent("PlayerSprite");
	m_SylphideLancerMirror = (CSpriteComponent*)FindComponent("PlayerSylphideLancerMirror");
	m_SkillBodyEffect = (CSpriteComponent*)FindComponent("PlayerSkillBodyEffect");
	m_PlayerOrb = (CSpriteComponent*)FindComponent("PlayerOrb");

	m_Body = (CColliderBox2D*)FindComponent("Body");
	m_Camera = (CCameraComponent*)FindComponent("Camera");

	m_Opacity = obj.m_Opacity;
}

CPlayer2D::~CPlayer2D()
{
	SAFE_DELETE(m_PlayerSkillInfo);
}

CVoidPressureOrb* CPlayer2D::GetVoidPressureOrb() const
{
	return m_VoidPressureOrb;
}

bool CPlayer2D::Init()
{
	m_BodySprite = CreateComponent<CSpriteComponent>("PlayerSprite");
	m_SylphideLancerMirror = CreateComponent<CSpriteComponent>("PlayerSylphideLancerMirror");
	m_SkillBodyEffect = CreateComponent<CSpriteComponent>("PlayerSkillBodyEffect");
	m_PlayerOrb = CreateComponent<CSpriteComponent>("PlayerOrb");

	m_Body = CreateComponent<CColliderBox2D>("PlayerBody");
	m_Body->SetExtent(25.f, 35.f);
	m_Body->SetOffset(0.f, 13.f, 0.f);

	m_Camera = CreateComponent<CCameraComponent>("Camera");

	m_DamageWidgetComponent = CreateComponent<CWidgetComponent>("PlayerDamageFont");
	m_DamageWidgetComponent->UseAlphaBlend(true);
	//m_DamageWidgetComponent->Enable(false);

	Vector3 WorldPos = m_BodySprite->GetWorldPos();

	CPlayerDamageFont* DamageFont = m_DamageWidgetComponent->CreateWidgetWindow<CPlayerDamageFont>("DamageFontWidget");

	DamageFont->SetPos(WorldPos.x - 30.f, WorldPos.y);



	m_Body->SetCollisionProfile("Player");

	m_Camera->OnViewportCenter();

	m_BodySprite->AddChild(m_PlayerOrb);
	m_BodySprite->AddChild(m_SylphideLancerMirror);
	m_BodySprite->AddChild(m_SkillBodyEffect);
	m_BodySprite->AddChild(m_DamageWidgetComponent);
	
	m_BodySprite->AddChild(m_Body);
	m_BodySprite->AddChild(m_Camera);


	m_BodySprite->SetTransparency(true);
	m_SylphideLancerMirror->SetTransparency(true);
	m_SkillBodyEffect->SetTransparency(true);
	m_PlayerOrb->SetTransparency(true);

	m_BodySprite->CreateAnimationInstance<CPlayerAnimation2D>();
	m_SkillBodyEffect->CreateAnimationInstance<CPlayerSkillBodyEffect>();
	m_PlayerOrb->CreateAnimationInstance<CPlayerOrb>();

	m_SkillBodyEffect->SetPivot(0.5f, 0.5f, 0.f);
	m_SkillBodyEffect->SetRelativePos(0.f, 11.f, 0.f);

	m_SylphideLancerMirror->CreateAnimationInstance<CSylphideLancerMirrorAnimation>();
	m_SylphideLancerMirror->SetInheritRotZ(false);
	m_SylphideLancerMirror->SetPivot(0.5f, 0.5f, 0.f);
	m_SylphideLancerMirror->SetRelativePos(50.f, 80.f, 0.f);
	m_SylphideLancerMirror->SetPivot(0.5f, 0.5f, 0.f);

	m_BodySprite->SetRelativePos(100.f, 100.f, 0.f);
	m_BodySprite->SetPivot(0.5f, 0.5f, 0.f);

	m_PlayerOrb->SetRelativePos(60.f, 50.f, 0.f);
	m_PlayerOrb->SetPivot(0.5f, 0.5f, 0.f);

	m_Body->AddCollisionCallback<CPlayer2D>(Collision_State::Begin, this, &CPlayer2D::CollisionBeginCallback);
	//m_Body->AddCollisionCallback<CPlayer2D>(Collision_State::Stay, this, &CPlayer2D::CollisionStayCallback);
	m_Body->AddCollisionCallback<CPlayer2D>(Collision_State::End, this, &CPlayer2D::CollisionEndCallback);
	SetGravityFactor(1000.f);

	CInput::GetInst()->CreateKey("MoveUp", VK_UP);
	CInput::GetInst()->CreateKey("MoveDown", VK_DOWN);
	CInput::GetInst()->CreateKey("MoveLeft", VK_LEFT);
	CInput::GetInst()->CreateKey("MoveRight", VK_RIGHT);
	CInput::GetInst()->CreateKey("SylphideLancer", 'Q');
	CInput::GetInst()->CreateKey("VoidPressure", 'W');
	CInput::GetInst()->CreateKey("LightTransforming", VK_SPACE);
	CInput::GetInst()->CreateKey("Jump", 'Z');
	//CInput::GetInst()->CreateKey("Flip", 'F');

	CInput::GetInst()->SetKeyCallback<CPlayer2D>("MoveUp", KeyState_Push, this, &CPlayer2D::MoveUp);
	CInput::GetInst()->SetKeyCallback<CPlayer2D>("MoveDown", KeyState_Push, this, &CPlayer2D::MoveDown);
	CInput::GetInst()->SetKeyCallback<CPlayer2D>("MoveLeft", KeyState_Push, this, &CPlayer2D::MoveLeft);
	CInput::GetInst()->SetKeyCallback<CPlayer2D>("MoveRight", KeyState_Push, this, &CPlayer2D::MoveRight);
	CInput::GetInst()->SetKeyCallback<CPlayer2D>("MoveLeft", KeyState_Up, this, &CPlayer2D::ReturnIdle);
	CInput::GetInst()->SetKeyCallback<CPlayer2D>("MoveRight", KeyState_Up, this, &CPlayer2D::ReturnIdle);

	CInput::GetInst()->SetKeyCallback<CPlayer2D>("SylphideLancer", KeyState_Down, this, &CPlayer2D::SylphideLancer);
	CInput::GetInst()->SetKeyCallback<CPlayer2D>("VoidPressure", KeyState_Push, this, &CPlayer2D::VoidPressure);
	CInput::GetInst()->SetKeyCallback<CPlayer2D>("VoidPressure", KeyState_Up, this, &CPlayer2D::VoidPressureCancle);
	CInput::GetInst()->SetKeyCallback<CPlayer2D>("LightTransforming", KeyState_Down, this, &CPlayer2D::LightTransforming);
	CInput::GetInst()->SetKeyCallback<CPlayer2D>("Jump", KeyState_Down, this, &CPlayer2D::Jump);
	//CInput::GetInst()->SetKeyCallback<CPlayer2D>("Flip", KeyState_Down, this, &CPlayer2D::FlipAll);

	//CInput::GetInst()->SetKeyCallback("DirUp", KeyState_Down, this, &CPlayer2D::GotoNextMap);
	//CInput::GetInst()->SetKeyCallback("MoveRight", KeyState_Up, this, &CPlayer2D::Stand);


	//CInput::GetInst()->SetKeyCallback<CPlayer2D>("MovePoint", KeyState_Down, this, &CPlayer2D::MovePointDown);


	return true;
}

void CPlayer2D::Update(float DeltaTime)
{
	CGameObject::Update(DeltaTime);

	m_PlayerSkillInfo->Update(DeltaTime);

	if (m_OnLightTransforming)
	{
		SkillInfo* Info = m_PlayerSkillInfo->FindSkillInfo("LightTransforming");

		if (Info && Info->Active == false)
		{
			m_OnLightTransforming = false;
			m_BodySprite->SetOpacity(1.f);
		}
	}

	if (m_OnJump)
	{
		AddWorldPos(0.f, DeltaTime * m_JumpForce, 0.f);
	}

	if (m_OnHit)
	{
		m_OnHitAccTime += DeltaTime;

		int Percent = int(m_OnHitAccTime * 10) % 11;

		switch (Percent)
		{
		case 1:
		case 3: 
		case 5:
		case 7:
		case 9:
			m_BodySprite->SetOpacity(m_HitOpacity);
			break;
		case 2:
		case 4:
		case 6:
		case 8:
			m_BodySprite->SetOpacity(1.f);
			break;
		}

		if (m_OnHitAccTime >= m_OnHitTime)
		{
			m_OnHit = false;
			m_OnHitAccTime = 0.f;
			m_BodySprite->SetOpacity(1.f);
		}
	}
	
	KnockBack(DeltaTime);
}

void CPlayer2D::PostUpdate(float DeltaTime)
{
	// 카메라가 화면 밖으로 나가는거도 보정을 한 뒤에 최종적으로 만들어지는 transform으로 뷰행렬을 만들어야하므로 
	// 무조건 CGameObject::PostUpdate보다 먼저 해줘야한다
	CameraTrack();

	CGameObject::PostUpdate(DeltaTime);
}

CPlayer2D* CPlayer2D::Clone()
{
	return new CPlayer2D(*this);
}

void CPlayer2D::SetScene(CScene* Scene)
{
	m_Scene = Scene;
}

void CPlayer2D::MoveUp(float DeltaTime)
{
	//m_BodySprite->AddRelativePos(m_BodySprite->GetWorldAxis(AXIS_Y) * 300.f * DeltaTime);
	m_Dir = PlayerDir::Up;
	GotoNextMap(DeltaTime);
}

void CPlayer2D::MoveDown(float DeltaTime)
{
	m_Dir = PlayerDir::Down;
	//m_BodySprite->AddRelativePos(m_BodySprite->GetWorldAxis(AXIS_Y) * -300.f * DeltaTime);
}

void CPlayer2D::MoveLeft(float DeltaTime)
{
	if (m_OnKnockBack)
		return;

	if (m_VoidPressure)
	{
		if (m_VoidPressure->IsEnable() && !m_VoidPressure->GetOnDestroy())
		{
			m_VoidPressure->AddWorldPos(-300.f * DeltaTime, 0.f, 0.f);
			return;
		}
	}

	// 보이드 프레셔 구체가 아직 생성된적 없거나 완전히 소멸되고나서 움직일 수 있게한다
	if (!m_VoidPressure || !m_VoidPressure->IsEnable())
	{
		if (m_BodySprite->IsFlip())
			FlipAll(DeltaTime);

		m_BodySprite->AddWorldPos(m_BodySprite->GetWorldAxis(AXIS_X) * -180.f * DeltaTime);

		if (!m_OnJump)
			m_BodySprite->ChangeAnimation("WalkLeft");

		m_Dir = PlayerDir::Left;
	}
}

void CPlayer2D::MoveRight(float DeltaTime)
{
	if (m_OnKnockBack)
		return;

	if (m_VoidPressure)
	{
		if (m_VoidPressure->IsEnable() && !m_VoidPressure->GetOnDestroy())
		{
			m_VoidPressure->AddWorldPos(300.f * DeltaTime, 0.f, 0.f);
			return;
		}
	}

	// 보이드 프레셔 구체가 아직 생성된적 없거나 완전히 소멸되고나서 움직일 수 있게한다
	if (!m_VoidPressure || !m_VoidPressure->IsEnable())
	{
 		if (!m_BodySprite->IsFlip())
			FlipAll(DeltaTime);

		m_BodySprite->AddWorldPos(m_BodySprite->GetWorldAxis(AXIS_X) * 180.f * DeltaTime);

		if (!m_OnJump)
			m_BodySprite->ChangeAnimation("WalkLeft");

		m_Dir = PlayerDir::Right;
	}
}

void CPlayer2D::Jump(float DeltaTime)
{
	if (m_VoidPressure && m_VoidPressure->IsEnable())
		return;

	if (m_OnJump || m_Gravity)
		return;


	if (m_Dir != PlayerDir::Down)
	{
		m_OnJump = true;
		//AddWorldPos(0.f, 5.f, 0.f);
		m_BodySprite->GetAnimationInstance()->ChangeAnimation("JumpLeft");
	}
	
	else if (!m_ListCollisionID.empty())
	{
		m_ListCollisionID.clear();
		m_Gravity = true;
		m_TileCollisionEnable = false;
		m_Dir = PlayerDir::None;
		m_BodySprite->GetAnimationInstance()->ChangeAnimation("JumpLeft");
	}

	m_Scene->GetResource()->SoundPlay("Jump");
}

void CPlayer2D::SylphideLancer(float DeltaTime)
{
	if (m_OnJump)
		return;

	SkillInfo* SkillInfo = m_PlayerSkillInfo->FindSkillInfo("SylphideLancer");

	if (SkillInfo->Active == true && SkillInfo->AccTime < SkillInfo->CoolTime)
		return;


	// Scene의 m_ObjList에서 몬스터 찾아서 여기서 실피드랜서 방향 설정해주기

	m_BodySprite->ChangeAnimation("HealLeft");

	CGameObject* NearMonster = m_Scene->FindIncludingNameObject("Onion", GetWorldPos(), Vector2(400.f, 70.f));


	m_Scene->GetResource()->SoundPlay("SylphideLancerUse");

	m_SylphideLancerMirror->ChangeAnimation("SylphideLancerMuzzle");

	Vector3 MirrorPos = m_SylphideLancerMirror->GetWorldPos();

	for (int i = 0; i < 2; ++i)
	{
		CSylphideLancer* Lancer = m_Scene->CreateGameObject<CSylphideLancer>("SylphideLancer");

		Lancer->SetAllSceneComponentsLayer("MovingObjFront");
		Lancer->SetLancerID(i);
		Lancer->SetWorldPos(MirrorPos.x, MirrorPos.y - 10.f + i * 30.f, MirrorPos.z);
		Lancer->SetCollisionProfile("PlayerAttack");
		Lancer->SetSpeed(-400.f);
		Vector3 LancerPos = Lancer->GetWorldPos();

		Vector3 MonsterWorldPos;

		if (NearMonster)
		{
			MonsterWorldPos = NearMonster->GetWorldPos();
			float Radian = atan((MonsterWorldPos.y - LancerPos.y)/(MonsterWorldPos.x - LancerPos.x));
			float Degree = RadianToDegree(Radian);

			Lancer->SetWorldRotation(0.f, 0.f, Degree);
		}

		else
		{
			Lancer->SetWorldRotation(GetWorldRot());
		}

		if (m_BodySprite->IsFlip())
		{
			Lancer->FlipAll(DeltaTime);
			Lancer->SetSpeed(400.f);
		}
	}

	SkillInfo->Active = true;
}

void CPlayer2D::VoidPressure(float DeltaTime)
{
	if (m_OnJump)
		return;

	SkillInfo* SkillInfo = m_PlayerSkillInfo->FindSkillInfo("VoidPressure");

	if (SkillInfo->Active == true && SkillInfo->AccTime < SkillInfo->CoolTime)
		return;

	if(!m_Scene->GetResource()->IsPlaying("VoidPressureLoop"))
		m_Scene->GetResource()->SoundPlay("VoidPressureLoop");

	if (!m_VoidPressure)
	{
		m_Scene->GetResource()->SoundPlay("VoidPressureUse");

		m_BodySprite->ChangeAnimation("HealLeft");

		// 충돌체를 가지는 GameObject를 상속받은 CVoidPressure 클래스를 생성할것(단순 SpriteComponent만 생성하는게 아니라)
		m_VoidPressure = m_Scene->CreateGameObject<CVoidPressure>("VoidPressure");
		m_VoidPressure->SetCollisionProfile("PlayerAttack");
		m_VoidPressure->SetAllSceneComponentsLayer("MovingObjFront");
		
		if (m_IsFlip)
			m_VoidPressure->FlipAll(DeltaTime);

		Vector3 RelativePos = m_SylphideLancerMirror->GetRelativePos();
		Vector3 WorldPos = m_SylphideLancerMirror->GetWorldPos();
		Vector3 PlayerWorldPos = m_BodySprite->GetWorldPos();

		float VoidPosX = PlayerWorldPos.x - RelativePos.x * 2.f;

		m_VoidPressure->SetWorldPos(VoidPosX, WorldPos.y, 0.f);
		m_VoidPressure->SetOriginPos(VoidPosX, WorldPos.y, 0.f);

		SkillInfo->Active = true;
	}

	if (m_VoidPressure && !m_VoidPressure->IsEnable())
	{
		m_VoidPressure->Enable(true);
		CSpriteComponent* Root = (CSpriteComponent*)m_VoidPressure->GetRootComponent();
		Root->ChangeAnimation("VoidPressureReadyLeft");

		Vector3 RelativePos = m_SylphideLancerMirror->GetRelativePos();
		Vector3 WorldPos = m_SylphideLancerMirror->GetWorldPos();
		Vector3 PlayerWorldPos = m_BodySprite->GetWorldPos();

		float VoidPosX = PlayerWorldPos.x - RelativePos.x * 2.f;

		m_VoidPressure->SetWorldPos(VoidPosX, WorldPos.y, 0.f);
		m_VoidPressure->SetOriginPos(VoidPosX, WorldPos.y, 0.f);
		m_VoidPressure->SetOnDestroy(false);

		SkillInfo->Active = true;
	}

	if (!m_VoidPressureOrb)
	{
		m_VoidPressureOrb = m_Scene->CreateGameObject<CVoidPressureOrb>("VoidPressureOrb");

		if (m_IsFlip)
			m_VoidPressureOrb->FlipAll(DeltaTime);

		m_VoidPressureOrb->SetAllSceneComponentsLayer("MovingObjFront");

		Vector3 RelativePos = m_SylphideLancerMirror->GetRelativePos();
		Vector3 WorldPos = m_SylphideLancerMirror->GetWorldPos();
		Vector3 PlayerWorldPos = m_BodySprite->GetWorldPos();

		float VoidPosX = PlayerWorldPos.x - RelativePos.x / 2.f;

		if (m_IsFlip)
			VoidPosX += 5.f;
		else
			VoidPosX -= 5.f;

		m_VoidPressureOrb->SetWorldPos(VoidPosX, WorldPos.y - 39.f, 0.f);
		m_VoidPressureOrb->SetOriginPos(VoidPosX, WorldPos.y - 39.f, 0.f);
	}

	if (m_VoidPressureOrb && !m_VoidPressureOrb->IsEnable())
	{
		m_VoidPressureOrb->Enable(true);
		Vector3 RelativePos = m_SylphideLancerMirror->GetRelativePos();
		Vector3 WorldPos = m_SylphideLancerMirror->GetWorldPos();
		Vector3 PlayerWorldPos = m_BodySprite->GetWorldPos();

		float VoidPosX = PlayerWorldPos.x - RelativePos.x / 2.f;

		if (m_IsFlip)
			VoidPosX += 5.f;
		else
			VoidPosX -= 5.f;

		m_VoidPressureOrb->SetWorldPos(VoidPosX, WorldPos.y - 39.f, 0.f);
		m_VoidPressureOrb->SetOriginPos(VoidPosX, WorldPos.y - 39.f, 0.f);
	}
}

void CPlayer2D::LightTransforming(float DeltaTime)
{
	if (m_OnJump)
		return;

	if (m_Dir == PlayerDir::None)
		return;

	if (!m_CurrentStage)
		return;

	SkillInfo* SkillInfo = m_PlayerSkillInfo->FindSkillInfo("LightTransforming");

	if (SkillInfo->Active == true && SkillInfo->AccTime < SkillInfo->CoolTime)
		return;

	m_OnLightTransforming = true;
	m_BodySprite->SetOpacity(0.5f);
	//m_Body->Enable(false);

	m_Scene->GetResource()->SoundPlay("LightTransforming");

	m_SkillBodyEffect->ChangeAnimation("LightTransformingLeft");

	if (m_Dir == PlayerDir::Right)
		m_BodySprite->AddWorldPos(200.f, 0.f, 0.f);

	else if (m_Dir == PlayerDir::Left)
		m_BodySprite->AddWorldPos(-200.f, 0.f, 0.f);

	SkillInfo->Active = true;

	bool AdjustX = false;
	bool AdjustY = false;
	Resolution RS = CDevice::GetInst()->GetResolution();
	Vector2 Ratio = m_Camera->GetRatio();

	if (m_CurrentStage)
	{
		Vector3 WorldSize = m_CurrentStage->GetWorldScale();

		Vector3 PlayerWorldPos = GetWorldPos();
		float CenterPos = (WorldSize.x + (WorldSize.x - RS.Width)) / 2.f;

		if (PlayerWorldPos.x + RS.Width * (1 - Ratio.x) >= WorldSize.x)
		{
			Vector3 CamWorldPos = m_Camera->GetWorldPos();
			float Pos = WorldSize.x - RS.Width * Ratio.x;
			Pos -= RS.Width * Ratio.x;
			m_Camera->SetWorldPos(Pos, CamWorldPos.y, CamWorldPos.z);
			AdjustX = true;
		}

		if (PlayerWorldPos.x - RS.Width * Ratio.x <= 0.f)
		{
			Vector3 CamWorldPos = m_Camera->GetWorldPos();
			m_Camera->SetWorldPos(0.f, CamWorldPos.y, CamWorldPos.z);
			AdjustX = true;
		}

		if (PlayerWorldPos.y + RS.Height * (1 - Ratio.y) >= WorldSize.y)
		{
			Vector3 CamWorldPos = m_Camera->GetWorldPos();
			float Pos = WorldSize.y - RS.Height * Ratio.y;
			Pos -= 1.f * RS.Height * Ratio.y;
			m_Camera->SetWorldPos(CamWorldPos.x, Pos, CamWorldPos.z);
			AdjustY = true;
		}

		if (PlayerWorldPos.y - RS.Height * Ratio.y <= 0.f)
		{
			Vector3 CamWorldPos = m_Camera->GetWorldPos();
			m_Camera->SetWorldPos(CamWorldPos.x, 0.f, CamWorldPos.z);
			AdjustY = true;
		}
	}

	if (!AdjustX)
	{
		Vector3 RelativePos = m_Camera->GetRelativePos();

		if (RelativePos.x < -1.f * RS.Width * Ratio.x - 1.f || RelativePos.x > -1.f * RS.Width * Ratio.x + 1.f)
			m_Camera->SetRelativePos(-1.f * RS.Width * Ratio.x, RelativePos.y, RelativePos.z);
	}

	if (!AdjustY)
	{
		Vector3 RelativePos = m_Camera->GetRelativePos();

		if (RelativePos.y < -1.f * RS.Height * Ratio.y - 1.f || RelativePos.y > -1.f * RS.Height * Ratio.y + 1.f)
			m_Camera->SetRelativePos(RelativePos.x, -1.f * RS.Height * Ratio.y, RelativePos.z);
	}

}

void CPlayer2D::VoidPressureCancle(float DeltaTime)
{
	m_Scene->GetResource()->SoundPlay("VoidPressureEnd");
	m_BodySprite->ChangeAnimation("IdleLeft");

	if (m_VoidPressure)
	{
		m_VoidPressure->SetVoidPressureLifeSpan(6.f);
		CSpriteComponent* SpriteComp = (CSpriteComponent*)(m_VoidPressure->GetRootComponent());
		SpriteComp->GetAnimationInstance()->ChangeAnimation("VoidPressureDestroy");
		m_VoidPressure->SetOnDestroy(true);
	}

	if (m_VoidPressureOrb)
	{
		CSpriteComponent* SpriteComp = (CSpriteComponent*)(m_VoidPressureOrb->GetRootComponent());
		SpriteComp->GetAnimationInstance()->ChangeAnimation("VoidPressureOrbDestroy");
	}
}

void CPlayer2D::CollisionBeginCallback(const CollisionResult& Result)
{
	CGameObject* Dest = Result.Dest->GetGameObject();

	if (Dest->GetTypeID() == typeid(CTileObject).hash_code())
	{
		m_BodySprite->ChangeAnimation("IdleLeft");
		m_Dir = PlayerDir::None;
		m_OnJump = false;
	}

	if (Dest->GetTypeID() == typeid(COnionMonster).hash_code())
	{
		if (m_OnHit)
			return;

		Vector3 DestPos = Dest->GetWorldPos();
		Vector3 MyPos = GetWorldPos();

		// 나보다 왼쪽에서 몬스터가 충돌 -> 오른쪽으로 튕겨나가야한다
		if (DestPos.x < MyPos.x)
			m_OnKnockBackLeft = false;
		else
			m_OnKnockBackLeft = true;

		m_OnKnockBack = true;

		m_GravityAccTime = 0.f;
		m_OnHit = true;
	}
}


void CPlayer2D::CollisionEndCallback(const CollisionResult& Result)
{
	int a = 3;
}

void CPlayer2D::CameraTrack()
{
	//CComponent* Component = GetRootComponent()->FindComponent("Camera");

	if (m_Camera)
	{
		CSceneMode* SceneMode = m_Scene->GetSceneMode();

		if (SceneMode->GetTypeID() == typeid(CLobbyScene).hash_code())
		{
			// Stage는 Pivot이 (0.f, 0.f)
			m_CurrentStage = ((CLobbyScene*)SceneMode)->GetStageObject();
		}

		else if (SceneMode->GetTypeID() == typeid(COnionScene).hash_code())
		{
			m_CurrentStage = ((COnionScene*)SceneMode)->GetStageObject();
		}

		else if (SceneMode->GetTypeID() == typeid(CWayToZakumScene).hash_code())
		{
			m_CurrentStage = ((CWayToZakumScene*)SceneMode)->GetStageObject();
		}

		else if (SceneMode->GetTypeID() == typeid(CLibrary2ndScene).hash_code())
		{
			m_CurrentStage = ((CLibrary2ndScene*)SceneMode)->GetStageObject();
		}

		if (m_CurrentStage)
		{
			Vector3 WorldSize = m_CurrentStage->GetWorldScale();
			Resolution RS = CDevice::GetInst()->GetResolution();

			Vector2 Ratio = m_Camera->GetRatio();
			Vector3 PlayerWorldPos = GetWorldPos();
			float CenterPos = (WorldSize.x + (WorldSize.x - RS.Width)) / 2.f;

			if (PlayerWorldPos.x + RS.Width * (1 - Ratio.x) >= WorldSize.x)
			{
				Vector3 CamWorldPos = m_Camera->GetWorldPos();
				float Pos = WorldSize.x - RS.Width * Ratio.x;
				Pos -= RS.Width * Ratio.x;
				m_Camera->SetWorldPos(Pos, CamWorldPos.y, CamWorldPos.z);
			}

			if (PlayerWorldPos.x - RS.Width * Ratio.x <= 0.f)
			{
				Vector3 CamWorldPos = m_Camera->GetWorldPos();
				m_Camera->SetWorldPos(0.f, CamWorldPos.y, CamWorldPos.z);
			}

			if (PlayerWorldPos.y + RS.Height * (1 - Ratio.y) >= WorldSize.y)
			{
				Vector3 CamWorldPos = m_Camera->GetWorldPos();
				float Pos = WorldSize.y - RS.Height * Ratio.y;
				Pos -= 1.f * RS.Height * Ratio.y;
				m_Camera->SetWorldPos(CamWorldPos.x, Pos, CamWorldPos.z);
			}

			if (PlayerWorldPos.y - RS.Height * Ratio.y <= 0.f)
			{
				Vector3 CamWorldPos = m_Camera->GetWorldPos();
				m_Camera->SetWorldPos(CamWorldPos.x, 0.f, CamWorldPos.z);
			}
		}

	}
}

void CPlayer2D::KnockBack(float DeltaTime)
{
	if (m_OnKnockBack)
	{
		// 왼쪽으로 튕겨나감
		if (m_OnKnockBackLeft)
		{
			Vector3 KnockBackDir = Vector3(-200.f * DeltaTime, 100.f * DeltaTime, 0.f);

			AddWorldPos(KnockBackDir);
		}

		else
		{
			Vector3 KnockBackDir = Vector3(200.f * DeltaTime, 100.f * DeltaTime, 0.f);

			AddWorldPos(KnockBackDir);
		}


		m_TileCollisionEnable = false;

		m_OnKnockBackAccTime += DeltaTime;

		if (m_OnKnockBackAccTime >= m_OnKnockBackTime)
		{
			m_OnKnockBack = false;
			m_OnKnockBackAccTime = 0.f;
		}
	}
}

void CPlayer2D::SetDamage(float Damage, bool Critical)
{
	if (m_OnHit)
		return;

	m_PlayerInfo.HP -= (int)Damage;
	CClientManager::GetInst()->GetCharacterStatusWindow()->SetCurrentHP(m_PlayerInfo.HP);

	float HPPercent = (float)m_PlayerInfo.HP / m_PlayerInfo.HPMax;

	CClientManager::GetInst()->GetCharacterStatusWindow()->SetHPPercent(HPPercent);

	//if (!m_IsChanging)
	//{
	//	if (m_MonsterInfo.HP <= 0.f)
	//	{
	//		// 죽는 애니메이션
	//	}

	//	else
	//	{
	//		m_Sprite->ChangeAnimation("OnionHitLeft");
	//	}

	//	m_IsChanging = true;
	//}

	//m_DamageWidgetComponent->Enable(true);
	PushDamageFont(Damage);
}

void CPlayer2D::PushDamageFont(float Damage)
{
	if (!m_DamageWidgetComponent->GetWidgetWindow()->GetViewport()->GetScene())
		m_DamageWidgetComponent->GetWidgetWindow()->GetViewport()->SetScene(m_Scene);

	((CPlayerDamageFont*)m_DamageWidgetComponent->GetWidgetWindow())->PushDamageNumber((int)Damage);
}


//void CPlayer2D::MovePointDown(float DeltaTime)
//{
//	Vector2 MousePos = CInput::GetInst()->GetMouseWorld2DPos();
//
//	Move(Vector3(MousePos.x, MousePos.y, 0.f));
//
//}


void CPlayer2D::SetVoidPressure(CVoidPressure* VoidPressure)
{
	m_VoidPressure = VoidPressure;
}

void CPlayer2D::SetVoidPressureOrb(CVoidPressureOrb* VoidPressureOrb)
{
	m_VoidPressureOrb = VoidPressureOrb;
}

void CPlayer2D::FlipAll(float DeltaTime)
{
	if (!m_IsFlip)
		m_IsFlip = true;

	else
		m_IsFlip = false;

	m_BodySprite->Flip();
	m_SkillBodyEffect->Flip();
	m_SylphideLancerMirror->Flip();
	m_PlayerOrb->Flip();

	Vector3 RelativePos = m_SylphideLancerMirror->GetRelativePos();

	RelativePos.x *= -1;

	m_SylphideLancerMirror->SetRelativePos(RelativePos);


	RelativePos = m_SkillBodyEffect->GetRelativePos();

	RelativePos.x *= -1;

	m_SkillBodyEffect->SetRelativePos(RelativePos);


	RelativePos = m_PlayerOrb->GetRelativePos();

	RelativePos.x *= -1;

	m_PlayerOrb->SetRelativePos(RelativePos);



	if (m_VoidPressure)
	{
		CSpriteComponent* Root = (CSpriteComponent*)m_VoidPressure->GetRootComponent();
		Root->Flip();

		Vector3 RelativePos = m_SylphideLancerMirror->GetRelativePos();
		Vector3 WorldPos = m_SylphideLancerMirror->GetWorldPos();
		Vector3 PlayerWorldPos = m_BodySprite->GetWorldPos();

		float VoidPosX = PlayerWorldPos.x - RelativePos.x * 2.f;

		m_VoidPressure->SetWorldPos(VoidPosX, WorldPos.y, 0.f);
		m_VoidPressure->SetOriginPos(VoidPosX, WorldPos.y, 0.f);
	}

	if (m_VoidPressureOrb)
	{
		CSpriteComponent* Root = (CSpriteComponent*)m_VoidPressureOrb->GetRootComponent();
		Root->Flip();

		Vector3 RelativePos = m_SylphideLancerMirror->GetRelativePos();
		Vector3 WorldPos = m_SylphideLancerMirror->GetWorldPos();
		Vector3 PlayerWorldPos = m_BodySprite->GetWorldPos();

		float VoidPosX = PlayerWorldPos.x - RelativePos.x / 2.f;

		if (m_IsFlip)
			VoidPosX += 5.f;
		else
			VoidPosX -= 5.f;

		m_VoidPressureOrb->SetWorldPos(VoidPosX, WorldPos.y - 39.f, 0.f);
		m_VoidPressureOrb->SetOriginPos(VoidPosX, WorldPos.y - 39.f, 0.f);
	}
}

void CPlayer2D::GotoNextMap(float DeltaTime)
{
	if (!m_Body->CheckPrevCollisionGameObjectType(typeid(CPortal).hash_code()))
		return;


	if (m_Scene->GetSceneMode()->GetTypeID() == typeid(CLobbyScene).hash_code())
	{
		CGameObject* Portal = m_Scene->FindObject("RightPortal");

		if (Portal)
		{
			CComponent* Body = ((CPortal*)Portal)->FindComponent("Body");

			if (Body)
			{
				// GotoNextMap함수는 CInput::Update에서 호출하는거라 아직 이번 프레임의 충돌 조사가 아직 안됐을 때니까
				// 이전 프레임 기준으로 검사한다
				bool Collision = ((CColliderBox2D*)Body)->CheckPrevCollision(m_Body);

				// LobbyScene의 오른쪽 Entrance 포탈에 충돌했고, 위쪽 방향키를 누르고 있을때 여기로 들어온다
				// ==> 양파 재배지 맵을 멀티쓰레드 활용해서 로딩후 SceneChange하기
				if (Collision)
				{
					m_ListCollisionID.clear();

					CLobbyScene* Scene = (CLobbyScene*)(GetScene()->GetSceneMode());
					CRenderManager::GetInst()->SetStartFadeIn(true);
					CSceneManager::GetInst()->SetFadeInEndCallback<CLobbyScene>(Scene, &CLobbyScene::CreateOnionScene);
				}
			}
		}

		Portal = m_Scene->FindObject("MiddlePortal");

		if(Portal)
		{
			CComponent* Body = ((CPortal*)Portal)->FindComponent("Body");

			if (Body)
			{
				bool Collision = ((CColliderBox2D*)Body)->CheckPrevCollision(m_Body);

				// LobbyScene의 오른쪽 Entrance 포탈에 충돌했고, 위쪽 방향키를 누르고 있을때 여기로 들어온다
				if (Collision)
				{
					m_ListCollisionID.clear();

					CLobbyScene* Scene = (CLobbyScene*)(GetScene()->GetSceneMode());
					CRenderManager::GetInst()->SetStartFadeIn(true);
					CSceneManager::GetInst()->SetFadeInEndCallback<CLobbyScene>(Scene, &CLobbyScene::CreateLibrary2ndScene);
				}
			}
		}
	}

	if (m_Scene->GetSceneMode()->GetTypeID() == typeid(COnionScene).hash_code())
	{
		CGameObject* Portal = m_Scene->FindObject("Portal1");

		if (Portal)
		{
			CComponent* Body = ((CPortal*)Portal)->FindComponent("Body");

			if (Body)
			{
				// GotoNextMap함수는 CInput::Update에서 호출하는거라 아직 이번 프레임의 충돌 조사가 아직 안됐을 때니까
				// 이전 프레임 기준으로 검사한다
				bool Collision = ((CColliderBox2D*)Body)->CheckPrevCollision(m_Body);

				// LobbyScene의 오른쪽 Entrance 포탈에 충돌했고, 위쪽 방향키를 누르고 있을때 여기로 들어온다
				// ==> 양파 재배지 맵을 멀티쓰레드 활용해서 로딩후 SceneChange하기
				if (Collision)
				{
					m_ListCollisionID.clear();

					COnionScene* Scene = (COnionScene*)(GetScene()->GetSceneMode());
					CRenderManager::GetInst()->SetStartFadeIn(true);
					CSceneManager::GetInst()->SetFadeInEndCallback<COnionScene>(Scene, &COnionScene::CreateLobbyScene);
				}
			}
		}
	}

	if (m_Scene->GetSceneMode()->GetTypeID() == typeid(CLibrary2ndScene).hash_code())
	{
		CGameObject* Portal = m_Scene->FindObject("Portal");

		if (Portal)
		{
			CComponent* Body = ((CPortal*)Portal)->FindComponent("Body");

			if (Body)
			{
				// GotoNextMap함수는 CInput::Update에서 호출하는거라 아직 이번 프레임의 충돌 조사가 아직 안됐을 때니까
				// 이전 프레임 기준으로 검사한다
				bool Collision = ((CColliderBox2D*)Body)->CheckPrevCollision(m_Body);

				// LobbyScene의 오른쪽 Entrance 포탈에 충돌했고, 위쪽 방향키를 누르고 있을때 여기로 들어온다
				// ==> 양파 재배지 맵을 멀티쓰레드 활용해서 로딩후 SceneChange하기
				if (Collision)
				{
					m_ListCollisionID.clear();

					CLibrary2ndScene* Scene = (CLibrary2ndScene*)(GetScene()->GetSceneMode());
					CRenderManager::GetInst()->SetStartFadeIn(true);
					CSceneManager::GetInst()->SetFadeInEndCallback<CLibrary2ndScene>(Scene, &CLibrary2ndScene::CreateLobbyScene);
				}
			}
		}
	}
}

void CPlayer2D::ProduceSecondSylphideLander(float DeltaTime)
{
	m_SkillBodyEffect->ChangeAnimation("SylphideLancerBodyEffectLeft");
	m_BodySprite->ChangeAnimation("IdleLeft");

	//if (m_IsFlip)
	//	m_BodySprite->Flip();

	Vector3 WorldPos = GetWorldPos();

	CGameObject* NearMonster = m_Scene->FindIncludingNameObject("Onion", WorldPos, Vector2(400.f, 70.f));
	Vector3 MirrorPos = m_SylphideLancerMirror->GetWorldPos();

	for (int i = 0; i < 2; ++i)
	{
		CSylphideLancer* Lancer = m_Scene->CreateGameObject<CSylphideLancer>("SylphideLancer");

		Lancer->SetAllSceneComponentsLayer("MovingObjFront");
		Lancer->SetLancerID(i + 2);
		Lancer->SetWorldPos(MirrorPos.x, MirrorPos.y - 10.f + i * 30.f, MirrorPos.z);
		Lancer->SetCollisionProfile("PlayerAttack");
		Lancer->SetSpeed(-400.f);

		CSpriteComponent* Root = (CSpriteComponent*)Lancer->GetRootComponent();
		Root->ChangeAnimation("SylphideLancerArrowPurple");

		Vector3 MonsterWorldPos;

		if (NearMonster)
		{
			Vector3 LancerPos = Lancer->GetWorldPos();
			MonsterWorldPos = NearMonster->GetWorldPos();
			float Radian = atan((MonsterWorldPos.y - LancerPos.y) / (MonsterWorldPos.x - LancerPos.x));
			float Degree = RadianToDegree(Radian);

			Lancer->SetWorldRotation(0.f, 0.f, Degree);
		}

		else
		{
			Lancer->SetWorldRotation(GetWorldRot());
		}

		if (m_BodySprite->IsFlip())
		{
			Lancer->FlipAll(DeltaTime);
			Lancer->SetSpeed(400.f);
		}
	}
}

void CPlayer2D::EffectEnd(float DeltaTime)
{
}

void CPlayer2D::ReturnIdle(float DeltaTime)
{
	m_BodySprite->ChangeAnimation("IdleLeft");
	m_Dir = PlayerDir::None;
}



