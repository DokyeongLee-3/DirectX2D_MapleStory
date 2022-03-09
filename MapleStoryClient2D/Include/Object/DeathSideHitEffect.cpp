
#include "DeathSideHitEffect.h"
#include "DeathSideHitEffect.h"
#include "Animation/AnimationSequence2DInstance.h"

CDeathSideHitEffect::CDeathSideHitEffect()
{
}

CDeathSideHitEffect::CDeathSideHitEffect(const CDeathSideHitEffect& obj) :
	CGameObject(obj)
{
	m_Sprite = obj.m_Sprite;
}

CDeathSideHitEffect::~CDeathSideHitEffect()
{
}

bool CDeathSideHitEffect::Init()
{
	return true;
}

void CDeathSideHitEffect::Start()
{
	m_Sprite = CreateComponent<CSpriteComponent>("DeathSideHitEffectSprite");

	SetRootComponent(m_Sprite);
	//m_Sprite->SetOpacity(0.5f);

	m_Sprite->CreateAnimationInstance<CAnimationSequence2DInstance>();

	m_Sprite->SetPivot(0.5f, 0.5f, 0.f);
	SetAllSceneComponentsLayer("MovingObjFront");

	CAnimationSequence2DInstance* Anim = m_Sprite->GetAnimationInstance();
	m_Sprite->SetTransparency(true);

	Anim->AddAnimation(TEXT("DeathSideHitEffect.sqc"), ANIMATION_PATH, "DeathSideHitEffect", false, 1.f);


	Anim->SetEndFunction<CDeathSideHitEffect>("DeathSideHitEffect", this, &CDeathSideHitEffect::EffectEnd);
}

void CDeathSideHitEffect::Update(float DeltaTime)
{
	CGameObject::Update(DeltaTime);
}

void CDeathSideHitEffect::PostUpdate(float DeltaTime)
{
	CGameObject::PostUpdate(DeltaTime);
}

CDeathSideHitEffect* CDeathSideHitEffect::Clone()
{
	return new CDeathSideHitEffect(*this);
}

void CDeathSideHitEffect::EffectEnd()
{
	Destroy();
}
