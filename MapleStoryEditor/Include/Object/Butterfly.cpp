
#include "Butterfly.h"
#include "Animation/AnimationSequence2DInstance.h"


CButterfly::CButterfly() :
	m_Sprite(nullptr)
{
	SetTypeID<CButterfly>();
}

CButterfly::CButterfly(const CButterfly& obj) :
	CGameObject(obj)
{
	m_Sprite = (CSpriteComponent*)FindComponent("ButterflySprite");
}

CButterfly::~CButterfly()
{
}

void CButterfly::Start()
{
	CGameObject::Start();
}

bool CButterfly::Init()
{
	m_Sprite = CreateComponent<CSpriteComponent>("ButterflySprite");


	SetRootComponent(m_Sprite);

	m_Sprite->SetTransparency(true);

	m_Sprite->CreateAnimationInstance<CAnimationSequence2DInstance>();

	m_Sprite->SetWorldScale(100.f, 100.f, 1.f);
	m_Sprite->SetRelativePos(500.f, 300.f, 0.f);
	m_Sprite->SetPivot(0.5f, 0.5f, 0.f);


	CAnimationSequence2DInstance* Anim = m_Sprite->GetAnimationInstance();

	Anim->AddAnimation(TEXT("Butterfly.sqc"), ANIMATION_PATH, "Butterfly", true, 0.6f);


	return true;
}

void CButterfly::Update(float DeltaTime)
{
	CGameObject::Update(DeltaTime);
}

void CButterfly::PostUpdate(float DeltaTime)
{
	CGameObject::PostUpdate(DeltaTime);
}

CButterfly* CButterfly::Clone()
{
	return new CButterfly(*this);
}

void CButterfly::Save(FILE* File)
{
	CGameObject::Save(File);
}

void CButterfly::Load(FILE* File)
{
	CGameObject::Load(File);

	m_Sprite = (CSpriteComponent*)FindComponent("ButterflySprite");
}

