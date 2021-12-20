#pragma once

#include "Animation/AnimationSequence2DInstance.h"

class CSylphideLancerArrowAnimation2D :
    public CAnimationSequence2DInstance
{
public:
	CSylphideLancerArrowAnimation2D();
	CSylphideLancerArrowAnimation2D(const CSylphideLancerArrowAnimation2D& Anim);
	~CSylphideLancerArrowAnimation2D();

public:
	virtual bool Init();
	virtual CSylphideLancerArrowAnimation2D* Clone();
};

