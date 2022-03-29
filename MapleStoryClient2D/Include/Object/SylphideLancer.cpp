
#include "SylphideLancer.h"
#include "../ClientManager.h"
#include "Engine.h"
#include "Player2D.h"
#include "Component/SpriteComponent.h"
#include "Animation/AnimationSequence2DInstance.h"
#include "Scene/Scene.h"
#include "Scene/SceneMode.h"
#include "../Scene/ZakumAltarScene.h"
#include "../Object/SylphideLancerHitEffect.h"
#include "Monster.h"
#include "ZakumBody.h"
#include "PlayerSkillInfo.h"
#include <time.h>

CSylphideLancer::CSylphideLancer() :
	m_Distance(0.f),
	m_Speed(350.f),
	m_Body(nullptr),
	m_LancerID(-1),
	m_ProduceLatterGroup(false)
{
	SetTypeID<CSylphideLancer>();
}

CSylphideLancer::CSylphideLancer(const CSylphideLancer& obj) :
	CGameObject(obj)
{
	m_Sprite = (CSpriteComponent*)FindComponent("SylphideLancerSprite");
	m_Body = (CColliderBox2D*)FindComponent("Body");
	m_Distance = obj.m_Distance;
}

CSylphideLancer::~CSylphideLancer()
{
}

bool CSylphideLancer::Init()
{
	m_Sprite = CreateComponent<CSpriteComponent>("SylphideLancerSprite");

	SetRootComponent(m_Sprite);

	m_Sprite->SetRelativeScale(50.f, 50.f, 1.f);
	m_Sprite->SetPivot(0.5f, 0.5f, 0.f);
	m_Sprite->SetTransparency(true);

	m_Body = CreateComponent<CColliderBox2D>("Body");
	m_Body->SetCollisionProfile("PlayerAttack");
	m_Body->SetExtent(40.f, 15.f);
	m_Body->AddCollisionCallback<CSylphideLancer>(Collision_State::Begin, this,
		&CSylphideLancer::CollisionBeginCallback);

	m_Sprite->AddChild(m_Body);

	m_Sprite->CreateAnimationInstance<CAnimationSequence2DInstance>();

	CAnimationSequence2DInstance* Anim = m_Sprite->GetAnimationInstance();

	Anim->AddAnimation(TEXT("SylphideLancerArrowLeft.sqc"), ANIMATION_PATH, "SylphideLancerArrowLeft", true, 0.3f);
	Anim->AddAnimation(TEXT("SylphideLancerArrowPurple.sqc"), ANIMATION_PATH, "SylphideLancerArrowPurple", true, 0.3f);

	return true;
}

void CSylphideLancer::Update(float DeltaTime)
{
	CGameObject::Update(DeltaTime);

	Vector3 Axis = GetWorldAxis(AXIS::AXIS_X);

	AddRelativePos(Axis * m_Speed * DeltaTime);

	float AbsoluteSpeed = abs(m_Speed);
	m_Distance += AbsoluteSpeed * DeltaTime;


	// 첫번째 그룹에 2개의 랜서(ID가 각각 0, 1)중 1번 ID인 랜서가 150이상의 거리를 이동했을때
	// 두번째 그룹에 2개의 랜서(ID가 각각 2, 3)을 추가적으로 플레이어가 생성하도록 해서 한번 스킬쓰면
	// 총 4개의 창이 나가도록 한다
	if (m_Distance >= 150.f && m_LancerID == 1 && !m_ProduceLatterGroup)
	{
		CPlayer2D* Player = (CPlayer2D*)(m_Scene->GetSceneMode()->GetPlayerObject());
		Player->ProduceSecondSylphideLander(DeltaTime);
		m_ProduceLatterGroup = true;
	}

	if (m_Distance >= 500.f)
	{
		m_Distance = 0.f;
		Destroy();
	}
}

void CSylphideLancer::PostUpdate(float DeltaTime)
{
	CGameObject::PostUpdate(DeltaTime);
}

CSylphideLancer* CSylphideLancer::Clone()
{
	return new CSylphideLancer(*this);
}

void CSylphideLancer::SetCollisionProfile(const std::string& Name)
{
	m_Body->SetCollisionProfile(Name);
}

void CSylphideLancer::FlipAll(float DeltaTime)
{
	m_Sprite->Flip();
}

void CSylphideLancer::CollisionBeginCallback(const CollisionResult& result)
{
	CGameObject* Dest = result.Dest->GetGameObject();

	if (Dest)
	{
		CPlayer2D* Player = ((CPlayer2D*)m_Scene->GetSceneMode()->GetPlayerObject());
		PlayerInfo Info = Player->GetInfo();

		CPlayerSkillInfo* SkillInfo = Player->GetPlayerSkillInfo();
		int SkillLv = SkillInfo->FindSkillInfo("SylphideLancer")->Level;

		int Factor = (int)Info.INT * Info.Level;

		int RamdomNumber = rand();

		int random = (RamdomNumber % (Factor / 20 + 1)) - (Factor / 30);

		float Damage = Factor / 10.f + random + SkillLv * 15;

		// 크리티컬 데미지가 뜬 경우 -> 추가적으로 이펙트 달아주기
		bool IsCritical = random > 0;

		if (m_Scene->GetSceneMode()->GetTypeID() == typeid(CZakumAltarScene).hash_code()
			&& Dest->GetTypeID() == typeid(CZakumBody).hash_code())
		{
			if (result.Dest->GetName().find("LeftArm1HandDefense") != std::string::npos)
			{
				((CZakumBody*)Dest)->SetCurrentCollisionID(0);
			}

			else if (result.Dest->GetName().find("LeftArm2HandDefense") != std::string::npos)
			{
				((CZakumBody*)Dest)->SetCurrentCollisionID(1);
			}

			else if (result.Dest->GetName().find("LeftArm3HandDefense") != std::string::npos)
			{
				((CZakumBody*)Dest)->SetCurrentCollisionID(2);
			}

			else if (result.Dest->GetName().find("LeftArm4HandDefense") != std::string::npos)
			{
				((CZakumBody*)Dest)->SetCurrentCollisionID(3);
			}

			else if (result.Dest->GetName().find("RightArm1HandDefense") != std::string::npos)
			{
				((CZakumBody*)Dest)->SetCurrentCollisionID(4);
			}

			else if (result.Dest->GetName().find("RightArm2HandDefense") != std::string::npos)
			{
				((CZakumBody*)Dest)->SetCurrentCollisionID(5);
			}

			else if (result.Dest->GetName().find("RightArm3HandDefense") != std::string::npos)
			{
				((CZakumBody*)Dest)->SetCurrentCollisionID(6);
			}

			else if (result.Dest->GetName().find("RightArm4HandDefense") != std::string::npos)
			{
				((CZakumBody*)Dest)->SetCurrentCollisionID(7);
			}

			else
			{
				((CZakumBody*)Dest)->SetCurrentCollisionID(-1);
			}
		}

		Dest->SetDamage(Damage, IsCritical);
		

		((CMonster*)Dest)->SetTrackState();

		m_Scene->GetResource()->SoundPlay("SylphideLancerHit");

		if (m_LancerID == 0)
		{
			Vector3 HitEffectPos = GetWorldPos();

			// 오른쪽으로 날아가다가 충돌
			if (m_Sprite->IsFlip())
				HitEffectPos.x += 30.f;

			// 왼쪽으로 날아가다가 충돌
			else
				HitEffectPos.x -= 30.f;

			CSylphideLancerHitEffect* HitEffect = m_Scene->CloneFromPrototype<CSylphideLancerHitEffect>(
				"SylphideLancerHitEffect", "SylphideLancerHitEffect",
				HitEffectPos);

			// 실피드랜서 첫번째 그룹이 150보다 적게 날아가서 충돌해서 소멸해도 두번째 그룹을 이어서 발사하도록한다
			if (m_Distance < 150.f)
			{
				CPlayer2D* Player = (CPlayer2D*)(m_Scene->GetSceneMode()->GetPlayerObject());
				Player->ProduceSecondSylphideLander(CEngine::GetInst()->GetDeltaTime());
				m_ProduceLatterGroup = true;
			}
		}

		// 두번째 그룹이 충돌
		else if (m_LancerID == 2)
		{
			Vector3 HitEffectPos = GetWorldPos();

			// 오른쪽으로 날아가다가 충돌
			if (m_Sprite->IsFlip())
				HitEffectPos.x += 30.f;

			// 왼쪽으로 날아가다가 충돌
			else
				HitEffectPos.x -= 30.f;

			CSylphideLancerHitEffect* HitEffect = m_Scene->CloneFromPrototype<CSylphideLancerHitEffect>(
				"SylphideLancerHitEffect", "SylphideLancerHitEffect",
				HitEffectPos);

			CSpriteComponent* Root = (CSpriteComponent*)HitEffect->GetRootComponent();

			Root->ChangeAnimation("SylphideLancerHitPurple");
		}

		Destroy();
	}
}

void CSylphideLancer::CollisionEndCallback(const CollisionResult& result)
{
}
