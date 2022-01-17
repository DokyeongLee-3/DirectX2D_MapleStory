
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


CMainScene::CMainScene()
{
	SetTypeID<CMainScene>();
}

CMainScene::~CMainScene()
{
}

bool CMainScene::Init()
{
	/*CreateMaterial();

	CreateAnimationSequence();

	CPlayer2D* Player = m_Scene->CreateGameObject<CPlayer2D>("Player");

	SetPlayerObject(Player);

	Player->SetAllSceneComponentsLayer("MovingObjFront");

	CMonster* Monster = m_Scene->CreateGameObject<CMonster>("Monster");

	CPixelTest* PixelTest = m_Scene->CreateGameObject<CPixelTest>("PixelTest");

	m_MainWidget = m_Scene->GetViewport()->CreateWidgetWindow<CMainWidget>("MainWidget");*/


	/*m_ConfigurationWindow = m_Scene->GetViewport()->CreateWidgetWindow<CConfigurationWindow>("ConfigurationWindow");
	m_ConfigurationWindow->SetZOrder(2);

	 m_Scene->GetResource()->LoadSound("Master", true, "BGM", "GlacierAdventure.mp3");
	m_Scene->GetResource()->SoundPlay("BGM");*/



	// For Test
	TestLoadScene();

	return true;
}

void CMainScene::SetStageObject(CStage* Stage)
{
	m_StageObject = Stage;
}

void CMainScene::CreateMaterial()
{
}

void CMainScene::CreateAnimationSequence()
{
	CreatePlayerAnimationSequence();
	CreateSkillAnimationSequence();
	CreateMonsterAnimationSequence();
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
