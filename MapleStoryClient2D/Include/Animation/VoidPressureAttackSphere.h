#pragma once

#include "Animation/AnimationSequence2DInstance.h"

class CVoidPressureAttackSphere :
    public CAnimationSequence2DInstance
{
public:
	CVoidPressureAttackSphere();
	CVoidPressureAttackSphere(const CVoidPressureAttackSphere& Anim);
	~CVoidPressureAttackSphere();


public:
	virtual bool Init();
	virtual CVoidPressureAttackSphere* Clone();

private:
	void EndSkillEffect();
	void DisableColliderComponent();
	void EndReadyEffect();
};

