#pragma once

#include "Animation/AnimationSequence2DInstance.h"

class CZakumBodyAnimation :
    public CAnimationSequence2DInstance
{
public:
	CZakumBodyAnimation();
	CZakumBodyAnimation(const CZakumBodyAnimation& Anim);
	~CZakumBodyAnimation();

public:
	virtual bool Init();
	virtual CZakumBodyAnimation* Clone();
};

