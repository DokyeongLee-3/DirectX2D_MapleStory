
#include "OnionScene.h"
#include "WayToZakumScene.h"
#include "LobbyScene.h"
#include "Scene/Scene.h"
#include "Scene/SceneResource.h"
#include "Scene/SceneManager.h"
#include "../Object/OnionMonster.h"
#include "../Object/SylphideLancerHitEffect.h"
#include "../Object/VoidPressureHitEffect.h"
#include "../Object/DeathSideHitEffect.h"
#include "Input.h"
#include "PathManager.h"
#include "../ClientManager.h"
#include "LoadingThread.h"
#include "Render/RenderManager.h"
#include "../Object/Player2D.h"
#include "RadishScene.h"

COnionScene::COnionScene()
{
	SetTypeID<COnionScene>();
}

COnionScene::~COnionScene()
{
	SAFE_DELETE(m_LoadingThread);

	CPlayer2D* Player = (CPlayer2D*)(m_Scene->GetPlayerObject());

	if (Player)
	{
		Player->SetVoidPressure(nullptr);
		Player->SetVoidPressureOrb(nullptr);
	}
}

void COnionScene::PushOnionMonster(COnionMonster* Monster)
{
	m_OnionMonsterList.push_back(Monster);
}

void COnionScene::SetStageObject(CStage* Stage)
{
	m_StageObject = Stage;
}

CLoadingThread* COnionScene::GetLoadingThread() const
{
	return m_LoadingThread;
}

void COnionScene::Start()
{
	CSceneMode::Start();

	if (m_PlayerObject)
	{
		CWidgetWindow* Window = ((CPlayer2D*)m_PlayerObject.Get())->GetDamageWidgetComponent()->GetWidgetWindow();

		Window->SetViewport(m_Scene->GetViewport());

		Window->GetViewport()->SetScene(m_Scene);

		Window = ((CPlayer2D*)m_PlayerObject.Get())->GetNameWidgetComponent()->GetWidgetWindow();

		Window->SetViewport(m_Scene->GetViewport());

		Window->GetViewport()->SetScene(m_Scene);
	}
}

bool COnionScene::Init()
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
	// Effect Hit들을 Scene의 m_mapPrototype에 만들어놓기
	CreateEffectPrototype();

	// 양파재배지 Scene 만들어놓은 scn파일까지의 경로
	const PathInfo* Info = CPathManager::GetInst()->FindPath(SCENE_PATH);

	char	FullPath[MAX_PATH] = {};

	if (Info)
		strcpy_s(FullPath, Info->PathMultibyte);

	strcat_s(FullPath, "OnionMap.scn");

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

	if (m_OnionMonsterList.size() < 5)
	{
		int Count = 0;
		auto iter = m_DeadPos.begin();
		auto iterEnd = m_DeadPos.end();

		for (; iter != iterEnd; )
		{
			COnionMonster* Onion = m_Scene->CreateGameObject<COnionMonster>("Onion");
			Onion->SetWorldPos((*iter));
			Onion->GetRootComponent()->SetLayerName("MovingObjFront");
			iter = m_DeadPos.erase(iter);
			iterEnd = m_DeadPos.end();
			((CSpriteComponent*)Onion->GetRootComponent())->SetOpacity(0.5f);

			m_OnionMonsterList.push_back(Onion);
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
	m_Scene->GetResource()->LoadSequence2D("PlayerLevelUpEffect.sqc");
	m_Scene->GetResource()->LoadSequence2D("PlayerDead.sqc");
}

void COnionScene::CreateSkillAnimationSequence()
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

	m_Scene->GetResource()->LoadSequence2D("DeathSide.sqc");
	m_Scene->GetResource()->LoadSequence2D("DeathSideHitEffect.sqc");
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
	CVoidPressureHitEffect* VoidPressureHitEffect = m_Scene->CreatePrototype<CVoidPressureHitEffect>("VoidPressureHitEffect");
	CDeathSideHitEffect* DeathSideHitEffect = m_Scene->CreatePrototype<CDeathSideHitEffect>("DeathSideHitEffect");
}

void COnionScene::LoadSound()
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

	m_Scene->GetResource()->LoadSound("Effect", false, "DeathSideHit", "DeathSideHit.mp3");
	m_Scene->GetResource()->LoadSound("Effect", false, "DeathSideUse", "DeathSideUse.mp3");
	m_Scene->GetResource()->LoadSound("Effect", false, "DeathSideVoice1", "DeathSideVoice1.mp3");
	m_Scene->GetResource()->LoadSound("Effect", false, "DeathSideVoice2", "DeathSideVoice2.mp3");

	m_Scene->GetResource()->LoadSound("Effect", false, "LevelUp", "LevelUp.mp3");
	m_Scene->GetResource()->LoadSound("Effect", false, "PickUpItem", "PickUpItem.mp3");
	m_Scene->GetResource()->LoadSound("Effect", false, "Tombstone", "Tombstone.mp3");
	m_Scene->GetResource()->LoadSound("Effect", false, "DropItem", "DropItem.mp3");
	m_Scene->GetResource()->LoadSound("Effect", false, "EatItem", "ItemEat.mp3");

	m_Scene->GetResource()->LoadSound("UI", false, "TabClick", "TabClick.mp3");
	m_Scene->GetResource()->LoadSound("UI", false, "UIOpen", "UIOpen.mp3");
}

void COnionScene::AddTileCollisionCallback()
{
	//CTileObject* FloorTile = (CTileObject*)m_Scene->FindObject("OnionFloorTileObj");

	//if (FloorTile)
	//{
	//	CColliderBox2D* Collider = (CColliderBox2D*)FloorTile->FindComponent("OnionFloorTileCollider");
	//	Collider->AddCollisionCallback<CTileObject>(Collision_State::Begin, FloorTile, &CTileObject::CollisionBeginCallback);
	//	//Collider->AddCollisionCallback<CTileObject>(Collision_State::Stay, FloorTile, &CTileObject::CollisionStayCallback);
	//	Collider->AddCollisionCallback<CTileObject>(Collision_State::End, FloorTile, &CTileObject::CollisionEndCallback);
	//}

	//for (int i = 0; i <= 5; ++i)
	//{
	//	char TileObjName[128] = {};
	//	sprintf_s(TileObjName, "TileObj%d", i);
	//	CTileObject* TileObj = (CTileObject*)m_Scene->FindObject(TileObjName);

	//	if (TileObj)
	//	{
	//		CColliderBox2D* TileCollider = (CColliderBox2D*)TileObj->FindComponent("TileMapCollider");

	//		if (TileCollider)
	//		{
	//			TileCollider->AddCollisionCallback<CTileObject>(Collision_State::Begin, TileObj, &CTileObject::CollisionBeginCallback);
	//			TileCollider->AddCollisionCallback<CTileObject>(Collision_State::End, TileObj, &CTileObject::CollisionEndCallback);
	//		}
	//	}
	//}
}

COnionMonster* COnionScene::FindOnionMonster(bool Right, const Vector3& MyPos, float DistXConstraint, float DistYConstraint)
{
	auto iter =	m_OnionMonsterList.begin();
	auto iterEnd = m_OnionMonsterList.end();

	for (; iter != iterEnd; ++iter)
	{
		if ((*iter)->GetTypeID() == typeid(COnionMonster).hash_code())
		{
			Vector3 MonsterPos = (*iter)->GetWorldPos();

			if (Right && MonsterPos.x < MyPos.x)
				continue;

			if (!Right && MonsterPos.x > MyPos.x)
				continue;


			if (abs(MonsterPos.x - MyPos.x) < DistXConstraint && abs(MonsterPos.y - MyPos.y) < DistYConstraint)
			{
				if (!(*iter)->GetBody()->IsEnable())
					continue;

				return (*iter);
			}
		}
	}

	return nullptr;
}

void COnionScene::CreateWayToZakumScene()
{
	m_Scene->GetResource()->SoundStop("OnionSceneBGM");

	CSceneManager::GetInst()->CreateNextScene(false);
	CWayToZakumScene* WayToZakumScene = CSceneManager::GetInst()->CreateSceneModeEmpty<CWayToZakumScene>(false);

	WayToZakumScene->SetPlayerObject(m_PlayerObject);

	// 다음 Scene에서의 위치를 Scene의 왼쪽에 위치하도록 잡아주기
	m_PlayerObject->SetWorldPos(250.f, 200.f, 0.f);

	m_LoadingThread = CThread::CreateThread<CLoadingThread>("WayToZakumSceneLoadingThread");
	m_LoadingThread->SetLoadingScene(ThreadLoadingScene::WayToZakum);

	m_LoadingThread->Start();
}

void COnionScene::CreateLobbyScene()
{
	m_Scene->GetResource()->SoundStop("OnionSceneBGM");

	CSceneManager::GetInst()->CreateNextScene(false);
	CLobbyScene* LobbyScene = CSceneManager::GetInst()->CreateSceneModeEmpty<CLobbyScene>(false);

	LobbyScene->SetPlayerObject(m_PlayerObject);

	// 다음 Scene에서의 위치를 Scene의 왼쪽에 위치하도록 잡아주기
	Vector3 WorldPos = m_PlayerObject->GetWorldPos();
	m_PlayerObject->SetWorldPos(1750.f, 300.f, WorldPos.z);

	//SAFE_DELETE(m_LoadingThread);
	m_LoadingThread = CThread::CreateThread<CLoadingThread>("LobbySceneLoadingThread");
	m_LoadingThread->SetLoadingScene(ThreadLoadingScene::Lobby);

	if (((CPlayer2D*)m_PlayerObject.Get())->IsDead())
	{
		((CPlayer2D*)m_PlayerObject.Get())->ReturnAlive();
	}


	m_LoadingThread->Start();
}

void COnionScene::CreateRadishScene()
{
	m_Scene->GetResource()->SoundStop("OnionSceneBGM");

	CSceneManager::GetInst()->CreateNextScene(false);
	CRadishScene* RadishScene = CSceneManager::GetInst()->CreateSceneModeEmpty<CRadishScene>(false);

	RadishScene->SetPlayerObject(m_PlayerObject);

	// 다음 Scene에서의 위치를 Scene의 왼쪽에 위치하도록 잡아주기
	Vector3 WorldPos = m_PlayerObject->GetWorldPos();
	m_PlayerObject->SetWorldPos(200.f, 300.f, WorldPos.z);

	//SAFE_DELETE(m_LoadingThread);
	m_LoadingThread = CThread::CreateThread<CLoadingThread>("RadishSceneLoadingThread");
	m_LoadingThread->SetLoadingScene(ThreadLoadingScene::Radish);

	m_LoadingThread->Start();
}

void COnionScene::DeleteOnionMonster(const std::string& Name)
{
	auto iter = m_OnionMonsterList.begin();
	auto iterEnd = m_OnionMonsterList.end();

	for (; iter != iterEnd; ++iter)
	{
		if ((*iter)->GetName() == Name)
		{
			m_OnionMonsterList.erase(iter);
			return;
		}
	}
}

void COnionScene::DeleteOnionMonster(COnionMonster* Monster)
{
	auto iter = m_OnionMonsterList.begin();
	auto iterEnd = m_OnionMonsterList.end();

	for (; iter != iterEnd; ++iter)
	{
		if ((*iter) == Monster)
		{
			m_OnionMonsterList.erase(iter);
			return;
		}
	}
}
