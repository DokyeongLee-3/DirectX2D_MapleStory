#pragma once

#include "Animation/AnimationSequence2DInstance.h"

class CCuzcoAnimation :
    public CAnimationSequence2DInstance
{
public:
	CCuzcoAnimation();
	CCuzcoAnimation(const CCuzcoAnimation& Anim);
	~CCuzcoAnimation();

public:
	virtual bool Init();
	virtual CCuzcoAnimation* Clone();
	void ReturnFly();
};

