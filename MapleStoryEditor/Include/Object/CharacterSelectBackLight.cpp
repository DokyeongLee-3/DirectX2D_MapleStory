
#include "CharacterSelectBackLight.h"
#include "Animation/AnimationSequence2DInstance.h"
#include "Input.h"
#include "../EditorManager.h"
#include "../Window/SpriteWindow.h"

CCharacterSelectBackLight::CCharacterSelectBackLight() :
	m_Sprite(nullptr)
{
	SetTypeID<CCharacterSelectBackLight>();
}

CCharacterSelectBackLight::CCharacterSelectBackLight(const CCharacterSelectBackLight& obj) :
	CGameObject(obj)
{
	m_Sprite = (CSpriteComponent*)FindComponent("CharacterSelectBackLightSprite");
}

CCharacterSelectBackLight::~CCharacterSelectBackLight()
{
}

void CCharacterSelectBackLight::Start()
{
	CGameObject::Start();
}

bool CCharacterSelectBackLight::Init()
{
	m_Sprite = CreateComponent<CSpriteComponent>("CharacterSelectBackLightSprite");

	SetRootComponent(m_Sprite);

	m_Sprite->SetTransparency(true);

	m_Sprite->CreateAnimationInstance<CAnimationSequence2DInstance>();

	m_Sprite->SetWorldScale(100.f, 100.f, 1.f);
	m_Sprite->SetRelativePos(500.f, 300.f, 0.f);
	//m_Sprite->SetPivot(0.5f, 0.5f, 0.f);




	CAnimationSequence2DInstance* Anim = m_Sprite->GetAnimationInstance();

	Anim->AddAnimation(TEXT("CharacterSelectBackLight.sqc"), ANIMATION_PATH, "CharacterSelectBackLight", true, 1.f);
	Anim->AddAnimation(TEXT("CharacterSelectPlayerWalkRight.sqc"), ANIMATION_PATH, "CharacterSelectPlayerWalkRight", true, 1.f);
	Anim->AddAnimation(TEXT("CharacterSelectPlayerStandRight.sqc"), ANIMATION_PATH, "CharacterSelectPlayerStandRight", true, 1.f);

	CInput::GetInst()->CreateKey("NextAnimation", 'N');
	CInput::GetInst()->SetKeyCallback<CCharacterSelectBackLight>("NextAnimation", KeyState_Down, this, &CCharacterSelectBackLight::NextAnimation);

	return true;
}

void CCharacterSelectBackLight::Update(float DeltaTime)
{
	CGameObject::Update(DeltaTime);
}

void CCharacterSelectBackLight::PostUpdate(float DeltaTime)
{
	CGameObject::PostUpdate(DeltaTime);
}

CCharacterSelectBackLight* CCharacterSelectBackLight::Clone()
{
	return new CCharacterSelectBackLight(*this);
}

void CCharacterSelectBackLight::Save(FILE* File)
{
	CGameObject::Save(File);
}

void CCharacterSelectBackLight::Load(FILE* File)
{
	CGameObject::Load(File);

	m_Sprite = (CSpriteComponent*)FindComponent("CharacterSelectBackLightSprite");

}

void CCharacterSelectBackLight::NextAnimation(float DeltaTime)
{
	CAnimationSequence2DInstance* Anim = m_Sprite->GetAnimationInstance();

	Anim->NextAnimation();
}
