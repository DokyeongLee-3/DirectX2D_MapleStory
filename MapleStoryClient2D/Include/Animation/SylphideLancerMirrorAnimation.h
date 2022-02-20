#pragma once

#include "Animation/AnimationSequence2DInstance.h"

class CSylphideLancerMirrorAnimation :
    public CAnimationSequence2DInstance
{
public:
	CSylphideLancerMirrorAnimation();
	CSylphideLancerMirrorAnimation(const CSylphideLancerMirrorAnimation& Anim);
	~CSylphideLancerMirrorAnimation();



public:
	virtual bool Init();
	virtual CSylphideLancerMirrorAnimation* Clone();

private:
	void EndSkillEffect();
};

