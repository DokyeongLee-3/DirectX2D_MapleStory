
#include "Player2D.h"
#include "Stage.h"
#include "Device.h"
#include "Portal.h"
#include "StaticMapObj.h"
#include "Scene/Scene.h"
#include "../Scene/LobbyScene.h"
#include "../Scene/OnionScene.h"
#include "../Scene/RadishScene.h"
#include "../Scene/WayToZakumScene.h"
#include "../Scene/Library2ndScene.h"
#include "../Scene/ZakumAltarScene.h"
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
#include "DeathSide.h"
#include "PlayerSkillInfo.h"
#include "../Widget/PlayerDamageFont.h"
#include "Engine.h"
#include "../Client.h"
#include "Component/ColliderComponent.h"
#include "Bill.h"
#include "Tomb.h"

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
	m_OnKnockBackTime(1.f),
	m_OnKnockBackAccTime(0.f),
	m_HitOpacity(0.7f),
	m_OnHit(false),
	m_OnHitTime(2.f),
	m_OnHitAccTime(0.f),
	m_OnLope(false),
	m_LopeJump(false),
	m_LopeEnable(true),
	m_IsChanging(false),
	m_Dead(false),
	m_PrevSameTileObjColliderCount(0),
	m_OnCameraShake(false),
	m_CameraShakeFrequency(0.1f),
	m_CameraShakeTime(1.f),
	m_AccCameraShakeTime(0.f),
	m_CameraShakeDir(0.f, 1.f),
	m_AccCameraShakeSingleDirTime(0.f)
{
	SetTypeID<CPlayer2D>();
	m_PlayerSkillInfo = new CPlayerSkillInfo;
	m_Gravity = true;

	m_JumpForce = 400.f;
	m_DirVector = Vector3(1.f, 0.f, 0.f);
}

CPlayer2D::CPlayer2D(const CPlayer2D& obj) :
	CGameObject(obj)
{
	m_BodySprite = (CSpriteComponent*)FindComponent("PlayerSprite");
	m_SylphideLancerMirror = (CSpriteComponent*)FindComponent("PlayerSylphideLancerMirror");
	m_SkillBodyEffect = (CSpriteComponent*)FindComponent("PlayerSkillBodyEffect");
	m_PlayerOrb = (CSpriteComponent*)FindComponent("PlayerOrb");
	m_DamageWidgetComponent = (CWidgetComponent*)FindComponent("PlayerDamageFont");

	m_Body = (CColliderBox2D*)FindComponent("Body");
	m_Camera = (CCameraComponent*)FindComponent("Camera");

	m_Opacity = obj.m_Opacity;
}

CPlayer2D::~CPlayer2D()
{
	SAFE_DELETE(m_PlayerSkillInfo);
}

CPlayerSkillInfo* CPlayer2D::GetPlayerSkillInfo() const
{
	return m_PlayerSkillInfo;
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
	m_DamageWidgetComponent = CreateComponent<CWidgetComponent>("PlayerDamageFont");

	m_Body = CreateComponent<CColliderBox2D>("PlayerBody");
	m_Body->SetExtent(14.f, 35.f);
	m_Body->SetOffset(0.f, 13.f, 0.f);

	m_Camera = CreateComponent<CCameraComponent>("Camera");
	m_Camera->SetInheritParentRotationPosZ(false);

	m_Body->SetCollisionProfile("Player");

	m_Camera->OnViewportCenter();

	SetRootComponent(m_BodySprite);

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

	m_SylphideLancerMirror->CreateAnimationInstance<CSylphideLancerMirrorAnimation>();
	m_SylphideLancerMirror->SetInheritRotZ(false);
	m_SylphideLancerMirror->SetPivot(0.5f, 0.5f, 0.f);
	m_SylphideLancerMirror->SetRelativePos(50.f, 80.f, 0.f);
	m_SylphideLancerMirror->SetPivot(0.5f, 0.5f, 0.f);

	//m_BodySprite->SetRelativePos(100.f, 100.f, 0.f);
	m_BodySprite->SetPivot(0.5f, 0.5f, 0.f);

	m_SkillBodyEffect->SetPivot(0.5f, 0.5f, 0.f);
	m_SkillBodyEffect->SetRelativePos(0.f, 11.f, -5.f);

	m_PlayerOrb->SetRelativePos(60.f, 50.f, 0.f);
	m_PlayerOrb->SetPivot(0.5f, 0.5f, 0.f);

	m_Body->AddCollisionCallback<CPlayer2D>(Collision_State::Begin, this, &CPlayer2D::CollisionBeginCallback);
	m_Body->AddCollisionCallback<CPlayer2D>(Collision_State::End, this, &CPlayer2D::CollisionEndCallback);
	SetGravityFactor(1100.f);


	m_DamageWidgetComponent->UseAlphaBlend(true);
	m_DamageWidgetComponent->SetLayerName("ScreenWidgetComponent");
	//m_DamageWidgetComponent->SetRelativePos(-20.f, 0.f, 0.f);

	CPlayerDamageFont* DamageFont = m_DamageWidgetComponent->CreateWidgetWindow<CPlayerDamageFont>("DamageFontWidget");

	CInput::GetInst()->CreateKey("MoveUp", VK_UP);
	CInput::GetInst()->CreateKey("MoveDown", VK_DOWN);
	CInput::GetInst()->CreateKey("MoveLeft", VK_LEFT);
	CInput::GetInst()->CreateKey("MoveRight", VK_RIGHT);
	CInput::GetInst()->CreateKey("SylphideLancer", 'Q');
	CInput::GetInst()->CreateKey("VoidPressure", 'W');
	CInput::GetInst()->CreateKey("LightTransforming", VK_SPACE);
	CInput::GetInst()->CreateKey("Jump", 'Z');
	CInput::GetInst()->CreateKey("DeathSide", 'E');
	CInput::GetInst()->CreateKey("PickItem", 'X');
	CInput::GetInst()->CreateKey("LevelUp", 'L');
	//CInput::GetInst()->CreateKey("Flip", 'F');

	CInput::GetInst()->SetKeyCallback<CPlayer2D>("MoveUp", KeyState_Push, this, &CPlayer2D::MoveUp);
	CInput::GetInst()->SetKeyCallback<CPlayer2D>("MoveDown", KeyState_Push, this, &CPlayer2D::MoveDown);
	CInput::GetInst()->SetKeyCallback<CPlayer2D>("MoveLeft", KeyState_Push, this, &CPlayer2D::MoveLeft);
	CInput::GetInst()->SetKeyCallback<CPlayer2D>("MoveRight", KeyState_Push, this, &CPlayer2D::MoveRight);
	CInput::GetInst()->SetKeyCallback<CPlayer2D>("MoveLeft", KeyState_Up, this, &CPlayer2D::ReturnIdle);
	CInput::GetInst()->SetKeyCallback<CPlayer2D>("MoveRight", KeyState_Up, this, &CPlayer2D::ReturnIdle);
	CInput::GetInst()->SetKeyCallback<CPlayer2D>("MoveUp", KeyState_Up, this, &CPlayer2D::RopeActionStop);
	CInput::GetInst()->SetKeyCallback<CPlayer2D>("MoveDown", KeyState_Up, this, &CPlayer2D::RopeActionStop);

	CInput::GetInst()->SetKeyCallback<CPlayer2D>("SylphideLancer", KeyState_Down, this, &CPlayer2D::SylphideLancer);
	CInput::GetInst()->SetKeyCallback<CPlayer2D>("VoidPressure", KeyState_Push, this, &CPlayer2D::VoidPressure);
	CInput::GetInst()->SetKeyCallback<CPlayer2D>("VoidPressure", KeyState_Up, this, &CPlayer2D::VoidPressureCancle);
	CInput::GetInst()->SetKeyCallback<CPlayer2D>("LightTransforming", KeyState_Down, this, &CPlayer2D::LightTransforming);
	CInput::GetInst()->SetKeyCallback<CPlayer2D>("Jump", KeyState_Down, this, &CPlayer2D::Jump);
	CInput::GetInst()->SetKeyCallback<CPlayer2D>("DeathSide", KeyState_Down, this, &CPlayer2D::DeathSide);
	CInput::GetInst()->SetKeyCallback<CPlayer2D>("PickItem", KeyState_Down, this, &CPlayer2D::PickItem);
	CInput::GetInst()->SetKeyCallback<CPlayer2D>("LevelUp", KeyState_Down, this, &CPlayer2D::LevelUp);
	//CInput::GetInst()->SetKeyCallback<CPlayer2D>("Flip", KeyState_Down, this, &CPlayer2D::FlipAll);

	//CInput::GetInst()->SetKeyCallback("DirUp", KeyState_Down, this, &CPlayer2D::GotoNextMap);
	//CInput::GetInst()->SetKeyCallback("MoveRight", KeyState_Up, this, &CPlayer2D::Stand);


	//CInput::GetInst()->SetKeyCallback<CPlayer2D>("MovePoint", KeyState_Down, this, &CPlayer2D::MovePointDown);

	m_GhostVector = Vector3(1.f, 0.f, 0.f);


	return true;
}

void CPlayer2D::Update(float DeltaTime)
{
	m_PrevFrameWorldPos = GetWorldPos();

	CGameObject::Update(DeltaTime);

	m_PlayerSkillInfo->Update(DeltaTime);

	if (m_Dead)
	{
		GetRootComponent()->AddWorldPos(m_GhostVector.x / 10.f, m_GhostVector.y / 10.f, 0.f);

		Vector3 Rot = Vector3(0.f, 0.f, DeltaTime * 100.f);

		Matrix	matRot;
		matRot.Rotation(Rot);

		m_GhostVector = m_GhostVector.TransformCoord(matRot);
	}

	if (m_OnLightTransforming && !m_Dead)
	{
		SkillInfo* Info = m_PlayerSkillInfo->FindSkillInfo("LightTransforming");

		if (Info && Info->Active == false)
		{
			m_OnLightTransforming = false;
			m_BodySprite->SetOpacity(1.f);
		}
	}

	if (m_OnJump && !m_Dead)
	{
		AddWorldPos(0.f, DeltaTime * m_JumpForce, 0.f);
	}

	if (m_OnHit && !m_Dead)
	{
		m_OnHitAccTime += DeltaTime;

		if (m_OnHitAccTime >= m_OnHitTime)
		{
			m_OnHit = false;
			m_OnHitAccTime = 0.f;
			m_BodySprite->SetOpacity(1.f);
			return;
		}

		int Percent = int(m_OnHitAccTime * 10) % 10;

		switch (Percent)
		{
		case 1:
		case 3: 
		case 5:
		case 7:
		case 9:
			m_BodySprite->SetOpacity(m_HitOpacity);
		break;

		case 0:
		case 2:
		case 4:
		case 6:
		case 8:
			m_BodySprite->SetOpacity(1.f);
		break;

		}

		KnockBack(DeltaTime);
	}

}

void CPlayer2D::PostUpdate(float DeltaTime)
{

	Vector3 CamWorldPos = m_Camera->GetWorldPos();

	m_CurrentFrameCameraMove = Vector2(CamWorldPos.x - m_PrevFrameCameraMove.x, CamWorldPos.y - m_PrevFrameCameraMove.y);
	// �̷��� ������ Camera�� transform�� Z���� 0���� ������ָ� ī�޶� Player�� Root Component�� ī�޶� �ڽ��� Z������ �����޾Ƽ� 
	// ũ��� Z���� ����, ��ȣ�� ������ ���� ī�޶��� m_matView[4][3]�� ����ż� �׸�ŭ �Ź� z�������� translation�ǰ� �ؼ� �������ż�
	// �ᱹ �� ���� ���񰪺��� ���� z���� ������ ���̾�(ex.���� �� �ڵ忡�� CoveringMapObj ���̾ ���� SceneComponent��)��
	// SceneComponent�� �ᱹ view��ȯ�Ŀ��� Z���� �������ż� ����� �ȵɰ��̴�
	m_Camera->SetWorldPos(CamWorldPos.x, CamWorldPos.y, 0.f);

	// ī�޶� ȭ�� ������ �����°ŵ� ������ �� �ڿ� ���������� ��������� transform���� ������� �������ϹǷ� 
	// ������ CGameObject::PostUpdate���� ���� ������Ѵ�
	CameraTrack();

	if (m_OnCameraShake)
	{
		CameraShake(DeltaTime);
	}

	CGameObject::PostUpdate(DeltaTime);

	Vector3 CurrentFrameWorldPos = GetWorldPos();
	m_CurrentFrameMove = CurrentFrameWorldPos - m_PrevFrameWorldPos;

	m_PrevFrameCameraMove = Vector2(m_Camera->GetWorldPos().x, m_Camera->GetWorldPos().y);

	CSceneMode* SceneMode = m_Scene->GetSceneMode();
	
	if (SceneMode->GetTypeID() == typeid(CWayToZakumScene).hash_code())
		((CWayToZakumScene*)SceneMode)->SetPerspective();

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
	m_Dir = PlayerDir::Up;

	if (m_OnLope)
	{
		AddWorldPos(0.f, 120.f * DeltaTime, 0.f);

		CAnimationSequence2DData* CurrentAnim = m_BodySprite->GetCurrentAnimation();
		CAnimationSequence2DInstance* Instance = m_BodySprite->GetAnimationInstance();

		if(CurrentAnim && Instance && Instance->CheckCurrentAnimation("Rope") && !CurrentAnim->IsLoop())
			CurrentAnim->SetLoop(true);

		if (m_Body->CheckPrevCollisionGameObjectType(typeid(CLopeTileObject).hash_code()))
		{
			CColliderBox2D* Component = (CColliderBox2D*)m_Body->FindPrevCollisionComponentByObjectType(typeid(CLopeTileObject).hash_code());

			if (Component)
			{
				// ������ �Ʒ����� ���� Ÿ�� �ö󰥶��� ���� -> ���� �ֻ���� �÷��̾� �߹� y��ǥ���� ���ų� ���ƾ��Ѵ�
				Box2DInfo Info = Component->GetInfo();
				Box2DInfo PlayerInfo = m_Body->GetInfo();

				// �� ��� ������ ������ �� �ö󰬴�
				if (Info.Center.y + Info.Length.y <= PlayerInfo.Center.y - PlayerInfo.Length.y + 3.f)
				{
					m_LopeJump = false;
					m_OnJump = false;
					m_OnLope = false;
					m_Gravity = true;
					m_LopeEnable = false;
					return;
				}
			}
		}
		return;
	}


	if (m_Body->CheckPrevCollisionGameObjectType(typeid(CLopeTileObject).hash_code()))
	{
		CColliderBox2D* Component = (CColliderBox2D*)m_Body->FindPrevCollisionComponentByObjectType(typeid(CLopeTileObject).hash_code());

		if (Component)
		{
			Box2DInfo MyInfo = m_Body->GetInfo();
			Box2DInfo ComponentInfo = Component->GetInfo();

			bool TileCollision = m_Body->CheckPrevCollisionGameObjectType(typeid(CTileObject).hash_code());

			// ���� ���ϴܿ� ó�� �޴޸��� ���� 
			// 1. Ÿ�Ͽ� ���� ���� �ֻ�ܿ� �÷��̾�� ������ �浹�� ��Ȳ���� ������Ű �����ٰ� �޴޸��� ��� �����ϱ� 
			// 2. ���� ������ �� �ö󰡼� m_Gravity = true�Ǿ �÷��̾ �������� ���߿� ������Ű�� ��� ������ �־ �ٽ� �ٿ� �޴޸��� �ʰ��ϱ�
			if (!TileCollision && m_LopeEnable && abs(MyInfo.Center.x - ComponentInfo.Center.x) < 8.f)
			{
				Vector3 MyPos = GetWorldPos();
				SetWorldPos(ComponentInfo.Center.x, MyPos.y, MyPos.z);

				m_Gravity = false;
				m_OnJump = false;
				m_OnLope = true;
				m_GravityAccTime = 0.f;
				m_BodySprite->ChangeAnimation("Rope");

			}
		}

		return;
	}

	GotoNextMap(DeltaTime);
}

void CPlayer2D::MoveDown(float DeltaTime)
{
	m_Dir = PlayerDir::Down;

	if (m_OnLope)
	{
		CAnimationSequence2DData* CurrentAnim = m_BodySprite->GetCurrentAnimation();
		CAnimationSequence2DInstance* Instance = m_BodySprite->GetAnimationInstance();

		if (CurrentAnim && Instance && Instance->CheckCurrentAnimation("Rope") && !CurrentAnim->IsLoop())
			CurrentAnim->SetLoop(true);

		AddWorldPos(0.f, -120.f * DeltaTime, 0.f);


		// ������ ������ �� ���������� üũ
		if (m_Body->CheckPrevCollisionGameObjectType(typeid(CLopeTileObject).hash_code()))
		{
			CColliderBox2D* Component = (CColliderBox2D*)m_Body->FindPrevCollisionComponentByObjectType(typeid(CLopeTileObject).hash_code());

			if (Component)
			{
				// ������ �Ʒ����� ���� Ÿ�� �ö󰥶��� ���� -> ���� �ֻ���� �÷��̾� �߹� y��ǥ���� ���ų� ���ƾ��Ѵ�
				Box2DInfo Info = Component->GetInfo();
				Box2DInfo PlayerInfo = m_Body->GetInfo();

				// �� ��� ������ ������ �� ��������
				if (Info.Center.y - Info.Length.y >= PlayerInfo.Center.y + PlayerInfo.Length.y - 3.f)
				{
					m_LopeJump = false;
					m_OnJump = false;
					m_OnLope = false;
					m_Gravity = true;
					return;
				}
			}
		}
		return;
	}

	// Ÿ�� ��� �����鼭 ���� �ֻ�ܿ��� ó�� �޴޷��� ��������
	if (m_Body->CheckPrevCollisionGameObjectType(typeid(CLopeTileObject).hash_code()))
	{
		CColliderBox2D* Component = (CColliderBox2D*)m_Body->FindPrevCollisionComponentByObjectType(typeid(CLopeTileObject).hash_code());

		if (Component)
		{
			Box2DInfo MyInfo = m_Body->GetInfo();
			Box2DInfo ComponentInfo = Component->GetInfo();

			bool TileCollision = m_Body->CheckPrevCollisionGameObjectType(typeid(CTileObject).hash_code());

			//  Ÿ�� ��� �����鼭 ���� �ֻ�ܿ��� ó�� �޴޷��� ��������
			if (TileCollision && MyInfo.Center.y > ComponentInfo.Center.y && abs(MyInfo.Center.x - ComponentInfo.Center.x) < 8.f)
			{
				SetWorldPos(Component->GetWorldPos().x, GetWorldPos().y, 0.f);

				m_Gravity = false;
				m_OnJump = false;
				m_OnLope = true;
				m_GravityAccTime = 0.f;
				m_BodySprite->ChangeAnimation("Rope");

			}
		}

		return;
	}
}

void CPlayer2D::MoveLeft(float DeltaTime)
{
	if (m_OnKnockBack || m_OnLope || m_Dead)
		return;

	if (m_VoidPressure)
	{
		if (m_VoidPressure->IsEnable() && !m_VoidPressure->GetOnDestroy())
		{
			m_VoidPressure->AddWorldPos(-300.f * DeltaTime, 0.f, 0.f);
			return;
		}
	}

	// ���̵� ������ ��ü�� ���� �������� ���ų� ������ �Ҹ�ǰ��� ������ �� �ְ��Ѵ�
	if (!m_VoidPressure || !m_VoidPressure->IsEnable())
	{
		bool IsFlip = m_BodySprite->IsFlip();

		// ���ǵ巣�� �������̶�� �������� ������ȯ �Ұ����ϰ� �ϱ�
		if (m_SylphideLancerMirror->GetCurrentAnimation())
			return;

		if (IsFlip)
			FlipAll(DeltaTime);

		std::vector<CColliderBox2D*>	vecCollider;

		m_Body->FindMultipleCollisionComponentByObjType<CColliderBox2D, CTileObject>(vecCollider);

		size_t Count = vecCollider.size();

		for (size_t i = 0; i < Count; ++i)
		{
			if (Count >= 2 && vecCollider[i]->GetWorldRot().z < 0.f && m_DirVector.y == 0.f)
			{
				Vector3 DirRot = vecCollider[i]->GetWorldRot();
				Matrix MatRot;
				MatRot.Rotation(DirRot);
				m_DirVector = m_DirVector.TransformCoord(MatRot);
				break;
			}
		}

		Vector3 CamWorldPos = m_Camera->GetWorldPos();
		m_PrevFrameCameraMove = Vector2(CamWorldPos.x, CamWorldPos.y);

		//m_BodySprite->AddWorldPos(m_BodySprite->GetWorldAxis(AXIS_X) * -180.f * DeltaTime);
		if(m_OnJump)
			m_RootComponent->AddWorldPos(Vector3(-180.f * DeltaTime, 0.f, 0.f) * m_DirVector);
		else
			m_RootComponent->AddWorldPos(Vector3(-180.f * DeltaTime, -180.f * DeltaTime, 0.f) * m_DirVector);

		if (!m_OnJump && !m_BodySprite->GetAnimationInstance()->CheckCurrentAnimation("WalkLeft"))
			m_BodySprite->ChangeAnimation("WalkLeft");

		m_Dir = PlayerDir::Left;
	}
}

void CPlayer2D::MoveRight(float DeltaTime)
{
 	if (m_OnKnockBack || m_OnLope || m_Dead)
		return;

	if (m_VoidPressure)
	{
		if (m_VoidPressure->IsEnable() && !m_VoidPressure->GetOnDestroy())
		{
			m_VoidPressure->AddWorldPos(300.f * DeltaTime, 0.f, 0.f);
			return;
		}
	}

	// ���̵� ������ ��ü�� ���� �������� ���ų� ������ �Ҹ�ǰ��� ������ �� �ְ��Ѵ�
	if (!m_VoidPressure || !m_VoidPressure->IsEnable())
	{
		bool IsFlip = m_BodySprite->IsFlip();

		// ���ǵ巣�� �������̶�� ���������� ������ȯ �Ұ����ϰ� �ϱ�
		if (m_SylphideLancerMirror->GetCurrentAnimation())
			return;

 		if (!m_BodySprite->IsFlip())
			FlipAll(DeltaTime);

		//m_BodySprite->AddWorldPos(m_BodySprite->GetWorldAxis(AXIS_X) * 180.f * DeltaTime);

		std::vector<CColliderBox2D*>	vecCollider;

		m_Body->FindMultipleCollisionComponentByObjType<CColliderBox2D, CTileObject>(vecCollider);

		size_t Count = vecCollider.size();

		//������ �������� ������ �������� ������ ������ �ȿö󰡰� �����ع������� ���� 
		// -> m_DirVector�� (1, 0, 0)�����µ� m_DirRotation�� (0, 0, -34) ����
		// �� ������ ���� �̵��ϸ鼭 ������ ������(CollisionBegin���� if (SameTileObjColliderCount == 2 && EarlyCollider)�� �ƴϰ� else�� ����
		// m_DirRotation�� 34�ǰ�, m_DirVector�� 34����ŭ TransformCoor��), �׷��� �浹ü ��ٷ� �ڿ� �浹ü 2�� ���ÿ� �ε���
		// if (SameTileObjColliderCount == 2 && EarlyCollider)�� ���µ� ColliderRot�� ���� �浹ü, EarlyCollider�� 34�� �浹ü�̴�
		// �׷��� ColliderRot�� 0 - 34�ؼ� - 34�� �ǰ�, m_DirRotation�� 0�� �ǰ� m_DirVector��(1, 0, 0)�� �Ǿ
		// �� ���¿��� ������ ����Ű�� ������ �̵��ϸ� ������ �ȿö󰡰� �װ� �հ� �����ع����Ƿ� �� ��츦 ����ó���Ѵ�
		for (size_t i = 0; i < Count; ++i)
		{
			if (Count >= 2 && vecCollider[i]->GetWorldRot().z > 0.f && m_DirVector.y == 0.f)
			{
				Vector3 DirRot = vecCollider[i]->GetWorldRot();
				Matrix MatRot;
				MatRot.Rotation(DirRot);
				m_DirVector = m_DirVector.TransformCoord(MatRot);
				break;
			}
		}

		Vector3 CamWorldPos = m_Camera->GetWorldPos();
		m_PrevFrameCameraMove = Vector2(CamWorldPos.x, CamWorldPos.y);

		if (m_OnJump)
			m_RootComponent->AddWorldPos(Vector3(180.f * DeltaTime, 0.f, 0.f) * m_DirVector);
		else
			m_RootComponent->AddWorldPos(Vector3(180.f * DeltaTime, 180.f * DeltaTime, 0.f) * m_DirVector);

		if (!m_OnJump && !m_BodySprite->GetAnimationInstance()->CheckCurrentAnimation("WalkLeft"))
			m_BodySprite->ChangeAnimation("WalkLeft");

		m_Dir = PlayerDir::Right;
	}
}

void CPlayer2D::Jump(float DeltaTime)
{
	if (m_VoidPressure && m_VoidPressure->IsEnable())
		return;

	if (m_OnJump || m_Gravity || m_Dead)
		return;

	if (m_OnLope)
	{
		m_LopeJump = true;
		m_OnJump = true;
		m_OnLope = false;
		m_Gravity = true;
		m_GravityAccTime = 0.f;

		m_BodySprite->GetAnimationInstance()->ChangeAnimation("JumpLeft");
		m_Scene->GetResource()->SoundPlay("Jump");

		return;
	}

	// ������ �����鼭 ����Ű������ ��� �ִ� Ÿ���� ���������ϹǷ� m_Dir�� Down�϶��� ���� �������� �ʵ����ϱ� ���� �ɷ�����
	// �� ������ �޴޷� �������� �Ʒ��� �����鼭 �����ص� �����ϴ� �ִϸ��̼��� ���۵ǰ� �Ѵ�
	if (m_Dir != PlayerDir::Down || m_OnLope)
	{
		m_OnJump = true;
		m_GravityAccTime = 0.f;
		m_BodySprite->GetAnimationInstance()->ChangeAnimation("JumpLeft");
	}
	
	else if (!m_ListCollisionID.empty())
	{
		m_ListCollisionID.clear();
		m_Gravity = true;
		m_TileCollisionEnable = false;
		m_OnLope = false;
		m_OnJump = false;
		m_Dir = PlayerDir::None;
		m_BodySprite->GetAnimationInstance()->ChangeAnimation("JumpLeft");
		m_Scene->GetResource()->SoundPlay("Jump");


		return;
	}

	if (m_Dir == PlayerDir::Down)
	{
		CColliderComponent* TileCollider = m_Body->FindPrevCollisionComponentByObjectType(typeid(CTileObject).hash_code());

		std::vector<CColliderBox2D*> vecCollider;
		m_Body->FindMultipleCollisionComponentByObjType<CColliderBox2D, CTileObject>(vecCollider);

		size_t Count = vecCollider.size();

		for (size_t i = 0; i < Count; ++i)
		{
			CTileObject* FloorTile = (CTileObject*)vecCollider[i]->GetGameObject();

			// ���ϴ� �ٴ� Ÿ���̸� ������ �����ϸ� �ȵȴ�
			if (FloorTile->IsBottomMostFloor())
				return;

			if (vecCollider[i]->GetWorldRot().z != 0.f)
				return;
		}

		//if (TileCollider)
		//{
		//	CTileObject* FloorTile = (CTileObject*)TileCollider->GetGameObject();

		//	// ���ϴ� �ٴ� Ÿ���̸� ������ �����ϸ� �ȵȴ�
		//	if (FloorTile->IsBottomMostFloor())
		//		return;

		//	if(FloorTile)
		//}

		m_Gravity = true;
		m_OnJump = false;
		m_OnLope = false;
		m_BodySprite->GetAnimationInstance()->ChangeAnimation("JumpLeft");
	}
	
	else
	{
		m_OnLope = false;
		m_Gravity = true;
		m_OnJump = true;
	}

	m_Scene->GetResource()->SoundPlay("Jump");
}

void CPlayer2D::SylphideLancer(float DeltaTime)
{
	if (m_OnJump || m_IsChanging || m_Dead)
		return;

	SkillInfo* SkillInfo = m_PlayerSkillInfo->FindSkillInfo("SylphideLancer");

	if (SkillInfo->Active == true && SkillInfo->AccTime < SkillInfo->CoolTime)
		return;

	if (SkillInfo->MPRequire > m_PlayerInfo.MP)
		return;

	// Scene�� m_ObjList���� ���� ã�Ƽ� ���⼭ ���ǵ巣�� ���� �������ֱ�

	m_BodySprite->ChangeAnimation("HealLeft");
	m_PlayerInfo.MP -= SkillInfo->MPRequire;

	CCharacterStatusWindow* Window = CClientManager::GetInst()->GetCharacterStatusWindow();
	Window->SetCurrentMP(m_PlayerInfo.MP);
	Window->SetMPPercent((float)m_PlayerInfo.MP / m_PlayerInfo.MPMax);

	CGameObject* NearMonster = nullptr;

	CSceneMode* Mode = m_Scene->GetSceneMode();

	// ��ó�� ��ų Ÿ���� �� ���� Ž��
	if (Mode)
	{
		// true�� �������� �ٶ󺸰� �����ϱ� �����ʿ��� Ÿ�� ���͸� ã�ƾ��Ѵ�
		bool Flip = m_BodySprite->IsFlip();

		if (m_Scene->GetSceneMode()->GetTypeID() == typeid(COnionScene).hash_code())
			NearMonster = ((COnionScene*)Mode)->FindOnionMonster(Flip, GetWorldPos(), 400.f, 120.f);
		else if (m_Scene->GetSceneMode()->GetTypeID() == typeid(CLibrary2ndScene).hash_code())
			NearMonster = ((CLibrary2ndScene*)Mode)->FindLowerClassBook(Flip, GetWorldPos(), 400.f, 120.f);
		else if (m_Scene->GetSceneMode()->GetTypeID() == typeid(CRadishScene).hash_code())
			NearMonster = ((CRadishScene*)Mode)->FindRadishMonster(Flip, GetWorldPos(), 400.f, 120.f);
	}

	m_Scene->GetResource()->SoundPlay("SylphideLancerUse");

	m_SylphideLancerMirror->ChangeAnimation("SylphideLancerMuzzle");

	CClientManager::GetInst()->GetSkillQuickSlotWindow()->SetSylphideLancerProgressBarPercent(1.f);

	Vector3 MirrorPos = m_SylphideLancerMirror->GetWorldPos();

	for (int i = 0; i < 2; ++i)
	{
		CSylphideLancer* Lancer = m_Scene->CreateGameObject<CSylphideLancer>("SylphideLancer");

		Lancer->SetAllSceneComponentsLayer("MovingObjFront");
		Lancer->SetLancerID(i);
		Lancer->SetWorldPos(MirrorPos.x, MirrorPos.y - 10.f + i * 30.f, GetWorldPos().z - 20.f);
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
	if (m_OnJump || m_IsChanging || m_Dead)
		return;

	SkillInfo* SkillInfo = m_PlayerSkillInfo->FindSkillInfo("VoidPressure");

	if (SkillInfo->Active == true && SkillInfo->AccTime < SkillInfo->CoolTime)
		return;

	if (SkillInfo->MPRequire > m_PlayerInfo.MP)
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

		m_VoidPressure->SetWorldPos(VoidPosX, WorldPos.y, PlayerWorldPos.z - 20.f);
		m_VoidPressure->SetOriginPos(VoidPosX, WorldPos.y, PlayerWorldPos.z - 20.f);

		SkillInfo->Active = true;

		m_PlayerInfo.MP -= SkillInfo->MPRequire;
		CCharacterStatusWindow* Window = CClientManager::GetInst()->GetCharacterStatusWindow();
		Window->SetCurrentMP(m_PlayerInfo.MP);
		Window->SetMPPercent((float)m_PlayerInfo.MP / m_PlayerInfo.MPMax);
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

		m_VoidPressure->SetWorldPos(VoidPosX, WorldPos.y, PlayerWorldPos.z - 20.f);
		m_VoidPressure->SetOriginPos(VoidPosX, WorldPos.y, PlayerWorldPos.z - 20.f);
		m_VoidPressure->SetOnDestroy(false);

		SkillInfo->Active = true;

		m_PlayerInfo.MP -= SkillInfo->MPRequire;
		CCharacterStatusWindow* Window = CClientManager::GetInst()->GetCharacterStatusWindow();
		Window->SetCurrentMP(m_PlayerInfo.MP);
		Window->SetMPPercent((float)m_PlayerInfo.MP / m_PlayerInfo.MPMax);
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

		m_VoidPressureOrb->SetWorldPos(VoidPosX, WorldPos.y - 39.f, PlayerWorldPos.z - 20.f);
		m_VoidPressureOrb->SetOriginPos(VoidPosX, WorldPos.y - 39.f, PlayerWorldPos.z - 20.f);
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
	if (m_OnJump || m_IsChanging || m_Dead)
		return;

	if (m_Dir == PlayerDir::None)
		return;

	if (!m_CurrentStage)
		return;

	SkillInfo* SkillInfo = m_PlayerSkillInfo->FindSkillInfo("LightTransforming");

	if (SkillInfo->Active == true && SkillInfo->AccTime < SkillInfo->CoolTime)
		return;

	if (SkillInfo->MPRequire > m_PlayerInfo.MP)
		return;

	m_OnLightTransforming = true;
	m_BodySprite->SetOpacity(0.5f);
	m_PlayerInfo.MP -= SkillInfo->MPRequire;

	CCharacterStatusWindow* Window = CClientManager::GetInst()->GetCharacterStatusWindow();
	Window->SetCurrentMP(m_PlayerInfo.MP);
	Window->SetMPPercent((float)m_PlayerInfo.MP / m_PlayerInfo.MPMax);

	//m_Body->Enable(false);

	m_Scene->GetResource()->SoundPlay("LightTransforming");

	m_SkillBodyEffect->ChangeAnimation("LightTransformingLeft");

	if (m_Dir == PlayerDir::Up)
	{
		std::vector<CStaticMapObj*>		vecMapObj;
		std::vector<CColliderBox2D*>	vecCollider;
		std::vector<CColliderBox2D*>	vecAllCollider;

		m_Scene->FindObjectByType<CStaticMapObj>(vecMapObj);

		size_t Count = vecMapObj.size();
		bool End = false;

		for (size_t i = 0; i < Count; ++i)
		{
			if (vecMapObj[i]->IsFloor())
				continue;

			vecMapObj[i]->FindComponentFromType<CColliderBox2D>(vecCollider);

			size_t Count2 = vecCollider.size();

			for (size_t j = 0; j < Count2; ++j)
			{
				vecAllCollider.push_back(vecCollider[j]);
			}

			vecCollider.clear();
		}

		std::sort(vecAllCollider.begin(), vecAllCollider.end(), SortCollider);

		Count = vecAllCollider.size();

		for (size_t j = 0; j < Count; ++j)
		{
			Box2DInfo DestInfo = vecAllCollider[j]->GetInfo();
			Box2DInfo PlayerInfo = m_Body->GetInfo();

			if (DestInfo.Center.x - DestInfo.Length.x < PlayerInfo.Center.x && DestInfo.Center.x + DestInfo.Length.x > PlayerInfo.Center.x
				&& DestInfo.Center.y - DestInfo.Length.y >= PlayerInfo.Center.y - PlayerInfo.Length.y
				&& DestInfo.Center.y + DestInfo.Length.y <= PlayerInfo.Center.y - PlayerInfo.Length.y + 200.f)
			{

				float Diff = DestInfo.Center.y + DestInfo.Length.y - (PlayerInfo.Center.y - PlayerInfo.Length.y + m_Body->GetOffset().y);

				m_RootComponent->AddWorldPos(0.f, Diff + 18.f, 0.f);
				End = true;
				break;
			}
		}

		if (!End)
		{
			std::vector<CTileObject*>	vecTileObj;
			vecCollider.clear();
			vecAllCollider.clear();
			m_Scene->FindObjectByType<CTileObject>(vecTileObj);

			Count = vecTileObj.size();

			for (size_t i = 0; i < Count; ++i)
			{
				if (vecTileObj[i]->IsBottomMostFloor())
					continue;

				vecTileObj[i]->FindComponentFromType<CColliderBox2D>(vecCollider);

				size_t Count2 = vecCollider.size();

				for (size_t j = 0; j < Count2; ++j)
				{
					vecAllCollider.push_back(vecCollider[j]);
				}

				vecCollider.clear();
			}

			std::sort(vecAllCollider.begin(), vecAllCollider.end(), SortCollider);

			Count = vecAllCollider.size();

			for (size_t j = 0; j < Count; ++j)
			{
				Box2DInfo DestInfo = vecAllCollider[j]->GetInfo();
				Box2DInfo PlayerInfo = m_Body->GetInfo();

				if (DestInfo.Center.x - DestInfo.Length.x < PlayerInfo.Center.x && DestInfo.Center.x + DestInfo.Length.x > PlayerInfo.Center.x
					&& DestInfo.Center.y - DestInfo.Length.y >= PlayerInfo.Center.y - PlayerInfo.Length.y
					&& DestInfo.Center.y + DestInfo.Length.y <= PlayerInfo.Center.y - PlayerInfo.Length.y + 200.f)
				{

					float Diff = DestInfo.Center.y + DestInfo.Length.y - (PlayerInfo.Center.y - PlayerInfo.Length.y + m_Body->GetOffset().y);

					m_RootComponent->AddWorldPos(0.f, Diff + 18.f, 0.f);
					break;
				}
			}
		}
	}

	else if (m_Dir == PlayerDir::Right)
		m_RootComponent->AddWorldPos(200.f, 0.f, 0.f);

	else if (m_Dir == PlayerDir::Left)
		m_RootComponent->AddWorldPos(-200.f, 0.f, 0.f);

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

void CPlayer2D::DeathSide(float DeltaTime)
{
	if (m_OnJump || m_IsChanging || m_Dead)
		return;

	if (CRenderManager::GetInst()->GetStartFadeOut())
		return;

	SkillInfo* SkillInfo = m_PlayerSkillInfo->FindSkillInfo("DeathSide");

	if (SkillInfo->Active == true && SkillInfo->AccTime < SkillInfo->CoolTime)
		return;

	if (SkillInfo->MPRequire > m_PlayerInfo.MP)
		return;

	m_PlayerInfo.MP -= SkillInfo->MPRequire;

	CCharacterStatusWindow* Window = CClientManager::GetInst()->GetCharacterStatusWindow();
	Window->SetCurrentMP(m_PlayerInfo.MP);
	Window->SetMPPercent((float)m_PlayerInfo.MP / m_PlayerInfo.MPMax);

	// Scene�� m_ObjList���� ���� ã�Ƽ� ���⼭ ���ǵ巣�� ���� �������ֱ�

	m_BodySprite->ChangeAnimation("HealLeft");

	m_Scene->GetResource()->SoundPlay("DeathSideUse");
	m_Scene->GetResource()->SoundPlay("DeathSideVoice2");

	Vector3 WorldPos = GetRootComponent()->GetWorldPos();

	CDeathSide* DeathSide = m_Scene->CreateGameObject<CDeathSide>("DeathSide");
	DeathSide->SetWorldPos(WorldPos.x, WorldPos.y, WorldPos.z - 10.f);
	DeathSide->SetAllSceneComponentsLayer("MovingObjFront");
	DeathSide->SetCollisionProfile("PlayerAttack");

	m_BodySprite->SetFadeApply(false);
	m_PlayerOrb->SetFadeApply(false);

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
		m_VoidPressure->GetBody()->Enable(false);
		m_VoidPressure->Enable(false);
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
	CGameObject* DestObj = Result.Dest->GetGameObject();

	CSceneMode* Mode = m_Scene->GetSceneMode();

	if (DestObj->GetTypeID() == typeid(CTileObject).hash_code())
	{
		CTileObject* TileObj = ((CTileObject*)DestObj);

		std::vector<CColliderBox2D*> vecCollider;

		m_Body->FindMultipleCollisionComponentByObjType<CColliderBox2D, CTileObject>(vecCollider);

		CColliderBox2D* TileObjCollider = nullptr;
		// ���� ���� �ϳ��� �浹ü�� �浹���̾���, ���� ������Ʈ ���� �� �ٸ� �浹ü�� ���� �� �浹�ؼ� �ΰ��� �浹ü��
		// �浹���϶�, ���� �浹�ϰ� �ִ� �浹ü
		CColliderBox2D* EarlyCollider = nullptr;

		size_t Count = vecCollider.size();
		int SameTileObjColliderCount = 0;

		for (size_t i = 0; i < Count; ++i)
		{
			if ((CTileObject*)vecCollider[i]->GetGameObject() == TileObj)
				++SameTileObjColliderCount;

			if (vecCollider[i] != Result.Dest)
				EarlyCollider = vecCollider[i];
		}

		if (Count == 0)
			return;

		TileObjCollider = (CColliderBox2D*)Result.Dest;
		Vector3 ColliderRot = TileObjCollider->GetWorldRot();

		// ����ó�� ����
		if (TileObjCollider)
		{
			Box2DInfo TileInfo = TileObjCollider->GetInfo();
			Box2DInfo MyInfo = m_Body->GetInfo();

			// ������ Ÿ�� �ö󰡴ٰ� �Ӹ��� Ÿ�� ���ϴ��̶� ������ �����ؾ��ϰ�
			// ������ Ÿ�� �������ٰ� ���� Ÿ�� �ֻ���̶� ������ Ÿ�Ͽ� �����ؾ��Ѵ�
			if (m_OnLope && MyInfo.Center.y < TileInfo.Center.y)
			{
				return;
			}

			// �����ϴٰ� �÷��̾� �Ӹ��� Ÿ�� �غκ��̶� �ε����� ����
			if ((m_OnJump || m_LopeJump) && GetCurrentFrameMove().y > 0.f)
				return;

			if ((m_OnJump || m_LopeJump) && MyInfo.Center.y - MyInfo.Length.y <= TileInfo.Center.y && TileObjCollider->GetWorldRot().z == 0.f)
				return;

			// �����̳� ���������� �̵��ϴٰ� Ÿ�ϰ� �浹�� ������ ���������µ� �׶� Ÿ�� ���κ��̶� �浹�ϸ� ����
			// ��, ������Ʈ ���� �ϳ��� �浹ü�� �ִ� ��쿡�� ����ó���� ���ش�
			if (Count == 1 && MyInfo.Center.y - MyInfo.Length.y < TileInfo.Center.y - TileInfo.Length.y)
				return;
		}


		if (SameTileObjColliderCount == 2 && EarlyCollider)
		{
			if (ColliderRot.z == 0.f)
			{
				m_DirVector = Vector3(1.f, 0.f, 0.f);
			}

			else
			{
				ColliderRot = ColliderRot - EarlyCollider->GetWorldRot();

				Matrix matRot;

				matRot.Rotation(ColliderRot);

				m_DirVector = m_DirVector.TransformCoord(matRot);
			}
			
			m_DirRotation = ColliderRot;
			m_PrevSameTileObjColliderCount = 2;
		}

		else
		{
			if (m_PrevSameTileObjColliderCount == 0)
			{
				m_DirRotation = ColliderRot;

				if (ColliderRot.z == 0.f)
				{
					m_DirVector = Vector3(1.f, 0.f, 0.f);
				}

				else
				{
					m_DirVector = Vector3(1.f, 0.f, 0.f);

					Matrix matRot;

					matRot.Rotation(m_DirRotation);

					m_DirVector = m_DirVector.TransformCoord(matRot);
				}
			}

			else
			{
				m_DirRotation = ColliderRot - m_DirRotation;

				if (ColliderRot.z == 0.f)
				{
					m_DirVector = Vector3(1.f, 0.f, 0.f);
				}

				else
				{
					Matrix matRot;

					matRot.Rotation(m_DirRotation);

					m_DirVector = m_DirVector.TransformCoord(matRot);
				}

				m_DirRotation = ColliderRot;
			}

			m_PrevSameTileObjColliderCount = 1;
		}


		//TileObjCollider = TileObj->FindComponentFromType<CColliderBox2D>();

		m_Dir = PlayerDir::None;

		m_LopeJump = false;
		m_OnJump = false;
		m_Gravity = false;
		m_OnLope = false;
		m_LopeEnable = true;
		m_GravityAccTime = 0.f;
		m_OnKnockBack = false;
		m_OnKnockBackAccTime = 0.f;

		m_BodySprite->ChangeAnimation("IdleLeft");
	}

	if (CRenderManager::GetInst()->GetStartFadeIn())
		return;

	if (DestObj->GetTypeID() == typeid(COnionMonster).hash_code() ||
		DestObj->GetTypeID() == typeid(CLowerClassBook).hash_code() ||
		DestObj->GetTypeID() == typeid(CRadishMonster).hash_code())
	{
		if (m_OnHit || m_OnLope || m_Dead)
			return;

		Vector3 DestPos = DestObj->GetWorldPos();
		Vector3 MyPos = GetWorldPos();

		// ������ ���ʿ��� ���Ͱ� �浹 -> ���������� ƨ�ܳ������Ѵ�
		if (DestPos.x < MyPos.x)
			m_OnKnockBackLeft = false;
		else
			m_OnKnockBackLeft = true;

		m_OnKnockBack = true;
		m_OnHit = true;

		m_GravityAccTime = 0.f;
		m_Gravity = true;

		if (m_OnKnockBackLeft && atan(m_DirVector.y / m_DirVector.x) < 0.f)
			m_Gravity = false;
		if (!m_OnKnockBackLeft && atan(m_DirVector.y / m_DirVector.x) > 0.f)
			m_Gravity = false;
	}
}


void CPlayer2D::CollisionEndCallback(const CollisionResult& Result)
{
	if (!Result.Dest)
		return;

	CGameObject* Dest = Result.Dest->GetGameObject();

	if (Dest->GetTypeID() == typeid(CTileObject).hash_code())
	{
		--m_PrevSameTileObjColliderCount;
	}

	if (Dest->GetTypeID() == typeid(CLopeTileObject).hash_code())
	{
		Box2DInfo MyInfo = m_Body->GetInfo();
		Box2DInfo LopeInfo = ((CColliderBox2D*)(Result.Dest))->GetInfo();

		// ���� ������ �� �ö󰡰ų� �� ������ ���
		if (MyInfo.Center.y - MyInfo.Length.y >= LopeInfo.Center.y + LopeInfo.Length.y ||
			MyInfo.Center.y + MyInfo.Length.y <= LopeInfo.Center.y - LopeInfo.Length.y )
		{
			m_OnLope = false;
			m_Gravity = true;
		}
	}

}

void CPlayer2D::CameraTrack()
{
	//CComponent* Component = GetRootComponent()->FindComponent("Camera");

	if (m_Camera)
	{
		CSceneMode* SceneMode = m_Scene->GetSceneMode();

		if (SceneMode->GetTypeID() == typeid(CLobbyScene).hash_code())
		{
			// Stage�� Pivot�� (0.f, 0.f)
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

		else if (SceneMode->GetTypeID() == typeid(CRadishScene).hash_code())
		{
			m_CurrentStage = ((CRadishScene*)SceneMode)->GetStageObject();
		}

		else if (SceneMode->GetTypeID() == typeid(CZakumAltarScene).hash_code())
		{
			m_CurrentStage = ((CZakumAltarScene*)SceneMode)->GetStageObject();
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

				m_CurrentFrameCameraMove.x = 0.f;
			}

			if (PlayerWorldPos.x - RS.Width * Ratio.x <= 0.f)
			{
				Vector3 CamWorldPos = m_Camera->GetWorldPos();
				m_Camera->SetWorldPos(0.f, CamWorldPos.y, CamWorldPos.z);

				m_CurrentFrameCameraMove.x = 0.f;
			}

			if (PlayerWorldPos.y + RS.Height * (1 - Ratio.y) >= WorldSize.y)
			{
				Vector3 CamWorldPos = m_Camera->GetWorldPos();
				float Pos = WorldSize.y - RS.Height * Ratio.y;
				Pos -= 1.f * RS.Height * Ratio.y;
				m_Camera->SetWorldPos(CamWorldPos.x, Pos, CamWorldPos.z);

				m_CurrentFrameCameraMove.y = 0.f;
			}

			if (PlayerWorldPos.y - RS.Height * Ratio.y <= 0.f)
			{
				Vector3 CamWorldPos = m_Camera->GetWorldPos();
				m_Camera->SetWorldPos(CamWorldPos.x, 0.f, CamWorldPos.z);

				m_CurrentFrameCameraMove.y = 0.f;
			}
		}

	}
}

void CPlayer2D::KnockBack(float DeltaTime)
{
	if (m_Dead)
		return;

	if (m_OnKnockBack)
	{
		if (m_OnJump)
			m_OnJump = false;


		// �������� ƨ�ܳ���
		if (m_OnKnockBackLeft)
		{
			if (atan(m_DirVector.y / m_DirVector.x) >= 0.f)
			{
				Vector3 KnockBackDir = Vector3(-200.f * DeltaTime, 100.f * DeltaTime, 0.f);

				AddWorldPos(KnockBackDir);
			}

			// ���� ���⸦ ���� �밢�� Ÿ���� ��� ������
			else
			{
				//Vector3 KnockBackDir = Vector3(0.f, 100.f * DeltaTime, 0.f) * m_DirVector;

				//AddWorldPos(KnockBackDir);
			}
		}

		// ���������� ƨ�ܳ���
		else
		{
			if (atan(m_DirVector.y / m_DirVector.x) <= 0.f)
			{
				Vector3 KnockBackDir = Vector3(200.f * DeltaTime, 100.f * DeltaTime, 0.f);

				AddWorldPos(KnockBackDir);
			}

			// ���� ���⸦ �밢�� Ÿ���� ��� ������
			else
			{
				//Vector3 KnockBackDir = Vector3(0.f, 100.f * DeltaTime, 0.f) * m_DirVector;

				//AddWorldPos(KnockBackDir);

				
			}
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

	CClientManager::GetInst()->GetCharacterStatusWindow()->SetCurrentHP(m_PlayerInfo.HP);
	CClientManager::GetInst()->GetCharacterStatusWindow()->SetHPPercent(HPPercent);

	PushDamageFont(Damage);

	if (m_PlayerInfo.HP < 0.f)
	{
		m_PlayerInfo.HP = 0;
		CClientManager::GetInst()->GetCharacterStatusWindow()->SetCurrentHP(0);
		CClientManager::GetInst()->GetCharacterStatusWindow()->SetHPPercent(0);
		Die();
	}
}

void CPlayer2D::PushDamageFont(float Damage)
{
	if (!m_DamageWidgetComponent->GetWidgetWindow()->GetViewport()->GetScene())
		m_DamageWidgetComponent->GetWidgetWindow()->GetViewport()->SetScene(m_Scene);

	((CPlayerDamageFont*)m_DamageWidgetComponent->GetWidgetWindow())->PushDamageNumber((int)Damage);
}

void CPlayer2D::PickItem(float DeltaTime)
{
	if (m_Body->CheckPrevCollisionGameObjectType(typeid(CBill).hash_code()))
	{
		CColliderComponent* BillCollider = m_Body->FindPrevCollisionComponentByObjectType(typeid(CBill).hash_code());

		if (BillCollider)
		{

			CBill* Bill = (CBill*)BillCollider->GetGameObject();

			if (!Bill->GetEatByPlayer())
			{
				m_Scene->GetResource()->SoundPlay("PickUpItem");

				int Money = Bill->GetMoney();

				CInventory* Inven = CClientManager::GetInst()->GetInventoryWindow();

				Inven->AddMoney(Money);

				m_PlayerInfo.Meso += Money;

				Bill->GetByPlayer();
			}
		}
	}
}

void CPlayer2D::FallTomb()
{
	CTomb* Tomb = m_Scene->CreateGameObject<CTomb>("Tomb");

	Vector3 MyPos = GetWorldPos();

	Tomb->SetWorldPos(MyPos.x - 8.f, CEngine::GetInst()->GetResolution().Height - 50.f, MyPos.z + 3.f);
}

void CPlayer2D::GetEXP(int EXP)
{
	m_PlayerInfo.EXP += (int)EXP;

	if (m_PlayerInfo.EXP >= m_PlayerInfo.EXPMax)
	{
		float DeltaTime = CEngine::GetInst()->GetDeltaTime();
		LevelUp(DeltaTime);
	}

	CCharacterEXP* EXPWindow = CClientManager::GetInst()->GetEXPWindow();

	EXPWindow->SetEXP(m_PlayerInfo.EXP);
}

void CPlayer2D::LevelUp(float DeltaTime)
{
	if (m_SkillBodyEffect->GetCurrentAnimation())
	{
		if (m_SkillBodyEffect->GetAnimationInstance()->CheckCurrentAnimation("PlayerLevelUpEffect"))
		{
			CAnimationSequence2DInstance* CurrentAnimInstance = m_SkillBodyEffect->GetAnimationInstance();

			CurrentAnimInstance->SetCurrentAnimationFrame(0);
			CurrentAnimInstance->SetCurrentAnimationTime(0.f);
			CurrentAnimInstance->SetCurrentAnimationNotifyInitialize();

			((CPlayerSkillBodyEffect*)m_SkillBodyEffect->GetAnimationInstance())->LevelUpEndEffect();
		}
	}

	CCharacterStatusWindow* StatusWindow = CClientManager::GetInst()->GetCharacterStatusWindow();
	CCharacterEXP* EXPWindow = CClientManager::GetInst()->GetEXPWindow();
	CStatWindow* StatWindow = CClientManager::GetInst()->GetStatWindow();

	m_PlayerInfo.HPMax = (int)(m_PlayerInfo.HPMax * 1.1f);
	m_PlayerInfo.MPMax = (int)(m_PlayerInfo.MPMax * 1.1f);
	m_PlayerInfo.HP = m_PlayerInfo.HPMax;
	m_PlayerInfo.MP = m_PlayerInfo.MPMax;
	StatusWindow->SetCurrentHP(m_PlayerInfo.HPMax);
	StatusWindow->SetCurrentMP(m_PlayerInfo.MPMax);
	StatusWindow->SetHPMax(m_PlayerInfo.HPMax);
	StatusWindow->SetMPMax(m_PlayerInfo.MPMax);
	StatusWindow->SetHPPercent(1.f);
	StatusWindow->SetMPPercent(1.f);

	m_PlayerInfo.EXP = 0;
	m_PlayerInfo.EXPMax = (int)(m_PlayerInfo.EXPMax * 1.2f);
	m_PlayerInfo.Level += 1;

	StatusWindow->SetLevel(m_PlayerInfo.Level);
	EXPWindow->SetEXPMax(m_PlayerInfo.EXPMax);
	EXPWindow->SetEXP(0);
	m_SkillBodyEffect->GetAnimationInstance()->ChangeAnimation("PlayerLevelUpEffect");
	m_Scene->GetResource()->SoundPlay("LevelUp");

	// Statâ�� �������� �� �ִ� AbilityPoint 5 �÷��ֱ�
	CStatWindow* Window = CClientManager::GetInst()->GetStatWindow();

	if (Window)
	{
		Window->AddAbilityPoint(5);

		Window->SetHPMax(m_PlayerInfo.HPMax);
		Window->SetMPMax(m_PlayerInfo.MPMax);
		Window->SetHP(m_PlayerInfo.HP);
		Window->SetMP(m_PlayerInfo.MP);
	}

	m_IsChanging = true;
}

void CPlayer2D::UpSTR(int Count)
{
	m_PlayerInfo.STR += Count;
}

void CPlayer2D::UpDEX(int Count)
{
	m_PlayerInfo.DEX += Count;
}

void CPlayer2D::UpINT(int Count)
{
	m_PlayerInfo.INT += Count;
}

void CPlayer2D::UpLUK(int Count)
{
	m_PlayerInfo.LUK += Count;
}

void CPlayer2D::Die()
{
	m_Scene->GetResource()->SoundPlay("Tombstone");
	m_BodySprite->ChangeAnimation("PlayerDead");

	m_Dead = true;
	m_PlayerOrb->GetAnimationInstance()->SetCurrentAnimation(nullptr);
	m_Body->Enable(false);
	m_Gravity = false;
	m_BodySprite->SetOpacity(1.f);

	FallTomb();

	DeadRound();

	CDyingNoticeWindow* Window = CClientManager::GetInst()->GetDyingNoticeWindow();

	if(Window)
		Window->Enable(true);
}

void CPlayer2D::DeadRound()
{
	//Vector3 CurrentPos = GetWorldPos();

	//m_BodySprite->SetInheritRotZ(true);

	AddWorldPos(0.f, 20.f, 0.f);

	//m_BodySprite->AddRelativePos(30.f, 0.f, 0.f);
}

void CPlayer2D::CameraShake(float DeltaTime)
{
	m_AccCameraShakeTime += DeltaTime;
	m_AccCameraShakeSingleDirTime += DeltaTime;

	if (m_AccCameraShakeTime > m_CameraShakeTime)
	{
		m_Camera->SetRelativePos(m_OriginRelativeCamPos);
		m_OnCameraShake = false;
		m_AccCameraShakeTime = 0.f;
		m_AccCameraShakeSingleDirTime = 0.f;
	}

	else if (m_AccCameraShakeSingleDirTime > m_CameraShakeFrequency)
	{
		m_CameraShakeDir.x *= -1.f;
		m_CameraShakeDir.y *= -1.f;
		m_AccCameraShakeSingleDirTime = 0.f;
	}

	m_Camera->AddWorldPos(m_CameraShakeDir.x * 340.f * DeltaTime, m_CameraShakeDir.y * 340.f * DeltaTime, 0.f);

}

void CPlayer2D::SetCameraShake(bool Shake)
{
	m_OriginRelativeCamPos = m_Camera->GetRelativePos();
	m_OnCameraShake = Shake;
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
	if (CRenderManager::GetInst()->GetStartFadeIn() || CRenderManager::GetInst()->GetStartFadeOut())
		return;

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
				// GotoNextMap�Լ��� CInput::Update���� ȣ���ϴ°Ŷ� ���� �̹� �������� �浹 ���簡 ���� �ȵ��� ���ϱ�
				// ���� ������ �������� �˻��Ѵ�
				bool Collision = ((CColliderBox2D*)Body)->CheckPrevCollision(m_Body);

				// LobbyScene�� ������ Entrance ��Ż�� �浹�߰�, ���� ����Ű�� ������ ������ ����� ���´�
				// ==> ���� ����� ���� ��Ƽ������ Ȱ���ؼ� �ε��� SceneChange�ϱ�
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

				// LobbyScene�� ������ Entrance ��Ż�� �浹�߰�, ���� ����Ű�� ������ ������ ����� ���´�
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

	else if (m_Scene->GetSceneMode()->GetTypeID() == typeid(COnionScene).hash_code())
	{
		CGameObject* LeftPortal = m_Scene->FindObject("Portal1");
		CGameObject* RightPortal = m_Scene->FindObject("Portal2");

		if (LeftPortal)
		{
			CComponent* LeftBody = ((CPortal*)LeftPortal)->FindComponent("Body");
			CComponent* RightBody = ((CPortal*)RightPortal)->FindComponent("Body");

			if (LeftBody && RightBody)
			{
				// GotoNextMap�Լ��� CInput::Update���� ȣ���ϴ°Ŷ� ���� �̹� �������� �浹 ���簡 ���� �ȵ��� ���ϱ�
				// ���� ������ �������� �˻��Ѵ�
				bool LeftCollision = ((CColliderBox2D*)LeftBody)->CheckPrevCollision(m_Body);
				bool RightCollision = ((CColliderBox2D*)RightBody)->CheckPrevCollision(m_Body);

				// LobbyScene�� ������ Entrance ��Ż�� �浹�߰�, ���� ����Ű�� ������ ������ ����� ���´�
				// ==> ���� ����� ���� ��Ƽ������ Ȱ���ؼ� �ε��� SceneChange�ϱ�
				if (LeftCollision)
				{
					m_ListCollisionID.clear();

					COnionScene* Scene = (COnionScene*)(GetScene()->GetSceneMode());
					CRenderManager::GetInst()->SetStartFadeIn(true);
					CSceneManager::GetInst()->SetFadeInEndCallback<COnionScene>(Scene, &COnionScene::CreateLobbyScene);
				}

				else if (RightCollision)
				{
					m_ListCollisionID.clear();

					COnionScene* Scene = (COnionScene*)(GetScene()->GetSceneMode());
					CRenderManager::GetInst()->SetStartFadeIn(true);
					CSceneManager::GetInst()->SetFadeInEndCallback<COnionScene>(Scene, &COnionScene::CreateRadishScene);
				}
			}
		}
	}

	else if (m_Scene->GetSceneMode()->GetTypeID() == typeid(CLibrary2ndScene).hash_code())
	{
		CGameObject* Portal = m_Scene->FindObject("Portal");

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
					m_ListCollisionID.clear();

					CLibrary2ndScene* Scene = (CLibrary2ndScene*)(GetScene()->GetSceneMode());
					CRenderManager::GetInst()->SetStartFadeIn(true);
					CSceneManager::GetInst()->SetFadeInEndCallback<CLibrary2ndScene>(Scene, &CLibrary2ndScene::CreateLobbyScene);
				}
			}
		}
	}

	else if (m_Scene->GetSceneMode()->GetTypeID() == typeid(CRadishScene).hash_code())
	{
		CGameObject* Portal = m_Scene->FindObject("LeftPortal");

		if (Portal)
		{
			CComponent* Body = ((CPortal*)Portal)->FindComponent("Body");

			if (Body)
			{
				bool Collision = ((CColliderBox2D*)Body)->CheckPrevCollision(m_Body);

				if (Collision)
				{
					m_ListCollisionID.clear();

					CRadishScene* Scene = (CRadishScene*)(GetScene()->GetSceneMode());
					CRenderManager::GetInst()->SetStartFadeIn(true);
					CSceneManager::GetInst()->SetFadeInEndCallback<CRadishScene>(Scene, &CRadishScene::CreateOnionScene);
				}
			}
		}
	}
}

void CPlayer2D::GotoZakumAltar()
{
	if (m_Scene->GetSceneMode()->GetTypeID() == typeid(CWayToZakumScene).hash_code())
	{
		m_ListCollisionID.clear();

		CWayToZakumScene* Scene = (CWayToZakumScene*)(GetScene()->GetSceneMode());
		CRenderManager::GetInst()->SetStartFadeIn(true);
		CSceneManager::GetInst()->SetFadeInEndCallback<CWayToZakumScene>(Scene, &CWayToZakumScene::CreateZakumAltarScene);
	}
}

void CPlayer2D::ProduceSecondSylphideLander(float DeltaTime)
{
	m_SkillBodyEffect->ChangeAnimation("SylphideLancerBodyEffectLeft");
	m_BodySprite->ChangeAnimation("IdleLeft");

	//if (m_IsFlip)
	//	m_BodySprite->Flip();

	Vector3 WorldPos = GetWorldPos();

	CGameObject* NearMonster = nullptr;

	CSceneMode* Mode = m_Scene->GetSceneMode();

	// ��ó�� ��ų Ÿ���� �� ���� Ž��
	if (Mode)
	{
		// true�� �������� �ٶ󺸰� �����ϱ� �����ʿ��� Ÿ�� ���͸� ã�ƾ��Ѵ�
		bool Flip = m_BodySprite->IsFlip();

		if (m_Scene->GetSceneMode()->GetTypeID() == typeid(COnionScene).hash_code())
			NearMonster = ((COnionScene*)Mode)->FindOnionMonster(Flip, GetWorldPos(), 400.f, 60.f);
		else if (m_Scene->GetSceneMode()->GetTypeID() == typeid(CLibrary2ndScene).hash_code())
			NearMonster = ((CLibrary2ndScene*)Mode)->FindLowerClassBook(Flip, GetWorldPos(), 400.f, 60.f);
		else if (m_Scene->GetSceneMode()->GetTypeID() == typeid(CRadishScene).hash_code())
			NearMonster = ((CRadishScene*)Mode)->FindRadishMonster(Flip, GetWorldPos(), 400.f, 120.f);
	}



	Vector3 MirrorPos = m_SylphideLancerMirror->GetWorldPos();

	for (int i = 0; i < 2; ++i)
	{
		CSylphideLancer* Lancer = m_Scene->CreateGameObject<CSylphideLancer>("SylphideLancer");

		Lancer->SetAllSceneComponentsLayer("MovingObjFront");
		Lancer->SetLancerID(i + 2);
		Lancer->SetWorldPos(MirrorPos.x, MirrorPos.y - 10.f + i * 30.f, GetWorldPos().z - 20.f);
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
	if (m_OnLope || m_OnJump || m_Dead)
		return;

	m_BodySprite->ChangeAnimation("IdleLeft");
	m_Dir = PlayerDir::None;
}

void CPlayer2D::RopeActionStop(float DeltaTime)
{
	if (m_OnLope)
	{
		CAnimationSequence2DData* CurrentAnim = m_BodySprite->GetCurrentAnimation();
		CAnimationSequence2DInstance* Instance = m_BodySprite->GetAnimationInstance();

		if (CurrentAnim && Instance && Instance->CheckCurrentAnimation("Rope"))
			CurrentAnim->SetLoop(false);
	}

	m_Dir = PlayerDir::None;
}

void CPlayer2D::ReturnFadeApply()
{
	m_BodySprite->SetFadeApply(true);
	m_PlayerOrb->SetFadeApply(true);
}

void CPlayer2D::ReturnAlive()
{
	m_Dead = false;
	m_BodySprite->ChangeAnimation("IdleLeft");
	m_Dir = PlayerDir::None;
	m_Gravity = true;

	m_PlayerOrb->GetAnimationInstance()->ChangeAnimation("PlayerOrb");
	m_Body->Enable(true);
	m_BodySprite->SetOpacity(1.f);

	CCharacterStatusWindow* StatusWindow = CClientManager::GetInst()->GetCharacterStatusWindow();

	if (StatusWindow)
	{
		StatusWindow->SetCurrentHP(m_PlayerInfo.HPMax);
		StatusWindow->SetCurrentMP(m_PlayerInfo.MPMax);
		StatusWindow->SetHPPercent(1.f);
		StatusWindow->SetMPPercent(1.f);
		m_PlayerInfo.HP = m_PlayerInfo.HPMax;
		m_PlayerInfo.MP = m_PlayerInfo.MPMax;
	}

	// Tombã�Ƽ� Destroy���ֱ�
	CTomb* Tomb = (CTomb*)m_Scene->FindObject("Tomb");

	if (Tomb)
		Tomb->Destroy();

	CDyingNoticeWindow* NoticeWindow = CClientManager::GetInst()->GetDyingNoticeWindow();

	if (NoticeWindow)
		NoticeWindow->Enable(false);
}



