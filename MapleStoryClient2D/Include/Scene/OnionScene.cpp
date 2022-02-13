
#include "OnionScene.h"
#include "WayToZakumScene.h"
#include "Scene/Scene.h"
#include "Scene/SceneResource.h"
#include "Scene/SceneManager.h"
#include "../Object/OnionMonster.h"
#include "../Object/SylphideLancerHitEffect.h"
#include "Input.h"
#include "PathManager.h"
#include "../ClientManager.h"
#include "LoadingThread.h"
#include "Render/RenderManager.h"

COnionScene::COnionScene()
{
	SetTypeID<COnionScene>();
}

COnionScene::~COnionScene()
{
	m_Scene->GetResource()->SoundStop("OnionSceneBGM");

	SAFE_DELETE(m_LoadingThread);
}

void COnionScene::SetStageObject(CStage* Stage)
{
	m_StageObject = Stage;
}

CLoadingThread* COnionScene::GetLoadingThread() const
{
	return m_LoadingThread;
}

bool COnionScene::Init()
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

	CreateAnimationSequence();
	// Effect Hit들을 Scene의 m_mapPrototype에 만들어놓기
	CreateEffectPrototype();

	// 양파재배지 Scene 만들어놓은 scn파일까지의 경로
	const PathInfo* Info = CPathManager::GetInst()->FindPath(SCENE_PATH);

	char	FullPath[MAX_PATH] = {};

	if (Info)
		strcpy_s(FullPath, Info->PathMultibyte);

	strcat_s(FullPath, "OnionMap.scn");

	m_Scene->LoadFullPath(FullPath);


	for (int i = 0; i < 5; ++i)
	{
		char MonsterName[128] = {};

		sprintf_s(MonsterName, "OnionMonster%d", i);

		COnionMonster* OnionMonster = m_Scene->CreateGameObject<COnionMonster>(MonsterName);
		OnionMonster->SetAllSceneComponentsLayer("MovingObjFront");
		OnionMonster->SetWorldPos(400.f + i * 250.f, 200.f, 0.f);
	}


	m_Scene->GetResource()->LoadSound("Master", true, "OnionSceneBGM", "TheFairyForest.mp3");
	m_Scene->GetResource()->SoundPlay("OnionSceneBGM");


	return true;
}

void COnionScene::Update(float DeltaTime)
{
	CSceneMode::Update(DeltaTime);

	if (m_LoadingThread)
	{
		CThreadQueue<LoadingMessage>* Queue = m_LoadingThread->GetLoadingQueue();

		if (!m_LoadingThread)
		{
			CSceneManager::GetInst()->ChangeNextScene();
			CRenderManager::GetInst()->SetStartFadeOut(true);
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
			}
		}
	}
}

void COnionScene::CreateMaterial()
{
}

void COnionScene::CreateAnimationSequence()
{
	CreatePlayerAnimationSequence();
	CreateSkillAnimationSequence();
	CreateMonsterAnimationSequence();
	CreateMapAnimationSequence();
}

void COnionScene::CreatePlayerAnimationSequence()
{
	m_Scene->GetResource()->LoadSequence2D("PlayerIdleLeft.sqc");
	m_Scene->GetResource()->LoadSequence2D("PlayerWalkLeft.sqc");
	m_Scene->GetResource()->LoadSequence2D("PlayerHealLeft.sqc");
	m_Scene->GetResource()->LoadSequence2D("PlayerRope.sqc");
	m_Scene->GetResource()->LoadSequence2D("PlayerJumpLeft.sqc");

}

void COnionScene::CreateSkillAnimationSequence()
{
	m_Scene->GetResource()->LoadSequence2D("PlayerOrb.sqc");

	m_Scene->GetResource()->LoadSequence2D("SylphideLancerBodyEffectLeft.sqc");
	m_Scene->GetResource()->LoadSequence2D("SylphideLancerArrowLeft.sqc");
	m_Scene->GetResource()->LoadSequence2D("SylphideLancerHit.sqc");
}

void COnionScene::CreateMonsterAnimationSequence()
{
	m_Scene->GetResource()->LoadSequence2D("OnionIdleLeft.sqc");
	m_Scene->GetResource()->LoadSequence2D("OnionWalkLeft.sqc");
	m_Scene->GetResource()->LoadSequence2D("OnionHitLeft.sqc");
	m_Scene->GetResource()->LoadSequence2D("OnionDieLeft.sqc");
}

void COnionScene::CreateMapAnimationSequence()
{
	m_Scene->GetResource()->LoadSequence2D("SingleHelixBlinkTree.sqc");
	m_Scene->GetResource()->LoadSequence2D("DoubleHelixBlinkTree.sqc");
	m_Scene->GetResource()->LoadSequence2D("BlinkTree.sqc");

	m_Scene->GetResource()->LoadSequence2D("Portal.sqc");
	m_Scene->GetResource()->LoadSequence2D("Butterfly.sqc");
	m_Scene->GetResource()->LoadSequence2D("LampLight.sqc");
}

void COnionScene::CreateEffectPrototype()
{
	CSylphideLancerHitEffect* SylphideLancerHitEffect = m_Scene->CreatePrototype<CSylphideLancerHitEffect>("SylphideLancerHitEffect");
}

void COnionScene::CreateWayToZakumScene()
{
	CSceneManager::GetInst()->CreateNextScene(false);
	CWayToZakumScene* WayToZakumScene = CSceneManager::GetInst()->CreateSceneModeEmpty<CWayToZakumScene>(false);

	WayToZakumScene->SetPlayerObject(m_PlayerObject);

	// 다음 Scene에서의 위치를 Scene의 왼쪽에 위치하도록 잡아주기
	m_PlayerObject->SetWorldPos(180.f, 200.f, 0.f);

	m_LoadingThread = CThread::CreateThread<CLoadingThread>("WayToZakumSceneLoadingThread");
	m_LoadingThread->SetLoadingScene(ThreadLoadingScene::WayToZakum);

	m_LoadingThread->Start();
}
