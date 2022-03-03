#pragma once

#include "Animation/AnimationSequence2DInstance.h"

class COnionMonsterAnimation :
    public CAnimationSequence2DInstance
{
public:
	COnionMonsterAnimation();
	COnionMonsterAnimation(const COnionMonsterAnimation& Anim);
	~COnionMonsterAnimation();

public:
	virtual bool Init();
	virtual COnionMonsterAnimation* Clone();
	void ReturnIdle();
};

