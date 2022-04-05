#pragma once

#include "Animation/AnimationSequence2DInstance.h"

class CPunkoAnimation :
    public CAnimationSequence2DInstance
{
public:
	CPunkoAnimation();
	CPunkoAnimation(const CPunkoAnimation& Anim);
	~CPunkoAnimation();

public:
	virtual bool Init();
	virtual CPunkoAnimation* Clone();
	void ReturnFly();
};

