
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
	// 이렇게 강제로 Camera의 transform중 Z값을 0으로 안잡아주면 카메라가 Player의 Root Component의 카메라가 자신의 Z값으로 물려받아서 
	// 크기는 Z값을 갖고, 부호는 음수인 값이 카메라의 m_matView[4][3]에 저장돼서 그만큼 매번 z방향으로 translation되게 해서 렌더링돼서
	// 결국 그 값의 절댓값보다 작은 z값을 가지는 레이어(ex.지금 이 코드에선 CoveringMapObj 레이어에 속한 SceneComponent들)나
	// SceneComponent는 결국 view변환후에는 Z값이 음수가돼서 출력이 안될것이다
	m_Camera->SetWorldPos(CamWorldPos.x, CamWorldPos.y, 0.f);

	// 카메라가 화면 밖으로 나가는거도 보정을 한 뒤에 최종적으로 만들어지는 transform으로 뷰행렬을 만들어야하므로 
	// 무조건 CGameObject::PostUpdate보다 먼저 해줘야한다
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
				// 로프를 아래에서 위로 타고 올라갈때만 적용 -> 로프 최상단이 플레이어 발밑 y좌표보다 높거나 같아야한다
				Box2DInfo Info = Component->GetInfo();
				Box2DInfo PlayerInfo = m_Body->GetInfo();

				// 이 경우 로프를 끝까지 다 올라갔다
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

			// 로프 최하단에 처음 메달릴때 조건 
			// 1. 타일에 서서 로프 최상단에 플레이어와 로프가 충돌한 상황에서 위방향키 누른다고 메달리는 경우 제외하기 
			// 2. 로프 끝까지 다 올라가서 m_Gravity = true되어서 플레이어가 떨어지는 와중에 위방향키를 계속 누르고 있어도 다시 줄에 메달리지 않게하기
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


		// 로프를 끝까지 다 내려갔는지 체크
		if (m_Body->CheckPrevCollisionGameObjectType(typeid(CLopeTileObject).hash_code()))
		{
			CColliderBox2D* Component = (CColliderBox2D*)m_Body->FindPrevCollisionComponentByObjectType(typeid(CLopeTileObject).hash_code());

			if (Component)
			{
				// 로프를 아래에서 위로 타고 올라갈때만 적용 -> 로프 최상단이 플레이어 발밑 y좌표보다 높거나 같아야한다
				Box2DInfo Info = Component->GetInfo();
				Box2DInfo PlayerInfo = m_Body->GetInfo();

				// 이 경우 로프를 끝까지 다 내려갔다
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

	// 타일 밟고 있으면서 로프 최상단에서 처음 메달려서 내려갈때
	if (m_Body->CheckPrevCollisionGameObjectType(typeid(CLopeTileObject).hash_code()))
	{
		CColliderBox2D* Component = (CColliderBox2D*)m_Body->FindPrevCollisionComponentByObjectType(typeid(CLopeTileObject).hash_code());

		if (Component)
		{
			Box2DInfo MyInfo = m_Body->GetInfo();
			Box2DInfo ComponentInfo = Component->GetInfo();

			bool TileCollision = m_Body->CheckPrevCollisionGameObjectType(typeid(CTileObject).hash_code());

			//  타일 밟고 있으면서 로프 최상단에서 처음 메달려서 내려갈때
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

	// 보이드 프레셔 구체가 아직 생성된적 없거나 완전히 소멸되고나서 움직일 수 있게한다
	if (!m_VoidPressure || !m_VoidPressure->IsEnable())
	{
		bool IsFlip = m_BodySprite->IsFlip();

		// 실피드랜서 동작중이라면 왼쪽으로 방향전환 불가능하게 하기
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

	// 보이드 프레셔 구체가 아직 생성된적 없거나 완전히 소멸되고나서 움직일 수 있게한다
	if (!m_VoidPressure || !m_VoidPressure->IsEnable())
	{
		bool IsFlip = m_BodySprite->IsFlip();

		// 실피드랜서 동작중이라면 오른쪽으로 방향전환 불가능하게 하기
		if (m_SylphideLancerMirror->GetCurrentAnimation())
			return;

 		if (!m_BodySprite->IsFlip())
			FlipAll(DeltaTime);

		//m_BodySprite->AddWorldPos(m_BodySprite->GetWorldAxis(AXIS_X) * 180.f * DeltaTime);

		std::vector<CColliderBox2D*>	vecCollider;

		m_Body->FindMultipleCollisionComponentByObjType<CColliderBox2D, CTileObject>(vecCollider);

		size_t Count = vecCollider.size();

		//평지랑 오르막길 만나는 지점에서 가끔씩 오르막 안올라가고 직진해버릴때가 있음 
		// -> m_DirVector는 (1, 0, 0)나오는데 m_DirRotation은 (0, 0, -34) 나옴
		// 그 이유는 왼쪽 이동하면서 내리막 내려감(CollisionBegin에서 if (SameTileObjColliderCount == 2 && EarlyCollider)이 아니고 else에 들어가서
		// m_DirRotation은 34되고, m_DirVector는 34도만큼 TransformCoor됨), 그러고 충돌체 곧바로 뒤에 충돌체 2개 동시에 부딪힌
		// if (SameTileObjColliderCount == 2 && EarlyCollider)에 들어가는데 ColliderRot은 평지 충돌체, EarlyCollider는 34도 충돌체이다
		// 그래서 ColliderRot은 0 - 34해서 - 34가 되고, m_DirRotation은 0이 되고 m_DirVector는(1, 0, 0)이 되어서
		// 이 상태에서 오른쪽 방향키로 오른쪽 이동하면 오르막 안올라가고 그걸 뚫고 직진해버리므로 그 경우를 예외처리한다
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

	// 밑으로 누르면서 점프키누르면 밟고 있는 타일을 내려가야하므로 m_Dir이 Down일때는 위로 점프하지 않도록하기 위해 걸러낸다
	// 단 로프에 메달려 있을때는 아래로 누르면서 점프해도 점프하는 애니메이션이 동작되게 한다
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

			// 최하단 바닥 타일이면 밑으로 낙하하면 안된다
			if (FloorTile->IsBottomMostFloor())
				return;

			if (vecCollider[i]->GetWorldRot().z != 0.f)
				return;
		}

		//if (TileCollider)
		//{
		//	CTileObject* FloorTile = (CTileObject*)TileCollider->GetGameObject();

		//	// 최하단 바닥 타일이면 밑으로 낙하하면 안된다
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

	// Scene의 m_ObjList에서 몬스터 찾아서 여기서 실피드랜서 방향 설정해주기

	m_BodySprite->ChangeAnimation("HealLeft");
	m_PlayerInfo.MP -= SkillInfo->MPRequire;

	CCharacterStatusWindow* Window = CClientManager::GetInst()->GetCharacterStatusWindow();
	Window->SetCurrentMP(m_PlayerInfo.MP);
	Window->SetMPPercent((float)m_PlayerInfo.MP / m_PlayerInfo.MPMax);

	CGameObject* NearMonster = nullptr;

	CSceneMode* Mode = m_Scene->GetSceneMode();

	// 근처의 스킬 타겟이 될 몬스터 탐색
	if (Mode)
	{
		// true면 오른쪽을 바라보고 있으니까 오른쪽에서 타겟 몬스터를 찾아야한다
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

	// Scene의 m_ObjList에서 몬스터 찾아서 여기서 실피드랜서 방향 설정해주기

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
		// 만약 원래 하나의 충돌체와 충돌중이었고, 같은 오브젝트 내에 또 다른 충돌체와 이제 막 충돌해서 두개의 충돌체와
		// 충돌중일때, 원래 충돌하고 있던 충돌체
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

		// 예외처리 구간
		if (TileObjCollider)
		{
			Box2DInfo TileInfo = TileObjCollider->GetInfo();
			Box2DInfo MyInfo = m_Body->GetInfo();

			// 로프를 타고 올라가다가 머리가 타일 최하단이랑 닿으면 무시해야하고
			// 로프를 타고 내려오다가 발이 타일 최상단이랑 닿으면 타일에 착지해야한다
			if (m_OnLope && MyInfo.Center.y < TileInfo.Center.y)
			{
				return;
			}

			// 점프하다가 플레이어 머리랑 타일 밑부분이랑 부딪히면 무시
			if ((m_OnJump || m_LopeJump) && GetCurrentFrameMove().y > 0.f)
				return;

			if ((m_OnJump || m_LopeJump) && MyInfo.Center.y - MyInfo.Length.y <= TileInfo.Center.y && TileObjCollider->GetWorldRot().z == 0.f)
				return;

			// 왼쪽이나 오른쪽으로 이동하다가 타일과 충돌이 끝나서 떨어지려는데 그때 타일 옆부분이랑 충돌하면 무시
			// 단, 오브젝트 내에 하나의 충돌체만 있는 경우에만 예외처리로 빼준다
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

		// 나보다 왼쪽에서 몬스터가 충돌 -> 오른쪽으로 튕겨나가야한다
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

		// 로프 끝까지 다 올라가거나 다 내려온 경우
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


		// 왼쪽으로 튕겨나감
		if (m_OnKnockBackLeft)
		{
			if (atan(m_DirVector.y / m_DirVector.x) >= 0.f)
			{
				Vector3 KnockBackDir = Vector3(-200.f * DeltaTime, 100.f * DeltaTime, 0.f);

				AddWorldPos(KnockBackDir);
			}

			// 음의 기울기를 가진 대각선 타일을 밟고 있을때
			else
			{
				//Vector3 KnockBackDir = Vector3(0.f, 100.f * DeltaTime, 0.f) * m_DirVector;

				//AddWorldPos(KnockBackDir);
			}
		}

		// 오른쪽으로 튕겨나감
		else
		{
			if (atan(m_DirVector.y / m_DirVector.x) <= 0.f)
			{
				Vector3 KnockBackDir = Vector3(200.f * DeltaTime, 100.f * DeltaTime, 0.f);

				AddWorldPos(KnockBackDir);
			}

			// 양의 기울기를 대각선 타일을 밟고 있을때
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

	// Stat창의 스탯찍을 수 있는 AbilityPoint 5 늘려주기
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
				// GotoNextMap함수는 CInput::Update에서 호출하는거라 아직 이번 프레임의 충돌 조사가 아직 안됐을 때니까
				// 이전 프레임 기준으로 검사한다
				bool LeftCollision = ((CColliderBox2D*)LeftBody)->CheckPrevCollision(m_Body);
				bool RightCollision = ((CColliderBox2D*)RightBody)->CheckPrevCollision(m_Body);

				// LobbyScene의 오른쪽 Entrance 포탈에 충돌했고, 위쪽 방향키를 누르고 있을때 여기로 들어온다
				// ==> 양파 재배지 맵을 멀티쓰레드 활용해서 로딩후 SceneChange하기
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

	// 근처의 스킬 타겟이 될 몬스터 탐색
	if (Mode)
	{
		// true면 오른쪽을 바라보고 있으니까 오른쪽에서 타겟 몬스터를 찾아야한다
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

	// Tomb찾아서 Destroy해주기
	CTomb* Tomb = (CTomb*)m_Scene->FindObject("Tomb");

	if (Tomb)
		Tomb->Destroy();

	CDyingNoticeWindow* NoticeWindow = CClientManager::GetInst()->GetDyingNoticeWindow();

	if (NoticeWindow)
		NoticeWindow->Enable(false);
}



