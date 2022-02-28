
#include "VoidPressureOrb.h"
#include "Component/SpriteComponent.h"
#include "../Animation/VoidPressureOrbAnimation.h"


CVoidPressureOrb::CVoidPressureOrb()
{
}

CVoidPressureOrb::CVoidPressureOrb(const CVoidPressureOrb& obj)	:
	CGameObject(obj)
{
	m_Sprite = (CSpriteComponent*)FindComponent("VoidPressureOrbSprite");
}

CVoidPressureOrb::~CVoidPressureOrb()
{
}

void CVoidPressureOrb::FlipAll(float DeltaTime)
{
	m_Sprite->Flip();
}

bool CVoidPressureOrb::Init()
{
	m_Sprite = CreateComponent<CSpriteComponent>("VoidPressureOrbSprite");
	m_Sprite->SetPivot(0.5f, 0.5f, 0.f);
	m_Sprite->SetTransparency(true);
	

	m_Sprite->CreateAnimationInstance<CVoidPressureOrbAnimation>();
	

	SetRootComponent(m_Sprite);

	return true;
}

void CVoidPressureOrb::Update(float DeltaTime)
{
	CGameObject::Update(DeltaTime);
}

void CVoidPressureOrb::PostUpdate(float DeltaTime)
{
	CGameObject::PostUpdate(DeltaTime);
}

CVoidPressureOrb* CVoidPressureOrb::Clone()
{
	return new CVoidPressureOrb(*this);
}

