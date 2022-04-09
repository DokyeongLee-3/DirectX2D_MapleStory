
#include "LobbyScene.h"
#include "OnionScene.h"
#include "WayToZakumScene.h"
#include "Library2ndScene.h"
#include "Scene/Scene.h"
#include "Scene/SceneResource.h"
#include "Scene/SceneManager.h"
#include "../Object/Player2D.h"
#include "Input.h"
#include "PathManager.h"
#include "Render/RenderManager.h"
#include "../ClientManager.h"
#include "LoadingThread.h"

#include "../Object/DustParticle.h"


CLobbyScene::CLobbyScene()	:
	m_LoadingThread(nullptr)
{
	SetTypeID<CLobbyScene>();
}

CLobbyScene::~CLobbyScene()
{
	SAFE_DELETE(m_LoadingThread);

	CPlayer2D* Player = (CPlayer2D*)(m_Scene->GetPlayerObject());

	if (Player)
	{
		Player->SetVoidPressure(nullptr);
		Player->SetVoidPressureOrb(nullptr);
	}
}

void CLobbyScene::SetStageObject(CStage* Stage)
{
	m_StageObject = Stage;
}


void CLobbyScene::Start()
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

bool CLobbyScene::Init()
{
	CreateAnimationSequence();

	LoadSound();

	if (!m_PlayerObject)
	{
		CPlayer2D* Player = m_Scene->CreateGameObject<CPlayer2D>("Player");

		SetPlayerObject(Player);
		Player->GetRootComponent()->SetLayerName("MovingObjFront");
		CWidgetComponent* DamageFontWidget = (CWidgetComponent*)Player->FindComponent("PlayerDamageFont");
		DamageFontWidget->SetLayerName("ScreenWidgetComponent");
		CWidgetComponent* PlayerNameWidget = (CWidgetComponent*)Player->FindComponent("PlayerNameWidgetComponent");
		PlayerNameWidget->SetLayerName("ScreenWidgetComponent");

		Player->SetWorldPos(400.f, 200.f, 610.f);

		CSceneComponent* BodyEffect = (CSceneComponent*)Player->FindComponent("PlayerSkillBodyEffect");

		BodyEffect->SetRelativePos(0.f, 11.f, -10.f);
	}

	CreateInGameWidgetWindow();


	m_Scene->GetResource()->SoundPlay("FairyAcademyBGM");

	// 요정학원 로비 Scene 만들어놓은 scn파일까지의 경로
	const PathInfo* Info = CPathManager::GetInst()->FindPath(SCENE_PATH);

	char	FullPath[MAX_PATH] = {};

	if (Info)
		strcpy_s(FullPath, Info->PathMultibyte);

	strcat_s(FullPath, "FairyAcademyLobby.scn");

	m_Scene->LoadFullPath(FullPath);

	AddTileCollisionCallback();

	if (CSceneManager::GetInst()->GetNextScene())
	{
		CScene* NextScene = CSceneManager::GetInst()->GetNextScene();
		CViewport* NewViewport = NextScene->GetViewport();

		CClientManager::GetInst()->GetInventoryWindow()->ToolTipWindowSceneChange(NextScene, NewViewport);
	}

	//CreateMaterial();
	//CDustParticle* DustParticle = m_Scene->CreateGameObject<CDustParticle>("DustParticle");

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

void CLobbyScene::CreateMaterial()
{
	// 자쿰팔 내려칠때 흙먼지 이펙트
	m_Scene->GetResource()->CreateMaterial<CMaterial>("Dust");
	CMaterial* Material = m_Scene->GetResource()->FindMaterial("Dust");

	Material->AddTexture(0, (int)Buffer_Shader_Type::Pixel, "Dust", TEXT("smokeparticle.png"));

	Material->SetShader("ParticleRenderShader");
	Material->SetRenderState("AlphaBlend");

	m_Scene->GetResource()->CreateParticle("Dust");
	CParticle* Particle = m_Scene->GetResource()->FindParticle("Dust");

	Particle->SetMaterial(Material);
	Particle->SetSpawnCountMax(800);
	Particle->SetLifeTimeMin(1.f);
	Particle->SetLifeTimeMax(1.5f);
	Particle->SetScaleMin(Vector3(64.f, 64.f, 1.f));
	Particle->SetScaleMax(Vector3(64.f, 64.f, 1.f));
	Particle->SetSpeedMin(50.f);
	Particle->SetSpeedMax(60.f);
	Particle->SetMoveDir(Vector3(0.f, 1.f, 0.f));
	Particle->SetStartMin(Vector3(-40.f, -10.f, 0.f));
	Particle->SetStartMax(Vector3(40.f, 10.f, 0.f));
	Particle->SetColorMin(Vector4(1.f, 1.f, 1.f, 0.9f));
	Particle->SetColorMax(Vector4(1.f, 1.f, 1.f, 0.f));
	Particle->SetMoveAngle(Vector3(0.f, 0.f, 90.f));
	//Particle->SetGravity(true);
	Particle->SetMove(true);
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
	m_Scene->GetResource()->LoadSequence2D("PlayerLevelUpEffect.sqc");
}

void CLobbyScene::CreateSkillAnimationSequence()
{
	m_Scene->GetResource()->LoadSequence2D("Blank.sqc");

	m_Scene->GetResource()->LoadSequence2D("PlayerOrb.sqc");

	m_Scene->GetResource()->LoadSequence2D("SylphideLancerMuzzleLeft.sqc");
	m_Scene->GetResource()->LoadSequence2D("SylphideLancerBodyEffectLeft.sqc");
	m_Scene->GetResource()->LoadSequence2D("SylphideLancerArrowLeft.sqc");
	m_Scene->GetResource()->LoadSequence2D("SylphideLancerArrowPurple.sqc");

	m_Scene->GetResource()->LoadSequence2D("VoidPressureStartLeft.sqc");
	m_Scene->GetResource()->LoadSequence2D("VoidPressureReadyLeft.sqc");
	m_Scene->GetResource()->LoadSequence2D("VoidPressureDestroy.sqc");
	m_Scene->GetResource()->LoadSequence2D("VoidPressureOrb.sqc");
	m_Scene->GetResource()->LoadSequence2D("VoidPressureOrbDestroy.sqc");

	m_Scene->GetResource()->LoadSequence2D("LightTransformingLeft.sqc");

	m_Scene->GetResource()->LoadSequence2D("DeathSide.sqc");
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

void CLobbyScene::LoadSound()
{
	m_Scene->GetResource()->LoadSound("BGM", true, "FairyAcademyBGM", "TheFairyAcademy.mp3");

	m_Scene->GetResource()->LoadSound("Effect", false, "Jump", "Jump.mp3");

	m_Scene->GetResource()->LoadSound("Effect", false, "SylphideLancerUse", "SylphideLancerUse.mp3");

	m_Scene->GetResource()->LoadSound("Effect", false, "VoidPressureUse", "VoidPressureUse.mp3");
	m_Scene->GetResource()->LoadSound("Effect", false, "VoidPressureLoop", "VoidPressureLoop.mp3");
	m_Scene->GetResource()->LoadSound("Effect", false, "VoidPressureEnd", "VoidPressureEnd.mp3");

	m_Scene->GetResource()->LoadSound("Effect", false, "LightTransforming", "LightTransformingUse.mp3");

	m_Scene->GetResource()->LoadSound("Effect", false, "DeathSideUse", "DeathSideUse.mp3");
	m_Scene->GetResource()->LoadSound("Effect", false, "DeathSideVoice1", "DeathSideVoice1.mp3");
	m_Scene->GetResource()->LoadSound("Effect", false, "DeathSideVoice2", "DeathSideVoice2.mp3");

	m_Scene->GetResource()->LoadSound("Effect", false, "LevelUp", "LevelUp.mp3");
	m_Scene->GetResource()->LoadSound("Effect", false, "PickUpItem", "PickUpItem.mp3");
	m_Scene->GetResource()->LoadSound("Effect", false, "DropItem", "DropItem.mp3");
	m_Scene->GetResource()->LoadSound("Effect", false, "EatItem", "ItemEat.mp3");

	m_Scene->GetResource()->LoadSound("UI", false, "TabClick", "TabClick.mp3");
	m_Scene->GetResource()->LoadSound("UI", false, "UIOpen", "UIOpen.mp3");
}

void CLobbyScene::AddTileCollisionCallback()
{
	//CTileObject* FloorTile = (CTileObject*)m_Scene->FindObject("LobbyTileObj");

	//if (FloorTile)
	//{
	//	CColliderBox2D* Collider = (CColliderBox2D*)FloorTile->FindComponent("LobbyTileCollider");
	//	Collider->AddCollisionCallback<CTileObject>(Collision_State::Begin, FloorTile, &CTileObject::CollisionBeginCallback);
	//	//Collider->AddCollisionCallback<CTileObject>(Collision_State::Stay, FloorTile, &CTileObject::CollisionStayCallback);
	//	Collider->AddCollisionCallback<CTileObject>(Collision_State::End, FloorTile, &CTileObject::CollisionEndCallback);
	//}

	for (int i = 1; i <= 10; ++i)
	{
		char Leaf[128] = {};
		sprintf_s(Leaf, "Leaf%d", i);
		CStaticMapObj* LeafObj = (CStaticMapObj*)m_Scene->FindObject(Leaf);

		if (LeafObj)
		{
			CColliderBox2D* LeafCollider = (CColliderBox2D*)LeafObj->FindComponent("LeafCollider");

			if(LeafCollider)
			{
				LeafCollider->AddCollisionCallback<CStaticMapObj>(Collision_State::Begin, LeafObj, &CStaticMapObj::CollisionBeginCallback);
				//LeafCollider->AddCollisionCallback<CStaticMapObj>(Collision_State::Stay, LeafObj, &CStaticMapObj::CollisionStayCallback);
				LeafCollider->AddCollisionCallback<CStaticMapObj>(Collision_State::End, LeafObj, &CStaticMapObj::CollisionEndCallback);

				LeafObj->SetCollisionID(i);
			}
		}
	}

	CStaticMapObj* Step = (CStaticMapObj*)m_Scene->FindObject("RightEntrance");

	if (Step)
	{
		for (int i = 0; i <= 1; ++i)
		{
			char StepName[128] = {};
			sprintf_s(StepName, "Step%d", i);
			CColliderBox2D* StepCollider = (CColliderBox2D*)Step->FindComponent(StepName);

			if (StepCollider)
			{
				StepCollider->AddCollisionCallback<CStaticMapObj>(Collision_State::Begin, Step, &CStaticMapObj::CollisionBeginCallback);
				StepCollider->AddCollisionCallback<CStaticMapObj>(Collision_State::End, Step, &CStaticMapObj::CollisionEndCallback);

				Step->SetCollisionID(i + 11);
			}

		}
	}

}

void CLobbyScene::CreateInGameWidgetWindow()
{
	if (!CClientManager::GetInst()->GetConfigurationWindow())
	{
		m_ConfigurationWindow = m_Scene->GetViewport()->CreateWidgetWindow<CConfigurationWindow>("ConfigurationWindow");
		m_ConfigurationWindow->SetZOrder(2);
		m_ConfigurationWindow->SetPos(200.f, 200.f);
		m_ConfigurationWindow->Enable(false);
		CClientManager::GetInst()->SetConfigurationWindow(m_ConfigurationWindow);
	}

	else
	{
		CConfigurationWindow* ConfigurationWindow = CClientManager::GetInst()->GetConfigurationWindow();
		m_Scene->GetViewport()->AddWindow(ConfigurationWindow);
	}

	if (!CClientManager::GetInst()->GetInventoryWindow())
	{
		m_Inventory = m_Scene->GetViewport()->CreateWidgetWindow<CInventory>("Inventory");
		m_Inventory->SetZOrder(2);
		m_Inventory->SetPos(250.f, 150.f);
		m_Inventory->Enable(false);
		CClientManager::GetInst()->SetInventoryWindow(m_Inventory);
	}

	else
	{
		CInventory* Inventory = CClientManager::GetInst()->GetInventoryWindow();
		m_Scene->GetViewport()->AddWindow(Inventory);
	}

	if (!CClientManager::GetInst()->GetBossMatchingWindow())
	{
		m_BossMatchingWindow = m_Scene->GetViewport()->CreateWidgetWindow<CBossMatching>("BossMatching");
		m_BossMatchingWindow->SetZOrder(2);
		m_BossMatchingWindow->SetPos(200.f, 100.f);
		m_BossMatchingWindow->Enable(false);
		CClientManager::GetInst()->SetBossMatchingWindow(m_BossMatchingWindow);
	}

	else
	{
		CBossMatching* BossMatching = CClientManager::GetInst()->GetBossMatchingWindow();
		m_Scene->GetViewport()->AddWindow(BossMatching);
	}

	if (!CClientManager::GetInst()->GetCharacterStatusWindow())
	{
		m_CharacterStatusWindow = m_Scene->GetViewport()->CreateWidgetWindow<CCharacterStatusWindow>("MainStatus");
		m_CharacterStatusWindow->SetPos(550.f, 13.f);
		m_CharacterStatusWindow->SetZOrder(1);
		m_CharacterStatusWindow->SetMouseCollisionEnable(false);
		CClientManager::GetInst()->SetCharacterStatusWindow(m_CharacterStatusWindow);
	}

	else
	{
		CCharacterStatusWindow* StatusWindow = CClientManager::GetInst()->GetCharacterStatusWindow();
		m_Scene->GetViewport()->AddWindow(StatusWindow);
	}

	if (!CClientManager::GetInst()->GetCharacterEXPWindow())
	{
		m_CharacterEXPWindow = m_Scene->GetViewport()->CreateWidgetWindow<CCharacterEXP>("EXPWindow");
		m_CharacterEXPWindow->SetZOrder(1);
		m_CharacterEXPWindow->SetMouseCollisionEnable(false);
		CClientManager::GetInst()->SetEXPWindow(m_CharacterEXPWindow);
	}

	else
	{
		CCharacterEXP* EXPWindow = CClientManager::GetInst()->GetCharacterEXPWindow();
		m_Scene->GetViewport()->AddWindow(EXPWindow);
	}

	if (!CClientManager::GetInst()->GetSkillQuickSlotWindow())
	{
		m_SkillQuickSlot = m_Scene->GetViewport()->CreateWidgetWindow<CSkillQuickSlotWindow>("SkillQuickSlot");
		m_SkillQuickSlot->SetZOrder(1);
		CClientManager::GetInst()->SetSkillQuickSlot(m_SkillQuickSlot);
	}

	else
	{
		CSkillQuickSlotWindow* SkillQuickSlot = CClientManager::GetInst()->GetSkillQuickSlotWindow();
		m_Scene->GetViewport()->AddWindow(SkillQuickSlot);
	}

	if (!CClientManager::GetInst()->GetStatWindow())
	{
		m_StatWindow = m_Scene->GetViewport()->CreateWidgetWindow<CStatWindow>("Stat");
		m_StatWindow->SetZOrder(2);
		m_StatWindow->SetPos(200.f, 100.f);
		m_StatWindow->Enable(false);
		CClientManager::GetInst()->SetStatWindow(m_StatWindow);
	}
	
	else
	{
		CStatWindow* StatWindow = CClientManager::GetInst()->GetStatWindow();
		m_Scene->GetViewport()->AddWindow(StatWindow);
	}

	if (!CClientManager::GetInst()->GetDyingNoticeWindow())
	{
		m_DyingNoticeWindow = m_Scene->GetViewport()->CreateWidgetWindow<CDyingNoticeWindow>("DyingNoticeWindow");
		m_DyingNoticeWindow->SetZOrder(2);
		m_DyingNoticeWindow->SetPos(490.f, 300.f);
		m_DyingNoticeWindow->Enable(false);
		CClientManager::GetInst()->SetDyingNoticeWindow(m_DyingNoticeWindow);
	}

	else
	{
		CDyingNoticeWindow* DyingNoticeWindow = CClientManager::GetInst()->GetDyingNoticeWindow();
		m_Scene->GetViewport()->AddWindow(DyingNoticeWindow);
	}

	if (!CClientManager::GetInst()->GetSkillPointWindow())
	{
		m_SkillPointWindow = m_Scene->GetViewport()->CreateWidgetWindow<CSkillPointWindow>("SkillPointWindow");
		m_SkillPointWindow->SetZOrder(2);
		m_SkillPointWindow->SetPos(520.f, 320.f);
		m_SkillPointWindow->Enable(false);
		CClientManager::GetInst()->SetSkillPointWindow(m_SkillPointWindow);
	}

	else
	{
		CSkillPointWindow* SkillPointWindow = CClientManager::GetInst()->GetSkillPointWindow();
		m_Scene->GetViewport()->AddWindow(SkillPointWindow);
	}
}

void CLobbyScene::CreateOnionScene()
{
	m_Scene->GetResource()->SoundStop("FairyAcademyBGM");

	CSceneManager::GetInst()->CreateNextScene(false);
	COnionScene* OnionScene = CSceneManager::GetInst()->CreateSceneModeEmpty<COnionScene>(false);

	OnionScene->SetPlayerObject(m_PlayerObject);

	((CPlayer2D*)m_PlayerObject.Get())->SetTileCollisionEnable(false);

	// 다음 Scene에서의 위치를 Scene의 왼쪽에 위치하도록 잡아주기
	Vector3 WorldPos = m_PlayerObject->GetWorldPos();
	m_PlayerObject->SetWorldPos(170.f, 270.f, WorldPos.z);

	m_LoadingThread = CThread::CreateThread<CLoadingThread>("OnionSceneLoadingThread");
	m_LoadingThread->SetLoadingScene(ThreadLoadingScene::Onion);

	m_LoadingThread->Start();
}

void CLobbyScene::CreateWayToZakumScene()
{
	m_Scene->GetResource()->SoundStop("FairyAcademyBGM");

	CSceneManager::GetInst()->CreateNextScene(false);
	CWayToZakumScene* WayToZakumScene = CSceneManager::GetInst()->CreateSceneModeEmpty<CWayToZakumScene>(false);

	WayToZakumScene->SetPlayerObject(m_PlayerObject);

	((CPlayer2D*)m_PlayerObject.Get())->SetTileCollisionEnable(false);

	// 다음 Scene에서의 위치를 Scene의 왼쪽에 위치하도록 잡아주기
	Vector3 WorldPos = m_PlayerObject->GetWorldPos();
	m_PlayerObject->SetWorldPos(250.f, 200.f, WorldPos.z);

	m_LoadingThread = CThread::CreateThread<CLoadingThread>("WayToZakumSceneLoadingThread");
	m_LoadingThread->SetLoadingScene(ThreadLoadingScene::WayToZakum);

	m_LoadingThread->Start();
}

void CLobbyScene::CreateLibrary2ndScene()
{
	m_Scene->GetResource()->SoundStop("FairyAcademyBGM");

	CSceneManager::GetInst()->CreateNextScene(false);
	CLibrary2ndScene* LibraryScene = CSceneManager::GetInst()->CreateSceneModeEmpty<CLibrary2ndScene>(false);

	LibraryScene->SetPlayerObject(m_PlayerObject);

	((CPlayer2D*)m_PlayerObject.Get())->SetTileCollisionEnable(false);

	// 다음 Scene에서의 위치를 Scene의 왼쪽에 위치하도록 잡아주기
	Vector3 WorldPos = m_PlayerObject->GetWorldPos();
	m_PlayerObject->SetWorldPos(510.f, 200.f, WorldPos.z);

	m_LoadingThread = CThread::CreateThread<CLoadingThread>("Library2ndSceneLoadingThread");
	m_LoadingThread->SetLoadingScene(ThreadLoadingScene::Library2nd);

	m_LoadingThread->Start();
}
