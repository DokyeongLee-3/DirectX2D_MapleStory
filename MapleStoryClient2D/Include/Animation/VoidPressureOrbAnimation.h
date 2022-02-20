#pragma once

#include "Animation/AnimationSequence2DInstance.h"

class CVoidPressureOrbAnimation :
    public CAnimationSequence2DInstance
{
public:
	CVoidPressureOrbAnimation();
	CVoidPressureOrbAnimation(const CVoidPressureOrbAnimation& Anim);
	~CVoidPressureOrbAnimation();


public:
	virtual bool Init();
	virtual CVoidPressureOrbAnimation* Clone();

private:
	void EndSkillEffect();
};

