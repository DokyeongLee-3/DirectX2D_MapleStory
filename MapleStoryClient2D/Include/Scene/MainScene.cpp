
#include "MainScene.h"
#include "Scene/Scene.h"
#include "../Object/Player2D.h"
#include "Scene/SceneResource.h"

CMainScene::CMainScene()
{
	SetTypeID<CMainScene>();
}

CMainScene::~CMainScene()
{
}

bool CMainScene::Init()
{
	CreateMaterial();

	CreateAnimationSequence();

	CPlayer2D* Player = m_Scene->CreateGameObject<CPlayer2D>("Player");

	SetPlayerObject(Player);

	return true;
}

void CMainScene::CreateMaterial()
{
}

void CMainScene::CreateAnimationSequence()
{
	CreatePlayerAnimationSequence();
	CreateSkillAnimationSequence();
}

void CMainScene::CreatePlayerAnimationSequence()
{
	// CreateAnimationSequence2D의 2번째 인자는 TextureName 
	m_Scene->GetResource()->CreateAnimationSequence2D("PlayerStandLeft", "PlayerAnimation",
		TEXT("Player/PlayerAnimation.png"));


	for (int i = 0; i < 3; ++i)
	{
		m_Scene->GetResource()->AddAnimationSequence2DFrame("PlayerStandLeft", Vector2(i * 91.f, 0.f), Vector2(91.f, 82.f));
	}


	m_Scene->GetResource()->CreateAnimationSequence2D("PlayerStandRight", "PlayerAnimation", 
		TEXT("Player/PlayerAnimation.png"));

	for (int i = 0; i < 3; ++i)
	{
		m_Scene->GetResource()->AddAnimationSequence2DFrame("PlayerStandRight", Vector2(i * 91.f, 92.f), Vector2(91.f, 82.f));
	}


	m_Scene->GetResource()->CreateAnimationSequence2D("PlayerWalkLeft", "PlayerAnimation", 
		TEXT("Player/PlayerAnimation.png"));

	for (int i = 0; i < 4; ++i)
	{
		m_Scene->GetResource()->AddAnimationSequence2DFrame("PlayerWalkLeft", Vector2(i * 101.f, 185.f), Vector2(101.f, 77.f));
	}


	m_Scene->GetResource()->CreateAnimationSequence2D("PlayerWalkRight", "PlayerAnimation",
		TEXT("Player/PlayerAnimation.png"));

	for (int i = 0; i < 4; ++i)
	{
		m_Scene->GetResource()->AddAnimationSequence2DFrame("PlayerWalkRight", Vector2(i * 101.f, 271.f), Vector2(101.f, 77.f));
	}

	m_Scene->GetResource()->CreateAnimationSequence2D("PlayerSkillActionLeft", "PlayerAnimation", 
		TEXT("Player/PlayerAnimation.png"));

	for (int i = 0; i < 3; ++i)
	{
		m_Scene->GetResource()->AddAnimationSequence2DFrame("PlayerSkillActionLeft", Vector2(i * 99.f, 358.f), Vector2(99.f, 99.f));
	}

	m_Scene->GetResource()->CreateAnimationSequence2D("PlayerSkillActionRight", "PlayerAnimation",
		TEXT("Player/PlayerAnimation.png"));

	for (int i = 0; i < 3; ++i)
	{
		m_Scene->GetResource()->AddAnimationSequence2DFrame("PlayerSkillActionRight", Vector2(i * 99.f, 467.f), Vector2(99.f, 99.f));
	}
}

void CMainScene::CreateSkillAnimationSequence()
{
	m_Scene->GetResource()->CreateAnimationSequence2D("Blank", "Blank",
		TEXT("Player/Blank.png"));

	m_Scene->GetResource()->AddAnimationSequence2DFrame("Blank", Vector2(0.f, 0.f), Vector2(10.f, 77.f));

	// CreateAnimationSequence2D의 2번째 인자는 TextureName 
	m_Scene->GetResource()->CreateAnimationSequence2D("SylphideLancerArrowLeft", "PlayerSkillEffect",
		TEXT("Player/Skill/PlayerSkillEffect.png"));

	for (int i = 0; i < 2; ++i)
	{
		m_Scene->GetResource()->AddAnimationSequence2DFrame("SylphideLancerArrowLeft", Vector2(i * 110.f, 358.f), Vector2(110.f, 42.f));
	}

	m_Scene->GetResource()->CreateAnimationSequence2D("SylphideLancerArrowRight", "PlayerSkillEffect",
		TEXT("Player/Skill/PlayerSkillEffect.png"));

	for (int i = 0; i < 2; ++i)
	{
		m_Scene->GetResource()->AddAnimationSequence2DFrame("SylphideLancerArrowRight", Vector2(220.f + i * 110.f, 358.f), Vector2(110.f, 42.f));
	}



	m_Scene->GetResource()->CreateAnimationSequence2D("SylphideLancerMirrorLeft", "PlayerSkillEffect", 
		TEXT("Player/Skill/PlayerSkillEffect.png"));

	for (int i = 0; i < 12; ++i)
	{
		m_Scene->GetResource()->AddAnimationSequence2DFrame("SylphideLancerMirrorLeft", Vector2(i * 151.f, 0.f), Vector2(151.f, 187.f));
	}


	m_Scene->GetResource()->CreateAnimationSequence2D("SylphideLancerMirrorRight", "PlayerSkillEffect", 
		TEXT("Player/Skill/PlayerSkillEffect.png"));

	for (int i = 0; i < 12; ++i)
	{
		m_Scene->GetResource()->AddAnimationSequence2DFrame("SylphideLancerMirrorRight", Vector2(i * 151.f, 171.f), Vector2(151.f, 187.f));
	}

	m_Scene->GetResource()->CreateAnimationSequence2D("SylphideLancerBodyEffectLeft", "PlayerSkillEffect",
		TEXT("Player/Skill/PlayerSkillEffect.png"));

	for (int i = 0; i < 5; ++i)
	{
		m_Scene->GetResource()->AddAnimationSequence2DFrame("SylphideLancerBodyEffectLeft", Vector2(i * 73.f, 400.f), Vector2(73.f, 105.f));
	}

	m_Scene->GetResource()->CreateAnimationSequence2D("SylphideLancerBodyEffectRight", "PlayerSkillEffect",
		TEXT("Player/Skill/PlayerSkillEffect.png"));

	for (int i = 0; i < 5; ++i)
	{
		m_Scene->GetResource()->AddAnimationSequence2DFrame("SylphideLancerBodyEffectRight", Vector2(358.f + i * 73.f, 400.f), Vector2(73.f, 105.f));
	}

	m_Scene->GetResource()->CreateAnimationSequence2D("SylphideLancerHitEffect", "PlayerSkillEffect",
		TEXT("Player/Skill/PlayerSkillEffect.png"));

	for (int i = 0; i < 5; ++i)
	{
		m_Scene->GetResource()->AddAnimationSequence2DFrame("SylphideLancerHitEffect", Vector2(i * 181.f, 505.f), Vector2(181.f, 175.f));
	}

	m_Scene->GetResource()->CreateAnimationSequence2D("VoidPressureAttackSphere", "PlayerSkillEffect",
		TEXT("Player/Skill/PlayerSkillEffect.png"));

	for (int i = 0; i < 10; ++i)
	{
		m_Scene->GetResource()->AddAnimationSequence2DFrame("VoidPressureAttackSphere", Vector2(i * 250.f, 680.f), Vector2(250.f, 250.f));
	}

	m_Scene->GetResource()->CreateAnimationSequence2D("VoidPressureSphere", "PlayerSkillEffect",
		TEXT("Player/Skill/PlayerSkillEffect.png"));

	for (int i = 0; i < 10; ++i)
	{
		m_Scene->GetResource()->AddAnimationSequence2DFrame("VoidPressureSphere", Vector2(i * 270.f, 956.f), Vector2(270.f, 280.f));
	}

	m_Scene->GetResource()->CreateAnimationSequence2D("VoidPressureAttackSphereDestroy", "PlayerSkillEffect",
		TEXT("Player/Skill/PlayerSkillEffect.png"));

	for (int i = 0; i < 6; ++i)
	{
		m_Scene->GetResource()->AddAnimationSequence2DFrame("VoidPressureAttackSphereDestroy", Vector2(i * 290.f, 1023.f), Vector2(300.f, 255.f));
	}

	m_Scene->GetResource()->CreateAnimationSequence2D("VoidPressureSphereDestroy", "PlayerSkillEffect",
		TEXT("Player/Skill/PlayerSkillEffect.png"));

	for (int i = 0; i < 4; ++i)
	{
		m_Scene->GetResource()->AddAnimationSequence2DFrame("VoidPressureSphereDestroy", Vector2(1400.f + i * 100.f, 1023.f), Vector2(100.f, 100.f));
	}


	m_Scene->GetResource()->CreateAnimationSequence2D("Orb", "PlayerSkillEffect",
		TEXT("Player/Skill/PlayerSkillEffect.png"));

	for (int i = 0; i < 10; ++i)
	{
		m_Scene->GetResource()->AddAnimationSequence2DFrame("Orb", Vector2(1500.f + i * 50.f, 50.f), Vector2(50.f, 50.f));
	}

	m_Scene->GetResource()->CreateAnimationSequence2D("LightTransforming", "PlayerSkillEffect",
		TEXT("Player/Skill/PlayerSkillEffect.png"));

	for (int i = 0; i < 6; ++i)
	{
		m_Scene->GetResource()->AddAnimationSequence2DFrame("LightTransforming", Vector2(1500.f + i * 170.f, 66.f), Vector2(170.f, 130.f));
	}
}
