
#include "RadishMonster.h"
#include "TileObject.h"
#include "../Animation/RadishMonsterAnimation.h"
#include "Scene/Scene.h"
#include "../Scene/RadishScene.h"
#include "Resource/Material/Material.h"
#include "Engine.h"
#include "../Widget/DamageFont.h"
#include "Player2D.h"
#include "Component/TileMapComponent.h"
#include "Render/RenderManager.h"

CRadishMonster::CRadishMonster()
{
	SetTypeID<CRadishMonster>();

	m_TileCollisionEnable = true;
	m_MonsterInfo.HP = 7000;
	m_MonsterInfo.HPMax = 7000;
	m_MonsterInfo.Level = 50;
	m_MonsterInfo.Attack = 10;

	m_FiniteStateTimeTable[(int)Monster_State::Idle] = (float)(rand() % 5) + 1.f;
	m_FiniteStateTimeTable[(int)Monster_State::Move] = (float)(rand() % 4) + 1.f;
}

CRadishMonster::CRadishMonster(const CRadishMonster& obj) :
	CMonster(obj)
{
	m_Sprite = (CSpriteComponent*)FindComponent("RadishMonsterSprite");
	m_Body = (CColliderBox2D*)FindComponent("Body");
}

CRadishMonster::~CRadishMonster()
{
}

void CRadishMonster::Start()
{
	CMonster::Start();

	m_Gravity = true;
	SetGravityFactor(900.f);

	m_DamageWidgetComponent = CreateComponent<CWidgetComponent>("DamageFont");
	m_DamageWidgetComponent->UseAlphaBlend(true);

	CDamageFont* DamageFont = m_DamageWidgetComponent->CreateWidgetWindow<CDamageFont>("DamageFontWidget");

	// 무조건 Child 추가하고 난 뒤에 Transform 건들여야된다!
	m_Sprite->AddChild(m_DamageWidgetComponent);

	m_DamageWidgetComponent->SetRelativePos(-20.f, 0.f, 0.f);
	m_DamageWidgetComponent->SetLayerName("ScreenWidgetComponent");

	CRadishMonsterAnimation* Instance = (CRadishMonsterAnimation*)m_Sprite->GetAnimationInstance();

	Instance->SetEndFunction<CRadishMonster>("RadishHitLeft", this, &CRadishMonster::ReturnIdle);
	Instance->SetEndFunction<CRadishMonster>("RadishDieLeft", this, &CRadishMonster::Die);

	m_Body->AddCollisionCallback<CRadishMonster>(Collision_State::Begin, this, &CRadishMonster::CollisionBeginCallback);
	m_Body->AddCollisionCallback<CRadishMonster>(Collision_State::End, this, &CRadishMonster::CollisionEndCallback);
	m_Body->Enable(true);

	int LeftStart = rand() % 2 - 1;
	if (LeftStart < 0)
		m_Sprite->Flip();
}

bool CRadishMonster::Init()
{
	m_Sprite = CreateComponent<CSpriteComponent>("RadishMonsterSprite");
	m_Body = CreateComponent<CColliderBox2D>("Body");
	m_Body->SetCollisionProfile("Monster");

	m_Sprite->AddChild(m_Body);


	m_Sprite->SetTransparency(true);
	//m_Sprite->SetOpacity(0.5f);

	//m_PaperBurn->SetMaterial(m_Sprite->GetMaterial());
	//m_PaperBurn->SetFinishTime(8.f);
	//m_PaperBurn->SetFinishCallback<CRadishMonster>(this, &CRadishMonster::Die);

	m_Sprite->CreateAnimationInstance<CRadishMonsterAnimation>();


	m_Sprite->SetRelativeScale(100.f, 100.f, 1.f);
	m_Sprite->SetRelativePos(500.f, 300.f, 0.f);
	m_Sprite->SetPivot(0.5f, 0.5f, 0.f);

	m_Body->SetExtent(20.f, 32.5f);
	m_Body->SetRelativePos(0.f, -5.f, 0.f);

	return true;
}

void CRadishMonster::Update(float DeltaTime)
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

void CRadishMonster::PostUpdate(float DeltaTime)
{
	CMonster::PostUpdate(DeltaTime);
}

CRadishMonster* CRadishMonster::Clone()
{
	return new CRadishMonster(*this);
}

void CRadishMonster::Save(FILE* File)
{
	CMonster::Save(File);
}

void CRadishMonster::Load(FILE* File)
{
	CMonster::Load(File);

	m_Sprite = (CSpriteComponent*)FindComponent("RadishMonsterSprite");

	m_Sprite->SetTransparency(true);

	m_Body = (CColliderBox2D*)FindComponent("Body");

	((CRadishScene*)(m_Scene->GetSceneMode()))->PushRadishMonster(this);
}

void CRadishMonster::SetDamage(float Damage, bool Critical)
{
	m_MonsterInfo.HP -= (int)Damage;

	if (m_MonsterInfo.HP <= 0.f)
	{
		PushDamageFont(Damage, Critical);

		if (!m_Body->IsEnable())
			return;

		m_Body->Enable(false);

		GetRootComponent()->DeleteChild("Body");
		m_Sprite->GetAnimationInstance()->ChangeAnimation("RadishDieLeft");
		m_IsChanging = true;

		DropItem();

		CSceneMode* SceneMode = m_Scene->GetSceneMode();
		if (SceneMode)
		{
			SceneMode->PushDeadPos(GetWorldPos());
			((CRadishScene*)SceneMode)->DeleteRadishMonster(this);
		}
	}

	else
	{
		m_MonsterState = Monster_State::Track;

		if (!m_IsChanging)
		{
			m_Sprite->ChangeAnimation("RadishHitLeft");

			m_IsChanging = true;
		}

		PushDamageFont(Damage, Critical);
	}
}

void CRadishMonster::PushDamageFont(float Damage, bool Critical)
{
	CMonster::PushDamageFont(Damage, Critical);
}

void CRadishMonster::ReturnIdle()
{
	m_IsChanging = false;

	if (m_MonsterState == Monster_State::Track)
		m_Sprite->ChangeAnimation("RadishWalkLeft");

	else
		m_Sprite->ChangeAnimation("RadishIdleLeft");
}

void CRadishMonster::FiniteState(float DeltaTime)
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

				AddWorldPos(Vector3(100.f * DeltaTime, 100.f * DeltaTime, 0.f) * m_DirVector);
			}

			else if (PlayerPos.x - MyPos.x < -10.f)
			{
				if (m_Sprite->IsFlip())
					m_Sprite->Flip();

				AddWorldPos(Vector3(-100.f * DeltaTime, -100.f * DeltaTime, 0.f) * m_DirVector);
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
			m_Sprite->ChangeAnimation("RadishIdleLeft");
			break;
		case Monster_State::Move:
			m_Sprite->ChangeAnimation("RadishWalkLeft");
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
				AddWorldPos(Vector3(100.f * DeltaTime, 100.f * DeltaTime, 0.f) * m_DirVector);
			}

			else
			{
				AddWorldPos(Vector3(-100.f * DeltaTime, -100.f * DeltaTime, 0.f) * m_DirVector);
			}
		}
		break;
		}
	}
}

void CRadishMonster::SetTrackState()
{
	CMonster::SetTrackState();
}

void CRadishMonster::CollisionBeginCallback(const CollisionResult& Result)
{
	CGameObject* DestObject = Result.Dest->GetGameObject();

	if (DestObject->GetTypeID() == typeid(CPlayer2D).hash_code())
	{
		if (CRenderManager::GetInst()->GetStartFadeIn())
			return;

		((CPlayer2D*)DestObject)->SetDamage((float)m_MonsterInfo.Attack, false);
	}

	if (DestObject->GetTypeID() == typeid(CTileObject).hash_code())
	{
		CTileObject* TileObj = ((CTileObject*)DestObject);
		CColliderBox2D* TileObjCollider = (CColliderBox2D*)Result.Dest;
		Vector3 ColliderRot = TileObjCollider->GetWorldRot();


		std::vector<CColliderBox2D*> vecCollider;

		m_Body->FindMultipleCollisionComponentByObjType<CColliderBox2D, CTileObject>(vecCollider);

		// 만약 원래 하나의 충돌체와 충돌중이었고, 같은 오브젝트 내에 또 다른 충돌체와 이제 막 충돌해서 두개의 충돌체와
		// 충돌중일때, 원래 충돌하고 있던 충돌체
		CColliderBox2D* EarlyCollider = nullptr;

		size_t Count = vecCollider.size();
		int SameTileObjColliderCount = 0;

		for (size_t i = 0; i < Count; ++i)
		{
			if ((CTileObject*)vecCollider[i]->GetGameObject() == TileObj)
				++SameTileObjColliderCount;

			if (vecCollider[i] != Result.Dest)
				EarlyCollider = vecCollider[i];
		}

		if (Count == 0)
			return;

		if (SameTileObjColliderCount == 2 && EarlyCollider)
		{
			ColliderRot = ColliderRot - EarlyCollider->GetWorldRot();

			if (ColliderRot.z == 0.f)
			{
				m_DirVector = Vector3(1.f, 0.f, 0.f);
			}

			else
			{
				Matrix matRot;

				matRot.Rotation(ColliderRot);

				m_DirVector = m_DirVector.TransformCoord(matRot);
			}

			m_DirRotation = ColliderRot;
		}

		else
		{
			m_DirRotation = ColliderRot - m_DirRotation;

			if (ColliderRot.z == 0.f)
			{
				m_DirVector = Vector3(1.f, 0.f, 0.f);
			}

			else
			{
				Matrix matRot;

				matRot.Rotation(m_DirRotation);

				m_DirVector = m_DirVector.TransformCoord(matRot);
			}

			m_DirRotation = ColliderRot;
		}

		m_Gravity = false;
		m_GravityAccTime = 0.f;
	}
}

void CRadishMonster::CollisionEndCallback(const CollisionResult& Result)
{
}


