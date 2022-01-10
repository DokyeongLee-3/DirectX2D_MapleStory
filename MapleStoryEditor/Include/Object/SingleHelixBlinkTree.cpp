
#include "SingleHelixBlinkTree.h"
#include "Animation/AnimationSequence2DInstance.h"


CSingleHelixBlinkTree::CSingleHelixBlinkTree() :
	m_Sprite(nullptr)
{
	SetTypeID<CSingleHelixBlinkTree>();
}

CSingleHelixBlinkTree::CSingleHelixBlinkTree(const CSingleHelixBlinkTree& obj) :
	CGameObject(obj)
{
	m_Sprite = (CSpriteComponent*)FindComponent("SingleHelixBlinkTreeSprite");
}

CSingleHelixBlinkTree::~CSingleHelixBlinkTree()
{
}

void CSingleHelixBlinkTree::Start()
{
	CGameObject::Start();
}

bool CSingleHelixBlinkTree::Init()
{
	m_Sprite = CreateComponent<CSpriteComponent>("SingleHelixBlinkTreeSprite");


	SetRootComponent(m_Sprite);

	m_Sprite->SetTransparency(true);

	m_Sprite->CreateAnimationInstance<CAnimationSequence2DInstance>();

	m_Sprite->SetWorldScale(100.f, 100.f, 1.f);
	m_Sprite->SetRelativePos(500.f, 300.f, 0.f);
	m_Sprite->SetPivot(0.5f, 0.5f, 0.f);


	CAnimationSequence2DInstance* Anim = m_Sprite->GetAnimationInstance();

	Anim->AddAnimation(TEXT("SingleHelixBlinkTree.sqc"), ANIMATION_PATH, "SingleHelixBlinkTree", true, 0.6f);


	return true;
}

void CSingleHelixBlinkTree::Update(float DeltaTime)
{
	CGameObject::Update(DeltaTime);
}

void CSingleHelixBlinkTree::PostUpdate(float DeltaTime)
{
	CGameObject::PostUpdate(DeltaTime);
}

CSingleHelixBlinkTree* CSingleHelixBlinkTree::Clone()
{
	return new CSingleHelixBlinkTree(*this);
}

void CSingleHelixBlinkTree::Save(FILE* File)
{
	CGameObject::Save(File);
}

void CSingleHelixBlinkTree::Load(FILE* File)
{
	CGameObject::Load(File);

	m_Sprite = (CSpriteComponent*)FindComponent("SingleHelixBlinkTreeSprite");
}

