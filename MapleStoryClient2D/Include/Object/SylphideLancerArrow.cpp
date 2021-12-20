
#include "SylphideLancerArrow.h"
#include "SylphideLancerArrowAnimation2D.h"
#include "Component/SpriteComponent.h"

CSylphideLancerArrow::CSylphideLancerArrow() :
	m_Distance(600.f),
	m_Speed(500.f)
{
	SetTypeID<CSylphideLancerArrow>();
}

CSylphideLancerArrow::CSylphideLancerArrow(const CSylphideLancerArrow& obj) :
	CGameObject(obj)
{
	m_Sprite = (CSpriteComponent*)FindComponent("SylphideLancerArrowSprite");
	m_Distance = obj.m_Distance;
}

CSylphideLancerArrow::~CSylphideLancerArrow()
{
}

bool CSylphideLancerArrow::Init()
{
	m_Sprite = CreateComponent<CSpriteComponent>("SylphideLancerArrowSprite");

	SetRootComponent(m_Sprite);

	m_Sprite->SetRelativeScale(110.f, 47.f, 1.f);
	m_Sprite->SetPivot(0.5f, 0.5f, 0.f);

	m_Sprite->CreateAnimationInstance<CSylphideLancerArrowAnimation2D>();
	m_Sprite->SetTransparency(true);

	return true;
}

void CSylphideLancerArrow::Update(float DeltaTime)
{
	CGameObject::Update(DeltaTime);

	float	Dist = m_Speed * DeltaTime;

	m_Distance -= Dist;

	if (m_Distance <= 0.f)
	{
		Destroy();
	}

	float XDir = 1.f;

	std::string CurAnim = m_Sprite->GetCurrentAnimationName();

	if (CurAnim.find("Left") != std::string::npos)
	{
		XDir = -1.f;
		AddRelativePos(GetWorldAxis(AXIS_X) * XDir * Dist);
	}

	else
	{
		AddRelativePos(GetWorldAxis(AXIS_X) * Dist);
	}


}

void CSylphideLancerArrow::PostUpdate(float DeltaTime)
{
	CGameObject::PostUpdate(DeltaTime);
}

CSylphideLancerArrow* CSylphideLancerArrow::Clone()
{
	return new CSylphideLancerArrow(*this);
}

void CSylphideLancerArrow::ChangeRootSpriteAnimation(const std::string& Name)
{
	m_Sprite->ChangeAnimation(Name);
}
