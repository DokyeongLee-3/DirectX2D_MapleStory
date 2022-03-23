
#include "NPCAmon.h"
#include "Animation/AnimationSequence2DInstance.h"

CNPCAmon::CNPCAmon()
{
	SetTypeID<CNPCAmon>();
}

CNPCAmon::CNPCAmon(const CNPCAmon& obj)	:
	CGameObject(obj)
{
}

CNPCAmon::~CNPCAmon()
{
	m_Sprite = (CSpriteComponent*)FindComponent("NPCSprite");
	m_Body = (CColliderBox2D*)FindComponent("Body");
}

void CNPCAmon::Start()
{
	CGameObject::Start();

	//m_NameWidget = CreateComponent<CWidgetComponent>("NameWidget");
	//m_NameWidget->SetFadeApply(true);
	//m_NameWidget->UseAlphaBlend(true);

	//CNPCName* NPCName = m_NameWidget->CreateWidgetWindow<CNPCName>("NPCName");
	//NPCName->SetNPCName("¾Æ¸ó");

	//m_Sprite->AddChild(m_NameWidget);
	//m_NameWidget->SetRelativePos(-18.f, -47.f, 0.f);
	//m_NameWidget->SetLayerName("ScreenWidgetComponent");
}

bool CNPCAmon::Init()
{
	m_Sprite = CreateComponent<CSpriteComponent>("NPCSprite");

	SetRootComponent(m_Sprite);

	SetPivot(0.5f, 0.5f, 0.f);


	m_Sprite->CreateAnimationInstance<CAnimationSequence2DInstance>();
	CAnimationSequence2DInstance* Instance = m_Sprite->GetAnimationInstance();

	Instance->AddAnimation(TEXT("NPCAmonIdle.sqc"), ANIMATION_PATH, "NPCAmonIdle", true, 1.5f);


	return true;
}

void CNPCAmon::Update(float DeltaTime)
{
	CGameObject::Update(DeltaTime);
}

void CNPCAmon::PostUpdate(float DeltaTime)
{
	CGameObject::PostUpdate(DeltaTime);
}

CNPCAmon* CNPCAmon::Clone()
{
	return new CNPCAmon(*this);
}


void CNPCAmon::Save(FILE* File)
{
	CGameObject::Save(File);
}

void CNPCAmon::Load(FILE* File)
{
	CGameObject::Load(File);
}


