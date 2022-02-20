
#include "WayToZakumScene.h"
#include "Scene/Scene.h"
#include "Scene/SceneResource.h"
#include "Scene/SceneManager.h"
#include "Input.h"
#include "PathManager.h"
#include "../ClientManager.h"
#include "Resource/Shader/StructuredBuffer.h"
#include "../Object/FlowingVolcano.h"
#include "../Object/Player2D.h"

CWayToZakumScene::CWayToZakumScene()
{
	SetTypeID<CWayToZakumScene>();
}

CWayToZakumScene::~CWayToZakumScene()
{
	CPlayer2D* Player = (CPlayer2D*)(m_Scene->GetPlayerObject());

	if (Player)
	{
		Player->SetVoidPressure(nullptr);
		Player->SetVoidPressureOrb(nullptr);
	}
}

void CWayToZakumScene::SetStageObject(CStage* Stage)
{
	m_StageObject = Stage;
}

bool CWayToZakumScene::Init()
{
	CCharacterStatusWindow* StatusWindow = CClientManager::GetInst()->GetCharacterStatusWindow();
	CInventory* Inventory = CClientManager::GetInst()->GetInventoryWindow();
	CSkillQuickSlotWindow* SkillQuickSlot = CClientManager::GetInst()->GetSkillQuickSlotWindow();
	CConfigurationWindow* ConfigurationWindow = CClientManager::GetInst()->GetConfigurationWindow();
	CCharacterEXP* EXPWindow = CClientManager::GetInst()->GetCharacterEXPWindow();
	CBossMatching* BossMatching = CClientManager::GetInst()->GetBossMatchingWindow();
	m_Scene->GetViewport()->AddWindow(StatusWindow);
	m_Scene->GetViewport()->AddWindow(Inventory);
	m_Scene->GetViewport()->AddWindow(SkillQuickSlot);
	m_Scene->GetViewport()->AddWindow(ConfigurationWindow);
	m_Scene->GetViewport()->AddWindow(EXPWindow);
	m_Scene->GetViewport()->AddWindow(BossMatching);

	// 보스맵 들어와서도 보스매칭 UI가 켜져있으면 꺼준다
	if (BossMatching->IsEnable())
		BossMatching->Enable(false);

	CreateAnimationSequence();
	CreateMaterial();

	// 자쿰으로 통하는 길 Scene 만들어놓은 scn파일까지의 경로
	const PathInfo* Info = CPathManager::GetInst()->FindPath(SCENE_PATH);

	char	FullPath[MAX_PATH] = {};

	if (Info)
		strcpy_s(FullPath, Info->PathMultibyte);

	strcat_s(FullPath, "WayToZakum.scn");

	m_Scene->LoadFullPath(FullPath);


	m_Scene->GetResource()->LoadSound("BGM", true, "WayToZakumBGM", "HellGate.mp3");
	m_Scene->GetResource()->SoundPlay("WayToZakumBGM");


	// 맵 하단에 왼쪽으로 흐르는 용암
	CFlowingVolcano* Volcano = m_Scene->CreateGameObject<CFlowingVolcano>("FlowingVolcano");

	return true;
}

void CWayToZakumScene::Update(float DeltaTime)
{
	CSceneMode::Update(DeltaTime);
}

void CWayToZakumScene::CreateMaterial()
{
	m_Scene->GetResource()->CreateMaterial<CMaterial>("Volcano");
	CMaterial* Material = m_Scene->GetResource()->FindMaterial("Volcano");

	Material->AddTexture(0, (int)Buffer_Shader_Type::Pixel, "Volcano", TEXT("Map/Zakum/FlowingVolcano.png"));

	Material->SetShader("MovingTileShader");
	Material->SetRenderState("AlphaBlend");
}

void CWayToZakumScene::CreateAnimationSequence()
{
	CreatePlayerAnimationSequence();
	CreateSkillAnimationSequence();
	CreateMapAnimationSequence();
}

void CWayToZakumScene::CreatePlayerAnimationSequence()
{
	m_Scene->GetResource()->LoadSequence2D("PlayerIdleLeft.sqc");
	m_Scene->GetResource()->LoadSequence2D("PlayerWalkLeft.sqc");
	m_Scene->GetResource()->LoadSequence2D("PlayerHealLeft.sqc");
	m_Scene->GetResource()->LoadSequence2D("PlayerRope.sqc");
	m_Scene->GetResource()->LoadSequence2D("PlayerJumpLeft.sqc");

}

void CWayToZakumScene::CreateSkillAnimationSequence()
{
	m_Scene->GetResource()->LoadSequence2D("PlayerOrb.sqc");

	m_Scene->GetResource()->LoadSequence2D("SylphideLancerBodyEffectLeft.sqc");
	m_Scene->GetResource()->LoadSequence2D("SylphideLancerArrowLeft.sqc");
	m_Scene->GetResource()->LoadSequence2D("SylphideLancerArrowPurple.sqc");
	m_Scene->GetResource()->LoadSequence2D("SylphideLancerHit.sqc");
}

void CWayToZakumScene::CreateMapAnimationSequence()
{

}
