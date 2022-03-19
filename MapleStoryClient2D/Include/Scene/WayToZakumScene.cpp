
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
#include "LoadingThread.h"
#include "Render/RenderManager.h"
#include "ZakumAltarScene.h"

CWayToZakumScene::CWayToZakumScene()
{
	SetTypeID<CWayToZakumScene>();
}

CWayToZakumScene::~CWayToZakumScene()
{
	SAFE_DELETE(m_LoadingThread);

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

void CWayToZakumScene::Start()
{
	CSceneMode::Start();

	if (m_PlayerObject)
	{
		CWidgetWindow* Window = ((CPlayer2D*)m_PlayerObject.Get())->GetDamageWidgetComponent()->GetWidgetWindow();

		Window->SetViewport(m_Scene->GetViewport());

		Window->GetViewport()->SetScene(m_Scene);
	}
}

bool CWayToZakumScene::Init()
{
	CCharacterStatusWindow* StatusWindow = CClientManager::GetInst()->GetCharacterStatusWindow();
	CInventory* Inventory = CClientManager::GetInst()->GetInventoryWindow();
	CSkillQuickSlotWindow* SkillQuickSlot = CClientManager::GetInst()->GetSkillQuickSlotWindow();
	CConfigurationWindow* ConfigurationWindow = CClientManager::GetInst()->GetConfigurationWindow();
	CCharacterEXP* EXPWindow = CClientManager::GetInst()->GetCharacterEXPWindow();
	CBossMatching* BossMatching = CClientManager::GetInst()->GetBossMatchingWindow();
	CStatWindow* StatWindow = CClientManager::GetInst()->GetStatWindow();
	CDyingNoticeWindow* DyingNoticeWindow = CClientManager::GetInst()->GetDyingNoticeWindow();

	m_Scene->GetViewport()->AddWindow(StatusWindow);
	m_Scene->GetViewport()->AddWindow(Inventory);
	m_Scene->GetViewport()->AddWindow(SkillQuickSlot);
	m_Scene->GetViewport()->AddWindow(ConfigurationWindow);
	m_Scene->GetViewport()->AddWindow(EXPWindow);
	m_Scene->GetViewport()->AddWindow(BossMatching);
	m_Scene->GetViewport()->AddWindow(StatWindow);
	m_Scene->GetViewport()->AddWindow(DyingNoticeWindow);

	if (BossMatching->IsEnable())
		BossMatching->Enable(false);


	CreateAnimationSequence();
	CreateMaterial();
	LoadSound();

	// 자쿰으로 통하는 길 Scene 만들어놓은 scn파일까지의 경로
	const PathInfo* Info = CPathManager::GetInst()->FindPath(SCENE_PATH);

	char	FullPath[MAX_PATH] = {};

	if (Info)
		strcpy_s(FullPath, Info->PathMultibyte);

	strcat_s(FullPath, "WayToZakum.scn");

	m_Scene->LoadFullPath(FullPath);

	m_Scene->GetResource()->SoundPlay("WayToZakumBGM");


	// 맵 하단에 왼쪽으로 흐르는 용암
	CFlowingVolcano* Volcano = m_Scene->CreateGameObject<CFlowingVolcano>("FlowingVolcano");

	if (m_PlayerObject)
	{
		m_PlayerObject->SetGravity(true);
		m_PlayerObject->SetTileCollisionEnable(false);
	}

	return true;
}

void CWayToZakumScene::Update(float DeltaTime)
{
	CSceneMode::Update(DeltaTime);

	if (m_LoadingThread)
	{
		CThreadQueue<LoadingMessage>* Queue = m_LoadingThread->GetLoadingQueue();

		if (!m_LoadingThread)
		{
			CSceneManager::GetInst()->ChangeNextScene();
			CRenderManager::GetInst()->SetStartFadeOut(true);
			((CPlayer2D*)m_Scene->GetPlayerObject())->GetPlayerBody()->Enable(true);
			return;
		}

		else if (!Queue->empty())
		{
			LoadingMessage	Msg = Queue->front();

			Queue->pop();

			//m_LoadingWidget->SetLoadingPercent(Msg.Percent);

			if (Msg.Complete)
			{
				CSceneManager::GetInst()->ChangeNextScene();
				CRenderManager::GetInst()->SetStartFadeOut(true);
				((CPlayer2D*)m_Scene->GetPlayerObject())->GetPlayerBody()->Enable(true);
			}
		}
	}
}

void CWayToZakumScene::PostUpdate(float DeltaTime)
{
	CSceneMode::PostUpdate(DeltaTime);
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
	m_Scene->GetResource()->LoadSequence2D("PlayerLevelUpEffect.sqc");
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
	m_Scene->GetResource()->LoadSequence2D("NPCAdobisIdle.sqc");
}

void CWayToZakumScene::LoadSound()
{
	m_Scene->GetResource()->LoadSound("BGM", true, "WayToZakumBGM", "HellGate.mp3");

	m_Scene->GetResource()->LoadSound("Effect", false, "Jump", "Jump.mp3");

	m_Scene->GetResource()->LoadSound("Effect", false, "SylphideLancerUse", "SylphideLancerUse.mp3");
	m_Scene->GetResource()->LoadSound("Effect", false, "SylphideLancerHit", "SylphideLancerHit.mp3");

	m_Scene->GetResource()->LoadSound("Effect", false, "VoidPressureUse", "VoidPressureUse.mp3");
	m_Scene->GetResource()->LoadSound("Effect", false, "VoidPressureLoop", "VoidPressureLoop.mp3");
	m_Scene->GetResource()->LoadSound("Effect", false, "VoidPressureEnd", "VoidPressureEnd.mp3");
	m_Scene->GetResource()->LoadSound("Effect", false, "VoidPressureHit", "VoidPressureHit.mp3");

	m_Scene->GetResource()->LoadSound("Effect", false, "LightTransforming", "LightTransformingUse.mp3");

	m_Scene->GetResource()->LoadSound("Effect", false, "DeathSideHit", "DeathSideHit.mp3");
	m_Scene->GetResource()->LoadSound("Effect", false, "DeathSideUse", "DeathSideUse.mp3");

	m_Scene->GetResource()->LoadSound("Effect", false, "LevelUp", "LevelUp.mp3");

	m_Scene->GetResource()->LoadSound("UI", false, "TabClick", "TabClick.mp3");
}

void CWayToZakumScene::SetPerspective()
{
	size_t Count = m_Scene->GetObjectCount();

	for (size_t i = 0; i < Count; ++i)
	{
		CGameObject* Object = m_Scene->GetGameObject((int)i);
		CPlayer2D* Player = (CPlayer2D*)m_Scene->GetPlayerObject();
		Vector2 CameraMove = Player->GetCurrentFrameCameraMove();

		std::string LayerName = Object->GetRootComponent()->GetLayerName();

		if (LayerName.find("Stage") != std::string::npos)
			Object->AddWorldPos(CameraMove.x / 2.f, 0.f, 0.f);
		else if (LayerName.find("MapObjBackMost") != std::string::npos)
			Object->AddWorldPos(CameraMove.x / 6.f, 0.f, 0.f);
		else if (LayerName.find("MapObjBack") != std::string::npos)
			Object->AddWorldPos(CameraMove.x / 13.f, 0.f, 0.f);
		else if (LayerName.find("MapObjMiddle") != std::string::npos)
			Object->AddWorldPos(CameraMove.x / 18.f, 0.f, 0.f);
		else if (LayerName.find("MapObjFront") != std::string::npos)
			Object->AddWorldPos(CameraMove.x / 28.f, 0.f, 0.f);
	}

}

void CWayToZakumScene::CreateZakumAltarScene()
{
	m_Scene->GetResource()->SoundStop("WayToZakumBGM");

	CSceneManager::GetInst()->CreateNextScene(false);
	CZakumAltarScene* ZakumAltarScene = CSceneManager::GetInst()->CreateSceneModeEmpty<CZakumAltarScene>(false);

	Vector3 PlayerPos = m_PlayerObject->GetWorldPos();

	ZakumAltarScene->SetPlayerObject(m_PlayerObject);

	// 다음 Scene에서의 위치를 Scene의 왼쪽에 위치하도록 잡아주기
	m_PlayerObject->SetWorldPos(920.f, 220.f, PlayerPos.z);

	m_LoadingThread = CThread::CreateThread<CLoadingThread>("ZakumAltarSceneLoadingThread");
	m_LoadingThread->SetLoadingScene(ThreadLoadingScene::ZakumAltar);

	m_LoadingThread->Start();
}
