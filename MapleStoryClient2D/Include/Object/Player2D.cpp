
#include "Player2D.h"
#include "Stage.h"
#include "Device.h"
#include "BulletTornaido.h"
#include "Portal.h"
#include "Scene/Scene.h"
#include "../Scene/LobbyScene.h"
#include "../Scene/OnionScene.h"
#include "../Scene/WayToZakumScene.h"
#include "Input.h"
#include "PlayerAnimation2D.h"
#include "../Animation/SylphideLancerMirrorAnimation.h"
#include "../Animation/PlayerSkillBodyEffect.h"
#include "../Animation/VoidPressureAttackSphere.h"
#include "../Animation/PlayerOrb.h"
#include "BulletCamera.h"
#include "Bullet.h"
#include "SylphideLancer.h"
#include "../Widget/SimpleHUD.h"
#include "Scene/SceneManager.h"
#include "Render/RenderManager.h"
#include "VoidPressure.h"
#include "VoidPressureOrb.h"
#include "PlayerSkillInfo.h"
#include "Scene/NavigationManager.h"

CPlayer2D::CPlayer2D() :
	m_BodySprite(nullptr),
	m_SylphideLancerMirror(nullptr),
	m_SkillBodyEffect(nullptr),
	m_VoidPressure(nullptr),
	m_VoidPressureOrb(nullptr),
	m_ScaleFactor(1.f),
	m_Opacity(1.f),
	m_IsFlip(false),
	m_Dir(PlayerDir::None),
	m_OnLightTransforming(false)
{
	SetTypeID<CPlayer2D>();
	m_PlayerSkillInfo = new CPlayerSkillInfo;
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
	m_SimpleHUDWidget = (CWidgetComponent*)FindComponent("SimpleHUD");

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
	m_Body->SetOffset(0.f, 10.f, 0.f);

	m_Camera = CreateComponent<CCameraComponent>("Camera");

	/*m_SimpleHUDWidget = CreateComponent<CWidgetComponent>("SimpleHUD");

	m_SimpleHUDWidget->CreateWidgetWindow<CSimpleHUD>("SimpleHUDWidget");*/

	SetRootComponent(m_BodySprite);

	m_Body->SetCollisionProfile("Player");

	m_Camera->OnViewportCenter();

	m_BodySprite->AddChild(m_PlayerOrb);
	m_BodySprite->AddChild(m_SylphideLancerMirror);
	m_BodySprite->AddChild(m_SkillBodyEffect);
	
	m_BodySprite->AddChild(m_Body);
	m_BodySprite->AddChild(m_Camera);
	//m_BodySprite->AddChild(m_SimpleHUDWidget);

	//m_SimpleHUDWidget->SetRelativePos(-50.f, 50.f, 0.f);

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
	m_Body->AddCollisionCallback<CPlayer2D>(Collision_State::Stay, this, &CPlayer2D::CollisionBeginCallback);
	m_Gravity = true;



	m_NavAgent = CreateComponent<CNavAgent>("NavAgent");


	CInput::GetInst()->CreateKey("MoveUp", VK_UP);
	CInput::GetInst()->CreateKey("MoveDown", VK_DOWN);
	CInput::GetInst()->CreateKey("MoveLeft", VK_LEFT);
	CInput::GetInst()->CreateKey("MoveRight", VK_RIGHT);
	CInput::GetInst()->CreateKey("SylphideLancer", 'Q');
	CInput::GetInst()->CreateKey("VoidPressure", 'W');
	CInput::GetInst()->CreateKey("LightTransforming", VK_SPACE);
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
			// Stage�� Pivot�� (0.f, 0.f)
			Stage = ((CLobbyScene*)SceneMode)->GetStageObject();
		}

		else if (SceneMode->GetTypeID() == typeid(COnionScene).hash_code())
		{
			Stage = ((COnionScene*)SceneMode)->GetStageObject();
		}

		else if (SceneMode->GetTypeID() == typeid(CWayToZakumScene).hash_code())
		{
			Stage = ((CWayToZakumScene*)SceneMode)->GetStageObject();
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
				Pos -= RS.Width * Ratio.x;
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

void CPlayer2D::MoveLeft(float DeltaTime)
{
	if (m_VoidPressure)
	{
		if (m_VoidPressure->IsEnable() && !m_VoidPressure->GetOnDestroy())
		{
			m_VoidPressure->AddWorldPos(-200.f * DeltaTime, 0.f, 0.f);
			return;
		}
	}

	// ���̵� ������ ��ü�� ���� �������� ���ų� ������ �Ҹ�ǰ��� ������ �� �ְ��Ѵ�
	if (!m_VoidPressure || !m_VoidPressure->IsEnable())
	{
		if (m_BodySprite->IsFlip())
			FlipAll(DeltaTime);

		m_BodySprite->AddRelativePos(m_BodySprite->GetWorldAxis(AXIS_X) * -300.f * DeltaTime);
		m_BodySprite->ChangeAnimation("WalkLeft");
		m_Dir = PlayerDir::Left;
	}
}

void CPlayer2D::MoveRight(float DeltaTime)
{
	if (m_VoidPressure)
	{
		if (m_VoidPressure->IsEnable() && !m_VoidPressure->GetOnDestroy())
		{
			m_VoidPressure->AddWorldPos(200.f * DeltaTime, 0.f, 0.f);
			return;
		}
	}

	// ���̵� ������ ��ü�� ���� �������� ���ų� ������ �Ҹ�ǰ��� ������ �� �ְ��Ѵ�
	if (!m_VoidPressure || !m_VoidPressure->IsEnable())
	{
 		if (!m_BodySprite->IsFlip())
			FlipAll(DeltaTime);

		m_BodySprite->AddRelativePos(m_BodySprite->GetWorldAxis(AXIS_X) * 300.f * DeltaTime);
		m_BodySprite->ChangeAnimation("WalkLeft");
		m_Dir = PlayerDir::Right;
	}
}

void CPlayer2D::SylphideLancer(float DeltaTime)
{
	SkillInfo* SkillInfo = m_PlayerSkillInfo->FindSkillInfo("SylphideLancer");

	if (SkillInfo->Active == true && SkillInfo->AccTime < SkillInfo->CoolTime)
		return;


	// Scene�� m_ObjList���� ���� ã�Ƽ� ���⼭ ���ǵ巣�� ���� �������ֱ�

	m_BodySprite->ChangeAnimation("HealLeft");

	CGameObject* NearMonster = m_Scene->FindIncludingNameObject("Monster", GetWorldPos(), 400.f);

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
	SkillInfo* SkillInfo = m_PlayerSkillInfo->FindSkillInfo("VoidPressure");

	if (SkillInfo->Active == true && SkillInfo->AccTime < SkillInfo->CoolTime)
		return;

	if(!m_Scene->GetResource()->IsPlaying("VoidPressureLoop"))
		m_Scene->GetResource()->SoundPlay("VoidPressureLoop");

	if (!m_VoidPressure)
	{
		m_Scene->GetResource()->SoundPlay("VoidPressureUse");

		m_BodySprite->ChangeAnimation("HealLeft");

		// �浹ü�� ������ GameObject�� ��ӹ��� CVoidPressure Ŭ������ �����Ұ�(�ܼ� SpriteComponent�� �����ϴ°� �ƴ϶�)
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
	if (m_Dir == PlayerDir::None)
		return;

	SkillInfo* SkillInfo = m_PlayerSkillInfo->FindSkillInfo("LightTransforming");

	if (SkillInfo->Active == true && SkillInfo->AccTime < SkillInfo->CoolTime)
		return;



	m_OnLightTransforming = true;
	m_BodySprite->SetOpacity(0.5f);
	m_Body->Enable(false);

	m_Scene->GetResource()->SoundPlay("LightTransforming");

	m_SkillBodyEffect->ChangeAnimation("LightTransformingLeft");

	if (m_Dir == PlayerDir::Right)
		m_BodySprite->AddWorldPos(100.f, 0.f, 0.f);

	else if(m_Dir == PlayerDir::Left)
		m_BodySprite->AddWorldPos(-100.f, 0.f, 0.f);

	SkillInfo->Active = true;
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
}

void CPlayer2D::CollisionStayCallback(const CollisionResult& Result)
{
}

void CPlayer2D::MovePointDown(float DeltaTime)
{
	Vector2 MousePos = CInput::GetInst()->GetMouseWorld2DPos();

	Move(Vector3(MousePos.x, MousePos.y, 0.f));

}




void CPlayer2D::Skill2(float DeltaTime)
{
	CBulletCamera* Bullet = m_Scene->CreateGameObject<CBulletCamera>("Bullet");

	//Bullet->SetWorldPos(GetWorldPos() + GetWorldAxis(AXIS_Y) * 75.f);
	//Bullet->SetWorldPos(m_SylphideLancerMuzzle->GetWorldPos());
	Bullet->SetWorldRotation(GetWorldRot());
	Bullet->SetCollisionProfile("PlayerAttack");
	
}

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
	if (m_Scene->GetSceneMode()->GetTypeID() == typeid(CLobbyScene).hash_code())
	{
		CGameObject* Portal = m_Scene->FindObject("RightPortal");

		if (Portal)
		{
			CComponent* Body = ((CPortal*)Portal)->FindComponent("Body");

			if (Body)
			{
				// GotoNextMap�Լ��� CInput::Update���� ȣ���ϴ°Ŷ� ���� �̹� �������� �浹 ���簡 ���� �ȵ��� ���ϱ�
				// ���� ������ �������� �˻��Ѵ�
				bool Collision = ((CColliderBox2D*)Body)->CheckPrevCollision(m_Body);

				// LobbyScene�� ������ Entrance ��Ż�� �浹�߰�, ���� ����Ű�� ������ ������ ����� ���´�
				// ==> ���� ����� ���� ��Ƽ������ Ȱ���ؼ� �ε��� SceneChange�ϱ�
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
	m_SkillBodyEffect->ChangeAnimation("SylphideLancerBodyEffectLeft");
	m_BodySprite->ChangeAnimation("IdleLeft");

	//if (m_IsFlip)
	//	m_BodySprite->Flip();

	Vector3 WorldPos = GetWorldPos();

	CGameObject* NearMonster = m_Scene->FindIncludingNameObject("Monster", WorldPos, 400.f);
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



