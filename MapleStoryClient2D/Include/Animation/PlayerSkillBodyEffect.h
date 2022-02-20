#pragma once

#include "Animation/AnimationSequence2DInstance.h"

class CPlayerSkillBodyEffect :
    public CAnimationSequence2DInstance
{
public:
	CPlayerSkillBodyEffect();
	CPlayerSkillBodyEffect(const CPlayerSkillBodyEffect& Anim);
	~CPlayerSkillBodyEffect();

public:
	virtual bool Init();
	virtual CPlayerSkillBodyEffect* Clone();

private:
	void EndSkillEffect();
};

