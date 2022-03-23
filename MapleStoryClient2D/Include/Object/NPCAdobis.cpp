
#include "NPCAdobis.h"
#include "Animation/AnimationSequence2DInstance.h"
#include "../Widget/NPCName.h"
#include "../Widget/AdobisDialog.h"

CNPCAdobis::CNPCAdobis()
{
	SetTypeID<CNPCAdobis>();
}

CNPCAdobis::CNPCAdobis(const CNPCAdobis& obj)	:
	CGameObject(obj)
{
	m_Sprite = (CSpriteComponent*)FindComponent("NPCSprite");
	m_Body = (CColliderBox2D*)FindComponent("Body");
}

CNPCAdobis::~CNPCAdobis()
{
}

void CNPCAdobis::Start()
{
	CGameObject::Start();

	m_NameWidget = CreateComponent<CWidgetComponent>("NameWidget");
	m_NameWidget->SetFadeApply(true);
	m_NameWidget->UseAlphaBlend(true);

	CNPCName* NPCName = m_NameWidget->CreateWidgetWindow<CNPCName>("NPCName");
	NPCName->SetNPCName("아도비스");
	


	m_Sprite->AddChild(m_NameWidget);
	m_NameWidget->SetRelativePos(-18.f, -47.f, 0.f);
	m_NameWidget->SetLayerName("ScreenWidgetComponent");

	m_Dialog = m_Scene->GetViewport()->CreateWidgetWindow<CAdobisDialog>("AdobisDialog");
	m_Dialog->Enable(false);
}

bool CNPCAdobis::Init()
{
	m_Sprite = CreateComponent<CSpriteComponent>("NPCSprite");
	m_Body = CreateComponent<CColliderBox2D>("Body");

	SetRootComponent(m_Sprite);
	m_Sprite->AddChild(m_Body);

	SetPivot(0.5f, 0.5f, 0.f);

	m_Body->SetExtent(25.f, 35.f);

	m_Sprite->CreateAnimationInstance<CAnimationSequence2DInstance>();
	CAnimationSequence2DInstance* Instance = m_Sprite->GetAnimationInstance();

	Instance->AddAnimation(TEXT("NPCAdobisIdle.sqc"), ANIMATION_PATH, "NPCAdobisIdle", true, 0.6f);

	return true;
}

void CNPCAdobis::Update(float DeltaTime)
{
	CGameObject::Update(DeltaTime);
}

void CNPCAdobis::PostUpdate(float DeltaTime)
{
	CGameObject::PostUpdate(DeltaTime);
}

CNPCAdobis* CNPCAdobis::Clone()
{
	return new CNPCAdobis(*this);
}

void CNPCAdobis::CollisionMouseBeginCallback(const CollisionResult& Result)
{
}

void CNPCAdobis::Save(FILE* File)
{
	CGameObject::Save(File);
}

void CNPCAdobis::Load(FILE* File)
{
	CGameObject::Load(File);

	m_Sprite = (CSpriteComponent*)FindComponent("NPCSprite");

	m_Body = (CColliderBox2D*)FindComponent("Body");

	m_Sprite->GetCurrentAnimation()->SetPlayTime(1.2f);
}

void CNPCAdobis::CreateAdobisDialog()
{
	m_Dialog->Enable(true);
}
