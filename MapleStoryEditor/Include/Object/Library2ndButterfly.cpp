
#include "Library2ndButterfly.h"
#include "Animation/AnimationSequence2DInstance.h"
#include "Input.h"


CLibrary2ndButterfly::CLibrary2ndButterfly() :
	m_Sprite(nullptr)
{
	SetTypeID<CLibrary2ndButterfly>();
}

CLibrary2ndButterfly::CLibrary2ndButterfly(const CLibrary2ndButterfly& obj) :
	CGameObject(obj)
{
	m_Sprite = (CSpriteComponent*)FindComponent("Library2ndButterflySprite");
}

CLibrary2ndButterfly::~CLibrary2ndButterfly()
{
	CGameObject::Start();
}

void CLibrary2ndButterfly::Start()
{
}

bool CLibrary2ndButterfly::Init()
{
	m_Sprite = CreateComponent<CSpriteComponent>("Library2ndButterflySprite");


	SetRootComponent(m_Sprite);

	m_Sprite->SetTransparency(true);

	m_Sprite->CreateAnimationInstance<CAnimationSequence2DInstance>();

	m_Sprite->SetWorldScale(100.f, 100.f, 1.f);
	m_Sprite->SetRelativePos(500.f, 300.f, 0.f);
	m_Sprite->SetPivot(0.5f, 0.5f, 0.f);


	CAnimationSequence2DInstance* Anim = m_Sprite->GetAnimationInstance();

	Anim->AddAnimation(TEXT("Library2ndButterfly.sqc"), ANIMATION_PATH, "Library2ndButterfly", true, 1.f);

	CInput::GetInst()->CreateKey("Flip", 'F');
	CInput::GetInst()->SetKeyCallback<CLibrary2ndButterfly>("Flip", KeyState_Down, this, &CLibrary2ndButterfly::FlipAll);

	return true;
}

void CLibrary2ndButterfly::Update(float DeltaTime)
{
	CGameObject::Update(DeltaTime);
}

void CLibrary2ndButterfly::PostUpdate(float DeltaTime)
{
	CGameObject::PostUpdate(DeltaTime);
}

CLibrary2ndButterfly* CLibrary2ndButterfly::Clone()
{
	return new CLibrary2ndButterfly(*this);
}

void CLibrary2ndButterfly::Save(FILE* File)
{
	CGameObject::Save(File);
}

void CLibrary2ndButterfly::Load(FILE* File)
{
	CGameObject::Load(File);

	m_Sprite = (CSpriteComponent*)FindComponent("Library2ndButterflySprite");
}

void CLibrary2ndButterfly::FlipAll(float DeltaTime)
{
	m_Sprite->Flip();
}


