
#include "Library2ndScene.h"
#include "LobbyScene.h"
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
#include "../Object/SylphideLancerHitEffect.h"
#include "../Object/VoidPressureHitEffect.h"
#include "../Object/DeathSideHitEffect.h"

CLibrary2ndScene::CLibrary2ndScene()
{
	SetTypeID<CLibrary2ndScene>();
}

CLibrary2ndScene::~CLibrary2ndScene()
{
	SAFE_DELETE(m_LoadingThread);

	CPlayer2D* Player = (CPlayer2D*)(m_Scene->GetPlayerObject());

	if (Player)
	{
		Player->SetVoidPressure(nullptr);
		Player->SetVoidPressureOrb(nullptr);
	}
}

void CLibrary2ndScene::PushLowerClassBook(CLowerClassBook* Book)
{
	m_LowerClassBookList.push_back(Book);
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
		CWidgetWindow* Window = ((CPlayer2D*)m_PlayerObject.Get())->GetDamageWidgetComponent()->GetWidgetWindow();

		Window->SetViewport(m_Scene->GetViewport());

		Window->GetViewport()->SetScene(m_Scene);

		Window = ((CPlayer2D*)m_PlayerObject.Get())->GetNameWidgetComponent()->GetWidgetWindow();

		Window->SetViewport(m_Scene->GetViewport());

		Window->GetViewport()->SetScene(m_Scene);
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
	CStatWindow* StatWindow = CClientManager::GetInst()->GetStatWindow();
	CDyingNoticeWindow* DyingNoticeWindow = CClientManager::GetInst()->GetDyingNoticeWindow();
	CSkillPointWindow* SkillPointWindow = CClientManager::GetInst()->GetSkillPointWindow();

	m_Scene->GetViewport()->AddWindow(StatusWindow);
	m_Scene->GetViewport()->AddWindow(Inventory);
	m_Scene->GetViewport()->AddWindow(SkillQuickSlot);
	m_Scene->GetViewport()->AddWindow(ConfigurationWindow);
	m_Scene->GetViewport()->AddWindow(EXPWindow);
	m_Scene->GetViewport()->AddWindow(BossMatching);
	m_Scene->GetViewport()->AddWindow(StatWindow);
	m_Scene->GetViewport()->AddWindow(DyingNoticeWindow);
	m_Scene->GetViewport()->AddWindow(SkillPointWindow);

	// 보스맵 들어와서도 보스매칭 UI가 켜져있으면 꺼준다
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

	strcat_s(FullPath, "Library2ndFloor.scn");

	m_Scene->LoadFullPath(FullPath);


	m_Scene->GetResource()->SoundPlay("FairyAcademyBGM");

	if (m_PlayerObject)
	{
		m_PlayerObject->SetGravity(true);
		m_PlayerObject->SetTileCollisionEnable(false);
	}

	if (CSceneManager::GetInst()->GetNextScene())
	{
		CScene* NextScene = CSceneManager::GetInst()->GetNextScene();
		CViewport* NewViewport = NextScene->GetViewport();

		CClientManager::GetInst()->GetInventoryWindow()->ToolTipWindowSceneChange(NextScene, NewViewport);
	}

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

	if (m_LowerClassBookList.size() < 5)
	{
		int Count = 0;
		auto iter = m_DeadPos.begin();
		auto iterEnd = m_DeadPos.end();

		for (; iter != iterEnd; )
		{
			CLowerClassBook* Book = m_Scene->CreateGameObject<CLowerClassBook>("LowerClassBook");
			Book->SetWorldPos((*iter));
			Book->GetRootComponent()->SetLayerName("MovingObjFront");
			iter = m_DeadPos.erase(iter);
			iterEnd = m_DeadPos.end();
			((CSpriteComponent*)Book->GetRootComponent())->SetOpacity(0.5f);

			m_LowerClassBookList.push_back(Book);
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
	CreateMonsterAnimationSequence();
	CreateEffectPrototype();
}

void CLibrary2ndScene::CreatePlayerAnimationSequence()
{
	m_Scene->GetResource()->LoadSequence2D("PlayerIdleLeft.sqc");
	m_Scene->GetResource()->LoadSequence2D("PlayerWalkLeft.sqc");
	m_Scene->GetResource()->LoadSequence2D("PlayerHealLeft.sqc");
	m_Scene->GetResource()->LoadSequence2D("PlayerRope.sqc");
	m_Scene->GetResource()->LoadSequence2D("PlayerJumpLeft.sqc");
	m_Scene->GetResource()->LoadSequence2D("PlayerLevelUpEffect.sqc");
	m_Scene->GetResource()->LoadSequence2D("PlayerDead.sqc");
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

	m_Scene->GetResource()->LoadSequence2D("DeathSide.sqc");
	m_Scene->GetResource()->LoadSequence2D("DeathSideHitEffect.sqc");
}

void CLibrary2ndScene::CreateMonsterAnimationSequence()
{
	m_Scene->GetResource()->LoadSequence2D("LowerClassBookIdleLeft.sqc");
	m_Scene->GetResource()->LoadSequence2D("LowerClassBookHitLeft.sqc");
	m_Scene->GetResource()->LoadSequence2D("LowerClassBookDieLeft.sqc");
	m_Scene->GetResource()->LoadSequence2D("LowerClassBookAttackLeft.sqc");
	m_Scene->GetResource()->LoadSequence2D("LowerClassBookAttackOrb.sqc");
}

void CLibrary2ndScene::CreateMapAnimationSequence()
{
	m_Scene->GetResource()->LoadSequence2D("Portal.sqc");
	m_Scene->GetResource()->LoadSequence2D("VerticalLampLight.sqc");
	m_Scene->GetResource()->LoadSequence2D("LampBigHelix.sqc");
	m_Scene->GetResource()->LoadSequence2D("Library2ndButterfly.sqc");
}

void CLibrary2ndScene::CreateEffectPrototype()
{
	CSylphideLancerHitEffect* SylphideLancerHitEffect = m_Scene->CreatePrototype<CSylphideLancerHitEffect>("SylphideLancerHitEffect");
	CVoidPressureHitEffect* VoidPressureHitEffect = m_Scene->CreatePrototype<CVoidPressureHitEffect>("VoidPressureHitEffect");
	CDeathSideHitEffect* DeathSideHitEffect = m_Scene->CreatePrototype<CDeathSideHitEffect>("DeathSideHitEffect");
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

void CLibrary2ndScene::AddTileCollisionCallback()
{
	/*CTileObject* FloorTile = (CTileObject*)m_Scene->FindObject("FloorTile");

	if (FloorTile)
	{
		CColliderBox2D* Collider = (CColliderBox2D*)FloorTile->FindComponent("FloorTileCollider");
		Collider->AddCollisionCallback<CTileObject>(Collision_State::Begin, FloorTile, &CTileObject::CollisionBeginCallback);
		Collider->AddCollisionCallback<CTileObject>(Collision_State::End, FloorTile, &CTileObject::CollisionEndCallback);
	}*/
}

void CLibrary2ndScene::DeleteLowerClassBook(const std::string& Name)
{
	auto iter = m_LowerClassBookList.begin();
	auto iterEnd = m_LowerClassBookList.end();

	for (; iter != iterEnd; ++iter)
	{
		if ((*iter)->GetName() == Name)
		{
			m_LowerClassBookList.erase(iter);
			return;
		}
	}
}

void CLibrary2ndScene::DeleteLowerClassBook(CLowerClassBook* Monster)
{
	auto iter = m_LowerClassBookList.begin();
	auto iterEnd = m_LowerClassBookList.end();

	for (; iter != iterEnd; ++iter)
	{
		if ((*iter) == Monster)
		{
			m_LowerClassBookList.erase(iter);
			return;
		}
	}
}

void CLibrary2ndScene::CreateLobbyScene()
{
	m_Scene->GetResource()->SoundStop("FairyAcademyBGM");

	CSceneManager::GetInst()->CreateNextScene(false);
	CLobbyScene* LobbyScene = CSceneManager::GetInst()->CreateSceneModeEmpty<CLobbyScene>(false);

	LobbyScene->SetPlayerObject(m_PlayerObject);

	// 다음 Scene에서의 위치를 Scene의 왼쪽에 위치하도록 잡아주기
	Vector3 WorldPos = m_PlayerObject->GetWorldPos();
	m_PlayerObject->SetWorldPos(981.f, 650.f, WorldPos.z);

	if (!m_PlayerObject->IsGravity())
		m_PlayerObject->SetGravity(true);

	//SAFE_DELETE(m_LoadingThread);
	m_LoadingThread = CThread::CreateThread<CLoadingThread>("LobbySceneLoadingThread");
	m_LoadingThread->SetLoadingScene(ThreadLoadingScene::Lobby);

	if (((CPlayer2D*)m_PlayerObject.Get())->IsDead())
	{
		((CPlayer2D*)m_PlayerObject.Get())->ReturnAlive();
	}

	m_LoadingThread->Start();
}

void CLibrary2ndScene::CreateWayToZakumScene()
{
	m_Scene->GetResource()->SoundStop("FairyAcademyBGM");

	CSceneManager::GetInst()->CreateNextScene(false);
	CWayToZakumScene* WayToZakumScene = CSceneManager::GetInst()->CreateSceneModeEmpty<CWayToZakumScene>(false);

	WayToZakumScene->SetPlayerObject(m_PlayerObject);


	// 다음 Scene에서의 위치를 Scene의 왼쪽에 위치하도록 잡아주기
	Vector3 WorldPos = m_PlayerObject->GetWorldPos();
	m_PlayerObject->SetWorldPos(250.f, 200.f, WorldPos.z);

	m_LoadingThread = CThread::CreateThread<CLoadingThread>("WayToZakumSceneLoadingThread");
	m_LoadingThread->SetLoadingScene(ThreadLoadingScene::WayToZakum);

	m_LoadingThread->Start();
}

CLowerClassBook* CLibrary2ndScene::FindLowerClassBook(bool Right, const Vector3& MyPos, float DistXConstraint, float DistYConstraint)
{
	auto iter = m_LowerClassBookList.begin();
	auto iterEnd = m_LowerClassBookList.end();

	for (; iter != iterEnd; ++iter)
	{
		if ((*iter)->GetTypeID() == typeid(CLowerClassBook).hash_code())
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
