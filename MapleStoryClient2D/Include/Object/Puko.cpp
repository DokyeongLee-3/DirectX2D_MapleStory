
#include "Puko.h"
#include "../Animation/PukoAnimation.h"
#include "../Widget/DamageFont.h"
#include "Player2D.h"
#include "Render/RenderManager.h"

CPuko::CPuko()
{
	SetTypeID<CPuko>();

	MonsterInfo Info = CClientManager::GetInst()->FindMonsterInfo("Puko");

	m_MonsterInfo.HP = Info.HP;
	m_MonsterInfo.HPMax = Info.HPMax;
	m_MonsterInfo.Level = Info.Level;
	m_MonsterInfo.Attack = Info.Attack;
}

CPuko::CPuko(const CPuko& obj)	:
	CMonster(obj)
{
	m_Sprite = (CSpriteComponent*)FindComponent("PukoMonsterSprite");
	m_Body = (CColliderBox2D*)FindComponent("Body");
}

CPuko::~CPuko()
{
}

void CPuko::SetTarget(CPlayer2D* Target)
{
	m_Target = Target;
}

bool CPuko::Init()
{
	m_Sprite = CreateComponent<CSpriteComponent>("PukoMonsterSprite");
	m_Body = CreateComponent<CColliderBox2D>("Body");

	m_Sprite->AddChild(m_Body);
	m_Body->SetWorldScale(40.f, 65.f, 1.f);

	m_Sprite->SetTransparency(true);

	m_Sprite->CreateAnimationInstance<CPukoAnimation>();


	m_Sprite->SetRelativeScale(100.f, 100.f, 1.f);
	m_Sprite->SetRelativePos(500.f, 300.f, 0.f);
	m_Sprite->SetPivot(0.5f, 0.5f, 0.f);


	m_Body->SetRelativePos(0.f, -5.f, 0.f);
	m_Body->SetWorldScale(90.f, 140.f, 1.f);
	m_Body->SetCollisionProfile("Monster");

	m_Sprite->SetLayerName("MovingObjFront");

	// Summon Animation할때는 맞지 않게하기
	m_Body->Enable(false);

	m_RotDirVector = Vector3(1.f, 0.f, 0.f);

	m_Body->AddCollisionCallback<CPuko>(Collision_State::Begin, this, &CPuko::CollisionBeginCallback);

	return true;
}

void CPuko::Start()
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

void CPuko::Update(float DeltaTime)
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
		if (TargetPosX - MyPosX > 85.f)
		{
			if (!m_Sprite->IsFlip())
				m_Sprite->Flip();

			AddWorldPos(20.f * DeltaTime, 0.f, 0.f);
		}

		// 플레이어가 왼쪽에 있을 경우
		else if (TargetPosX - MyPosX < -85.f)
		{
			if (m_Sprite->IsFlip())
				m_Sprite->Flip();

			AddWorldPos(-20.f * DeltaTime, 0.f, 0.f);
		}
	}
}

void CPuko::PostUpdate(float DeltaTime)
{
	CMonster::PostUpdate(DeltaTime);
}

CPuko* CPuko::Clone()
{
	return new CPuko(*this);
}

void CPuko::Save(FILE* File)
{
	CMonster::Save(File);
}

void CPuko::Load(FILE* File)
{
	CMonster::Load(File);
}

void CPuko::CollisionBeginCallback(const CollisionResult& Result)
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

void CPuko::CollisionEndCallback(const CollisionResult& Result)
{
}

void CPuko::SetDamage(float Damage, bool Critical)
{
	m_MonsterInfo.HP -= (int)Damage;

	if (m_MonsterInfo.HP <= 0.f)
	{
		PushDamageFont(Damage, Critical);

		if (!m_Body->IsEnable())
			return;

		m_Body->Enable(false);

		GetRootComponent()->DeleteChild("Body");
		m_Sprite->GetAnimationInstance()->ChangeAnimation("PukoDie");
		m_IsChanging = true;
	}

	else
	{
		if (!m_IsChanging)
			m_IsChanging = true;

		PushDamageFont(Damage, Critical);
	}
}
