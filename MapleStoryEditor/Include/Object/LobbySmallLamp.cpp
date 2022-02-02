
#include "LobbySmallLamp.h"
#include "Animation/AnimationSequence2DInstance.h"


CLobbySmallLamp::CLobbySmallLamp() :
	m_Sprite(nullptr)
{
	SetTypeID<CLobbySmallLamp>();
}

CLobbySmallLamp::CLobbySmallLamp(const CLobbySmallLamp& obj) :
	CGameObject(obj)
{
	m_Sprite = (CSpriteComponent*)FindComponent("LobbySmallLampSprite");
	//m_Body = (CColliderBox2D*)FindComponent("Body");

	m_HelixSprite = (CSpriteComponent*)FindComponent("LobbySmallLampHelixSprite");
	m_ButterflySprite = (CSpriteComponent*)FindComponent("LobbySmallLampButterflySprite");
	m_VerticalLightSprite = (CSpriteComponent*)FindComponent("VerticalLightSprite");
	m_LampBrightImage = (CSpriteComponent*)FindComponent("LampBrightImage");
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
	m_Sprite = CreateComponent<CSpriteComponent>("LobbySmallLampSprite");
	m_HelixSprite = CreateComponent<CSpriteComponent>("LobbySmallLampHelixSprite");
	m_ButterflySprite = CreateComponent<CSpriteComponent>("LobbySmallLampButterflySprite");
	m_VerticalLightSprite = CreateComponent<CSpriteComponent>("VerticalLightSprite");
	m_LampBrightImage = CreateComponent<CSpriteComponent>("LampBrightImage");

	m_DragCollider = CreateComponent<CDragCollider>("DragCollider");

	m_DragCollider->SetCollisionProfile("DragCollider");

	SetRootComponent(m_Sprite);
	m_Sprite->AddChild(m_DragCollider);
	m_Sprite->AddChild(m_HelixSprite);
	m_Sprite->AddChild(m_ButterflySprite);
	m_Sprite->AddChild(m_VerticalLightSprite);
	m_Sprite->AddChild(m_LampBrightImage);

	SetRootComponent(m_Sprite);

	m_Sprite->SetTransparency(true);
	m_HelixSprite->SetTransparency(true);
	m_ButterflySprite->SetTransparency(true);
	m_VerticalLightSprite->SetTransparency(true);
	m_LampBrightImage->SetTransparency(true);

	//m_Sprite->CreateAnimationInstance<CAnimationSequence2DInstance>();
	m_HelixSprite->CreateAnimationInstance<CAnimationSequence2DInstance>();
	m_ButterflySprite->CreateAnimationInstance<CAnimationSequence2DInstance>();
	m_VerticalLightSprite->CreateAnimationInstance<CAnimationSequence2DInstance>();

	m_Sprite->SetWorldScale(100.f, 100.f, 1.f);
	m_Sprite->SetRelativePos(500.f, 300.f, 0.f);
	m_Sprite->SetPivot(0.5f, 0.5f, 0.f);

	m_HelixSprite->SetPivot(0.5f, 0.5f, 0.f);
	m_ButterflySprite->SetPivot(0.5f, 0.5f, 0.f);
	m_VerticalLightSprite->SetPivot(0.5f, 0.5f, 0.f);
	m_LampBrightImage->SetPivot(0.5f, 0.5f, 0.f);

	m_DragCollider->SetWorldScale(40.f, 40.f, 1.f);



	//Anim->AddAnimation(TEXT("LobbySmallLamp.sqc"), ANIMATION_PATH, "LobbySmallLamp", true, 1.3f);

	//Anim = m_HelixSprite->GetAnimationInstance();

	CAnimationSequence2DInstance* Anim = m_HelixSprite->GetAnimationInstance();

	Anim->AddAnimation(TEXT("LampBigHelix.sqc"), ANIMATION_PATH, "LampBigHelix", true, 1.4f);

	Anim = m_ButterflySprite->GetAnimationInstance();

	Anim->AddAnimation(TEXT("Library2ndButterfly.sqc"), ANIMATION_PATH, "Library2ndButterfly", true, 1.f);

	Anim = m_VerticalLightSprite->GetAnimationInstance();

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

	m_Sprite = (CSpriteComponent*)FindComponent("LobbySmallLampSprite");

	m_Sprite->SetTransparency(true);


	m_HelixSprite = (CSpriteComponent*)FindComponent("LobbySmallLampHelixSprite");

	m_HelixSprite->SetTransparency(true);

	m_HelixSprite->GetCurrentAnimation()->SetPlayTime(1.3f);


	m_ButterflySprite = (CSpriteComponent*)FindComponent("LobbySmallLampButterflySprite");

	m_ButterflySprite->SetTransparency(true);

	m_ButterflySprite->GetCurrentAnimation()->SetPlayTime(1.f);


	m_VerticalLightSprite = (CSpriteComponent*)FindComponent("VerticalLightSprite");

	m_VerticalLightSprite->SetTransparency(true);

	m_VerticalLightSprite->GetCurrentAnimation()->SetPlayTime(1.f);
}

