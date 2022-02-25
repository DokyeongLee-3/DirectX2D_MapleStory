
#include "Monster.h"
#include "Scene/Scene.h"
#include "Resource/Material/Material.h"
#include "../Animation/MonsterAnimation.h"
#include "../Widget/SimpleHUD.h"
#include "Engine.h"

CMonster::CMonster()	:
	m_HP(3)
{
	SetTypeID<CMonster>();
}

CMonster::CMonster(const CMonster& obj) :
	CGameObject(obj)
{
	m_Sprite = (CSpriteComponent*)FindComponent("MonsterSprite");
	m_Body = (CColliderCircle*)FindComponent("Body");
	m_SimpleHUDWidget = (CWidgetComponent*)FindComponent("SimpleHUD");
	m_PaperBurn = (CPaperBurnComponent*)FindComponent("PaperBurn");
}

CMonster::~CMonster()
{
}

void CMonster::Start()
{
	CGameObject::Start();

	m_PaperBurn->SetFinishCallback<CMonster>(this, &CMonster::PaperBurnEnd);
}

bool CMonster::Init()
{
	m_Sprite = CreateComponent<CSpriteComponent>("MonsterSprite");
	m_Body = CreateComponent<CColliderCircle>("Body");
	m_PaperBurn = CreateComponent<CPaperBurnComponent>("PaperBurn");

	//m_Body->AddCollisionCallback<CMonster>(Collision_State::Begin, this, &CMonster::CollisionCallback);

	m_SimpleHUDWidget = CreateComponent<CWidgetComponent>("SimpleHUD");

	m_SimpleHUD = m_SimpleHUDWidget->CreateWidgetWindow<CSimpleHUD>("SimpleHUDWidget");


	SetRootComponent(m_Sprite); 

	m_Sprite->AddChild(m_Body);
	m_Sprite->AddChild(m_SimpleHUDWidget);

	m_PaperBurn->SetMaterial(m_Sprite->GetMaterial());
	m_PaperBurn->SetFinishTime(8.f);

	m_SimpleHUDWidget->SetRelativePos(-50.f, 50.f, 0.f);

	m_SimpleHUD->SetText(TEXT("¶ì¸ð"));
	m_SimpleHUD->SetHPPercent(1.f);
	m_SimpleHUD->SetHPDir(ProgressBar_Dir::BottomToTop);

	m_Sprite->SetTransparency(true);
	//m_Sprite->SetOpacity(0.5f);

	m_Sprite->CreateAnimationInstance<CMonsterAnimation>();
	CMonsterAnimation* Instance = (CMonsterAnimation*)m_Sprite->GetAnimationInstance();

	Instance->SetEndFunction<CMonster>("RadishHitLeft", this, &CMonster::ReturnIdle);
	Instance->SetEndFunction<CMonster>("RadishDieLeft", this, &CMonster::Die);
	

	m_Sprite->SetRelativeScale(100.f, 100.f, 1.f);
	m_Sprite->SetRelativePos(500.f, 300.f, 0.f);
	m_Sprite->SetPivot(0.5f, 0.5f, 0.f);


	m_Body->AddCollisionMouseCallback(Collision_State::Begin, this, &CMonster::OnMouseBegin);
	m_Body->AddCollisionMouseCallback(Collision_State::End, this, &CMonster::OnMouseEnd);

	m_Body->AddCollisionCallback(Collision_State::Begin, this, &CMonster::OnCollisionBegin);
	m_Body->AddCollisionCallback(Collision_State::End, this, &CMonster::OnCollisionEnd);



	m_HP = 3;

	return true;
}

void CMonster::Update(float DeltaTime)
{
	CGameObject::Update(DeltaTime);
}

void CMonster::PostUpdate(float DeltaTime)
{
	CGameObject::PostUpdate(DeltaTime);
}

CMonster* CMonster::Clone()
{
	return new CMonster(*this);
}

void CMonster::CollisionCallback(const CollisionResult& result)
{
	m_HP -= 10;
	m_Sprite->ChangeAnimation("RadishHitLeft");

	if (m_HP <= 0.f)
	{
		GetRootComponent()->DeleteChild("Body");
		m_Sprite->GetAnimationInstance()->ChangeAnimation("RadishDieLeft");
		m_Body->Destroy();
	}
}

void CMonster::Die()
{
	Destroy();
}

void CMonster::ReturnIdle()
{
	m_Sprite->ChangeAnimation("RadishIdleLeft");
}

void CMonster::OnMouseBegin(const CollisionResult& result)
{
	CEngine::GetInst()->SetMouseState(Mouse_State::State1);
}

void CMonster::OnMouseEnd(const CollisionResult& result)
{
	CEngine::GetInst()->SetMouseState(Mouse_State::Normal);
}

void CMonster::OnCollisionBegin(const CollisionResult& result)
{
	--m_HP;

	if (m_HP <= 0)
	{
		m_PaperBurn->StartPaperBurn();
		m_Body->Enable(false);
	}
}

void CMonster::OnCollisionEnd(const CollisionResult & result)
{
}

void CMonster::PaperBurnEnd()
{
	Destroy();
}
