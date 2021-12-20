#pragma once

#include "Animation/AnimationSequence2DInstance.h"

class CSylphideLancerEffectAnimation2D :
    public CAnimationSequence2DInstance
{
public:
	CSylphideLancerEffectAnimation2D();
	CSylphideLancerEffectAnimation2D(const CSylphideLancerEffectAnimation2D& Anim);
	~CSylphideLancerEffectAnimation2D();

public:
	virtual bool Init();
	virtual CSylphideLancerEffectAnimation2D* Clone();

private:
	void EndSkillEffect();
};

