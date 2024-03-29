
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

	//AddAnimation(TEXT("ZakumSummon.sqc"), ANIMATION_PATH, "ZakumSummon", false);
	AddAnimation(TEXT("CuzcoFly.sqc"), ANIMATION_PATH, "CuzcoFly", true);
	AddAnimation(TEXT("CuzcoDie.sqc"), ANIMATION_PATH, "CuzcoDie", true);

	return true;
}

CCuzcoAnimation* CCuzcoAnimation::Clone()
{
	return new CCuzcoAnimation(*this);
}
