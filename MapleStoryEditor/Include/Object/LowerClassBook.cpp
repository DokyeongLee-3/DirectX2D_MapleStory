
#include "LowerClassBook.h"
#include "Animation/AnimationSequence2DInstance.h"
#include "Input.h"

CLowerClassBook::CLowerClassBook() :
	m_Sprite(nullptr),
	m_Body(nullptr)
{
	SetTypeID<CLowerClassBook>();
}

CLowerClassBook::CLowerClassBook(const CLowerClassBook& obj) :
	CGameObject(obj)
{
	m_Sprite = (CSpriteComponent*)FindComponent("LowerClassBookSprite");
	m_Body = (CColliderBox2D*)FindComponent("Body");
}

CLowerClassBook::~CLowerClassBook()
{
}

void CLowerClassBook::Start()
{
	CGameObject::Start();
}

bool CLowerClassBook::Init()
{
	m_Sprite = CreateComponent<CSpriteComponent>("LowerClassBookSprite");
	m_Body = CreateComponent<CColliderBox2D>("Body");

	m_Body->SetCollisionProfile("Monster");

	SetRootComponent(m_Sprite);
	m_Sprite->AddChild(m_Body);

	m_Sprite->SetTransparency(true);

	m_Sprite->CreateAnimationInstance<CAnimationSequence2DInstance>();

	m_Sprite->SetWorldScale(100.f, 100.f, 1.f);
	m_Sprite->SetRelativePos(500.f, 300.f, 0.f);
	m_Sprite->SetPivot(0.5f, 0.5f, 0.f);




	CAnimationSequence2DInstance* Anim = m_Sprite->GetAnimationInstance();

	// ÇÏ±Þ ¸¶¹ýÃ¥Àº IdleÀÌ¶û Move¶û °°Àº ½ÃÄö½º
	Anim->AddAnimation(TEXT("LowerClassBookIdleLeft.sqc"), ANIMATION_PATH, "LowerClassBookIdleLeft", true, 1.f);
	Anim->AddAnimation(TEXT("LowerClassBookIdleLeft.sqc"), ANIMATION_PATH, "LowerClassBookMoveLeft", true, 1.f);
	Anim->AddAnimation(TEXT("LowerClassBookHitLeft.sqc"), ANIMATION_PATH, "LowerClassBookHitLeft", true, 1.f);
	Anim->AddAnimation(TEXT("LowerClassBookDieLeft.sqc"), ANIMATION_PATH, "LowerClassBookDieLeft", true, 1.f);
	Anim->AddAnimation(TEXT("LowerClassBookAttackLeft.sqc"), ANIMATION_PATH, "LowerClassBookAttackLeft", true, 1.f);

	Anim->AddAnimation(TEXT("LowerClassBookAttackOrb.sqc"), ANIMATION_PATH, "LowerClassBookAttackOrb", true, 1.f);

	CInput::GetInst()->CreateKey("Flip", 'F');
	CInput::GetInst()->SetKeyCallback<CLowerClassBook>("Flip", KeyState_Down, this, &CLowerClassBook::FlipAll);
	CInput::GetInst()->CreateKey("NextAnimation", 'N');
	CInput::GetInst()->SetKeyCallback<CLowerClassBook>("NextAnimation", KeyState_Down, this, &CLowerClassBook::NextAnimation);

	return true;
}

void CLowerClassBook::Update(float DeltaTime)
{
	CGameObject::Update(DeltaTime);
}

void CLowerClassBook::PostUpdate(float DeltaTime)
{
	CGameObject::PostUpdate(DeltaTime);
}

CLowerClassBook* CLowerClassBook::Clone()
{
	return new CLowerClassBook(*this);
}

void CLowerClassBook::Save(FILE* File)
{
	CGameObject::Save(File);
}

void CLowerClassBook::Load(FILE* File)
{
	CGameObject::Load(File);

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