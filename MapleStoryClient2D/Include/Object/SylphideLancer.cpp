
#include "SylphideLancer.h"
#include "../ClientManager.h"
#include "Engine.h"
#include "Player2D.h"
#include "Component/SpriteComponent.h"
#include "Animation/AnimationSequence2DInstance.h"
#include "Scene/Scene.h"
#include "Scene/SceneMode.h"
#include "../Object/SylphideLancerHitEffect.h"
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
	Anim->AddAnimation(TEXT("SylphideLancerArrowPurple.sqc"), ANIMATION_PATH, "SylphideLancerArrowPurple", true, 0.3f);

	return true;
}

void CSylphideLancer::Update(float DeltaTime)
{
	CGameObject::Update(DeltaTime);

	Vector3 Axis = GetWorldAxis(AXIS::AXIS_X);

	AddRelativePos(Axis * m_Speed * DeltaTime);

	float AbsoluteSpeed = abs(m_Speed);
	m_Distance += AbsoluteSpeed * DeltaTime;


	// ù��° �׷쿡 2���� ����(ID�� ���� 0, 1)�� 1�� ID�� ������ 150�̻��� �Ÿ��� �̵�������
	// �ι�° �׷쿡 2���� ����(ID�� ���� 2, 3)�� �߰������� �÷��̾ �����ϵ��� �ؼ� �ѹ� ��ų����
	// �� 4���� â�� �������� �Ѵ�
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

void CSylphideLancer::CollisionBeginCallback(const CollisionResult& result)
{
	PlayerInfo Info = ((CPlayer2D*)m_Scene->GetSceneMode()->GetPlayerObject())->GetInfo();

	int Factor = (int)Info.INT * Info.Level;

	int RamdomNumber = rand();

	int random = (RamdomNumber % (Factor / 20 + 1)) - (Factor / 30);

	float Damage = Factor / 10.f + random;

	// ũ��Ƽ�� �������� �� ��� -> �߰������� ����Ʈ �޾��ֱ�
	bool IsCritical = random > 0;

	result.Dest->GetGameObject()->SetDamage(Damage, IsCritical);
	m_Scene->GetResource()->SoundPlay("SylphideLancerHit");

	if (m_LancerID == 0)
	{
		Vector3 HitEffectPos = GetWorldPos();

		// ���������� ���ư��ٰ� �浹
		if (m_Sprite->IsFlip())
			HitEffectPos.x += 30.f;

		// �������� ���ư��ٰ� �浹
		else
			HitEffectPos.x -= 30.f;

		CSylphideLancerHitEffect* HitEffect = m_Scene->CloneFromPrototype<CSylphideLancerHitEffect>(
			"SylphideLancerHitEffect", "SylphideLancerHitEffect",
			HitEffectPos);

		// ���ǵ巣�� ù��° �׷��� 150���� ���� ���ư��� �浹�ؼ� �Ҹ��ص� �ι�° �׷��� �̾ �߻��ϵ����Ѵ�
		if (m_Distance < 150.f)
		{
			CPlayer2D* Player = (CPlayer2D*)(m_Scene->GetSceneMode()->GetPlayerObject());
			Player->ProduceSecondSylphideLander(CEngine::GetInst()->GetDeltaTime());
			m_ProduceLatterGroup = true;
		}
	}

	// �ι�° �׷��� �浹
	else if(m_LancerID == 2)
	{
		Vector3 HitEffectPos = GetWorldPos();

		// ���������� ���ư��ٰ� �浹
		if (m_Sprite->IsFlip())
			HitEffectPos.x += 30.f;

		// �������� ���ư��ٰ� �浹
		else
			HitEffectPos.x -= 30.f;

		CSylphideLancerHitEffect* HitEffect = m_Scene->CloneFromPrototype<CSylphideLancerHitEffect>(
			"SylphideLancerHitEffect", "SylphideLancerHitEffect",
			HitEffectPos);

		CSpriteComponent* Root = (CSpriteComponent*)HitEffect->GetRootComponent();

		Root->ChangeAnimation("SylphideLancerHitPurple");
	}

	Destroy();
}

void CSylphideLancer::CollisionEndCallback(const CollisionResult& result)
{
}
