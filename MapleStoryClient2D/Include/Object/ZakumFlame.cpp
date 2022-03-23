
#include "ZakumFlame.h"
#include "Player2D.h"
#include "../Animation/ZakumFlameAnimation.h"
#include "../Scene/ZakumAltarScene.h"
#include "Scene/SceneManager.h"

CZakumFlame::CZakumFlame() :
	m_CollisionFrequency(2.5f),
	m_AccTime(0.f),
	m_CollisionStart(false)
{
	SetTypeID<CZakumFlame>();
}

CZakumFlame::CZakumFlame(const CZakumFlame& obj)	:
	CMonster(obj)
{
}

CZakumFlame::~CZakumFlame()
{
}

void CZakumFlame::Start()
{
	CMonster::Start();
}

bool CZakumFlame::Init()
{
	m_Sprite = CreateComponent<CSpriteComponent>("ZakumFlameSprite");
	m_Body = CreateComponent<CColliderBox2D>("Body");
	m_Body->SetCollisionProfile("Monster");

	m_Sprite->AddChild(m_Body);


	m_Sprite->SetTransparency(true);

	m_Sprite->CreateAnimationInstance<CZakumFlameAnimation>();


	m_Sprite->SetRelativeScale(100.f, 100.f, 1.f);
	m_Sprite->SetRelativePos(500.f, 300.f, 0.f);
	m_Sprite->SetPivot(0.5f, 0.5f, 0.f);

	m_Body->SetRelativePos(0.f, -10.f, 0.f);
	m_Body->SetWorldScale(270.f, 30.f, 1.f);
	m_Body->AddCollisionCallback<CZakumFlame>(Collision_State::Begin, this, &CZakumFlame::CollisionBeginCallback);

	return true;
}

void CZakumFlame::Update(float DeltaTime)
{
	
	CMonster::Update(DeltaTime);

	if (m_CollisionStart)
	{
		m_AccTime += DeltaTime;
		
		if (m_AccTime >= m_CollisionFrequency)
		{
			m_AccTime = 0.f;
			m_CollisionStart = false;

			CPlayer2D* Player = (CPlayer2D*)m_Scene->GetPlayerObject();
			CColliderBox2D* PlayerBody = Player->GetPlayerBody();

			m_Body->DeletePrevCollision(PlayerBody);
			PlayerBody->DeletePrevCollision(m_Body);
		}
	}
}

void CZakumFlame::PostUpdate(float DeltaTime)
{
	CMonster::PostUpdate(DeltaTime);
}

CZakumFlame* CZakumFlame::Clone()
{
	return new CZakumFlame(*this);
}

void CZakumFlame::CollisionBeginCallback(const CollisionResult& Result)
{
	CGameObject* DestObj = Result.Dest->GetGameObject();

	if (DestObj->GetTypeID() == typeid(CPlayer2D).hash_code())
	{
		CPlayer2D* Player = (CPlayer2D*)DestObj;

		// 플레이어가 죽은 상태거나 Scene전환중이면 충돌처리 X
		if (Player->IsDead() || CSceneManager::GetInst()->GetNextScene())
			return;

		if (m_Scene->GetSceneMode()->GetTypeID() != typeid(CZakumAltarScene).hash_code())
			return;

		Player->SetDamage(300.f);
		m_CollisionStart = true;
	}
}
