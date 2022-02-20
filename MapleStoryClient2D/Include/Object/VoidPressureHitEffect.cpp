
#include "VoidPressureHitEffect.h"
#include "Animation/AnimationSequence2DInstance.h"

CVoidPressureHitEffect::CVoidPressureHitEffect()
{
}

CVoidPressureHitEffect::CVoidPressureHitEffect(const CVoidPressureHitEffect& obj) :
	CGameObject(obj)
{
	m_Sprite = obj.m_Sprite;
}

CVoidPressureHitEffect::~CVoidPressureHitEffect()
{
}

bool CVoidPressureHitEffect::Init()
{
	return true;
}

void CVoidPressureHitEffect::Start()
{
	m_Sprite = CreateComponent<CSpriteComponent>("VoidPressureHitEffectSprite");

	SetRootComponent(m_Sprite);
	//m_Sprite->SetOpacity(0.5f);

	m_Sprite->CreateAnimationInstance<CAnimationSequence2DInstance>();

	m_Sprite->SetPivot(0.5f, 0.5f, 0.f);
	SetAllSceneComponentsLayer("MovingObjFront");

	CAnimationSequence2DInstance* Anim = m_Sprite->GetAnimationInstance();
	m_Sprite->SetTransparency(true);

	Anim->AddAnimation(TEXT("VoidPressureHit.sqc"), ANIMATION_PATH, "VoidPressureHit", false, 0.5f);


	Anim->SetEndFunction<CVoidPressureHitEffect>("VoidPressureHit", this, &CVoidPressureHitEffect::EffectEnd);
}

void CVoidPressureHitEffect::Update(float DeltaTime)
{
	CGameObject::Update(DeltaTime);
}

void CVoidPressureHitEffect::PostUpdate(float DeltaTime)
{
	CGameObject::PostUpdate(DeltaTime);
}

CVoidPressureHitEffect* CVoidPressureHitEffect::Clone()
{
	return new CVoidPressureHitEffect(*this);
}

void CVoidPressureHitEffect::EffectEnd()
{
	Destroy();
}
