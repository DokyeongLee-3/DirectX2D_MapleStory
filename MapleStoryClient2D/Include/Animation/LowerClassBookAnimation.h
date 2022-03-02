#pragma once

#include "Animation/AnimationSequence2DInstance.h"

class CLowerClassBookAnimation :
	public CAnimationSequence2DInstance
{
public:
	CLowerClassBookAnimation();
	CLowerClassBookAnimation(const CLowerClassBookAnimation& Anim);
	~CLowerClassBookAnimation();

public:
	virtual bool Init();
	virtual CLowerClassBookAnimation* Clone();
};
