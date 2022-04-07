
#include "VoidPressure.h"
#include "VoidPressureOrb.h"
#include "../Animation/VoidPressureAttackSphere.h"
#include "Scene/Scene.h"
#include "Player2D.h"
#include "VoidPressureHitEffect.h"
#include "Monster.h"
#include "ZakumBody.h"
#include "../Scene/ZakumAltarScene.h"
#include "PlayerSkillInfo.h"

CVoidPressure::CVoidPressure() :
	m_Distance(400.f),
	m_CollisionFrequency(0.4f),
	m_AccCollisionFrequency(0.f),
	m_VoidPressureLifeSpan(6.f),
	m_OnDestroy(false)
{
}

CVoidPressure::CVoidPressure(const CVoidPressure& obj)	:
	CGameObject(obj)
{
	m_Sprite = (CSpriteComponent*)FindComponent("VoidPressureSprite");
}

CVoidPressure::~CVoidPressure()
{
}

bool CVoidPressure::Init()
{
	m_Sprite = CreateComponent<CSpriteComponent>("VoidPressureSprite");
	m_Sprite->SetPivot(0.5f, 0.5f, 0.f);
	m_Sprite->SetTransparency(true);
	m_Sprite->SetRelativeScale(50.f, 50.f, 1.f);

	m_Sprite->CreateAnimationInstance<CVoidPressureAttackSphere>();

	m_Body = CreateComponent<CColliderCircle>("VoidPressureSpriteBody");

	SetRootComponent(m_Sprite);

	m_Sprite->AddChild(m_Body);

	m_Body->SetCollisionProfile("PlayerAttack");
	m_Body->AddCollisionCallback<CVoidPressure>(Collision_State::Begin, this,
		&CVoidPressure::CollisionBeginCallback);

	m_VoidPressureLifeSpan = 6.f;

	return true;
}

void CVoidPressure::Update(float DeltaTime)
{
	CGameObject::Update(DeltaTime);

	if (m_VoidPressureLifeSpan <= 0.f)
	{
		CPlayer2D* Player = (CPlayer2D*)m_Scene->GetPlayerObject();

		Player->VoidPressureCancle(DeltaTime);

		m_VoidPressureLifeSpan = 6.f;
	}

	if (!m_OnDestroy)
	{
		m_VoidPressureLifeSpan -= DeltaTime;

		Vector3 Pos = GetWorldPos();

		// 구체가 지정된 이동 범위보다 더 많이 가려고하면 더 이상 못가게 해야한다
		if (m_OriginPos.x - Pos.x >= m_Distance)
		{
			SetWorldPos(m_OriginPos.x - m_Distance, Pos.y, Pos.z);
		}

		else if (m_OriginPos.x - Pos.x <= -m_Distance)
		{
			SetWorldPos(m_OriginPos.x + m_Distance, Pos.y, Pos.z);
		}
	}
}

void CVoidPressure::PostUpdate(float DeltaTime)
{
	CGameObject::PostUpdate(DeltaTime);

	if (!m_OnDestroy && m_Enable && !m_Body->IsEnable())
	{
		if (m_AccCollisionFrequency >= m_CollisionFrequency)
		{
			m_AccCollisionFrequency = 0.f;
			m_Body->Enable(true);
			return;
		}

		m_AccCollisionFrequency += DeltaTime;
	}
}

CVoidPressure* CVoidPressure::Clone()
{
	return new CVoidPressure(*this);
}

void CVoidPressure::SetCollisionProfile(const std::string& Name)
{
	m_Body->SetCollisionProfile(Name);
}

void CVoidPressure::FlipAll(float DeltaTime)
{
	m_Sprite->Flip();
}

void CVoidPressure::CollisionBeginCallback(const CollisionResult& result)
{
	m_Scene->GetResource()->SoundPlay("VoidPressureHit");

	CPlayer2D* Player = ((CPlayer2D*)m_Scene->GetSceneMode()->GetPlayerObject());
	PlayerInfo Info = Player->GetInfo();

	CPlayerSkillInfo* SkillInfo = Player->GetPlayerSkillInfo();
	int SkillLv = SkillInfo->FindSkillInfo("VoidPressure")->Level;

	int Factor = Info.INT * Info.Level;

	int RamdomNumber = rand();

	int random = (RamdomNumber % (Factor / 20 + 1)) - (Factor / 30);

	float Damage = Factor / 10.f + random + 10 * SkillLv;

	// 크리티컬 데미지가 뜬 경우 -> 추가적으로 이펙트 달아주기
	bool IsCritical = random > 0;

	CGameObject* Dest = result.Dest->GetGameObject();
	((CMonster*)Dest)->SetTrackState();

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

	m_Body->Enable(false);
	m_Scene->GetResource()->SoundPlay("VoidPressureHit");

	Vector3 HitEffectPos = result.Dest->GetWorldPos();

	CVoidPressureHitEffect* HitEffect = m_Scene->CloneFromPrototype<CVoidPressureHitEffect>(
		"VoidPressureHitEffect", "VoidPressureHitEffect",
		Vector3(HitEffectPos.x, HitEffectPos.y, HitEffectPos.z - 10.f));
}

void CVoidPressure::CollisionEndCallback(const CollisionResult& result)
{
}
