
#include "VoidPressure.h"
#include "VoidPressureOrb.h"
#include "../Animation/VoidPressureAttackSphere.h"
#include "Scene/Scene.h"
#include "Player2D.h"
#include "VoidPressureHitEffect.h"
#include "Monster.h"

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

		// ��ü�� ������ �̵� �������� �� ���� �������ϸ� �� �̻� ������ �ؾ��Ѵ�
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

	PlayerInfo Info = ((CPlayer2D*)m_Scene->GetSceneMode()->GetPlayerObject())->GetInfo();

	int Factor = Info.INT * Info.Level;

	int RamdomNumber = rand();

	int random = (RamdomNumber % (Factor / 20 + 1)) - (Factor / 30);

	float Damage = Factor / 10.f + random;

	// ũ��Ƽ�� �������� �� ��� -> �߰������� ����Ʈ �޾��ֱ�
	bool IsCritical = random > 0;

	CGameObject* Dest = result.Dest->GetGameObject();
	((CMonster*)Dest)->SetTrackState();

	Dest->SetDamage(Damage, IsCritical);

	m_Body->Enable(false);
	m_Scene->GetResource()->SoundPlay("VoidPressureHit");

	Vector3 HitEffectPos = Dest->GetWorldPos();

	CVoidPressureHitEffect* HitEffect = m_Scene->CloneFromPrototype<CVoidPressureHitEffect>(
		"VoidPressureHitEffect", "VoidPressureHitEffect",
		HitEffectPos);
}

void CVoidPressure::CollisionEndCallback(const CollisionResult& result)
{
}
