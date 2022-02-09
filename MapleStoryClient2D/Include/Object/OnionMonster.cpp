
#include "OnionMonster.h"
#include "Scene/Scene.h"
#include "Resource/Material/Material.h"
#include "OnionMonsterAnimation.h"
#include "Engine.h"
#include "../Widget/DamageFont.h"

COnionMonster::COnionMonster() :
	m_HP(10000.f),
	m_IsChanging(false)
{
	SetTypeID<COnionMonster>();
}

COnionMonster::COnionMonster(const COnionMonster& obj) :
	CGameObject(obj)
{
	m_Sprite = (CSpriteComponent*)FindComponent("OnionMonsterSprite");
	m_Body = (CColliderCircle*)FindComponent("Body");
}

COnionMonster::~COnionMonster()
{
}

bool COnionMonster::Init()
{
	m_Sprite = CreateComponent<CSpriteComponent>("OnionMonsterSprite");
	m_Body = CreateComponent<CColliderCircle>("Body");
	//m_PaperBurn = CreateComponent<CPaperBurnComponent>("PaperBurn");


	m_Body->AddCollisionCallback<COnionMonster>(Collision_State::Begin, this, &COnionMonster::CollisionCallback);


	SetRootComponent(m_Sprite);

	m_DamageWidgetComponent = CreateComponent<CWidgetComponent>("DamageFont");
	m_DamageWidgetComponent->UseAlphaBlend(true);

	Vector3 WorldPos = m_Sprite->GetWorldPos();
	
	CDamageFont* DamageFont = m_DamageWidgetComponent->CreateWidgetWindow<CDamageFont>("DamageFontWidget");

	DamageFont->SetPos(WorldPos.x - 20.f, WorldPos.y);

	m_Sprite->AddChild(m_Body);
	m_Sprite->AddChild(m_DamageWidgetComponent);

	m_Sprite->SetTransparency(true);
	//m_Sprite->SetOpacity(0.5f);

	//m_PaperBurn->SetMaterial(m_Sprite->GetMaterial());
	//m_PaperBurn->SetFinishTime(8.f);
	//m_PaperBurn->SetFinishCallback<COnionMonster>(this, &COnionMonster::Die);

	m_Sprite->CreateAnimationInstance<COnionMonsterAnimation>();
	COnionMonsterAnimation* Instance = (COnionMonsterAnimation*)m_Sprite->GetAnimationInstance();

	Instance->SetEndFunction<COnionMonster>("OnionHitLeft", this, &COnionMonster::ReturnIdle);
	Instance->SetEndFunction<COnionMonster>("OnionDieLeft", this, &COnionMonster::Die);


	m_Sprite->SetRelativeScale(100.f, 100.f, 1.f);
	m_Sprite->SetRelativePos(500.f, 300.f, 0.f);
	m_Sprite->SetPivot(0.5f, 0.5f, 0.f);


	return true;
}

void COnionMonster::Update(float DeltaTime)
{
	CGameObject::Update(DeltaTime);
}

void COnionMonster::PostUpdate(float DeltaTime)
{
	CGameObject::PostUpdate(DeltaTime);
}

COnionMonster* COnionMonster::Clone()
{
	return new COnionMonster(*this);
}

void COnionMonster::CollisionCallback(const CollisionResult& result)
{
	
}

void COnionMonster::SetDamage(float Damage, bool Critical)
{
	m_HP -= Damage;

	if (!m_IsChanging)
	{
		if (m_HP <= 0.f)
		{
			//m_PaperBurn->StartPaperBurn();
			m_Body->Enable(false);


			GetRootComponent()->DeleteChild("Body");
			m_Sprite->GetAnimationInstance()->ChangeAnimation("OnionDieLeft");
		}

		else
		{
			m_Sprite->ChangeAnimation("OnionHitLeft");
		}

		m_IsChanging = true;
	}

	PushDamageFont(Damage, Critical);
}

void COnionMonster::PushDamageFont(float Damage, bool Critical)
{
	((CDamageFont*)m_DamageWidgetComponent->GetWidgetWindow())->PushDamageNumber((int)Damage, Critical);
}

void COnionMonster::Die()
{
	Destroy();
}

void COnionMonster::ReturnIdle()
{
	m_IsChanging = false;
	m_Sprite->ChangeAnimation("OnionIdleLeft");
}
