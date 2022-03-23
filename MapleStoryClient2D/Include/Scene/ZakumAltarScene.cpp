
#include "ZakumAltarScene.h"
#include "WayToZakumScene.h"
#include "Scene/Scene.h"
#include "Scene/SceneResource.h"
#include "Scene/SceneManager.h"
#include "../Object/Player2D.h"
#include "../Object/ZakumAltarFlowingVolcano.h"
#include "../Object/SylphideLancerHitEffect.h"
#include "../Object/VoidPressureHitEffect.h"
#include "../Object/DeathSideHitEffect.h"
#include "PathManager.h"
#include "../ClientManager.h"
#include "LoadingThread.h"
#include "Render/RenderManager.h"
#include "../Widget/BossInfoWindow.h"
#include "../Object/NPCAmon.h"

CZakumAltarScene::CZakumAltarScene()
{
	SetTypeID<CZakumAltarScene>();
}

CZakumAltarScene::~CZakumAltarScene()
{
	SAFE_DELETE(m_LoadingThread);

	CPlayer2D* Player = (CPlayer2D*)(m_Scene->GetPlayerObject());

	if (Player)
	{
		Player->SetVoidPressure(nullptr);
		Player->SetVoidPressureOrb(nullptr);
	}
}

void CZakumAltarScene::SetStageObject(CStage* Stage)	
{
	m_StageObject = Stage;
}

void CZakumAltarScene::Start()
{
	CSceneMode::Start();

	if (m_PlayerObject)
	{
		CWidgetWindow* Window = ((CPlayer2D*)m_PlayerObject.Get())->GetDamageWidgetComponent()->GetWidgetWindow();

		Window->SetViewport(m_Scene->GetViewport());

		Window->GetViewport()->SetScene(m_Scene);
	}
}

bool CZakumAltarScene::Init()
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

	CBossInfoWindow* BossInfo = m_Scene->GetViewport()->CreateWidgetWindow<CBossInfoWindow>("BossInfoWindow");

	CreateAnimationSequence();
	CreateMaterial();
	CreateParticle();
	LoadSound();

	// 자쿰으로 통하는 길 Scene 만들어놓은 scn파일까지의 경로
	const PathInfo* Info = CPathManager::GetInst()->FindPath(SCENE_PATH);

	char	FullPath[MAX_PATH] = {};

	if (Info)
		strcpy_s(FullPath, Info->PathMultibyte);

	strcat_s(FullPath, "ZakumAltar.scn");

	m_Scene->LoadFullPath(FullPath);

	m_Scene->GetResource()->SoundPlay("ZakumAltarBGM");

	if (m_PlayerObject)
	{
		m_PlayerObject->SetGravity(true);
		m_PlayerObject->SetTileCollisionEnable(false);
	}

	// 맵 하단에 왼쪽으로 흐르는 용암
	CZakumAltarFlowingVolcano* Volcano = m_Scene->CreateGameObject<CZakumAltarFlowingVolcano>("ZakumAltarFlowingVolcano");

	AddFloorCollider();

	return true;
}

void CZakumAltarScene::Update(float DeltaTime)
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

	//CNPCAmon* Amon = m_Scene->CreateObject<CNPCAmon>("NPCAMon");
	//Amon->
}

void CZakumAltarScene::PostUpdate(float DeltaTime)
{
	CSceneMode::PostUpdate(DeltaTime);
}

void CZakumAltarScene::CreateMaterial()
{
	m_Scene->GetResource()->CreateMaterial<CMaterial>("ZakumAltarFlowingVolcano");
	CMaterial* Material = m_Scene->GetResource()->FindMaterial("ZakumAltarFlowingVolcano");

	Material->AddTexture(0, (int)Buffer_Shader_Type::Pixel, "ZakumAltarFlowingVolcano", TEXT("Map/ZakumAltar/ZakumAltarFlowingVolcano.png"));

	Material->SetShader("MovingTileShader");
	Material->SetRenderState("AlphaBlend");

	// 자쿰팔 내려칠때 흙먼지 이펙트
	/*m_Scene->GetResource()->CreateMaterial<CMaterial>("Dust");
	Material = m_Scene->GetResource()->FindMaterial("Dust");

	Material->AddTexture(0, (int)Buffer_Shader_Type::Pixel, "Dust", TEXT("smokeparticle.png"));

	Material->SetShader("ParticleRenderShader");
	Material->SetRenderState("AlphaBlend");*/
}

void CZakumAltarScene::CreateParticle()
{
	//m_Scene->GetResource()->CreateParticle("DustLeft");
	//CParticle* Particle = m_Scene->GetResource()->FindParticle("DustLeft");

	//CMaterial* Material = m_Scene->GetResource()->FindMaterial("Dust");

	//Particle->SetMaterial(Material);
	//Particle->SetSpawnCountMax(800);
	//Particle->SetLifeTimeMin(1.f);
	//Particle->SetLifeTimeMax(1.5f);
	//Particle->SetScaleMin(Vector3(64.f, 64.f, 1.f));
	//Particle->SetScaleMax(Vector3(64.f, 64.f, 1.f));
	//Particle->SetSpeedMin(50.f);
	//Particle->SetSpeedMax(60.f);
	//Particle->SetMoveDir(Vector3(0.f, 1.f, 0.f));
	//Particle->SetStartMin(Vector3(-40.f, -10.f, 0.f));
	//Particle->SetStartMax(Vector3(40.f, 10.f, 0.f));
	//Particle->SetColorMin(Vector4(1.f, 1.f, 1.f, 0.9f));
	//Particle->SetColorMax(Vector4(1.f, 1.f, 1.f, 0.f));
	//Particle->SetMoveAngle(Vector3(0.f, 0.f, 90.f));
	////Particle->SetGravity(true);
	//Particle->SetMove(true);

	//////////////////////////////////////////

	//m_Scene->GetResource()->CreateParticle("DustRight");
	//Particle = m_Scene->GetResource()->FindParticle("DustRight");

	//Material = m_Scene->GetResource()->FindMaterial("Dust");

	//Particle->SetMaterial(Material);
	//Particle->SetSpawnCountMax(700);
	//Particle->SetLifeTimeMin(1.f);
	//Particle->SetLifeTimeMax(1.5f);
	//Particle->SetScaleMin(Vector3(64.f, 64.f, 1.f));
	//Particle->SetScaleMax(Vector3(64.f, 64.f, 1.f));
	//Particle->SetSpeedMin(30.f);
	//Particle->SetSpeedMax(50.f);
	//Particle->SetMoveDir(Vector3(0.7f, 0.25f, 0.f));
	//Particle->SetStartMin(Vector3(-40.f, -10.f, 0.f));
	//Particle->SetStartMax(Vector3(40.f, 10.f, 0.f));
	//Particle->SetColorMin(Vector4(1.f, 1.f, 1.f, 0.9f));
	//Particle->SetColorMax(Vector4(1.f, 1.f, 1.f, 0.f));
	//Particle->SetMoveAngle(Vector3(0.f, 0.f, 10.f));
	////Particle->SetGravity(true);
	//Particle->SetMove(true);

	////////////////////////////////////////

	//m_Scene->GetResource()->CreateParticle("DustCenter");
	//Particle = m_Scene->GetResource()->FindParticle("DustCenter");

	//Material = m_Scene->GetResource()->FindMaterial("Dust");

	//Particle->SetMaterial(Material);
	//Particle->SetSpawnCountMax(700);
	//Particle->SetLifeTimeMin(1.f);
	//Particle->SetLifeTimeMax(1.5f);
	//Particle->SetScaleMin(Vector3(64.f, 64.f, 1.f));
	//Particle->SetScaleMax(Vector3(64.f, 64.f, 1.f));
	//Particle->SetSpeedMin(10.f);
	//Particle->SetSpeedMax(30.f);
	//Particle->SetMoveDir(Vector3(0.f, 1.f, 0.f));
	//Particle->SetStartMin(Vector3(-20.f, -20.f, 0.f));
	//Particle->SetStartMax(Vector3(20.f, 20.f, 0.f));
	//Particle->SetColorMin(Vector4(1.f, 1.f, 1.f, 0.9f));
	//Particle->SetColorMax(Vector4(1.f, 1.f, 1.f, 0.f));
	//Particle->SetMoveAngle(Vector3(0.f, 0.f, 90.f));
	////Particle->SetGravity(true);
	//Particle->SetMove(true);
}

void CZakumAltarScene::CreateAnimationSequence()
{
	CreatePlayerAnimationSequence();
	CreateSkillAnimationSequence();
	CreateMonsterAnimationSequence();
	CreateMapAnimationSequence();
	CreateEffectPrototype();
}

void CZakumAltarScene::CreateEffectPrototype()
{
	CSylphideLancerHitEffect* SylphideLancerHitEffect = m_Scene->CreatePrototype<CSylphideLancerHitEffect>("SylphideLancerHitEffect");
	CVoidPressureHitEffect* VoidPressureHitEffect = m_Scene->CreatePrototype<CVoidPressureHitEffect>("VoidPressureHitEffect");
	CDeathSideHitEffect* DeathSideHitEffect = m_Scene->CreatePrototype<CDeathSideHitEffect>("DeathSideHitEffect");
}

void CZakumAltarScene::CreatePlayerAnimationSequence()
{
	m_Scene->GetResource()->LoadSequence2D("PlayerIdleLeft.sqc");
	m_Scene->GetResource()->LoadSequence2D("PlayerWalkLeft.sqc");
	m_Scene->GetResource()->LoadSequence2D("PlayerHealLeft.sqc");
	m_Scene->GetResource()->LoadSequence2D("PlayerRope.sqc");
	m_Scene->GetResource()->LoadSequence2D("PlayerJumpLeft.sqc");
	m_Scene->GetResource()->LoadSequence2D("PlayerLevelUpEffect.sqc");
	m_Scene->GetResource()->LoadSequence2D("PlayerDead.sqc");
}

void CZakumAltarScene::CreateMonsterAnimationSequence()
{
	m_Scene->GetResource()->LoadSequence2D("ZakumNormalIdle.sqc");
	m_Scene->GetResource()->LoadSequence2D("ZakumDestroyIdle.sqc");
	m_Scene->GetResource()->LoadSequence2D("ZakumLeftHand.sqc");
	m_Scene->GetResource()->LoadSequence2D("ZakumRightHand.sqc");
	m_Scene->GetResource()->LoadSequence2D("ZakumLeftHandSmash.sqc");
	m_Scene->GetResource()->LoadSequence2D("ZakumLeftHandSmash2.sqc");
	m_Scene->GetResource()->LoadSequence2D("ZakumArmReturnLeft1.sqc");
	m_Scene->GetResource()->LoadSequence2D("ZakumArmReturnLeft2.sqc");
	m_Scene->GetResource()->LoadSequence2D("ZakumClapLeft.sqc");
	m_Scene->GetResource()->LoadSequence2D("ZakumClapRight.sqc");
	m_Scene->GetResource()->LoadSequence2D("ZakumPinkShockWave.sqc");

	m_Scene->GetResource()->LoadSequence2D("NPCAmonIdle.sqc");
}

void CZakumAltarScene::CreateSkillAnimationSequence()
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

void CZakumAltarScene::CreateMapAnimationSequence()
{
}

void CZakumAltarScene::LoadSound()
{
	m_Scene->GetResource()->LoadSound("BGM", true, "ZakumAltarBGM", "ZakumAltarBGM.mp3");

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
	m_Scene->GetResource()->LoadSound("Effect", false, "DropItem", "DropItem.mp3");

	m_Scene->GetResource()->LoadSound("UI", false, "TabClick", "TabClick.mp3");
}

CColliderBox2D* CZakumAltarScene::FindZakumTarget(bool Right, const Vector3& MyPos, float DistXConstraint, float DistYConstraint)
{
	CZakumBody* Zakum = (CZakumBody*)m_Scene->FindObject("ZakumBody");

	if (Zakum)
	{
		std::vector<CColliderBox2D*>	vecCollider;

		Zakum->FindComponentFromType<CColliderBox2D>(vecCollider);
		CColliderBox2D* ZakumBody = (CColliderBox2D*)Zakum->FindComponent("Body");

		if (ZakumBody)
			vecCollider.push_back(ZakumBody);

		std::sort(vecCollider.begin(), vecCollider.end(), SortColliderByX);
		size_t Count = vecCollider.size();

		for (size_t i = 0; i < Count; ++i)
		{
			if (vecCollider[i]->GetName().find("DefenseCollider") != std::string::npos ||
				vecCollider[i]->GetName() == "Body")
			{
				Vector3 TargetPos = vecCollider[i]->GetWorldPos();

				if (Right && TargetPos.x < MyPos.x)
					continue;

				if (!Right && TargetPos.x > MyPos.x)
					continue;


				if (abs(TargetPos.x - MyPos.x) < DistXConstraint && abs(TargetPos.y - MyPos.y) < DistYConstraint)
				{
					if (!vecCollider[i]->IsEnable())
						continue;

					return vecCollider[i];
				}
			}
		}
	}

	return nullptr;
}

void CZakumAltarScene::SetPerspective()
{
}

void CZakumAltarScene::AddFloorCollider()
{
	for (int i = 0; i <= 2; ++i)
	{
		char Floor[128] = {};
		sprintf_s(Floor, "Floor%d", i);
		CStaticMapObj* FloorObj = (CStaticMapObj*)m_Scene->FindObject(Floor);

		if (FloorObj)
		{
			CColliderBox2D* FloorCollider = (CColliderBox2D*)FloorObj->FindComponent("FloorCollider");

			if (FloorCollider)
			{
				FloorCollider->AddCollisionCallback<CStaticMapObj>(Collision_State::Begin, FloorObj, &CStaticMapObj::CollisionBeginCallback);
				FloorCollider->AddCollisionCallback<CStaticMapObj>(Collision_State::End, FloorObj, &CStaticMapObj::CollisionEndCallback);

				FloorObj->SetCollisionID(i);
				FloorObj->SetIsFloor(true);
			}
		}
	}

	for (int i = 0; i <= 5; ++i)
	{
		char FootholdName[128] = {};
		sprintf_s(FootholdName, "Foothold%d", i);
		CTileObject* FootholdObj = (CTileObject*)m_Scene->FindObject(FootholdName);

		if (FootholdObj)
		{
			CColliderBox2D* FootholdCollider = (CColliderBox2D*)FootholdObj->FindComponent("FootholdCollider");

			if (FootholdCollider)
			{
				FootholdCollider->AddCollisionCallback<CTileObject>(Collision_State::Begin, FootholdObj, &CTileObject::CollisionBeginCallback);
				FootholdCollider->AddCollisionCallback<CTileObject>(Collision_State::End, FootholdObj, &CTileObject::CollisionEndCallback);
			}
		}
	}
}

void CZakumAltarScene::CreateWayToZakumScene()
{
	m_Scene->GetResource()->SoundStop("ZakumAltarBGM");

	CSceneManager::GetInst()->CreateNextScene(false);
	CWayToZakumScene* WayToZakumScene = CSceneManager::GetInst()->CreateSceneModeEmpty<CWayToZakumScene>(false);

	WayToZakumScene->SetPlayerObject(m_PlayerObject);

	// 다음 Scene에서의 위치를 Scene의 왼쪽에 위치하도록 잡아주기
	//Vector3 WorldPos = m_PlayerObject->GetWorldPos();
	m_PlayerObject->SetWorldPos(250.f, 200.f, 610.f);

	//SAFE_DELETE(m_LoadingThread);
	m_LoadingThread = CThread::CreateThread<CLoadingThread>("WayToZakumSceneLoadingThread");
	m_LoadingThread->SetLoadingScene(ThreadLoadingScene::WayToZakum);

	if (((CPlayer2D*)m_PlayerObject.Get())->IsDead())
	{
		((CPlayer2D*)m_PlayerObject.Get())->ReturnAlive();
	}

	m_LoadingThread->Start();

}
