
#include "ZakumClapSmallCircleShockWave.h"

CZakumClapSmallCircleShockWave::CZakumClapSmallCircleShockWave() :
	m_AccTime(0.f),
	m_AlternativeColorTime(0.15f)
{
	SetTypeID<CZakumClapSmallCircleShockWave>();

	m_LifeSpan = 0.8f;
}

CZakumClapSmallCircleShockWave::CZakumClapSmallCircleShockWave(const CZakumClapSmallCircleShockWave& obj)	:
	CGameObject(obj)
{
}

CZakumClapSmallCircleShockWave::~CZakumClapSmallCircleShockWave()
{
}

void CZakumClapSmallCircleShockWave::Start()
{
	CGameObject::Start();
}

bool CZakumClapSmallCircleShockWave::Init()
{
	m_Sprite = CreateComponent<CSpriteComponent>("ClapSmallShockWaveSprite");

	SetRootComponent(m_Sprite);

	m_Sprite->SetTransparency(true);
	m_Sprite->SetPivot(0.5f, 0.5f, 0.f);
	//m_Sprite->SetOpacity(1.f);

	SetSpriteTexture("ClapSmallShockWaveSprite", TEXT("Monster/ClapSmallCircleShockWave.png"));

	return true;
}

void CZakumClapSmallCircleShockWave::Update(float DeltaTime)
{
	CGameObject::Update(DeltaTime);

	m_AccTime += DeltaTime;

	if (m_AccTime >= m_AlternativeColorTime)
	{
		m_AccTime = 0.f;

		Vector4 BaseColor = m_Sprite->GetBaseColor();

		if (BaseColor.x == 1.f)
			m_Sprite->SetBaseColor(0.1f, 0.1f, 0.1f, 0.9f);

		else
			m_Sprite->SetBaseColor(1.f, 1.f, 1.f, 1.f);
	}
	
}

void CZakumClapSmallCircleShockWave::PostUpdate(float DeltaTime)
{
	CGameObject::PostUpdate(DeltaTime);
}

CZakumClapSmallCircleShockWave* CZakumClapSmallCircleShockWave::Clone()
{
	return new CZakumClapSmallCircleShockWave(*this);
}

void CZakumClapSmallCircleShockWave::SetSpriteTexture(const std::string& Name, const TCHAR* FileName)
{
	m_Sprite->SetTexture(0, 0, (int)Buffer_Shader_Type::Pixel, Name, FileName);

	float Width = (float)m_Sprite->GetMaterial()->GetTextureWidth();
	float Height = (float)m_Sprite->GetMaterial()->GetTextureHeight();

	m_Sprite->SetWorldScale(Width, Height, 1.f);
}
