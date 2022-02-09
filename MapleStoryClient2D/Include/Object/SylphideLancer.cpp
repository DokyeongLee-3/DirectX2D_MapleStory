
#include "SylphideLancer.h"
#include "../ClientManager.h"
#include "Engine.h"
#include "Player2D.h"
#include "Component/SpriteComponent.h"
#include "Animation/AnimationSequence2DInstance.h"
#include "Scene/Scene.h"
#include "Scene/SceneMode.h"
#include "SylphideLancerHitEffect.h"
#include <time.h>

CSylphideLancer::CSylphideLancer() :
	m_Distance(0.f),
	m_Speed(350.f),
	m_Body(nullptr),
	m_LancerID(-1),
	m_ProduceLatterGroup(false)
{
	SetTypeID<CSylphideLancer>();
}

CSylphideLancer::CSylphideLancer(const CSylphideLancer& obj) :
	CGameObject(obj)
{
	m_Sprite = (CSpriteComponent*)FindComponent("SylphideLancerSprite");
	m_Body = (CColliderBox2D*)FindComponent("Body");
	m_Distance = obj.m_Distance;
}

CSylphideLancer::~CSylphideLancer()
{
}

bool CSylphideLancer::Init()
{
	m_Sprite = CreateComponent<CSpriteComponent>("SylphideLancerSprite");

	SetRootComponent(m_Sprite);

	m_Sprite->SetRelativeScale(50.f, 50.f, 1.f);
	m_Sprite->SetPivot(0.5f, 0.5f, 0.f);
	m_Sprite->SetTransparency(true);

	m_Body = CreateComponent<CColliderBox2D>("Body");
	m_Body->SetCollisionProfile("PlayerAttack");
	m_Body->SetExtent(40.f, 15.f);
	m_Body->AddCollisionCallback<CSylphideLancer>(Collision_State::Begin, this,
		&CSylphideLancer::CollisionBeginCallback);

	m_Sprite->AddChild(m_Body);

	m_Sprite->CreateAnimationInstance<CAnimationSequence2DInstance>();

	CAnimationSequence2DInstance* Anim = m_Sprite->GetAnimationInstance();

	Anim->AddAnimation(TEXT("SylphideLancerArrowLeft.sqc"), ANIMATION_PATH, "SylphideLancerArrowLeft", true, 0.3f);

	return true;
}

void CSylphideLancer::Update(float DeltaTime)
{
	CGameObject::Update(DeltaTime);

	Vector3 Axis = GetWorldAxis(AXIS::AXIS_X);

	AddRelativePos(Axis * m_Speed * DeltaTime);


	m_Distance += m_Speed * DeltaTime;


	// 첫번째 그룹에 2개의 랜서(ID가 각각 0, 1)중 1번 ID인 랜서가 150이상의 거리를 이동했을때
	// 두번째 그룹에 2개의 랜서(ID가 각각 2, 3)을 추가적으로 플레이어가 생성하도록 해서 한번 스킬쓰면
	// 총 4개의 창이 나가도록 한다
	if (m_Distance > 150.f && m_LancerID == 1 && !m_ProduceLatterGroup)
	{
		CPlayer2D* Player = (CPlayer2D*)(m_Scene->GetSceneMode()->GetPlayerObject());
		Player->ProduceSecondSylphideLander(DeltaTime);
		m_ProduceLatterGroup = true;
	}

	if (m_Distance >= 500.f)
	{
		m_Distance = 0.f;
		Destroy();
	}
}

void CSylphideLancer::PostUpdate(float DeltaTime)
{
	CGameObject::PostUpdate(DeltaTime);
}

CSylphideLancer* CSylphideLancer::Clone()
{
	return new CSylphideLancer(*this);
}

void CSylphideLancer::SetCollisionProfile(const std::string& Name)
{
	m_Body->SetCollisionProfile(Name);
}

void CSylphideLancer::FlipAll(float DeltaTime)
{
	m_Sprite->Flip();
}

/*
* 	int Factor = (int)Info.INT * Info.Level;
*/

void CSylphideLancer::CollisionBeginCallback(const CollisionResult& result)
{
	PlayerInfo Info = ((CPlayer2D*)m_Scene->GetSceneMode()->GetPlayerObject())->GetInfo();

	int Factor = (int)Info.INT * Info.Level;

	int RamdomNumber = rand();

	int random = (RamdomNumber % (Factor / 20 + 1)) - (Factor / 30);

	float Damage = Factor / 10.f + random;

	// 크리티컬 데미지가 뜬 경우 -> 추가적으로 이펙트 달아주기
	bool IsCritical = random > 0;

	result.Dest->GetGameObject()->SetDamage(Damage, IsCritical);

	if (m_LancerID == 0)
	{
		Vector3 HitEffectPos = GetWorldPos();

		// 오른쪽으로 날아가다가 충돌
		if (m_Sprite->IsFlip())
			HitEffectPos.x += 30.f;

		// 왼쪽으로 날아가다가 충돌
		else
			HitEffectPos.x -= 30.f;

		CSylphideLancerHitEffect* HitEffect = m_Scene->CloneFromPrototype<CSylphideLancerHitEffect>(
			"SylphideLancerHitEffect", "SylphideLancerHitEffect",
			HitEffectPos);

		// 실피드랜서 첫번째 그룹이 150보다 적게 날아가서 충돌해서 소멸해도 두번째 그룹을 이어서 발사하도록한다
		if (m_Distance < 150.f)
		{
			CPlayer2D* Player = (CPlayer2D*)(m_Scene->GetSceneMode()->GetPlayerObject());
			Player->ProduceSecondSylphideLander(CEngine::GetInst()->GetDeltaTime());
			m_ProduceLatterGroup = true;
		}
	}

	Destroy();
}

void CSylphideLancer::CollisionEndCallback(const CollisionResult& result)
{
}
