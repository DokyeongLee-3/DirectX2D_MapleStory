
#include "Library2ndScene.h"
#include "LobbyScene.h"
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

CLibrary2ndScene::CLibrary2ndScene()
{
	SetTypeID<CLibrary2ndScene>();
}

CLibrary2ndScene::~CLibrary2ndScene()
{
	m_Scene->GetResource()->SoundStop("FairyAcademyBGM");

	SAFE_DELETE(m_LoadingThread);

	CPlayer2D* Player = (CPlayer2D*)(m_Scene->GetPlayerObject());

	if (Player)
	{
		Player->SetVoidPressure(nullptr);
		Player->SetVoidPressureOrb(nullptr);
	}
}

void CLibrary2ndScene::SetStageObject(CStage* Stage)
{
	m_StageObject = Stage;
}

void CLibrary2ndScene::Start()
{
	CSceneMode::Start();

	if (m_PlayerObject)
	{
		((CPlayer2D*)m_PlayerObject.Get())->GetDamageWidgetComponent()->GetWidgetWindow()->GetViewport()->SetScene(m_Scene);
	}
}

bool CLibrary2ndScene::Init()
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

	// ������ ���ͼ��� ������Ī UI�� ���������� ���ش�
	if (BossMatching->IsEnable())
		BossMatching->Enable(false);

	CreateAnimationSequence();
	CreateMaterial();
	LoadSound();

	// �������� ���ϴ� �� Scene �������� scn���ϱ����� ���
	const PathInfo* Info = CPathManager::GetInst()->FindPath(SCENE_PATH);

	char	FullPath[MAX_PATH] = {};

	if (Info)
		strcpy_s(FullPath, Info->PathMultibyte);

	strcat_s(FullPath, "Library2ndFloor.scn");

	m_Scene->LoadFullPath(FullPath);


	m_Scene->GetResource()->SoundPlay("FairyAcademyBGM");

	if (m_PlayerObject)
	{
		m_PlayerObject->SetGravity(true);
		m_PlayerObject->SetTileCollisionEnable(false);
	}

	AddTileCollisionCallback();

	return true;
}

void CLibrary2ndScene::Update(float DeltaTime)
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

void CLibrary2ndScene::CreateMaterial()
{

}

void CLibrary2ndScene::CreateAnimationSequence()
{
	CreatePlayerAnimationSequence();
	CreateSkillAnimationSequence();
	CreateMapAnimationSequence();
}

void CLibrary2ndScene::CreatePlayerAnimationSequence()
{
	m_Scene->GetResource()->LoadSequence2D("PlayerIdleLeft.sqc");
	m_Scene->GetResource()->LoadSequence2D("PlayerWalkLeft.sqc");
	m_Scene->GetResource()->LoadSequence2D("PlayerHealLeft.sqc");
	m_Scene->GetResource()->LoadSequence2D("PlayerRope.sqc");
	m_Scene->GetResource()->LoadSequence2D("PlayerJumpLeft.sqc");

}

void CLibrary2ndScene::CreateSkillAnimationSequence()
{
	m_Scene->GetResource()->LoadSequence2D("PlayerOrb.sqc");

	m_Scene->GetResource()->LoadSequence2D("Blank.sqc");
	m_Scene->GetResource()->LoadSequence2D("SylphideLancerArrowLeft.sqc");
	m_Scene->GetResource()->LoadSequence2D("SylphideLancerArrowPurple.sqc");
	m_Scene->GetResource()->LoadSequence2D("SylphideLancerHit.sqc");
	m_Scene->GetResource()->LoadSequence2D("SylphideLancerHitPurple.sqc");

	m_Scene->GetResource()->LoadSequence2D("SylphideLancerMuzzleLeft.sqc");

	m_Scene->GetResource()->LoadSequence2D("VoidPressureStartLeft.sqc");
	m_Scene->GetResource()->LoadSequence2D("VoidPressureReadyLeft.sqc");
	m_Scene->GetResource()->LoadSequence2D("VoidPressureDestroy.sqc");
	m_Scene->GetResource()->LoadSequence2D("VoidPressureOrb.sqc");
	m_Scene->GetResource()->LoadSequence2D("VoidPressureOrbDestroy.sqc");
	m_Scene->GetResource()->LoadSequence2D("VoidPressureHit.sqc");

	m_Scene->GetResource()->LoadSequence2D("LightTransformingLeft.sqc");
}

void CLibrary2ndScene::CreateMapAnimationSequence()
{
	m_Scene->GetResource()->LoadSequence2D("Portal.sqc");
	m_Scene->GetResource()->LoadSequence2D("VerticalLampLight.sqc");
	m_Scene->GetResource()->LoadSequence2D("LampBigHelix.sqc");
	m_Scene->GetResource()->LoadSequence2D("Library2ndButterfly.sqc");
}

void CLibrary2ndScene::LoadSound()
{
	m_Scene->GetResource()->LoadSound("BGM", true, "FairyAcademyBGM", "TheFairyAcademy.mp3");

	m_Scene->GetResource()->LoadSound("Effect", false, "Jump", "Jump.mp3");

	m_Scene->GetResource()->LoadSound("Effect", false, "SylphideLancerUse", "SylphideLancerUse.mp3");
	m_Scene->GetResource()->LoadSound("Effect", false, "SylphideLancerHit", "SylphideLancerHit.mp3");

	m_Scene->GetResource()->LoadSound("Effect", false, "VoidPressureUse", "VoidPressureUse.mp3");
	m_Scene->GetResource()->LoadSound("Effect", false, "VoidPressureLoop", "VoidPressureLoop.mp3");
	m_Scene->GetResource()->LoadSound("Effect", false, "VoidPressureEnd", "VoidPressureEnd.mp3");
	m_Scene->GetResource()->LoadSound("Effect", false, "VoidPressureHit", "VoidPressureHit.mp3");

	m_Scene->GetResource()->LoadSound("Effect", false, "LightTransforming", "LightTransformingUse.mp3");
}

void CLibrary2ndScene::AddTileCollisionCallback()
{
	CTileObject* FloorTile = (CTileObject*)m_Scene->FindObject("FloorTile");

	if (FloorTile)
	{
		CColliderBox2D* Collider = (CColliderBox2D*)FloorTile->FindComponent("FloorTileCollider");
		Collider->AddCollisionCallback<CTileObject>(Collision_State::Begin, FloorTile, &CTileObject::CollisionBeginCallback);
		Collider->AddCollisionCallback<CTileObject>(Collision_State::End, FloorTile, &CTileObject::CollisionEndCallback);
	}
}

void CLibrary2ndScene::CreateLobbyScene()
{
	CSceneManager::GetInst()->CreateNextScene(false);
	CLobbyScene* LobbyScene = CSceneManager::GetInst()->CreateSceneModeEmpty<CLobbyScene>(false);

	LobbyScene->SetPlayerObject(m_PlayerObject);

	// ���� Scene������ ��ġ�� Scene�� ���ʿ� ��ġ�ϵ��� ����ֱ�
	m_PlayerObject->SetWorldPos(981.f, 640.f, 0.f);

	//SAFE_DELETE(m_LoadingThread);
	m_LoadingThread = CThread::CreateThread<CLoadingThread>("LobbySceneLoadingThread");
	m_LoadingThread->SetLoadingScene(ThreadLoadingScene::Lobby);

	m_LoadingThread->Start();
}
