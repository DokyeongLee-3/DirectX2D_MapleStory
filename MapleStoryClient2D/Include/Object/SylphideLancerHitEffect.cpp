
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
	m_Sprite = CreateComponent<CSpriteComponent>("SylphideLancerHitEffectSprite");

	SetRootComponent(m_Sprite);

	m_Sprite->SetTransparency(true);
	//m_Sprite->SetOpacity(0.5f);

	m_Sprite->CreateAnimationInstance<CAnimationSequence2DInstance>();


	m_Sprite->SetPivot(0.5f, 0.5f, 0.f);


	return true;
}

void CSylphideLancerHitEffect::Start()
{
	SetAllSceneComponentsLayer("MovingObjFront");

	CAnimationSequence2DInstance* Anim = m_Sprite->GetAnimationInstance();

	Anim->AddAnimation(TEXT("SylphideLancerHit.sqc"), ANIMATION_PATH, "SylphideLancerHit", false, 0.8f);

	Anim->SetEndFunction<CSylphideLancerHitEffect>("SylphideLancerHit", this, &CSylphideLancerHitEffect::EffectEnd);
}

void CSylphideLancerHitEffect::Update(float DeltaTime)
{
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
