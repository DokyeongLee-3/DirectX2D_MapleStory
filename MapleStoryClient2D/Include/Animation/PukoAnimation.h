#pragma once

#include "Animation/AnimationSequence2DInstance.h"

class CPukoAnimation :
    public CAnimationSequence2DInstance
{
public:
	CPukoAnimation();
	CPukoAnimation(const CPukoAnimation& Anim);
	~CPukoAnimation();

public:
	virtual bool Init();
	virtual CPukoAnimation* Clone();
	void ReturnFly();
};

