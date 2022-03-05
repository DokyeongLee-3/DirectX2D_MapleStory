
#include "RadishScene.h"
#include "WayToZakumScene.h"
#include "LobbyScene.h"
#include "Scene/Scene.h"
#include "Scene/SceneResource.h"
#include "Scene/SceneManager.h"
#include "../Object/RadishMonster.h"
#include "../Object/SylphideLancerHitEffect.h"
#include "../Object/VoidPressureHitEffect.h"
#include "Input.h"
#include "PathManager.h"
#include "../ClientManager.h"
#include "LoadingThread.h"
#include "Render/RenderManager.h"
#include "../Object/Player2D.h"
#include "../Object/RadishMonster.h"
#include "OnionScene.h"

CRadishScene::CRadishScene()
{
	SetTypeID<CRadishScene>();
}

CRadishScene::~CRadishScene()
{
	SAFE_DELETE(m_LoadingThread);

	CPlayer2D* Player = (CPlayer2D*)(m_Scene->GetPlayerObject());

	if (Player)
	{
		Player->SetVoidPressure(nullptr);
		Player->SetVoidPressureOrb(nullptr);
	}
}

void CRadishScene::PushRadishMonster(CRadishMonster* Monster)
{
	m_RadishMonsterList.push_back(Monster);
}

void CRadishScene::SetStageObject(CStage* Stage)
{
	m_StageObject = Stage;
}

CLoadingThread* CRadishScene::GetLoadingThread() const
{
	return m_LoadingThread;
}

void CRadishScene::Start()
{
	CSceneMode::Start();

	if (m_PlayerObject)
	{
		CWidgetWindow* Window = ((CPlayer2D*)m_PlayerObject.Get())->GetDamageWidgetComponent()->GetWidgetWindow();

		Window->SetViewport(m_Scene->GetViewport());

		Window->GetViewport()->SetScene(m_Scene);
	}
}

bool CRadishScene::Init()
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
	// Effect Hit���� Scene�� m_mapPrototype�� ��������
	CreateEffectPrototype();

	const PathInfo* Info = CPathManager::GetInst()->FindPath(SCENE_PATH);

	char	FullPath[MAX_PATH] = {};

	if (Info)
		strcpy_s(FullPath, Info->PathMultibyte);

	strcat_s(FullPath, "RadishMap.scn");

	m_Scene->LoadFullPath(FullPath);


	LoadSound();

	if (m_PlayerObject)
	{
		m_PlayerObject->SetGravity(true);
		m_PlayerObject->SetTileCollisionEnable(false);
	}

	m_Scene->GetResource()->SoundPlay("OnionSceneBGM");

	return true;
}

void CRadishScene::Update(float DeltaTime)
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

void CRadishScene::CreateMaterial()
{
}

void CRadishScene::CreateAnimationSequence()
{
	CreatePlayerAnimationSequence();
	CreateSkillAnimationSequence();
	CreateMonsterAnimationSequence();
	CreateMapAnimationSequence();
}

void CRadishScene::CreatePlayerAnimationSequence()
{
	m_Scene->GetResource()->LoadSequence2D("PlayerIdleLeft.sqc");
	m_Scene->GetResource()->LoadSequence2D("PlayerWalkLeft.sqc");
	m_Scene->GetResource()->LoadSequence2D("PlayerHealLeft.sqc");
	m_Scene->GetResource()->LoadSequence2D("PlayerRope.sqc");
	m_Scene->GetResource()->LoadSequence2D("PlayerJumpLeft.sqc");

}

void CRadishScene::CreateSkillAnimationSequence()
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

void CRadishScene::CreateMonsterAnimationSequence()
{
	m_Scene->GetResource()->LoadSequence2D("RadishIdleLeft.sqc");
	m_Scene->GetResource()->LoadSequence2D("RadishWalkLeft.sqc");
	m_Scene->GetResource()->LoadSequence2D("RadishHitLeft.sqc");
	m_Scene->GetResource()->LoadSequence2D("RadishDieLeft.sqc");
}

void CRadishScene::CreateMapAnimationSequence()
{
	m_Scene->GetResource()->LoadSequence2D("SingleHelixBlinkTree.sqc");
	m_Scene->GetResource()->LoadSequence2D("DoubleHelixBlinkTree.sqc");
	m_Scene->GetResource()->LoadSequence2D("BlinkTree.sqc");

	m_Scene->GetResource()->LoadSequence2D("Portal.sqc");
	m_Scene->GetResource()->LoadSequence2D("Butterfly.sqc");
	m_Scene->GetResource()->LoadSequence2D("LampLight.sqc");
}

void CRadishScene::CreateEffectPrototype()
{
	CSylphideLancerHitEffect* SylphideLancerHitEffect = m_Scene->CreatePrototype<CSylphideLancerHitEffect>("SylphideLancerHitEffect");
	CVoidPressureHitEffect* VoidPressureHitEffect = m_Scene->CreatePrototype<CVoidPressureHitEffect>("VoidPressureHitEffect");
}

void CRadishScene::LoadSound()
{
	m_Scene->GetResource()->LoadSound("BGM", true, "OnionSceneBGM", "TheFairyForest.mp3");

	m_Scene->GetResource()->LoadSound("Effect", false, "Jump", "Jump.mp3");

	m_Scene->GetResource()->LoadSound("Effect", false, "SylphideLancerUse", "SylphideLancerUse.mp3");
	m_Scene->GetResource()->LoadSound("Effect", false, "SylphideLancerHit", "SylphideLancerHit.mp3");

	m_Scene->GetResource()->LoadSound("Effect", false, "VoidPressureUse", "VoidPressureUse.mp3");
	m_Scene->GetResource()->LoadSound("Effect", false, "VoidPressureLoop", "VoidPressureLoop.mp3");
	m_Scene->GetResource()->LoadSound("Effect", false, "VoidPressureEnd", "VoidPressureEnd.mp3");
	m_Scene->GetResource()->LoadSound("Effect", false, "VoidPressureHit", "VoidPressureHit.mp3");

	m_Scene->GetResource()->LoadSound("Effect", false, "LightTransforming", "LightTransformingUse.mp3");
}

void CRadishScene::AddTileCollisionCallback()
{
	/*CTileObject* FloorTile = (CTileObject*)m_Scene->FindObject("RadishFloorTileObj");

	if (FloorTile)
	{
		CColliderBox2D* Collider = (CColliderBox2D*)FloorTile->FindComponent("RadishFloorTileCollider");
		Collider->AddCollisionCallback<CTileObject>(Collision_State::Begin, FloorTile, &CTileObject::CollisionBeginCallback);
		Collider->AddCollisionCallback<CTileObject>(Collision_State::End, FloorTile, &CTileObject::CollisionEndCallback);
	}

	for (int i = 0; i <= 5; ++i)
	{
		char TileObjName[128] = {};
		sprintf_s(TileObjName, "TileObj%d", i);
		CTileObject* TileObj = (CTileObject*)m_Scene->FindObject(TileObjName);

		if (TileObj)
		{
			CColliderBox2D* TileCollider = (CColliderBox2D*)TileObj->FindComponent("TileMapCollider");

			if (TileCollider)
			{
				TileCollider->AddCollisionCallback<CTileObject>(Collision_State::Begin, TileObj, &CTileObject::CollisionBeginCallback);
				TileCollider->AddCollisionCallback<CTileObject>(Collision_State::End, TileObj, &CTileObject::CollisionEndCallback);
			}
		}
	}*/
}

CRadishMonster* CRadishScene::FindRadishMonster(bool Right, const Vector3& MyPos, float DistXConstraint, float DistYConstraint)
{
	auto iter = m_RadishMonsterList.begin();
	auto iterEnd = m_RadishMonsterList.end();

	for (; iter != iterEnd; ++iter)
	{
		if ((*iter)->GetTypeID() == typeid(CRadishMonster).hash_code())
		{
			Vector3 MonsterPos = (*iter)->GetWorldPos();

			if (Right && MonsterPos.x < MyPos.x)
				continue;

			if (!Right && MonsterPos.x > MyPos.x)
				continue;


			if (abs(MonsterPos.x - MyPos.x) < DistXConstraint && MonsterPos.y - MyPos.y > 0.f && MonsterPos.y - MyPos.y < DistYConstraint)
			{
				if (!(*iter)->GetBody()->IsEnable())
					continue;

				return (*iter);
			}
		}
	}

	return nullptr;
}

void CRadishScene::CreateWayToZakumScene()
{
	m_Scene->GetResource()->SoundStop("RadishSceneBGM");

	CSceneManager::GetInst()->CreateNextScene(false);
	CWayToZakumScene* WayToZakumScene = CSceneManager::GetInst()->CreateSceneModeEmpty<CWayToZakumScene>(false);

	WayToZakumScene->SetPlayerObject(m_PlayerObject);

	// ���� Scene������ ��ġ�� Scene�� ���ʿ� ��ġ�ϵ��� ����ֱ�
	m_PlayerObject->SetWorldPos(180.f, 200.f, 0.f);

	m_LoadingThread = CThread::CreateThread<CLoadingThread>("WayToZakumSceneLoadingThread");
	m_LoadingThread->SetLoadingScene(ThreadLoadingScene::WayToZakum);

	m_LoadingThread->Start();
}

void CRadishScene::CreateLobbyScene()
{
	m_Scene->GetResource()->SoundStop("RadishSceneBGM");

	CSceneManager::GetInst()->CreateNextScene(false);
	CLobbyScene* LobbyScene = CSceneManager::GetInst()->CreateSceneModeEmpty<CLobbyScene>(false);

	LobbyScene->SetPlayerObject(m_PlayerObject);

	// ���� Scene������ ��ġ�� Scene�� ���ʿ� ��ġ�ϵ��� ����ֱ�
	m_PlayerObject->SetWorldPos(1750.f, 300.f, 0.f);

	//SAFE_DELETE(m_LoadingThread);
	m_LoadingThread = CThread::CreateThread<CLoadingThread>("LobbySceneLoadingThread");
	m_LoadingThread->SetLoadingScene(ThreadLoadingScene::Lobby);

	m_LoadingThread->Start();
}

void CRadishScene::CreateOnionScene()
{
	m_Scene->GetResource()->SoundStop("OnionSceneBGM");

	CSceneManager::GetInst()->CreateNextScene(false);
	COnionScene* OnionScene = CSceneManager::GetInst()->CreateSceneModeEmpty<COnionScene>(false);

	OnionScene->SetPlayerObject(m_PlayerObject);

	// ���� Scene������ ��ġ�� Scene�� ���ʿ� ��ġ�ϵ��� ����ֱ�
	m_PlayerObject->SetWorldPos(200.f, 300.f, 0.f);

	//SAFE_DELETE(m_LoadingThread);
	m_LoadingThread = CThread::CreateThread<CLoadingThread>("OnionSceneLoadingThread");
	m_LoadingThread->SetLoadingScene(ThreadLoadingScene::Onion);

	m_LoadingThread->Start();
}