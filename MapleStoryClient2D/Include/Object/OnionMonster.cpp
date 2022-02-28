
#include "OnionMonster.h"
#include "../Animation/OnionMonsterAnimation.h"
#include "Scene/Scene.h"
#include "Resource/Material/Material.h"
#include "Engine.h"
#include "../Widget/DamageFont.h"
#include "Player2D.h"
#include "TileObject.h"
#include "Component/TileMapComponent.h"

COnionMonster::COnionMonster() :
	m_IsChanging(false)
{
	SetTypeID<COnionMonster>();
	m_Gravity = false;
	m_TileCollisionEnable = true;
	m_MonsterInfo.HP = 5000;
	m_MonsterInfo.HPMax = 5000;
	m_MonsterInfo.Level = 50;
	m_MonsterInfo.Attack = 10;

	m_MonsterState = Monster_State::Idle;

	m_FiniteStateTimeTable[(int)Monster_State::Idle] = (float)(rand() % 5) + 1.f;
	m_FiniteStateTimeTable[(int)Monster_State::Move] = (float)(rand() % 4) + 1.f;
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

void COnionMonster::Start()
{
	CGameObject::Start();

	std::string Name = m_Name;

	m_DamageWidgetComponent = CreateComponent<CWidgetComponent>("DamageFont");
	m_DamageWidgetComponent->UseAlphaBlend(true);

	CDamageFont* DamageFont = m_DamageWidgetComponent->CreateWidgetWindow<CDamageFont>("DamageFontWidget");

	Vector3 WorldPos = m_Sprite->GetWorldPos();
	// 무조건 Child 추가하고 Transform 건들여야된다!
	m_Sprite->AddChild(m_DamageWidgetComponent);

	m_DamageWidgetComponent->SetRelativePos(-20.f, 0.f, 0.f);

	//m_DamageWidgetComponent->SetWorldPos(WorldPos.x - 20.f, WorldPos.y, 0.f);



	//DamageFont->SetPos(WorldPos.x - 20.f, WorldPos.y);

	COnionMonsterAnimation* Instance = (COnionMonsterAnimation*)m_Sprite->GetAnimationInstance();

	Instance->SetEndFunction<COnionMonster>("OnionHitLeft", this, &COnionMonster::ReturnIdle);
	Instance->SetEndFunction<COnionMonster>("OnionDieLeft", this, &COnionMonster::Die);

	m_Body->AddCollisionCallback<COnionMonster>(Collision_State::Begin, this, &COnionMonster::CollisionBeginCallback);
	//m_Body->AddCollisionCallback<COnionMonster>(Collision_State::Stay, this, &COnionMonster::CollisionStayCallback);
	m_Body->AddCollisionCallback<COnionMonster>(Collision_State::End, this, &COnionMonster::CollisionEndCallback);
}

bool COnionMonster::Init()
{
	m_Sprite = CreateComponent<CSpriteComponent>("OnionMonsterSprite");
	m_Body = CreateComponent<CColliderCircle>("Body");
	m_Body->SetRadius(30.f);
	//m_PaperBurn = CreateComponent<CPaperBurnComponent>("PaperBurn");

	SetRootComponent(m_Sprite);

	m_Sprite->AddChild(m_Body);

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

	FiniteState(DeltaTime);

	CTileMapComponent* TileMapCom = m_Body->CheckCurrentFrameCollisionByType<CTileMapComponent>();

	if (TileMapCom)
	{
		if (EdgeTileCheck(TileMapCom, m_Body->GetWorldPos(), m_Body->GetWorldScale()))
			m_Sprite->Flip();
	}

}

void COnionMonster::PostUpdate(float DeltaTime)
{
	CGameObject::PostUpdate(DeltaTime);
}

COnionMonster* COnionMonster::Clone()
{
	return new COnionMonster(*this);
}

void COnionMonster::SetDamage(float Damage, bool Critical)
{
	m_MonsterInfo.HP -= (int)Damage;

	if (!m_IsChanging)
	{
		if (m_MonsterInfo.HP <= 0.f)
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

	//m_DamageWidgetComponent->Enable(true);
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

void COnionMonster::FiniteState(float DeltaTime)
{
	if (m_IsChanging)
		return;

	if (m_MonsterState == Monster_State::Track)
	{


		return;
	}

	m_AccTime += DeltaTime;

	if (m_AccTime >= m_FiniteStateTimeTable[(int)m_MonsterState])
	{

		m_AccTime = 0.f;
		CClientManager::GetInst()->NextMonsterState(m_MonsterState);

		switch (m_MonsterState)
		{
		case Monster_State::Idle:
			m_Sprite->ChangeAnimation("OnionIdleLeft");
			break;
		case Monster_State::Move:
			m_Sprite->ChangeAnimation("OnionWalkLeft");
			break;
		}
	}

	else
	{
		switch(m_MonsterState)
		{
			case Monster_State::Idle :
				break;
			case Monster_State::Move :
			{
				// 오른쪽을 바라보고 있을때
				if (m_Sprite->IsFlip())
				{
					AddWorldPos(100.f * DeltaTime, 0.f, 0.f);
				}

				else
				{
					AddWorldPos(-100.f * DeltaTime, 0.f, 0.f);
				}
			}
				break;
		}
	}	
}

void COnionMonster::CollisionBeginCallback(const CollisionResult& Result)
{
	CGameObject* Object = Result.Dest->GetGameObject();

	if (Object->GetTypeID() == typeid(CPlayer2D).hash_code())
	{
		((CPlayer2D*)Object)->SetDamage((float)m_MonsterInfo.Attack, false);
	}
}

void COnionMonster::CollisionEndCallback(const CollisionResult& Result)
{
}

void COnionMonster::Save(FILE* File)
{
	CGameObject::Save(File);
}

void COnionMonster::Load(FILE* File)
{
	CGameObject::Load(File);

	m_Sprite = (CSpriteComponent*)FindComponent("OnionMonsterSprite");

	m_Sprite->SetTransparency(true);

	m_Body = (CColliderCircle*)FindComponent("Body");
}
