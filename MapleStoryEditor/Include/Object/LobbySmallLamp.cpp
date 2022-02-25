
#include "LobbySmallLamp.h"
#include "Animation/AnimationSequence2DInstance.h"


CLobbySmallLamp::CLobbySmallLamp()
{
	SetTypeID<CLobbySmallLamp>();
}

CLobbySmallLamp::CLobbySmallLamp(const CLobbySmallLamp& obj) :
	CGameObject(obj)
{
	m_RootComponent = (CSpriteComponent*)FindComponent("LobbySmallLampRoot");
	m_VerticalLightLeftSprite = (CSpriteComponent*)FindComponent("LobbySmallLampLightLeftSprite");
	m_VerticalLightRightSprite = (CSpriteComponent*)FindComponent("LobbySmallLampLightRightSprite");
	//m_Body = (CColliderBox2D*)FindComponent("Body");

	m_HelixSprite = (CSpriteComponent*)FindComponent("LobbySmallLampHelixSprite");
	m_ButterflySprite = (CSpriteComponent*)FindComponent("LobbySmallLampButterflySprite");
}

CLobbySmallLamp::~CLobbySmallLamp()
{
}

void CLobbySmallLamp::Start()
{
	CGameObject::Start();
}

bool CLobbySmallLamp::Init()
{
	m_RootComponent = CreateComponent<CSpriteComponent>("LobbySmallLampRoot");
	m_HelixSprite = CreateComponent<CSpriteComponent>("LobbySmallLampHelixSprite");
	m_ButterflySprite = CreateComponent<CSpriteComponent>("LobbySmallLampButterflySprite");
	m_VerticalLightLeftSprite = CreateComponent<CSpriteComponent>("LobbySmallLampLightLeftSprite");
	m_VerticalLightRightSprite = CreateComponent<CSpriteComponent>("LobbySmallLampLightRightSprite");

	SetRootComponent(m_RootComponent);
	m_RootComponent->AddChild(m_VerticalLightLeftSprite);
	m_RootComponent->AddChild(m_VerticalLightRightSprite);
	m_RootComponent->AddChild(m_HelixSprite);
	m_RootComponent->AddChild(m_ButterflySprite);

	m_RootComponent->SetTransparency(true);
	m_VerticalLightLeftSprite->SetTransparency(true);
	m_VerticalLightRightSprite->SetTransparency(true);
	m_HelixSprite->SetTransparency(true);
	m_ButterflySprite->SetTransparency(true);

	//m_Sprite->CreateAnimationInstance<CAnimationSequence2DInstance>();
	m_HelixSprite->CreateAnimationInstance<CAnimationSequence2DInstance>();
	m_ButterflySprite->CreateAnimationInstance<CAnimationSequence2DInstance>();
	m_VerticalLightLeftSprite->CreateAnimationInstance<CAnimationSequence2DInstance>();
	m_VerticalLightRightSprite->CreateAnimationInstance<CAnimationSequence2DInstance>();

	m_RootComponent->SetWorldScale(100.f, 100.f, 1.f);
	m_RootComponent->SetRelativePos(500.f, 300.f, 0.f);
	m_RootComponent->SetPivot(0.5f, 0.5f, 0.f);

	m_HelixSprite->SetPivot(0.5f, 0.5f, 0.f);
	m_ButterflySprite->SetPivot(0.5f, 0.5f, 0.f);
	m_VerticalLightLeftSprite->SetPivot(0.5f, 0.5f, 0.f);
	m_VerticalLightRightSprite->SetPivot(0.5f, 0.5f, 0.f);


	m_VerticalLightRightSprite->Flip();

	//Anim->AddAnimation(TEXT("LobbySmallLamp.sqc"), ANIMATION_PATH, "LobbySmallLamp", true, 1.3f);

	//Anim = m_HelixSprite->GetAnimationInstance();

	CAnimationSequence2DInstance* Anim = m_HelixSprite->GetAnimationInstance();

	Anim->AddAnimation(TEXT("LampBigHelix.sqc"), ANIMATION_PATH, "LampBigHelix", true, 1.4f);

	Anim = m_ButterflySprite->GetAnimationInstance();

	Anim->AddAnimation(TEXT("Library2ndButterfly.sqc"), ANIMATION_PATH, "Library2ndButterfly", true, 1.f);

	Anim = m_VerticalLightLeftSprite->GetAnimationInstance();

	Anim->AddAnimation(TEXT("VerticalLampLight.sqc"), ANIMATION_PATH, "VerticalLampLight", true, 1.f);

	Anim = m_VerticalLightRightSprite->GetAnimationInstance();

	Anim->AddAnimation(TEXT("VerticalLampLight.sqc"), ANIMATION_PATH, "VerticalLampLight", true, 1.f);

	return true;
}

void CLobbySmallLamp::Update(float DeltaTime)
{
	CGameObject::Update(DeltaTime);
}

void CLobbySmallLamp::PostUpdate(float DeltaTime)
{
	CGameObject::PostUpdate(DeltaTime);
}

CLobbySmallLamp* CLobbySmallLamp::Clone()
{
	return new CLobbySmallLamp(*this);
}

void CLobbySmallLamp::Save(FILE* File)
{
	CGameObject::Save(File);
}

void CLobbySmallLamp::Load(FILE* File)
{
	CGameObject::Load(File);

	m_RootComponent = (CSpriteComponent*)FindComponent("LobbySmallLampRoot");

	m_RootComponent->SetTransparency(true);


	m_HelixSprite = (CSpriteComponent*)FindComponent("LobbySmallLampHelixSprite");

	m_HelixSprite->SetTransparency(true);

	m_HelixSprite->GetCurrentAnimation()->SetPlayTime(1.3f);


	m_ButterflySprite = (CSpriteComponent*)FindComponent("LobbySmallLampButterflySprite");

	m_ButterflySprite->SetTransparency(true);

	m_ButterflySprite->GetCurrentAnimation()->SetPlayTime(1.f);


	m_VerticalLightLeftSprite = (CSpriteComponent*)FindComponent("LobbySmallLampLightLeftSprite");

	m_VerticalLightLeftSprite->SetTransparency(true);

	m_VerticalLightLeftSprite->GetCurrentAnimation()->SetPlayTime(0.7f);

	m_VerticalLightRightSprite = (CSpriteComponent*)FindComponent("LobbySmallLampLightRightSprite");

	m_VerticalLightRightSprite->SetTransparency(true);

	m_VerticalLightRightSprite->GetCurrentAnimation()->SetPlayTime(0.7f);
}

