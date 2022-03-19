
#include "ZakumClapPinkShockWave.h"
#include "Animation/AnimationSequence2DInstance.h"

CZakumClapPinkShockWave::CZakumClapPinkShockWave()	
{
	SetTypeID<CZakumClapPinkShockWave>();
}

CZakumClapPinkShockWave::CZakumClapPinkShockWave(const CZakumClapPinkShockWave& obj)	:
	CGameObject(obj)
{
}

CZakumClapPinkShockWave::~CZakumClapPinkShockWave()
{
}

void CZakumClapPinkShockWave::Start()
{
	CGameObject::Start();
}

bool CZakumClapPinkShockWave::Init()
{
	m_Sprite = CreateComponent<CSpriteComponent>("ClapSmallShockWaveSprite");

	SetRootComponent(m_Sprite);

	m_Sprite->SetTransparency(true);
	m_Sprite->SetPivot(0.5f, 0.5f, 0.f);
	//m_Sprite->SetOpacity(1.f);

	m_Sprite->CreateAnimationInstance<CAnimationSequence2DInstance>();
	CAnimationSequence2DInstance* Instance = m_Sprite->GetAnimationInstance();

	Instance->AddAnimation("ZakumPinkShockWave", "ZakumPinkShockWave", false, 0.6f);

	Instance->SetEndFunction("ZakumPinkShockWave", this, &CZakumClapPinkShockWave::Destroy);

	return true;
}

void CZakumClapPinkShockWave::Update(float DeltaTime)
{
	CGameObject::Update(DeltaTime);
}

void CZakumClapPinkShockWave::PostUpdate(float DeltaTime)
{
	CGameObject::PostUpdate(DeltaTime);
}

CZakumClapPinkShockWave* CZakumClapPinkShockWave::Clone()
{
	return new CZakumClapPinkShockWave(*this);
}

void CZakumClapPinkShockWave::SetSpriteTexture(const std::string& Name, const TCHAR* FileName)
{
	m_Sprite->SetTexture(0, 0, (int)Buffer_Shader_Type::Pixel, Name, FileName);

	float Width = (float)m_Sprite->GetMaterial()->GetTextureWidth();
	float Height = (float)m_Sprite->GetMaterial()->GetTextureHeight();

	m_Sprite->SetWorldScale(Width, Height, 1.f);
}

void CZakumClapPinkShockWave::Destroy()
{
	CGameObject::Destroy();
}
