
#include "NPCAmon.h"
#include "Animation/AnimationSequence2DInstance.h"
#include "../Widget/NPCName.h"
#include "../Scene/ZakumAltarScene.h"
#include "Player2D.h"
#include "Scene/SceneManager.h"
#include "Render/RenderManager.h"
#include "../Widget/AmonDialog.h"

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
}

void CNPCAmon::Start()
{
	CGameObject::Start();

	m_NameWidget = CreateComponent<CWidgetComponent>("NameWidget");
	m_NameWidget->SetFadeApply(true);
	m_NameWidget->UseAlphaBlend(true);

	CNPCName* NPCName = m_NameWidget->CreateWidgetWindow<CNPCName>("NPCName");
	NPCName->SetNPCName("¾Æ¸ó");

	m_Sprite->AddChild(m_NameWidget);
	m_NameWidget->SetRelativePos(-18.f, -47.f, 0.f);
	m_NameWidget->SetLayerName("ScreenWidgetComponent");

	m_Dialog = m_Scene->GetViewport()->CreateWidgetWindow<CAmonDialog>("AmonDialog");
	m_Dialog->Enable(false);
	m_Dialog->SetNPCAmon(this);
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

void CNPCAmon::Save(FILE* File)
{
	CGameObject::Save(File);
}


void CNPCAmon::Load(FILE* File)
{
	CGameObject::Load(File);

	m_Sprite = (CSpriteComponent*)FindComponent("NPCSprite");

	m_Body = (CColliderBox2D*)FindComponent("Body");

	m_Sprite->GetCurrentAnimation()->SetPlayTime(1.5f);
}

CNPCAmon* CNPCAmon::Clone()
{
	return new CNPCAmon(*this);
}

void CNPCAmon::CreateAmonDialog()
{
	m_Dialog->Enable(true);
}

void CNPCAmon::GoToWayToZakumScene()
{
	CSceneMode* Mode = m_Scene->GetSceneMode();
	
	if (Mode->GetTypeID() == typeid(CZakumAltarScene).hash_code())
	{
		CZakumAltarScene* AltarScene = (CZakumAltarScene*)Mode;

		CPlayer2D* Player = (CPlayer2D*)m_Scene->GetPlayerObject();

		Player->ClearListCollision();

		CRenderManager::GetInst()->SetStartFadeIn(true);
		CSceneManager::GetInst()->SetFadeInEndCallback<CZakumAltarScene>(AltarScene, &CZakumAltarScene::CreateWayToZakumScene);
	}
}
