#pragma once

#include "Animation/AnimationSequence2DInstance.h"

class CZakumFlameAnimation :
    public CAnimationSequence2DInstance
{
public:
	CZakumFlameAnimation();
	CZakumFlameAnimation(const CZakumFlameAnimation& Anim);
	~CZakumFlameAnimation();

public:
	virtual bool Init();
	virtual CZakumFlameAnimation* Clone();
};

