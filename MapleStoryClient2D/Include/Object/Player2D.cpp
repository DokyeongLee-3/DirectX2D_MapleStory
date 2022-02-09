
#include "Player2D.h"
#include "Stage.h"
#include "Device.h"
#include "BulletTornaido.h"
#include "Portal.h"
#include "Scene/Scene.h"
#include "../Scene/LobbyScene.h"
#include "../Scene/OnionScene.h"
#include "Input.h"
#include "PlayerAnimation2D.h"
#include "SylphideLancerEffectAnimation2D.h"
#include "PlayerSkillBodyEffect.h"
#include "BulletCamera.h"
#include "Bullet.h"
#include "SylphideLancer.h"
#include "../Widget/SimpleHUD.h"
#include "Scene/SceneManager.h"
#include "Render/RenderManager.h"
#include "BubbleParticle.h"

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
	m_Camera = (CCameraComponent*)FindComponent("Camera");
	m_SimpleHUDWidget = (CWidgetComponent*)FindComponent("SimpleHUD");

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
	
	m_Body = CreateComponent<CColliderBox2D>("PlayerBody");

	m_Camera = CreateComponent<CCameraComponent>("Camera");

	m_SimpleHUDWidget = CreateComponent<CWidgetComponent>("SimpleHUD");

	m_SimpleHUDWidget->CreateWidgetWindow<CSimpleHUD>("SimpleHUDWidget");

	SetRootComponent(m_BodySprite);

	m_Body->SetCollisionProfile("Player");

	m_Camera->OnViewportCenter();

	m_BodySprite->AddChild(m_SylphideLancerMuzzle);
	//m_BodySprite->AddChild(m_SkillBodyEffect);
	//m_BodySprite->AddChild(m_VoidPressureAttackSphere);
	//m_BodySprite->AddChild(m_VoidPressureSphere);
	
	m_BodySprite->AddChild(m_Body);
	m_BodySprite->AddChild(m_Camera);
	m_BodySprite->AddChild(m_SimpleHUDWidget);

	m_SimpleHUDWidget->SetRelativePos(-50.f, 50.f, 0.f);

	m_BodySprite->SetTransparency(true);
	//m_SylphideLancerMirror->SetTransparency(true);
	//m_SkillBodyEffect->SetTransparency(true);

	m_BodySprite->CreateAnimationInstance<CPlayerAnimation2D>();
	//m_SylphideLancerMirror->CreateAnimationInstance<CSylphideLancerEffectAnimation2D>();
	//m_SkillBodyEffect->CreateAnimationInstance<CPlayerSkillBodyEffect>();



	m_SylphideLancerMuzzle->SetInheritRotZ(true);
	m_SylphideLancerMuzzle->SetPivot(0.5f, 0.5f, 0.f);

	//m_SylphideLancerMirror->SetRelativeScale(120.f, 200.f, 1.f);
	//m_SylphideLancerMirror->SetOpacity(0.f);
	//m_SylphideLancerMirror->SetInheritRotZ(true);
	//m_SylphideLancerMirror->SetPivot(0.5f, 0.5f, 0.f);

	m_BodySprite->SetRelativeScale(Vector3(91.f, 82.f, 1.f) * m_ScaleFactor);
	m_BodySprite->SetRelativePos(100.f, 100.f, 0.f);
	m_BodySprite->SetPivot(0.5f, 0.5f, 0.f);

	CInput::GetInst()->SetKeyCallback<CPlayer2D>("MoveUp", KeyState_Push, this, &CPlayer2D::MoveUp);
	CInput::GetInst()->SetKeyCallback<CPlayer2D>("MoveDown", KeyState_Push, this, &CPlayer2D::MoveDown);
	CInput::GetInst()->SetKeyCallback<CPlayer2D>("RotationZInv", KeyState_Push, this, &CPlayer2D::RotationZInv);
	CInput::GetInst()->SetKeyCallback<CPlayer2D>("RotationZ", KeyState_Push, this, &CPlayer2D::RotationZ);
	CInput::GetInst()->SetKeyCallback<CPlayer2D>("Skill1", KeyState_Down, this, &CPlayer2D::SylphideLancer);

	CInput::GetInst()->SetKeyCallback<CPlayer2D>("Skill2", KeyState_Down, this, &CPlayer2D::Skill2);
	CInput::GetInst()->SetKeyCallback<CPlayer2D>("Flip", KeyState_Down, this, &CPlayer2D::FlipAll);

	//CInput::GetInst()->SetKeyCallback("DirUp", KeyState_Down, this, &CPlayer2D::GotoNextMap);
	//CInput::GetInst()->SetKeyCallback("MoveRight", KeyState_Up, this, &CPlayer2D::Stand);

	return true;
}

void CPlayer2D::Update(float DeltaTime)
{
	CGameObject::Update(DeltaTime);
}

void CPlayer2D::PostUpdate(float DeltaTime)
{
	CComponent* Component = GetRootComponent()->FindComponent("Camera");

	if (Component)
	{
		CSceneMode* SceneMode = m_Scene->GetSceneMode();

		CStage* Stage = nullptr;

		if (SceneMode->GetTypeID() == typeid(CLobbyScene).hash_code())
		{
			// Stage는 Pivot이 (0.f, 0.f)
			Stage = ((CLobbyScene*)SceneMode)->GetStageObject();
		}

		else if (SceneMode->GetTypeID() == typeid(COnionScene).hash_code())
		{
			Stage = ((COnionScene*)SceneMode)->GetStageObject();
		}

		if(Stage)
		{
			Vector3 WorldSize = Stage->GetWorldScale();
			Resolution RS = CDevice::GetInst()->GetResolution();

			Vector2 Ratio = ((CCameraComponent*)Component)->GetRatio();
			Vector3 PlayerWorldPos = GetWorldPos();

			if (PlayerWorldPos.x + RS.Width * (1 - Ratio.x) >= WorldSize.x)
			{
				Vector3 CamWorldPos = ((CCameraComponent*)Component)->GetWorldPos();
				float Pos = WorldSize.x - RS.Width * Ratio.x;
				Pos += -1.f * RS.Width * Ratio.x;
				((CCameraComponent*)Component)->SetWorldPos(Pos, CamWorldPos.y, CamWorldPos.z);
			}

			if (PlayerWorldPos.x - RS.Width * Ratio.x <= 0.f)
			{
				Vector3 CamWorldPos = ((CCameraComponent*)Component)->GetWorldPos();
				((CCameraComponent*)Component)->SetWorldPos(0.f, CamWorldPos.y, CamWorldPos.z);
			}

			if (PlayerWorldPos.y + RS.Height * (1 - Ratio.y) >= WorldSize.y)
			{
				Vector3 CamWorldPos = ((CCameraComponent*)Component)->GetWorldPos();
				float Pos = WorldSize.y - RS.Height * Ratio.y;
				Pos += -1.f * RS.Height * Ratio.y;
				((CCameraComponent*)Component)->SetWorldPos(CamWorldPos.x, Pos, CamWorldPos.z);
			}

			if (PlayerWorldPos.y - RS.Height * Ratio.y <= 0.f)
			{
				Vector3 CamWorldPos = ((CCameraComponent*)Component)->GetWorldPos();
				((CCameraComponent*)Component)->SetWorldPos(CamWorldPos.x, 0.f, CamWorldPos.z);
			}
		}

	}

	CGameObject::PostUpdate(DeltaTime);
}

CPlayer2D* CPlayer2D::Clone()
{
	return new CPlayer2D(*this);
}

void CPlayer2D::SetScene(CScene* Scene)
{
	m_Scene = Scene;

	if(m_SimpleHUDWidget)
		m_SimpleHUDWidget->GetWidgetWindow()->GetViewport()->SetScene(Scene);
}

void CPlayer2D::MoveUp(float DeltaTime)
{
	m_BodySprite->AddRelativePos(m_BodySprite->GetWorldAxis(AXIS_Y) * 300.f * DeltaTime);

	GotoNextMap(DeltaTime);
}

void CPlayer2D::MoveDown(float DeltaTime)
{
	m_BodySprite->AddRelativePos(m_BodySprite->GetWorldAxis(AXIS_Y) * -300.f * DeltaTime);
}

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

	// Scene의 m_ObjList에서 몬스터 찾아서 여기서 실피드랜서 방향 설정해주기

	Vector3 WorldPos = GetWorldPos();

	CGameObject* NearMonster = m_Scene->FindIncludingNameObject("Monster", WorldPos, 400.f);

	for (int i = 0; i < 2; ++i)
	{
		CSylphideLancer* Lancer = m_Scene->CreateGameObject<CSylphideLancer>("SylphideLancer");
		Lancer->SetAllSceneComponentsLayer("MovingObjFront");
		Lancer->SetLancerID(i);

		Vector3 MonsterWorldPos;

		if (NearMonster)
		{
			MonsterWorldPos = NearMonster->GetWorldPos();
			float Radian = atan((MonsterWorldPos.y - WorldPos.y)/(MonsterWorldPos.x - WorldPos.x));
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
		}

		Lancer->SetWorldPos(WorldPos.x, WorldPos.y + i * 30.f, 0.f);
		Lancer->SetCollisionProfile("PlayerAttack");
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
	Bullet->SetCollisionProfile("PlayerAttack");
	
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

void CPlayer2D::GotoNextMap(float DeltaTime)
{
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
					CLobbyScene* Scene = (CLobbyScene*)(GetScene()->GetSceneMode());
					CRenderManager::GetInst()->SetStartFadeIn(true);
					CSceneManager::GetInst()->SetFadeInEndCallback<CLobbyScene>(Scene, &CLobbyScene::CreateOnionScene);
				}
			}
		}
	}
}

void CPlayer2D::ProduceSecondSylphideLander(float DeltaTime)
{
	Vector3 WorldPos = GetWorldPos();

	CGameObject* NearMonster = m_Scene->FindIncludingNameObject("Monster", WorldPos, 400.f);

	for (int i = 0; i < 2; ++i)
	{
		CSylphideLancer* Lancer = m_Scene->CreateGameObject<CSylphideLancer>("SylphideLancer");
		Lancer->SetAllSceneComponentsLayer("MovingObjFront");
		Lancer->SetLancerID(i + 2);

		Vector3 MonsterWorldPos;

		if (NearMonster)
		{
			MonsterWorldPos = NearMonster->GetWorldPos();
			float Radian = atan((MonsterWorldPos.y - WorldPos.y) / (MonsterWorldPos.x - WorldPos.x));
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
		}

		Lancer->SetWorldPos(WorldPos.x, WorldPos.y + i * 30.f, 0.f);
		Lancer->SetCollisionProfile("PlayerAttack");
	}
}



