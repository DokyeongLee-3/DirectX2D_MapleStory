
#include "LowerClassBook.h"
#include "../Scene/Library2ndScene.h"
#include "../Animation/LowerClassBookAnimation.h"
#include "Input.h"
#include "Component/TileMapComponent.h"
#include "Player2D.h"
#include "../Widget/DamageFont.h"
#include "Render/RenderManager.h"

CLowerClassBook::CLowerClassBook()
{
	SetTypeID<CLowerClassBook>();

	m_TileCollisionEnable = true;
	m_MonsterInfo.HP = 7000;
	m_MonsterInfo.HPMax = 7000;
	m_MonsterInfo.Level = 50;
	m_MonsterInfo.Attack = 10;


	m_FiniteStateTimeTable[(int)Monster_State::Idle] = (float)(rand() % 5) + 1.f;
	m_FiniteStateTimeTable[(int)Monster_State::Move] = (float)(rand() % 4) + 1.f;
}

CLowerClassBook::CLowerClassBook(const CLowerClassBook& obj) :
	CMonster(obj)
{
	m_Sprite = (CSpriteComponent*)FindComponent("LowerClassBookSprite");
	m_Body = (CColliderBox2D*)FindComponent("Body");
}

CLowerClassBook::~CLowerClassBook()
{
}

void CLowerClassBook::Start()
{
	CMonster::Start();

	m_Sprite->SetTransparency(true);


	m_DamageWidgetComponent = CreateComponent<CWidgetComponent>("DamageFont");
	m_DamageWidgetComponent->UseAlphaBlend(true);

	CDamageFont* DamageFont = m_DamageWidgetComponent->CreateWidgetWindow<CDamageFont>("DamageFontWidget");

	// 무조건 Child 추가하고 난 뒤에 Transform 건들여야된다!
	m_Sprite->AddChild(m_DamageWidgetComponent);

	m_DamageWidgetComponent->SetRelativePos(-20.f, 0.f, 0.f);

	//m_DamageWidgetComponent->SetWorldPos(WorldPos.x - 20.f, WorldPos.y, 0.f);



	CLowerClassBookAnimation* Instance = (CLowerClassBookAnimation*)m_Sprite->GetAnimationInstance();

	Instance->SetEndFunction<CLowerClassBook>("LowerClassBookHitLeft", this, &CLowerClassBook::ReturnIdle);
	Instance->SetEndFunction<CLowerClassBook>("LowerClassBookDieLeft", this, &CLowerClassBook::Die);

	m_Body->AddCollisionCallback<CLowerClassBook>(Collision_State::Begin, this, &CLowerClassBook::CollisionBeginCallback);
	m_Body->AddCollisionCallback<CLowerClassBook>(Collision_State::End, this, &CLowerClassBook::CollisionEndCallback);

	int LeftStart = rand() % 2 - 1;
	if (LeftStart < 0)
		m_Sprite->Flip();

}

bool CLowerClassBook::Init()
{
	m_Sprite = CreateComponent<CSpriteComponent>("LowerClassBookSprite");
	m_Body = CreateComponent<CColliderBox2D>("Body");

	m_Body->SetCollisionProfile("Monster");

	SetRootComponent(m_Sprite);
	m_Sprite->AddChild(m_Body);

	m_Sprite->SetTransparency(true);

	m_Sprite->SetWorldScale(100.f, 100.f, 1.f);
	m_Sprite->SetRelativePos(500.f, 300.f, 0.f);
	m_Sprite->SetPivot(0.5f, 0.5f, 0.f);


	return true;
}

void CLowerClassBook::Update(float DeltaTime)
{
	CMonster::Update(DeltaTime);

	FiniteState(DeltaTime);

	CTileMapComponent* TileMapCom = m_Body->CheckCurrentFrameCollisionByType<CTileMapComponent>();

	if (TileMapCom)
	{
		if (EdgeTileCheck(TileMapCom, m_Body->GetWorldPos(), m_Body->GetWorldScale()))
		{
			m_MonsterState = Monster_State::Move;
			m_Sprite->Flip();
		}
	}
}

void CLowerClassBook::PostUpdate(float DeltaTime)
{
	CMonster::PostUpdate(DeltaTime);
}

CLowerClassBook* CLowerClassBook::Clone()
{
	return new CLowerClassBook(*this);
}

void CLowerClassBook::Save(FILE* File)
{
	CMonster::Save(File);
}

void CLowerClassBook::Load(FILE* File)
{
	CMonster::Load(File);

	m_Sprite = (CSpriteComponent*)FindComponent("LowerClassBookSprite");

	m_Body = (CColliderBox2D*)FindComponent("Body");

	m_Sprite->AddChild(m_Body);

	CSceneMode* Mode = GetScene()->GetSceneMode();

	if (Mode)
	{
		((CLibrary2ndScene*)Mode)->PushLowerClassBook(this);
	}
}

void CLowerClassBook::FlipAll(float DeltaTime)
{
	m_Sprite->Flip();
}

void CLowerClassBook::ReturnIdle()
{
	if (m_MonsterInfo.HP <= 0.f)
	{
		m_Body->Enable(false);
		m_IsChanging = true;

		GetRootComponent()->DeleteChild("Body");
		m_Sprite->GetAnimationInstance()->ChangeAnimation("LowerClassBookDieLeft");

		return;
	}

	m_IsChanging = false;
	m_Sprite->ChangeAnimation("LowerClassBookIdleLeft");
}

void CLowerClassBook::SetDamage(float Damage, bool Critical)
{
	m_MonsterInfo.HP -= (int)Damage;

	if (m_MonsterInfo.HP <= 0.f)
	{
		m_Body->Enable(false);

		GetRootComponent()->DeleteChild("Body");
		m_Sprite->GetAnimationInstance()->ChangeAnimation("LowerClassBookDieLeft");
		m_IsChanging = true;

		PushDamageFont(Damage, Critical);

		DropItem();
	}

	else
	{
		m_MonsterState = Monster_State::Track;

		if (!m_IsChanging)
		{
			m_Sprite->ChangeAnimation("LowerClassBookHitLeft");

			m_IsChanging = true;
		}

		PushDamageFont(Damage, Critical);
	}
}

void CLowerClassBook::PushDamageFont(float Damage, bool Critical)
{
	CMonster::PushDamageFont(Damage, Critical);
}

void CLowerClassBook::FiniteState(float DeltaTime)
{
	if (m_IsChanging)
		return;

	if (m_MonsterState == Monster_State::Track)
	{
		CPlayer2D* Player = (CPlayer2D*)m_Scene->GetPlayerObject();

		if (Player)
		{
			Vector3 PlayerPos = Player->GetWorldPos();
			Vector3 MyPos = GetWorldPos();

			if (abs(PlayerPos.y - MyPos.y) > 200.f)
			{
				m_AccTime = 0.f;
				m_MonsterState = Monster_State::Idle;
				return;
			}

			if (PlayerPos.x - MyPos.x > 10.f)
			{
				if (!m_Sprite->IsFlip())
					m_Sprite->Flip();

				AddWorldPos(100.f * DeltaTime, 0.f, 0.f);
			}

			else if (PlayerPos.x - MyPos.x < -10.f)
			{
				if (m_Sprite->IsFlip())
					m_Sprite->Flip();

				AddWorldPos(-100.f * DeltaTime, 0.f, 0.f);
			}
		}

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

			break;
		case Monster_State::Move:

			break;
		}
	}

	else
	{
		switch (m_MonsterState)
		{
		case Monster_State::Idle:
			break;
		case Monster_State::Move:
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

void CLowerClassBook::SetTrackState()
{
	CMonster::SetTrackState();
}

void CLowerClassBook::CollisionBeginCallback(const CollisionResult& Result)
{
	CGameObject* Object = Result.Dest->GetGameObject();

	if (Object->GetTypeID() == typeid(CPlayer2D).hash_code())
	{
		if (CRenderManager::GetInst()->GetStartFadeIn())
			return;

		((CPlayer2D*)Object)->SetDamage((float)m_MonsterInfo.Attack, false);
	}
}

void CLowerClassBook::CollisionEndCallback(const CollisionResult& Result)
{
}

