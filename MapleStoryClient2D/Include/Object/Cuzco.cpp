
#include "Cuzco.h"
#include "../Animation/CuzcoAnimation.h"
#include "Player2D.h"
#include "../Widget/DamageFont.h"
#include "Render/RenderManager.h"

CCuzco::CCuzco()
{
	SetTypeID<CCuzco>();

	MonsterInfo Info = CClientManager::GetInst()->FindMonsterInfo("Cuzco");

	m_MonsterInfo.HP = Info.HP;
	m_MonsterInfo.HPMax = Info.HPMax;
	m_MonsterInfo.Level = Info.Level;
	m_MonsterInfo.Attack = Info.Attack;

}

CCuzco::CCuzco(const CCuzco& obj)	:
	CMonster(obj)
{
	m_Sprite = (CSpriteComponent*)FindComponent("CuzcoMonsterSprite");
	m_Body = (CColliderBox2D*)FindComponent("Body");
}

CCuzco::~CCuzco()
{
}

void CCuzco::SetTarget(CPlayer2D* Target)
{
	m_Target = Target;
}

void CCuzco::SetDamage(float Damage, bool Critical)
{
	m_MonsterInfo.HP -= (int)Damage;

	if (m_MonsterInfo.HP <= 0.f)
	{
		PushDamageFont(Damage, Critical);

		if (!m_Body->IsEnable())
			return;

		m_Body->Enable(false);

		GetRootComponent()->DeleteChild("Body");
		m_Sprite->GetAnimationInstance()->ChangeAnimation("CuzcoDie");
		m_IsChanging = true;
	}

	else
	{
		if (!m_IsChanging)
			m_IsChanging = true;

		PushDamageFont(Damage, Critical);
	}
}

bool CCuzco::Init()
{
	m_Sprite = CreateComponent<CSpriteComponent>("CuzcoMonsterSprite");
	m_Body = CreateComponent<CColliderBox2D>("Body");

	m_Sprite->AddChild(m_Body);
	m_Body->SetWorldScale(40.f, 65.f, 1.f);

	m_Sprite->SetTransparency(true);

	m_Sprite->CreateAnimationInstance<CCuzcoAnimation>();


	m_Sprite->SetRelativeScale(100.f, 100.f, 1.f);
	m_Sprite->SetRelativePos(500.f, 300.f, 0.f);
	m_Sprite->SetPivot(0.5f, 0.5f, 0.f);


	m_Body->SetRelativePos(0.f, -5.f, 0.f);
	m_Body->SetWorldScale(90.f, 140.f, 1.f);
	m_Body->SetCollisionProfile("Monster");

	m_Sprite->SetLayerName("MovingObjFront");

	// Summon Animation�Ҷ��� ���� �ʰ��ϱ�
	m_Body->Enable(false);

	m_RotDirVector = Vector3(1.f, 0.f, 0.f);

	m_Body->AddCollisionCallback<CCuzco>(Collision_State::Begin, this, &CCuzco::CollisionBeginCallback);

	return true;
}

void CCuzco::Start()
{
	CMonster::Start();

	m_DamageWidgetComponent = CreateComponent<CWidgetComponent>("DamageFont");
	m_DamageWidgetComponent->UseAlphaBlend(true);

	CDamageFont* DamageFont = m_DamageWidgetComponent->CreateWidgetWindow<CDamageFont>("DamageFontWidget");

	// ������ Child �߰��ϰ� �� �ڿ� Transform �ǵ鿩�ߵȴ�!
	m_Sprite->AddChild(m_DamageWidgetComponent);

	m_DamageWidgetComponent->SetRelativePos(-20.f, 0.f, 0.f);
	m_DamageWidgetComponent->SetLayerName("ScreenWidgetComponent");
}

void CCuzco::Update(float DeltaTime)
{
	CMonster::Update(DeltaTime);

	// Body�� Enable�̶� �ǹ̴� ��ȯ �ִϸ��̼��� ������ Fly �ִϸ��̼� �����ϸ� Body�� Enable�� ���ֹǷ� �̶����� �� �˵��� �̵��ϱ� �����ص��ȴ�
	if (m_Target && m_Active && m_Body->IsEnable())
	{
		GetRootComponent()->AddWorldPos(m_RotDirVector.x / 10.f, m_RotDirVector.y / 10.f, 0.f);

		Vector3 Rot = Vector3(0.f, 0.f, DeltaTime * 100.f);

		Matrix	matRot;
		matRot.Rotation(Rot);

		m_RotDirVector = m_RotDirVector.TransformCoord(matRot);

		float TargetPosX = m_Target->GetWorldPos().x;
		float MyPosX = GetWorldPos().x;

		// �÷��̾ �����ʿ� ���� ���
		if (TargetPosX - MyPosX > 15.f)
		{
			if (!m_Sprite->IsFlip())
				m_Sprite->Flip();

			AddWorldPos(50.f * DeltaTime, 0.f, 0.f);
		}

		// �÷��̾ ���ʿ� ���� ���
		else if (TargetPosX - MyPosX < -15.f)
		{
			if (m_Sprite->IsFlip())
				m_Sprite->Flip();

			AddWorldPos(-50.f * DeltaTime, 0.f, 0.f);
		}
	}
}

void CCuzco::PostUpdate(float DeltaTime)
{
	CMonster::PostUpdate(DeltaTime);
}

CCuzco* CCuzco::Clone()
{
	return new CCuzco(*this);
}

void CCuzco::Save(FILE* File)
{
	CMonster::Save(File);
}

void CCuzco::Load(FILE* File)
{
	CMonster::Load(File);
}

void CCuzco::CollisionBeginCallback(const CollisionResult& Result)
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

void CCuzco::CollisionEndCallback(const CollisionResult& Result)
{
}