
#include "OnionScene.h"
#include "Scene/Scene.h"
#include "Scene/SceneResource.h"
#include "Scene/SceneManager.h"
#include "../Object/OnionMonster.h"
#include "../Object/SylphideLancerHitEffect.h"
#include "Input.h"
#include "PathManager.h"

COnionScene::COnionScene()
{
	SetTypeID<COnionScene>();
}

COnionScene::~COnionScene()
{
}

void COnionScene::SetStageObject(CStage* Stage)
{
	m_StageObject = Stage;
}

bool COnionScene::Init()
{
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


	for (int i = 0; i < 5; ++i)
	{
		char MonsterName[128] = {};

		sprintf_s(MonsterName, "OnionMonster%d", i);

		COnionMonster* OnionMonster = m_Scene->CreateGameObject<COnionMonster>(MonsterName);
		OnionMonster->SetAllSceneComponentsLayer("MovingObjFront");
		OnionMonster->SetWorldPos(400.f + i * 250.f, 200.f, 0.f);
	}

	CInput::GetInst()->CreateKey("TurnOffUIWindow", VK_ESCAPE);

	CInput::GetInst()->SetKeyCallback<COnionScene>("TurnOffUIWindow", Key_State::KeyState_Down, this, &COnionScene::TurnOffWindow);


	m_Scene->GetResource()->LoadSound("Master", true, "BGM", "TheFairyForest.mp3");
	m_Scene->GetResource()->SoundPlay("BGM");


	return true;
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

}

void COnionScene::CreateSkillAnimationSequence()
{
	m_Scene->GetResource()->LoadSequence2D("PlayerOrb.sqc");

	m_Scene->GetResource()->LoadSequence2D("SylphideLancerBodyEffectLeft.sqc");
	m_Scene->GetResource()->LoadSequence2D("SylphideLancerArrowLeft.sqc");
	m_Scene->GetResource()->LoadSequence2D("SylphideLancerHit.sqc");
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
}

void COnionScene::TurnOffWindow(float DeltaTime)
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
