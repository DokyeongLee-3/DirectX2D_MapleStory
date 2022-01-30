
#include "DoubleHelixBlinkTree.h"
#include "Animation/AnimationSequence2DInstance.h"


CDoubleHelixBlinkTree::CDoubleHelixBlinkTree() :
	m_Sprite(nullptr)
{
	SetTypeID<CDoubleHelixBlinkTree>();
}

CDoubleHelixBlinkTree::CDoubleHelixBlinkTree(const CDoubleHelixBlinkTree& obj) :
	CGameObject(obj)
{
	m_Sprite = (CSpriteComponent*)FindComponent("DoubleHelixBlinkTreeSprite");
}

CDoubleHelixBlinkTree::~CDoubleHelixBlinkTree()
{
}

void CDoubleHelixBlinkTree::Start()
{
	CGameObject::Start();
}

bool CDoubleHelixBlinkTree::Init()
{
	m_Sprite = CreateComponent<CSpriteComponent>("DoubleHelixBlinkTreeSprite");


	SetRootComponent(m_Sprite);

	m_Sprite->SetTransparency(true);

	m_Sprite->CreateAnimationInstance<CAnimationSequence2DInstance>();

	m_Sprite->SetWorldScale(100.f, 100.f, 1.f);
	m_Sprite->SetRelativePos(500.f, 300.f, 0.f);
	m_Sprite->SetPivot(0.5f, 0.5f, 0.f);


	CAnimationSequence2DInstance* Anim = m_Sprite->GetAnimationInstance();

	Anim->AddAnimation(TEXT("DoubleHelixBlinkTree.sqc"), ANIMATION_PATH, "DoubleHelixBlinkTree", true, 1.5f);


	return true;
}

void CDoubleHelixBlinkTree::Update(float DeltaTime)
{
	CGameObject::Update(DeltaTime);
}

void CDoubleHelixBlinkTree::PostUpdate(float DeltaTime)
{
	CGameObject::PostUpdate(DeltaTime);
}

CDoubleHelixBlinkTree* CDoubleHelixBlinkTree::Clone()
{
	return new CDoubleHelixBlinkTree(*this);
}

void CDoubleHelixBlinkTree::Save(FILE* File)
{
	CGameObject::Save(File);
}

void CDoubleHelixBlinkTree::Load(FILE* File)
{
	CGameObject::Load(File);

	m_Sprite = (CSpriteComponent*)FindComponent("DoubleHelixBlinkTreeSprite");

	m_Sprite->GetAnimationInstance()->FindAnimation("DoubleHelixBlinkTree")->SetPlayTime(2.f);
}

