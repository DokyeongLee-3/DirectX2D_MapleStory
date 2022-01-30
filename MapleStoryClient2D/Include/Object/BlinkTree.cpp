
#include "BlinkTree.h"
#include "Animation/AnimationSequence2DInstance.h"


CBlinkTree::CBlinkTree() :
	m_Sprite(nullptr)
{
	SetTypeID<CBlinkTree>();
}

CBlinkTree::CBlinkTree(const CBlinkTree& obj) :
	CGameObject(obj)
{
	m_Sprite = (CSpriteComponent*)FindComponent("BlinkTreeSprite");
}

CBlinkTree::~CBlinkTree()
{
}

void CBlinkTree::Start()
{
	CGameObject::Start();
}

bool CBlinkTree::Init()
{
	m_Sprite = CreateComponent<CSpriteComponent>("BlinkTreeSprite");

	SetRootComponent(m_Sprite);

	m_Sprite->SetTransparency(true);

	m_Sprite->CreateAnimationInstance<CAnimationSequence2DInstance>();

	m_Sprite->SetWorldScale(100.f, 100.f, 1.f);
	m_Sprite->SetRelativePos(500.f, 300.f, 0.f);
	m_Sprite->SetPivot(0.5f, 0.5f, 0.f);

	//m_Sprite->SetLayerName("MapObjBack");

	CAnimationSequence2DInstance* Anim = m_Sprite->GetAnimationInstance();

	Anim->AddAnimation(TEXT("BlinkTree.sqc"), ANIMATION_PATH, "BlinkTree", true, 0.6f);

	return true;
}

void CBlinkTree::Update(float DeltaTime)
{
	CGameObject::Update(DeltaTime);
}

void CBlinkTree::PostUpdate(float DeltaTime)
{
	CGameObject::PostUpdate(DeltaTime);
}

CBlinkTree* CBlinkTree::Clone()
{
	return new CBlinkTree(*this);
}

void CBlinkTree::Save(FILE* File)
{
	CGameObject::Save(File);
}

void CBlinkTree::Load(FILE* File)
{
	CGameObject::Load(File);

	m_Sprite = (CSpriteComponent*)FindComponent("BlinkTreeSprite");
}


