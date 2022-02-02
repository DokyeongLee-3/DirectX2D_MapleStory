
#include "LobbyBigLamp.h"
#include "Animation/AnimationSequence2DInstance.h"


CLobbyBigLamp::CLobbyBigLamp() :
	m_Sprite(nullptr)
{
	SetTypeID<CLobbyBigLamp>();
}

CLobbyBigLamp::CLobbyBigLamp(const CLobbyBigLamp& obj) :
	CGameObject(obj)
{
	m_Sprite = (CSpriteComponent*)FindComponent("LobbyBigLampSprite");
	m_HelixSprite = (CSpriteComponent*)FindComponent("LobbyBigLampHelixSprite");
	m_ButterflySprite = (CSpriteComponent*)FindComponent("LobbyBigLampButterflySprite");

	//m_DragCollider = CreateComponent<CDragCollider>("DragCollider");
}

CLobbyBigLamp::~CLobbyBigLamp()
{
}

void CLobbyBigLamp::Start()
{
	CGameObject::Start();
}

bool CLobbyBigLamp::Init()
{
	m_Sprite = CreateComponent<CSpriteComponent>("LobbyBigLampSprite");
	m_HelixSprite = CreateComponent<CSpriteComponent>("LobbyBigLampHelixSprite");
	m_ButterflySprite = CreateComponent<CSpriteComponent>("LobbyBigLampButterflySprite");

	m_DragCollider = CreateComponent<CDragCollider>("DragCollider");

	m_DragCollider->SetCollisionProfile("DragCollider");

	SetRootComponent(m_Sprite);
	m_Sprite->AddChild(m_DragCollider);
	m_Sprite->AddChild(m_HelixSprite);
	m_Sprite->AddChild(m_ButterflySprite);

	SetRootComponent(m_Sprite);

	m_Sprite->SetTransparency(true);
	m_HelixSprite->SetTransparency(true);
	m_ButterflySprite->SetTransparency(true);

	//m_Sprite->CreateAnimationInstance<CAnimationSequence2DInstance>();
	m_HelixSprite->CreateAnimationInstance<CAnimationSequence2DInstance>();
	m_ButterflySprite->CreateAnimationInstance<CAnimationSequence2DInstance>();

	m_Sprite->SetWorldScale(100.f, 100.f, 1.f);
	m_Sprite->SetRelativePos(500.f, 300.f, 0.f);
	m_Sprite->SetPivot(0.5f, 0.5f, 0.f);

	m_HelixSprite->SetPivot(0.5f, 0.5f, 0.f);
	m_ButterflySprite->SetPivot(0.5f, 0.5f, 0.f);

	m_DragCollider->SetWorldScale(40.f, 40.f, 1.f);



	//Anim->AddAnimation(TEXT("LobbyBigLamp.sqc"), ANIMATION_PATH, "LobbyBigLamp", true, 1.3f);

	//Anim = m_HelixSprite->GetAnimationInstance();

	CAnimationSequence2DInstance* Anim = m_HelixSprite->GetAnimationInstance();

	Anim->AddAnimation(TEXT("LampBigHelix.sqc"), ANIMATION_PATH, "LampBigHelix", true, 1.4f);

	Anim = m_ButterflySprite->GetAnimationInstance();

	Anim->AddAnimation(TEXT("Library2ndButterfly.sqc"), ANIMATION_PATH, "Library2ndButterfly", true, 1.f);

	return true;
}

void CLobbyBigLamp::Update(float DeltaTime)
{
	CGameObject::Update(DeltaTime);
}

void CLobbyBigLamp::PostUpdate(float DeltaTime)
{
	CGameObject::PostUpdate(DeltaTime);
}

CLobbyBigLamp* CLobbyBigLamp::Clone()
{
	return new CLobbyBigLamp(*this);
}

void CLobbyBigLamp::Save(FILE* File)
{
	CGameObject::Save(File);
}

void CLobbyBigLamp::Load(FILE* File)
{
	CGameObject::Load(File);

	m_Sprite = (CSpriteComponent*)FindComponent("LobbyBigLampSprite");

	m_Sprite->SetTransparency(true);


	m_HelixSprite = (CSpriteComponent*)FindComponent("LobbyBigLampHelixSprite");

	m_HelixSprite->SetTransparency(true);

	m_HelixSprite->GetCurrentAnimation()->SetPlayTime(1.3f);


	m_ButterflySprite = (CSpriteComponent*)FindComponent("LobbyBigLampButterflySprite");

	m_ButterflySprite->SetTransparency(true);

	m_ButterflySprite->GetCurrentAnimation()->SetPlayTime(1.f);
}

