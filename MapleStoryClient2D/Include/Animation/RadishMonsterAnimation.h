#pragma once

#include "Animation/AnimationSequence2DInstance.h"

class CRadishMonsterAnimation :
    public CAnimationSequence2DInstance
{
public:
	CRadishMonsterAnimation();
	CRadishMonsterAnimation(const CRadishMonsterAnimation& Anim);
	~CRadishMonsterAnimation();

public:
	virtual bool Init();
	virtual CRadishMonsterAnimation* Clone();
};

