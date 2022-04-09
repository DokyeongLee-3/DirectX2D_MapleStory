
#include "Punko.h"
#include "../Animation/PunkoAnimation.h"
#include "../Widget/DamageFont.h"
#include "Player2D.h"
#include "Render/RenderManager.h"

CPunko::CPunko()
{
	SetTypeID<CPunko>();

	MonsterInfo Info = CClientManager::GetInst()->FindMonsterInfo("Punko");

	m_MonsterInfo.HP = Info.HP;
	m_MonsterInfo.HPMax = Info.HPMax;
	m_MonsterInfo.Level = Info.Level;
	m_MonsterInfo.Attack = Info.Attack;
}

CPunko::CPunko(const CPunko& obj)	:
	CMonster(obj)
{
	m_Sprite = (CSpriteComponent*)FindComponent("PunkoMonsterSprite");
	m_Body = (CColliderBox2D*)FindComponent("Body");
}

CPunko::~CPunko()
{
}

void CPunko::SetTarget(CPlayer2D* Target)
{
	m_Target = Target;
}

void CPunko::SetDamage(float Damage, bool Critical)
{
	m_MonsterInfo.HP -= (int)Damage;

	if (m_MonsterInfo.HP <= 0.f)
	{
		PushDamageFont(Damage, Critical);

		if (!m_Body->IsEnable())
			return;

		m_Body->Enable(false);

		GetRootComponent()->DeleteChild("Body");
		m_Sprite->GetAnimationInstance()->ChangeAnimation("PunkoDie");
		m_IsChanging = true;
	}

	else
	{
		if (!m_IsChanging)
			m_IsChanging = true;

		PushDamageFont(Damage, Critical);
	}
}

bool CPunko::Init()
{
	m_Sprite = CreateComponent<CSpriteComponent>("PunkoMonsterSprite");
	m_Body = CreateComponent<CColliderBox2D>("Body");

	m_Sprite->AddChild(m_Body);
	m_Body->SetWorldScale(40.f, 65.f, 1.f);

	m_Sprite->SetTransparency(true);

	m_Sprite->CreateAnimationInstance<CPunkoAnimation>();


	m_Sprite->SetRelativeScale(100.f, 100.f, 1.f);
	m_Sprite->SetRelativePos(500.f, 300.f, 0.f);
	m_Sprite->SetPivot(0.5f, 0.5f, 0.f);


	m_Body->SetRelativePos(0.f, -5.f, 0.f);
	m_Body->SetWorldScale(90.f, 140.f, 1.f);
	m_Body->SetCollisionProfile("Monster");

	// Summon Animation할때는 맞지 않게하기
	m_Body->Enable(false);

	m_Sprite->SetLayerName("MovingObjFront");

	m_RotDirVector = Vector3(1.f, 0.f, 0.f);

	m_Body->AddCollisionCallback<CPunko>(Collision_State::Begin, this, &CPunko::CollisionBeginCallback);

	return true;
}

void CPunko::Start()
{
	CMonster::Start();

	m_DamageWidgetComponent = CreateComponent<CWidgetComponent>("DamageFont");
	m_DamageWidgetComponent->UseAlphaBlend(true);

	CDamageFont* DamageFont = m_DamageWidgetComponent->CreateWidgetWindow<CDamageFont>("DamageFontWidget");

	// 무조건 Child 추가하고 난 뒤에 Transform 건들여야된다!
	m_Sprite->AddChild(m_DamageWidgetComponent);

	m_DamageWidgetComponent->SetRelativePos(-20.f, 0.f, 0.f);
	m_DamageWidgetComponent->SetLayerName("ScreenWidgetComponent");
}

void CPunko::Update(float DeltaTime)
{
	CMonster::Update(DeltaTime);

	// Body가 Enable이란 의미는 소환 애니메이션이 끝났고 Fly 애니메이션 시작하면 Body를 Enable로 해주므로 이때부턴 원 궤도로 이동하기 시작해도된다
	if (m_Target && m_Active && m_Body->IsEnable())
	{
		GetRootComponent()->AddWorldPos(m_RotDirVector.x / 10.f, m_RotDirVector.y / 10.f, 0.f);

		Vector3 Rot = Vector3(0.f, 0.f, DeltaTime * 100.f);

		Matrix	matRot;
		matRot.Rotation(Rot);

		m_RotDirVector = m_RotDirVector.TransformCoord(matRot);

		float TargetPosX = m_Target->GetWorldPos().x;
		float MyPosX = GetWorldPos().x;

		// 플레이어가 오른쪽에 있을 경우
		if (TargetPosX - MyPosX > 50.f)
		{
			if (!m_Sprite->IsFlip())
				m_Sprite->Flip();

			AddWorldPos(35.f * DeltaTime, 0.f, 0.f);
		}

		// 플레이어가 왼쪽에 있을 경우
		else if (TargetPosX - MyPosX < -50.f)
		{
			if (m_Sprite->IsFlip())
				m_Sprite->Flip();

			AddWorldPos(-35.f * DeltaTime, 0.f, 0.f);
		}
	}
}

void CPunko::PostUpdate(float DeltaTime)
{
	CMonster::PostUpdate(DeltaTime);
}

CPunko* CPunko::Clone()
{
	return new CPunko(*this);
}

void CPunko::Save(FILE* File)
{
	CMonster::Save(File);
}

void CPunko::Load(FILE* File)
{
	CMonster::Load(File);
}

void CPunko::CollisionBeginCallback(const CollisionResult& Result)
{
	CGameObject* Object = Result.Dest->GetGameObject();

	if (Object->GetTypeID() == typeid(CPlayer2D).hash_code())
	{
		if (CRenderManager::GetInst()->GetStartFadeIn())
			return;

		((CPlayer2D*)Object)->SetDamage((float)m_MonsterInfo.Attack, false);
		((CPlayer2D*)Object)->SetOnHit(true);
	}
}

void CPunko::CollisionEndCallback(const CollisionResult& Result)
{
}
