#pragma once

#include "Animation/AnimationSequence2DInstance.h"

class CDeathSideAnimation :
    public CAnimationSequence2DInstance
{
public:
	CDeathSideAnimation();
	CDeathSideAnimation(const CDeathSideAnimation& Anim);
	~CDeathSideAnimation();

	// ���� Scene��ȯ�Ҷ� FadeIn/Out�Ǵ� �ӵ�
	float m_OriginFadeInSpeed;
	float m_OriginFadeOutSpeed;

public:
	virtual bool Init();
	virtual CDeathSideAnimation* Clone();

	void EndSkillEffect();

public:
	void RestoreFadeInOutSpeed();
};

