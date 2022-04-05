
#include "CuzcoAnimation.h"

CCuzcoAnimation::CCuzcoAnimation()
{
	SetTypeID<CCuzcoAnimation>();
}

CCuzcoAnimation::CCuzcoAnimation(const CCuzcoAnimation& Anim)	:
	CAnimationSequence2DInstance(Anim)
{
}

CCuzcoAnimation::~CCuzcoAnimation()
{
}

bool CCuzcoAnimation::Init()
{
	if (!CAnimationSequence2DInstance::Init())
		return false;

	AddAnimation(TEXT("ZakumSummon.sqc"), ANIMATION_PATH, "ZakumSummon", false);
	AddAnimation(TEXT("CuzcoFly.sqc"), ANIMATION_PATH, "CuzcoFly", true);

	SetEndFunction<CCuzcoAnimation>("ZakumSummon", this, &CCuzcoAnimation::ReturnFly);

	return true;
}

CCuzcoAnimation* CCuzcoAnimation::Clone()
{
	return new CCuzcoAnimation(*this);
}

void CCuzcoAnimation::ReturnFly()
{
	ChangeAnimation("CuzcoFly");
}
