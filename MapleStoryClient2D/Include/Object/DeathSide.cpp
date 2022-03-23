
#include "DeathSide.h"
#include "Component/SpriteComponent.h"
#include "../Animation/DeathSideAnimation.h"
#include "Animation/AnimationSequence2DInstance.h"
#include "DeathSideHitEffect.h"
#include "Player2D.h"
#include "Scene/Scene.h"
#include "Scene/SceneMode.h"
#include "Monster.h"
#include "ZakumBody.h"
#include "../Scene/ZakumAltarScene.h"

CDeathSide::CDeathSide()	:
	m_HitCount(6)
{
	SetTypeID<CDeathSide>();
}

CDeathSide::CDeathSide(const CDeathSide& obj)
{
}

CDeathSide::~CDeathSide()
{
}

bool CDeathSide::Init()
{
	m_Sprite = CreateComponent<CSpriteComponent>("DeathSideSprite");

	m_Sprite->SetRelativeScale(50.f, 50.f, 1.f);
	m_Sprite->SetPivot(0.5f, 0.5f, 0.f);
	m_Sprite->SetTransparency(true);

	m_Sprite->CreateAnimationInstance<CDeathSideAnimation>();

	m_Body = CreateComponent<CColliderBox2D>("Body");
	m_Body->SetCollisionProfile("PlayerAttack");
	m_Body->SetExtent(600.f, 70.f);
	m_Body->AddCollisionCallback<CDeathSide>(Collision_State::Begin, this,
		&CDeathSide::CollisionBeginCallback);
	m_Body->Enable(false);


	CAnimationSequence2DInstance* Instance = m_Sprite->GetAnimationInstance();
	Instance->AddNotify<CDeathSide>("DeathSide", "DeathSide", 7, this, &CDeathSide::EnableCollider);

	SetRootComponent(m_Sprite);

	m_Sprite->AddChild(m_Body);


	return true;
}

void CDeathSide::Update(float DeltaTime)
{
	CGameObject::Update(DeltaTime);
}

void CDeathSide::PostUpdate(float DeltaTime)
{
	CGameObject::PostUpdate(DeltaTime);
}

CDeathSide* CDeathSide::Clone()
{
	return new CDeathSide(*this);
}

void CDeathSide::SetCollisionProfile(const std::string& Name)
{
	m_Body->SetCollisionProfile(Name);
}

void CDeathSide::CollisionBeginCallback(const CollisionResult& result)
{
	for (int i = 0; i < m_HitCount; ++i)
	{
		PlayerInfo Info = ((CPlayer2D*)m_Scene->GetSceneMode()->GetPlayerObject())->GetInfo();

		int Factor = (Info.INT + 2) * Info.Level;

		int RamdomNumber = rand();

		int random = (RamdomNumber % (Factor / 20 + 1)) - (Factor / 30);

		float Damage = Factor / 10.f + random;

		// 크리티컬 데미지가 뜬 경우 -> 추가적으로 이펙트 달아주기
		bool IsCritical = random > 0;

		CGameObject* Dest = result.Dest->GetGameObject();

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
	}

	m_Body->Enable(false);
	m_Scene->GetResource()->SoundPlay("DeathSideHit");

	Vector3 HitEffectPos = result.Dest->GetWorldPos();

	CDeathSideHitEffect* HitEffect = m_Scene->CloneFromPrototype<CDeathSideHitEffect>(
		"DeathSideHitEffect", "DeathSideHitEffect",
		HitEffectPos);
}

void CDeathSide::EnableCollider()
{
	m_Body->Enable(true);
}
