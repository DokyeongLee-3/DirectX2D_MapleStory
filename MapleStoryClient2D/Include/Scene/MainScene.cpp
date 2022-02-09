
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
#include "../Object/SmokeParticle.h"
#include "../Object/SparkParticle.h"
#include "../Object/RainParticle.h"
#include "Input.h"

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

	CMonster* Monster = m_Scene->CreateGameObject<CMonster>("Monster");



	m_MainWidget = m_Scene->GetViewport()->CreateWidgetWindow<CMainWidget>("MainWidget");

	m_ConfigurationWindow = m_Scene->GetViewport()->CreateWidgetWindow<CConfigurationWindow>("ConfigurationWindow");
	m_ConfigurationWindow->SetZOrder(2);
	m_ConfigurationWindow->SetPos(200.f, 200.f);

	m_Inventory = m_Scene->GetViewport()->CreateWidgetWindow<CInventory>("Inventory");
	m_Inventory->SetZOrder(3);
	m_Inventory->SetPos(300.f, 100.f);

	m_CharacterStatusWindow = m_Scene->GetViewport()->CreateWidgetWindow<CCharacterStatusWindow>("MainStatus");
	m_CharacterStatusWindow->SetPos(550.f, 13.f);

	m_CharacterEXPWindow = m_Scene->GetViewport()->CreateWidgetWindow<CCharacterEXP>("EXPWindow");

	m_SkillQuickSlot = m_Scene->GetViewport()->CreateWidgetWindow<CSkillQuickSlotWindow>("SkillQuickSlot");
	m_SkillQuickSlot->SetZOrder(2);


	m_Scene->GetResource()->LoadSound("Master", true, "BGM", "GlacierAdventure.mp3");
	m_Scene->GetResource()->SoundPlay("BGM");


	//CSmokeParticle* SmokeParticleLeft = m_Scene->CreateGameObject<CSmokeParticle>("SmokeParticleLeft");
	//SmokeParticleLeft->SetParticle("SmokeLeft");

	//CSmokeParticle* SmokeParticleRight = m_Scene->CreateGameObject<CSmokeParticle>("SmokeParticleRight");
	//SmokeParticleRight->SetParticle("SmokeRight");

	//CSmokeParticle* SmokeParticleCenter = m_Scene->CreateGameObject<CSmokeParticle>("SmokeParticleCenter");
	//SmokeParticleCenter->SetParticle("SmokeCenter");

	/*CRainParticle* RainParticle = m_Scene->CreateGameObject<CRainParticle>("RainParticle1");
	RainParticle->SetParticle("Rain1");*/

	//CRainParticle* RainParticle = m_Scene->CreateGameObject<CRainParticle>("RainParticle2");
	//RainParticle->SetParticle("Rain2");
	//RainParticle->GetRootComponent()->SetRelativePos(100.f, 520.f, 0.f);

	CBubbleParticle* BubbleParticle = m_Scene->CreateGameObject<CBubbleParticle>("BubbleParticle");

	BubbleParticle = m_Scene->CreateGameObject<CBubbleParticle>("BubbleParticle");

	BubbleParticle->SetRelativePos(-100.f, 0.f, 0.f);


	CInput::GetInst()->CreateKey("TurnOffUIWindow", VK_ESCAPE);

	CInput::GetInst()->SetKeyCallback<CMainScene>("TurnOffUIWindow", Key_State::KeyState_Down, this, &CMainScene::TurnOffWindow);


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

	/*m_Scene->GetResource()->CreateMaterial<CMaterial>("Rain");
	CMaterial* Material = m_Scene->GetResource()->FindMaterial("Rain");

	Material->AddTexture(0, (int)Buffer_Shader_Type::Pixel, "Rain", TEXT("Particle/HardRain.png"));*/

	Material->SetShader("ParticleRenderShader");
	Material->SetRenderState("AlphaBlend");
}

void CMainScene::CreateAnimationSequence()
{
	CreatePlayerAnimationSequence();
	CreateSkillAnimationSequence();
	CreateMonsterAnimationSequence();
	CreateMapAnimationSequence();
}

void CMainScene::CreateParticle()
{
	m_Scene->GetResource()->CreateParticle("Bubble");
	CParticle* Particle = m_Scene->GetResource()->FindParticle("Bubble");

	CMaterial* Material = m_Scene->GetResource()->FindMaterial("Bubble");

	Particle->SetMaterial(Material);
	Particle->SetSpawnCountMax(1000);
	Particle->SetLifeTimeMin(3.f);
	Particle->SetLifeTimeMax(5.f);
	Particle->SetScaleMin(Vector3(20.f, 20.f, 1.f));
	Particle->SetScaleMax(Vector3(50.f, 50.f, 1.f));
	Particle->SetSpeedMin(100.f);
	Particle->SetSpeedMax(300.f);
	Particle->SetMoveDir(Vector3(0.f, 1.f, 0.f));
	Particle->SetStartMin(Vector3(-30.f, -30.f, 0.f));
	Particle->SetStartMax(Vector3(30.f, 30.f, 0.f));
	Particle->SetColorMin(Vector4(0.2f, 0.1f, 0.8f, 1.f));
	Particle->SetColorMax(Vector4(0.2f, 0.1f, 0.8f, 1.f));
	Particle->SetMoveAngle(Vector3(0.f, 0.f, 30.f));
	Particle->SetGravity(true);
	Particle->SetMove(true);

	//m_Scene->GetResource()->CreateParticle("SmokeLeft");
	//CParticle* Particle = m_Scene->GetResource()->FindParticle("SmokeLeft");

	//CMaterial* Material = m_Scene->GetResource()->FindMaterial("Smoke");

	//Particle->SetMaterial(Material);
	//Particle->SetSpawnCountMax(1000);
	//Particle->SetLifeTimeMin(2.f);
	//Particle->SetLifeTimeMax(2.5f);
	//Particle->SetScaleMin(Vector3(64.f, 64.f, 1.f));
	//Particle->SetScaleMax(Vector3(64.f, 64.f, 1.f));
	//Particle->SetSpeedMin(30.f);
	//Particle->SetSpeedMax(50.f);
	//Particle->SetMoveDir(Vector3(-0.7f, 0.25f, 0.f));
	//Particle->SetStartMin(Vector3(-40.f, -10.f, 0.f));
	//Particle->SetStartMax(Vector3(40.f, 10.f, 0.f));
	//Particle->SetColorMin(Vector4(1.f, 1.f, 1.f, 0.9f));
	//Particle->SetColorMax(Vector4(1.f, 1.f, 1.f, 0.f));
	//Particle->SetMoveAngle(Vector3(0.f, 0.f, 10.f));
	////Particle->SetGravity(true);
	//Particle->SetMove(true);

	//////////////////////////////////////////

	//m_Scene->GetResource()->CreateParticle("SmokeRight");
	//Particle = m_Scene->GetResource()->FindParticle("SmokeRight");

	//Material = m_Scene->GetResource()->FindMaterial("Smoke");

	//Particle->SetMaterial(Material);
	//Particle->SetSpawnCountMax(1000);
	//Particle->SetLifeTimeMin(2.f);
	//Particle->SetLifeTimeMax(2.5f);
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

	//////////////////////////////////////////

	//m_Scene->GetResource()->CreateParticle("SmokeCenter");
	//Particle = m_Scene->GetResource()->FindParticle("SmokeCenter");

	//Material = m_Scene->GetResource()->FindMaterial("Smoke");

	//Particle->SetMaterial(Material);
	//Particle->SetSpawnCountMax(3000);
	//Particle->SetLifeTimeMin(2.f);
	//Particle->SetLifeTimeMax(2.5f);
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

	/*m_Scene->GetResource()->CreateParticle("Rain1");
	CParticle* Particle = m_Scene->GetResource()->FindParticle("Rain1");

	CMaterial* Material = m_Scene->GetResource()->FindMaterial("Rain");

	Particle->SetMaterial(Material);
	Particle->SetSpawnCountMax(80);
	Particle->SetLifeTimeMin(3.f);
	Particle->SetLifeTimeMax(4.5f);
	Particle->SetScaleMin(Vector3(50.f, 85.f, 1.f));
	Particle->SetScaleMax(Vector3(50.f, 85.f, 1.f));
	Particle->SetSpeedMin(100.f);
	Particle->SetSpeedMax(150.f);
	Particle->SetMoveDir(Vector3(0.f, -1.f, 0.f));
	Particle->SetStartMin(Vector3(500.f, -50.f, 0.f));
	Particle->SetStartMax(Vector3(-500.f, 50.f, 0.f));
	Particle->SetColorMin(Vector4(1.f, 1.f, 1.f, 1.f));
	Particle->SetColorMax(Vector4(1.f, 1.f, 1.f, 0.7f));
	Particle->SetMoveAngle(Vector3(0.f, 0.f, 0.f));
	Particle->SetGravity(true);
	Particle->SetMove(true);*/

	/*m_Scene->GetResource()->CreateParticle("Rain2");
	CParticle* Particle = m_Scene->GetResource()->FindParticle("Rain2");

	CMaterial* Material = m_Scene->GetResource()->FindMaterial("Rain");

	Particle->SetSpawnTime(0.05f);
	Particle->SetMaterial(Material);
	Particle->SetSpawnCountMax(100);
	Particle->SetLifeTimeMin(3.f);
	Particle->SetLifeTimeMax(4.5f);
	Particle->SetScaleMin(Vector3(50.f, 85.f, 1.f));
	Particle->SetScaleMax(Vector3(50.f, 85.f, 1.f));
	Particle->SetSpeedMin(150.f);
	Particle->SetSpeedMax(200.f);
	Particle->SetMoveDir(Vector3(0.f, -1.f, 0.f));
	Particle->SetStartMin(Vector3(600.f, -40.f, 0.f));
	Particle->SetStartMax(Vector3(-600.f, 40.f, 0.f));
	Particle->SetColorMin(Vector4(1.f, 1.f, 1.f, 0.9f));
	Particle->SetColorMax(Vector4(1.f, 1.f, 1.f, 0.6f));
	Particle->SetMoveAngle(Vector3(0.f, 0.f, 0.f));
	Particle->SetGravity(true);
	Particle->SetMove(true);*/
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

void CMainScene::CreateMapAnimationSequence()
{
	m_Scene->GetResource()->LoadSequence2D("SingleHelixBlinkTree.sqc");
	m_Scene->GetResource()->LoadSequence2D("DoubleHelixBlinkTree.sqc");
	m_Scene->GetResource()->LoadSequence2D("BlinkTree.sqc");

	m_Scene->GetResource()->LoadSequence2D("Portal.sqc");
	m_Scene->GetResource()->LoadSequence2D("Butterfly.sqc");
	m_Scene->GetResource()->LoadSequence2D("LampLight.sqc");
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

void CMainScene::TurnOffWindow(float DeltaTime)
{
	CWidgetWindow* TopMostWindow = m_Scene->GetViewport()->FindTopMostWindow();

	// 캐릭터 정보창이나 경험치 UI Window같은 것들은 끄면 안됨
	if (TopMostWindow)
	{	
		std::string Name = TopMostWindow->GetName();

		if (Name.find("MainStatus") != std::string::npos || Name.find("EXPWindow") != std::string::npos || Name.find("SkillQuickSlot") != std::string::npos)
			return;

		TopMostWindow->SetZOrder(0);
		TopMostWindow->Enable(false);
	}
}
