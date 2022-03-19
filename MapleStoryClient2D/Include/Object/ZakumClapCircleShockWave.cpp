
#include "ZakumClapCircleShockWave.h"
#include "Animation/AnimationSequence2DInstance.h"

CZakumClapCircleShockWave::CZakumClapCircleShockWave()
{
	SetTypeID<CZakumClapCircleShockWave>();
}

CZakumClapCircleShockWave::CZakumClapCircleShockWave(const CZakumClapCircleShockWave& obj)	:
	CGameObject(obj)
{
}

CZakumClapCircleShockWave::~CZakumClapCircleShockWave()
{
}

void CZakumClapCircleShockWave::SetShockWaveOwner(CZakumBody* Zakum)
{
	m_ShockWaveOwner = Zakum;
}

void CZakumClapCircleShockWave::Start()
{
	CGameObject::Start();
}

bool CZakumClapCircleShockWave::Init()
{
	m_Sprite = CreateComponent<CSpriteComponent>("ClapShockWaveSprite");
	SetRootComponent(m_Sprite);

	SetSpriteTexture("ClapShockWaveSprite", TEXT("Monster/ClapCircleShockWave.png"));

	m_Sprite->SetTransparency(true);
	m_Sprite->SetPivot(0.5f, 0.5f, 0.f);
	m_Sprite->SetOpacity(0.85f);

	m_Sprite->CreateAnimationInstance<CAnimationSequence2DInstance>();

	CAnimationSequence2DInstance* Instance = m_Sprite->GetAnimationInstance();

	Instance->AddAnimation(TEXT("ZakumClapCircleShockWave.sqc"), ANIMATION_PATH, "ZakumClapCircleShockWave", true, 0.7f);

	Instance->SetEndFunction<CZakumClapCircleShockWave>("ZakumClapCircleShockWave", this, &CZakumClapCircleShockWave::Destroy);

	return true;
}

void CZakumClapCircleShockWave::Update(float DeltaTime)
{
	CGameObject::Update(DeltaTime);
}

void CZakumClapCircleShockWave::PostUpdate(float DeltaTime)
{
	CGameObject::PostUpdate(DeltaTime);
}

CZakumClapCircleShockWave* CZakumClapCircleShockWave::Clone()
{
	return new CZakumClapCircleShockWave(*this);
}

void CZakumClapCircleShockWave::SetSpriteTexture(const std::string& Name, const TCHAR* FileName)
{
	m_Sprite->SetTexture(0, 0, (int)Buffer_Shader_Type::Pixel, Name, FileName);

	float Width = (float)m_Sprite->GetMaterial()->GetTextureWidth();
	float Height = (float)m_Sprite->GetMaterial()->GetTextureHeight();

	m_Sprite->SetWorldScale(Width, Height, 1.f);
}

void CZakumClapCircleShockWave::Destroy()
{
	CGameObject::Destroy();
}
