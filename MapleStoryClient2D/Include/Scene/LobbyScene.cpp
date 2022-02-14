
#include "LobbyScene.h"
#include "OnionScene.h"
#include "WayToZakumScene.h"
#include "Scene/Scene.h"
#include "Scene/SceneResource.h"
#include "Scene/SceneManager.h"
#include "../Object/Player2D.h"
#include "Input.h"
#include "PathManager.h"
#include "Render/RenderManager.h"
#include "../ClientManager.h"
#include "LoadingThread.h"

CLobbyScene::CLobbyScene()	:
	m_LoadingThread(nullptr)
{
	SetTypeID<CLobbyScene>();
}

CLobbyScene::~CLobbyScene()
{
	m_Scene->GetResource()->SoundStop("FairyAcademyBGM");

	SAFE_DELETE(m_LoadingThread);
}

void CLobbyScene::SetStageObject(CStage* Stage)
{
	m_StageObject = Stage;
}


bool CLobbyScene::Init()
{
	CreateAnimationSequence();

	CPlayer2D* Player = m_Scene->CreateGameObject<CPlayer2D>("Player");

	SetPlayerObject(Player);

	Player->SetWorldPos(400.f, 200.f, 0.f);

	Player->SetAllSceneComponentsLayer("MovingObjFront");

	m_ConfigurationWindow = m_Scene->GetViewport()->CreateWidgetWindow<CConfigurationWindow>("ConfigurationWindow");
	m_ConfigurationWindow->SetZOrder(2);
	m_ConfigurationWindow->SetPos(200.f, 200.f);
	m_ConfigurationWindow->Enable(false);
	CClientManager::GetInst()->SetConfigurationWindow(m_ConfigurationWindow);

	m_Inventory = m_Scene->GetViewport()->CreateWidgetWindow<CInventory>("Inventory");
	m_Inventory->SetZOrder(2);
	m_Inventory->SetPos(250.f, 150.f);
	m_Inventory->Enable(false);
	CClientManager::GetInst()->SetInventoryWindow(m_Inventory);

	m_BossMatchingWindow = m_Scene->GetViewport()->CreateWidgetWindow<CBossMatching>("BossMatching");
	m_BossMatchingWindow->SetZOrder(2);
	m_BossMatchingWindow->SetPos(200.f, 100.f);
	m_BossMatchingWindow->Enable(false);
	CClientManager::GetInst()->SetBossMatchingWindow(m_BossMatchingWindow);

	m_CharacterStatusWindow = m_Scene->GetViewport()->CreateWidgetWindow<CCharacterStatusWindow>("MainStatus");
	m_CharacterStatusWindow->SetPos(550.f, 13.f);
	m_CharacterStatusWindow->SetZOrder(1);
	m_CharacterStatusWindow->SetMouseCollisionEnable(false);
	CClientManager::GetInst()->SetCharacterStatusWindow(m_CharacterStatusWindow);

	m_CharacterEXPWindow = m_Scene->GetViewport()->CreateWidgetWindow<CCharacterEXP>("EXPWindow");
	m_CharacterEXPWindow->SetZOrder(1);
	m_CharacterEXPWindow->SetMouseCollisionEnable(false);
	CClientManager::GetInst()->SetEXPWindow(m_CharacterEXPWindow);

	m_SkillQuickSlot = m_Scene->GetViewport()->CreateWidgetWindow<CSkillQuickSlotWindow>("SkillQuickSlot");
	m_SkillQuickSlot->SetZOrder(1);
	CClientManager::GetInst()->SetSkillQuickSlot(m_SkillQuickSlot);

	m_Scene->GetResource()->LoadSound("BGM", true, "FairyAcademyBGM", "TheFairyAcademy.mp3");
	m_Scene->GetResource()->SoundPlay("FairyAcademyBGM");

	// �����п� �κ� Scene �������� scn���ϱ����� ���
	const PathInfo* Info = CPathManager::GetInst()->FindPath(SCENE_PATH);

	char	FullPath[MAX_PATH] = {};

	if (Info)
		strcpy_s(FullPath, Info->PathMultibyte);

	strcat_s(FullPath, "FairyAcademyLobby.scn");

	m_Scene->LoadFullPath(FullPath);

	return true;
}

void CLobbyScene::Update(float DeltaTime)
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

void CLobbyScene::CreateMaterial()
{
}

void CLobbyScene::CreateAnimationSequence()
{
	CreatePlayerAnimationSequence();
	CreateSkillAnimationSequence();
	CreateMapAnimationSequence();
}

void CLobbyScene::CreatePlayerAnimationSequence()
{
	m_Scene->GetResource()->LoadSequence2D("PlayerIdleLeft.sqc");
	m_Scene->GetResource()->LoadSequence2D("PlayerWalkLeft.sqc");
	m_Scene->GetResource()->LoadSequence2D("PlayerHealLeft.sqc");
	m_Scene->GetResource()->LoadSequence2D("PlayerRope.sqc");
	m_Scene->GetResource()->LoadSequence2D("PlayerJumpLeft.sqc");
}

void CLobbyScene::CreateSkillAnimationSequence()
{
	m_Scene->GetResource()->LoadSequence2D("Blank.sqc");

	m_Scene->GetResource()->LoadSequence2D("PlayerOrb.sqc");

	m_Scene->GetResource()->LoadSequence2D("SylphideLancerMuzzleLeft.sqc");
	m_Scene->GetResource()->LoadSequence2D("SylphideLancerBodyEffectLeft.sqc");
	m_Scene->GetResource()->LoadSequence2D("SylphideLancerArrowLeft.sqc");
	m_Scene->GetResource()->LoadSequence2D("SylphideLancerArrowPurple.sqc");
}

void CLobbyScene::CreateMapAnimationSequence()
{
	m_Scene->GetResource()->LoadSequence2D("Portal.sqc");
	m_Scene->GetResource()->LoadSequence2D("Butterfly.sqc");
	m_Scene->GetResource()->LoadSequence2D("LampLight.sqc");
	m_Scene->GetResource()->LoadSequence2D("VerticalLampLight.sqc");
	m_Scene->GetResource()->LoadSequence2D("Library2ndButterfly.sqc");
	m_Scene->GetResource()->LoadSequence2D("Library2ndSmallButterfly.sqc");
	m_Scene->GetResource()->LoadSequence2D("LampBigHelix.sqc");
	m_Scene->GetResource()->LoadSequence2D("LampSmallHelix.sqc");
}

void CLobbyScene::CreateOnionScene()
{
	CSceneManager::GetInst()->CreateNextScene(false);
	COnionScene* OnionScene = CSceneManager::GetInst()->CreateSceneModeEmpty<COnionScene>(false);

	OnionScene->SetPlayerObject(m_PlayerObject);

	// ���� Scene������ ��ġ�� Scene�� ���ʿ� ��ġ�ϵ��� ����ֱ�
	m_PlayerObject->SetWorldPos(180.f, 200.f, 0.f);

	m_LoadingThread = CThread::CreateThread<CLoadingThread>("OnionSceneLoadingThread");
	m_LoadingThread->SetLoadingScene(ThreadLoadingScene::Onion);

	m_LoadingThread->Start();
}

void CLobbyScene::CreateWayToZakumScene()
{
	CSceneManager::GetInst()->CreateNextScene(false);
	CWayToZakumScene* WayToZakumScene = CSceneManager::GetInst()->CreateSceneModeEmpty<CWayToZakumScene>(false);

	WayToZakumScene->SetPlayerObject(m_PlayerObject);

	// ���� Scene������ ��ġ�� Scene�� ���ʿ� ��ġ�ϵ��� ����ֱ�
	m_PlayerObject->SetWorldPos(180.f, 200.f, 0.f);

	m_LoadingThread = CThread::CreateThread<CLoadingThread>("WayToZakumSceneLoadingThread");
	m_LoadingThread->SetLoadingScene(ThreadLoadingScene::WayToZakum);

	m_LoadingThread->Start();
}