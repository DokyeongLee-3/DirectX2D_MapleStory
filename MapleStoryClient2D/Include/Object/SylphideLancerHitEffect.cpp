
#include "SylphideLancerHitEffect.h"
#include "Animation/AnimationSequence2DInstance.h"

CSylphideLancerHitEffect::CSylphideLancerHitEffect()
{
}

CSylphideLancerHitEffect::CSylphideLancerHitEffect(const CSylphideLancerHitEffect& obj)	:
	CGameObject(obj)
{
	m_Sprite = obj.m_Sprite;
}

CSylphideLancerHitEffect::~CSylphideLancerHitEffect()
{
}

bool CSylphideLancerHitEffect::Init()
{
	return true;
}

void CSylphideLancerHitEffect::Start()
{
	m_Sprite = CreateComponent<CSpriteComponent>("SylphideLancerHitEffectSprite");

	SetRootComponent(m_Sprite);
	//m_Sprite->SetOpacity(0.5f);

	m_Sprite->CreateAnimationInstance<CAnimationSequence2DInstance>();

	m_Sprite->SetPivot(0.5f, 0.5f, 0.f);
	SetAllSceneComponentsLayer("MovingObjFront");

	CAnimationSequence2DInstance* Anim = m_Sprite->GetAnimationInstance();
	m_Sprite->SetTransparency(true);

	Anim->AddAnimation(TEXT("SylphideLancerHit.sqc"), ANIMATION_PATH, "SylphideLancerHit", false, 0.8f);
	Anim->AddAnimation(TEXT("SylphideLancerHitPurple.sqc"), ANIMATION_PATH, "SylphideLancerHitPurple", false, 0.8f);

	Anim->SetEndFunction<CSylphideLancerHitEffect>("SylphideLancerHit", this, &CSylphideLancerHitEffect::EffectEnd);
	Anim->SetEndFunction<CSylphideLancerHitEffect>("SylphideLancerHitPurple", this, &CSylphideLancerHitEffect::EffectEnd);
}

void CSylphideLancerHitEffect::Update(float DeltaTime)
{
	CAnimationSequence2DData* Data = m_Sprite->GetCurrentAnimation();

	CGameObject::Update(DeltaTime);
}

void CSylphideLancerHitEffect::PostUpdate(float DeltaTime)
{
	CGameObject::PostUpdate(DeltaTime);
}

CSylphideLancerHitEffect* CSylphideLancerHitEffect::Clone()
{
	return new CSylphideLancerHitEffect(*this);
}

void CSylphideLancerHitEffect::EffectEnd()
{
	Destroy();
}
