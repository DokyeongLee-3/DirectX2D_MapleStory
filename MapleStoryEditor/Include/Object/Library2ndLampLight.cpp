
#include "Library2ndLampLight.h"
#include "Animation/AnimationSequence2DInstance.h"
#include "Input.h"


CLibrary2ndLampLight::CLibrary2ndLampLight() :
	m_Sprite(nullptr)
{
	SetTypeID<CLibrary2ndLampLight>();
}

CLibrary2ndLampLight::CLibrary2ndLampLight(const CLibrary2ndLampLight& obj) :
	CGameObject(obj)
{
	m_Sprite = (CSpriteComponent*)FindComponent("Library2ndLampLightSprite");
}

CLibrary2ndLampLight::~CLibrary2ndLampLight()
{
}

void CLibrary2ndLampLight::Start()
{
	CGameObject::Start();
}

bool CLibrary2ndLampLight::Init()
{
	m_Sprite = CreateComponent<CSpriteComponent>("Library2ndLampLightSprite");


	SetRootComponent(m_Sprite);

	m_Sprite->SetTransparency(true);

	m_Sprite->CreateAnimationInstance<CAnimationSequence2DInstance>();

	m_Sprite->SetWorldScale(100.f, 100.f, 1.f);
	m_Sprite->SetRelativePos(500.f, 300.f, 0.f);
	m_Sprite->SetPivot(0.5f, 0.5f, 0.f);


	CAnimationSequence2DInstance* Anim = m_Sprite->GetAnimationInstance();

	Anim->AddAnimation(TEXT("Library2ndLampLight.sqc"), ANIMATION_PATH, "Library2ndLampLight", true, 1.f);

	CInput::GetInst()->CreateKey("Flip", 'F');
	CInput::GetInst()->SetKeyCallback<CLibrary2ndLampLight>("Flip", KeyState_Down, this, &CLibrary2ndLampLight::FlipAll);

	return true;
}

void CLibrary2ndLampLight::Update(float DeltaTime)
{
	CGameObject::Update(DeltaTime);
}

void CLibrary2ndLampLight::PostUpdate(float DeltaTime)
{
	CGameObject::PostUpdate(DeltaTime);
}

CLibrary2ndLampLight* CLibrary2ndLampLight::Clone()
{
	return new CLibrary2ndLampLight(*this);
}

void CLibrary2ndLampLight::Save(FILE* File)
{
	CGameObject::Save(File);
}

void CLibrary2ndLampLight::Load(FILE* File)
{
	CGameObject::Load(File);

	m_Sprite = (CSpriteComponent*)FindComponent("Library2ndLampLightSprite");
}

void CLibrary2ndLampLight::FlipAll(float DeltaTime)
{
	m_Sprite->Flip();
}


