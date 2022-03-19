#pragma once

#include "Animation/AnimationSequence2DInstance.h"

class CZakumHandAnimation :
    public CAnimationSequence2DInstance
{
public:
	CZakumHandAnimation();
	CZakumHandAnimation(const CZakumHandAnimation& Anim);
	~CZakumHandAnimation();

public:
	virtual bool Init();
	virtual CZakumHandAnimation* Clone();
};

