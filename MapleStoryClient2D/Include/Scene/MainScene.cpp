
#include "MainScene.h"
#include "Scene/Scene.h"
#include "Scene/Viewport.h"
#include "../Object/Player2D.h"
#include "../Object/Monster.h"
#include "../Object/PixelTest.h"
#include "Scene/SceneResource.h"
#include "Scene/SceneManager.h"
#include "PathManager.h"
#include "Engine.h"
#include "../Object/StaticMapObj.h"
#include "Resource/Sound/SoundManager.h"
#include "../Widget/Inventory.h"
#include "../Widget/CharacterStatusWindow.h"
#include "../Widget/CharacterEXP.h"
#include "../Object/BubbleParticle.h"

CMainScene::CMainScene()
{
	SetTypeID<CMainScene>();
}

CMainScene::~CMainScene()
{
}

// CLoadingScene::Init에서 만든 LoadingThread가 실행해주는 중
bool CMainScene::Init()
{
	CreateMaterial();

	CreateAnimationSequence();

	CreateParticle();

	CPlayer2D* Player = m_Scene->CreateGameObject<CPlayer2D>("Player");

	SetPlayerObject(Player);

	Player->SetAllSceneComponentsLayer("MovingObjFront");

	/*for (int i = 0; i < 20; ++i)
	{
		char MonsterName[MAX_PATH] = {};

		sprintf_s(MonsterName,"Monster%d", i);

		CMonster* Monster = m_Scene->CreateGameObject<CMonster>(MonsterName);
		Monster->SetWorldPos(i * 50.f, i * 50.f, 0.f);
	}*/

	//CPixelTest* PixelTest = m_Scene->CreateGameObject<CPixelTest>("PixelTest");

	m_MainWidget = m_Scene->GetViewport()->CreateWidgetWindow<CMainWidget>("MainWidget");


	m_ConfigurationWindow = m_Scene->GetViewport()->CreateWidgetWindow<CConfigurationWindow>("ConfigurationWindow");
	m_ConfigurationWindow->SetZOrder(2);
	m_ConfigurationWindow->SetPos(200.f, 200.f);

	CInventory* Inventory = m_Scene->GetViewport()->CreateWidgetWindow<CInventory>("Inventory");
	Inventory->SetZOrder(3);
	Inventory->SetPos(300.f, 100.f);

	CCharacterStatusWindow* MainStatus = m_Scene->GetViewport()->CreateWidgetWindow<CCharacterStatusWindow>("MainStatus");
	MainStatus->SetPos(550.f, 30.f);

	CCharacterEXP* EXPWindow = m_Scene->GetViewport()->CreateWidgetWindow<CCharacterEXP>("EXPWindow");


	m_Scene->GetResource()->LoadSound("Master", true, "BGM", "GlacierAdventure.mp3");
	m_Scene->GetResource()->SoundPlay("BGM");


	CBubbleParticle* BubbleParticle = m_Scene->CreateGameObject<CBubbleParticle>("BubbleParticle");










	// For Test
	//TestLoadScene();

	return true;
}

void CMainScene::SetStageObject(CStage* Stage)
{
	m_StageObject = Stage;
}

void CMainScene::CreateMaterial()
{
	m_Scene->GetResource()->CreateMaterial<CMaterial>("Bubble");
	CMaterial* Material = m_Scene->GetResource()->FindMaterial("Bubble");

	Material->AddTexture(0, (int)Buffer_Shader_Type::Pixel, "Bubble", TEXT("Particle/Bubbles99px.png"));

	Material->SetShader("ParticleRenderShader");
}

void CMainScene::CreateAnimationSequence()
{
	CreatePlayerAnimationSequence();
	CreateSkillAnimationSequence();
	CreateMonsterAnimationSequence();
}

void CMainScene::CreateParticle()
{
	m_Scene->GetResource()->CreateParticle("Bubble");
	CParticle* Particle = m_Scene->GetResource()->FindParticle("Bubble");

	CMaterial* Material = m_Scene->GetResource()->FindMaterial("Bubble");

	Particle->SetMaterial(Material);
	Particle->SetSpawnCountMax(1000);
	Particle->SetLifeTimeMin(1.f);
	Particle->SetLifeTimeMax(2.f);
	Particle->SetScaleMin(Vector3(20.f, 20.f, 1.f));
	Particle->SetScaleMax(Vector3(50.f, 50.f, 1.f));
	Particle->SetSpeedMin(100.f);
	Particle->SetSpeedMax(300.f);
	Particle->SetMoveDir(Vector3(0.f, 1.f, 0.f));
	Particle->SetStartMin(Vector3(-300.f, -300.f, 0.f));
	Particle->SetStartMax(Vector3(300.f, 300.f, 0.f));
	Particle->SetMove(true);
}

void CMainScene::CreatePlayerAnimationSequence()
{
	m_Scene->GetResource()->LoadSequence2D("PlayerIdleLeft.sqc");
	m_Scene->GetResource()->LoadSequence2D("PlayerWalkLeft.sqc");
	m_Scene->GetResource()->LoadSequence2D("PlayerHealLeft.sqc");
	m_Scene->GetResource()->LoadSequence2D("PlayerRope.sqc");
	m_Scene->GetResource()->LoadSequence2D("PlayerJumpLeft.sqc");

}

void CMainScene::CreateSkillAnimationSequence()
{
	m_Scene->GetResource()->LoadSequence2D("PlayerOrb.sqc");

	m_Scene->GetResource()->LoadSequence2D("SylphideLancerBodyEffectLeft.sqc");
	m_Scene->GetResource()->LoadSequence2D("SylphideLancerArrowLeft.sqc");
}

void CMainScene::CreateMonsterAnimationSequence()
{
	m_Scene->GetResource()->LoadSequence2D("RadishIdleLeft.sqc");
	m_Scene->GetResource()->LoadSequence2D("RadishWalkLeft.sqc");
	m_Scene->GetResource()->LoadSequence2D("RadishHitLeft.sqc");
	m_Scene->GetResource()->LoadSequence2D("RadishDieLeft.sqc");
}

void CMainScene::TestLoadScene()
{
	TCHAR   FilePath[MAX_PATH] = {};

	OPENFILENAME    OpenFile = {};

	OpenFile.lStructSize = sizeof(OPENFILENAME);
	OpenFile.hwndOwner = CEngine::GetInst()->GetWindowHandle();
	OpenFile.lpstrFilter = TEXT("모든파일\0*.*\0Scene File\0*.scn");
	OpenFile.lpstrFile = FilePath;
	OpenFile.nMaxFile = MAX_PATH;
	OpenFile.lpstrInitialDir = CPathManager::GetInst()->FindPath(SCENE_PATH)->Path;

	if (GetOpenFileName(&OpenFile) != 0)
	{
		char    ConvertFullPath[MAX_PATH] = {};

		int Length = WideCharToMultiByte(CP_ACP, 0, FilePath, -1, 0, 0, 0, 0);
		WideCharToMultiByte(CP_ACP, 0, FilePath, -1, ConvertFullPath, Length, 0, 0);

		CSceneManager::GetInst()->GetScene()->LoadFullPath(ConvertFullPath);
	}
	

}
