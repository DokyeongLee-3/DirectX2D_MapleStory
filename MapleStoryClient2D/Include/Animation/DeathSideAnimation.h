#pragma once

#include "Animation/AnimationSequence2DInstance.h"

class CDeathSideAnimation :
    public CAnimationSequence2DInstance
{
public:
	CDeathSideAnimation();
	CDeathSideAnimation(const CDeathSideAnimation& Anim);
	~CDeathSideAnimation();

	// 원래 Scene전환할때 FadeIn/Out되는 속도
	float m_OriginFadeInSpeed;
	float m_OriginFadeOutSpeed;

public:
	virtual bool Init();
	virtual CDeathSideAnimation* Clone();

	void EndSkillEffect();

public:
	void RestoreFadeInOutSpeed();
};

