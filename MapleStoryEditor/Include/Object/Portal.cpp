
#include "Portal.h"
#include "Animation/AnimationSequence2DInstance.h"
#include "Input.h"
#include "../EditorManager.h"
#include "../Window/SpriteWindow.h"

CPortal::CPortal() :
	m_Sprite(nullptr),
	m_Body(nullptr)
{
	SetTypeID<CPortal>();
}

CPortal::CPortal(const CPortal& obj) :
	CGameObject(obj)
{
	m_Sprite = (CSpriteComponent*)FindComponent("PortalSprite");
	m_Body = (CColliderBox2D*)FindComponent("Body");
}

CPortal::~CPortal()
{
}

void CPortal::Start()
{
	CGameObject::Start();
}

bool CPortal::Init()
{
	m_Sprite = CreateComponent<CSpriteComponent>("PortalSprite");
	m_Body = CreateComponent<CColliderBox2D>("Body");

	m_Body->SetCollisionProfile("MapCollider");

	SetRootComponent(m_Sprite);
	m_Sprite->AddChild(m_Body);

	m_Sprite->SetTransparency(true);

	m_Sprite->CreateAnimationInstance<CAnimationSequence2DInstance>();

	m_Sprite->SetWorldScale(100.f, 100.f, 1.f);
	m_Sprite->SetRelativePos(500.f, 300.f, 0.f);
	m_Sprite->SetPivot(0.5f, 0.5f, 0.f);




	CAnimationSequence2DInstance* Anim = m_Sprite->GetAnimationInstance();

	Anim->AddAnimation(TEXT("Portal.sqc"), ANIMATION_PATH, "Portal", true, 0.6f);


	return true;
}

void CPortal::Update(float DeltaTime)
{
	CGameObject::Update(DeltaTime);
}

void CPortal::PostUpdate(float DeltaTime)
{
	CGameObject::PostUpdate(DeltaTime);
}

CPortal* CPortal::Clone()
{
	return new CPortal(*this);
}

void CPortal::Save(FILE* File)
{
	CGameObject::Save(File);
}

void CPortal::Load(FILE* File)
{
	CGameObject::Load(File);

	m_Sprite = (CSpriteComponent*)FindComponent("PlayerSkillSprite");
}

