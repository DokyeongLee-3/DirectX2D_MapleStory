
#include "BlinkTree.h"
#include "Animation/AnimationSequence2DInstance.h"
#include "Input.h"


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
	m_DragCollider = CreateComponent<CDragCollider>("DragCollider");

	m_DragCollider->SetCollisionProfile("DragCollider"); 

	SetRootComponent(m_Sprite);
	m_Sprite->AddChild(m_DragCollider);

	m_Sprite->SetTransparency(true);

	m_Sprite->CreateAnimationInstance<CAnimationSequence2DInstance>();

	m_Sprite->SetWorldScale(100.f, 100.f, 1.f);
	m_Sprite->SetRelativePos(500.f, 300.f, 0.f);
	m_Sprite->SetPivot(0.5f, 0.5f, 0.f);

	//m_Sprite->SetLayerName("MapObjBack");

	CAnimationSequence2DInstance* Anim = m_Sprite->GetAnimationInstance();

	Anim->AddAnimation(TEXT("BlinkTree.sqc"), ANIMATION_PATH, "BlinkTree", true, 0.6f);

	CInput::GetInst()->CreateKey("Flip", 'F');
	CInput::GetInst()->SetKeyCallback<CBlinkTree>("Flip", KeyState_Down, this, &CBlinkTree::FlipAll);

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

void CBlinkTree::FlipAll(float DeltaTime)
{
	m_Sprite->Flip();
}


