
#include "LowerClassBook.h"
#include "Animation/AnimationSequence2DInstance.h"
#include "Input.h"
#include "../Animation/LowerClassBookAnimation.h"

CLowerClassBook::CLowerClassBook() :
	m_Sprite(nullptr),
	m_Body(nullptr)
{
	SetTypeID<CLowerClassBook>();
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
}

bool CLowerClassBook::Init()
{
	m_Sprite = CreateComponent<CSpriteComponent>("LowerClassBookSprite");
	m_Body = CreateComponent<CColliderBox2D>("Body");

	m_Body->SetCollisionProfile("Monster");

	SetRootComponent(m_Sprite);
	m_Sprite->AddChild(m_Body);

	m_Sprite->SetTransparency(true);

	m_Sprite->CreateAnimationInstance<CLowerClassBookAnimation>();

	m_Sprite->SetWorldScale(100.f, 100.f, 1.f);
	m_Sprite->SetRelativePos(500.f, 300.f, 0.f);
	m_Sprite->SetPivot(0.5f, 0.5f, 0.f);


	return true;
}

void CLowerClassBook::Update(float DeltaTime)
{
	CMonster::Update(DeltaTime);
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
}

void CLowerClassBook::FlipAll(float DeltaTime)
{
	m_Sprite->Flip();
}

void CLowerClassBook::NextAnimation(float DeltaTime)
{
	CAnimationSequence2DInstance* Anim = m_Sprite->GetAnimationInstance();

	Anim->NextAnimation();
}