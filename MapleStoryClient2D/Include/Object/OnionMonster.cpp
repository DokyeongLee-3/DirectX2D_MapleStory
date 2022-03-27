
#include "OnionMonster.h"
#include "../Animation/OnionMonsterAnimation.h"
#include "Scene/Scene.h"
#include "../Scene/OnionScene.h"
#include "Resource/Material/Material.h"
#include "Engine.h"
#include "../Widget/DamageFont.h"
#include "Player2D.h"
#include "TileObject.h"
#include "Component/TileMapComponent.h"
#include "Render/RenderManager.h"
#include "ItemOnion.h"

COnionMonster::COnionMonster()
{
	SetTypeID<COnionMonster>();

	MonsterInfo Info = CClientManager::GetInst()->FindMonsterInfo("OnionMonster");

	m_TileCollisionEnable = true;
	m_MonsterInfo.HP = Info.HP;
	m_MonsterInfo.HPMax = Info.HPMax;
	m_MonsterInfo.Level = Info.Level;
	m_MonsterInfo.Attack = Info.Attack;

	m_FiniteStateTimeTable[(int)Monster_State::Idle] = (float)(rand() % 5) + 1.f;
	m_FiniteStateTimeTable[(int)Monster_State::Move] = (float)(rand() % 4) + 1.f;
}

COnionMonster::COnionMonster(const COnionMonster& obj) :
	CMonster(obj)
{
	m_Sprite = (CSpriteComponent*)FindComponent("OnionMonsterSprite");
	m_Body = (CColliderCircle*)FindComponent("Body");
}

COnionMonster::~COnionMonster()
{
}

void COnionMonster::Start()
{
	CMonster::Start();

	m_DamageWidgetComponent = CreateComponent<CWidgetComponent>("DamageFont");
	m_DamageWidgetComponent->UseAlphaBlend(true);

	CDamageFont* DamageFont = m_DamageWidgetComponent->CreateWidgetWindow<CDamageFont>("DamageFontWidget");

	// 무조건 Child 추가하고 난 뒤에 Transform 건들여야된다!
	m_Sprite->AddChild(m_DamageWidgetComponent);

	m_DamageWidgetComponent->SetRelativePos(-20.f, 0.f, 0.f);
	m_DamageWidgetComponent->SetLayerName("ScreenWidgetComponent");
	//m_DamageWidgetComponent->SetWorldPos(WorldPos.x - 20.f, WorldPos.y, 0.f);



	//DamageFont->SetPos(WorldPos.x - 20.f, WorldPos.y);

	COnionMonsterAnimation* Instance = (COnionMonsterAnimation*)m_Sprite->GetAnimationInstance();

	Instance->SetEndFunction<COnionMonster>("OnionHitLeft", this, &COnionMonster::ReturnIdle);
	Instance->SetEndFunction<COnionMonster>("OnionDieLeft", this, &COnionMonster::Die);

	m_Body->AddCollisionCallback<COnionMonster>(Collision_State::Begin, this, &COnionMonster::CollisionBeginCallback);
	m_Body->AddCollisionCallback<COnionMonster>(Collision_State::End, this, &COnionMonster::CollisionEndCallback);
	m_Body->Enable(true);

	int LeftStart = rand() % 2 - 1;
	if (LeftStart < 0)
		m_Sprite->Flip();
}

bool COnionMonster::Init()
{
	CMonster::Init();

	m_Sprite = CreateComponent<CSpriteComponent>("OnionMonsterSprite");
	m_Body = CreateComponent<CColliderCircle>("Body");
	m_Body->SetRadius(30.f);
	//m_PaperBurn = CreateComponent<CPaperBurnComponent>("PaperBurn");

	SetRootComponent(m_Sprite);

	m_Sprite->AddChild(m_Body);

	m_Sprite->SetTransparency(true);
	m_Body->SetCollisionProfile("Monster");
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
	CMonster::Update(DeltaTime);

	FiniteState(DeltaTime);

	CTileMapComponent* TileMapCom = m_Body->CheckCurrentFrameCollisionByType<CTileMapComponent>();

	if (TileMapCom)
	{
		if (EdgeTileCheck(TileMapCom, m_Body->GetWorldPos(), m_Body->GetWorldScale()))
		{
			m_MonsterState = Monster_State::Move;
			m_Sprite->Flip();

			float TileCenterX = TileMapCom->GetWorldPos().x + TileMapCom->GetWorldScale().x / 2.f;

			if (m_TurnTime < 0.1f)
			{
				float BodyPosX = m_Body->GetWorldPos().x;
				float TileCenterPosX = TileMapCom->GetWorldPos().x + TileMapCom->GetWorldScale().x / 2.f;

				// 우측 EdgeTile에 끼었을때
				if (BodyPosX > TileCenterPosX)
				{
					float Adjust = -1.f;

					while (EdgeTileCheck(TileMapCom, m_Body->GetWorldPos(), m_Body->GetWorldScale()))
					{
						AddWorldPos(Adjust, 0.f, 0.f);
						Adjust -= 3.f;
					}
				}

				// 좌측 EdgeTile에 끼었을때
				else
				{
					float Adjust = 1.f;

					while (EdgeTileCheck(TileMapCom, m_Body->GetWorldPos(), m_Body->GetWorldScale()))
					{
						AddWorldPos(Adjust, 0.f, 0.f);
						Adjust += 3.f;
					}
				}

			}

			m_TurnTime = 0.f;
		}
	}

	CSpriteComponent* Root = (CSpriteComponent*)m_RootComponent.Get();
	float Opacity = Root->GetOpacity();	

	if (Opacity < 1.f)
	{
		if (Opacity + DeltaTime > 1.f)
			Root->SetOpacity(1.f);
		else
			Root->SetOpacity(Opacity + DeltaTime);
	}
}

void COnionMonster::PostUpdate(float DeltaTime)
{
	CMonster::PostUpdate(DeltaTime);
}

COnionMonster* COnionMonster::Clone()
{
	return new COnionMonster(*this);
}

void COnionMonster::SetDamage(float Damage, bool Critical)
{
	m_MonsterInfo.HP -= (int)Damage;

	if (m_MonsterInfo.HP <= 0.f)
	{
		PushDamageFont(Damage, Critical);

		if (!m_Body->IsEnable())
			return;

		m_Body->Enable(false);

		GetRootComponent()->DeleteChild("Body");
		m_Sprite->GetAnimationInstance()->ChangeAnimation("OnionDieLeft");
		m_IsChanging = true;

		DropBill();
		DropItemOnion();


		CSceneMode* SceneMode = m_Scene->GetSceneMode();
		if (SceneMode)
		{
			SceneMode->PushDeadPos(GetWorldPos());
			((COnionScene*)SceneMode)->DeleteOnionMonster(this);
		}
	}

	else
	{
		m_MonsterState = Monster_State::Track;

		if (!m_IsChanging)
		{
			m_Sprite->ChangeAnimation("OnionHitLeft");

			m_IsChanging = true;
		}

		PushDamageFont(Damage, Critical);
	}
}

void COnionMonster::PushDamageFont(float Damage, bool Critical)
{
	//((CDamageFont*)m_DamageWidgetComponent->GetWidgetWindow())->PushDamageNumber((int)Damage, Critical);
	CMonster::PushDamageFont(Damage, Critical);
}

void COnionMonster::SetTrackState()
{
	CMonster::SetTrackState();
}

void COnionMonster::ReturnIdle()
{
	m_IsChanging = false;

	if (m_MonsterState == Monster_State::Track)
		m_Sprite->ChangeAnimation("OnionWalkLeft");

	else
		m_Sprite->ChangeAnimation("OnionIdleLeft");
}

void COnionMonster::FiniteState(float DeltaTime)
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

			else if(PlayerPos.x - MyPos.x < -10.f)
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

CItemOnion* COnionMonster::DropItemOnion()
{

	CItemOnion* ItemOnion = m_Scene->CreateGameObject<CItemOnion>("ItemOnion");

	Vector3 Pos = GetWorldPos();

	ItemOnion->SetWorldPos(Pos.x + 20.f, Pos.y + 10.f, Pos.z);

	return ItemOnion;
}


void COnionMonster::CollisionBeginCallback(const CollisionResult& Result)
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

void COnionMonster::CollisionEndCallback(const CollisionResult& Result)
{
}

void COnionMonster::Save(FILE* File)
{
	CMonster::Save(File);
}

void COnionMonster::Load(FILE* File)
{
	CMonster::Load(File);

	m_Sprite = (CSpriteComponent*)FindComponent("OnionMonsterSprite");

	m_Sprite->SetTransparency(true);

	m_Body = (CColliderCircle*)FindComponent("Body");

	((COnionScene*)(m_Scene->GetSceneMode()))->PushOnionMonster(this);
}
