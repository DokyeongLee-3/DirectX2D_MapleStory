
#include "AnimationLoadObject.h"
#include "Component/SpriteComponent.h"
#include "Animation/AnimationSequence2DInstance.h"

CAnimationLoadObject::CAnimationLoadObject()
{
}

CAnimationLoadObject::CAnimationLoadObject(const CAnimationLoadObject& obj) :
	CGameObject(obj)
{
	m_Sprite = (CSpriteComponent*)FindComponent("AnimationLoadComponent");
}

CAnimationLoadObject::~CAnimationLoadObject()
{
}

bool CAnimationLoadObject::Init()
{
	m_Sprite = CreateComponent<CSpriteComponent>("AnimationLoadComponent");

	SetRootComponent(m_Sprite);

	m_Sprite->CreateAnimationInstance<CAnimationSequence2DInstance>();

	m_Sprite->SetRender(false);

	return true;
}

void CAnimationLoadObject::Update(float DeltaTime)
{
	CGameObject::Update(DeltaTime);
}

void CAnimationLoadObject::PostUpdate(float DeltaTime)
{
	CGameObject::PostUpdate(DeltaTime);
}

CAnimationLoadObject* CAnimationLoadObject::Clone()
{
	return new CAnimationLoadObject(*this);
}
