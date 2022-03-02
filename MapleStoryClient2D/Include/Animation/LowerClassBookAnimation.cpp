
#include "LowerClassBookAnimation.h"

CLowerClassBookAnimation::CLowerClassBookAnimation()
{
	SetTypeID<CLowerClassBookAnimation>();
}

CLowerClassBookAnimation::CLowerClassBookAnimation(const CLowerClassBookAnimation& Anim) :
	CAnimationSequence2DInstance(Anim)
{
}

CLowerClassBookAnimation::~CLowerClassBookAnimation()
{
}

bool CLowerClassBookAnimation::Init()
{
	if (!CAnimationSequence2DInstance::Init())
		return false;

	AddAnimation(TEXT("LowerClassBookIdleLeft.sqc"), ANIMATION_PATH, "LowerClassBookIdleLeft", true);
	AddAnimation(TEXT("LowerClassBookHitLeft.sqc"), ANIMATION_PATH, "LowerClassBookHitLeft", false, 0.6f);
	AddAnimation(TEXT("LowerClassBookDieLeft.sqc"), ANIMATION_PATH, "LowerClassBookDieLeft", false, 1.5f);
	AddAnimation(TEXT("LowerClassBookAttackLeft.sqc"), ANIMATION_PATH, "LowerClassBookAttackLeft", false, 2.f);

	return true;
}

CLowerClassBookAnimation* CLowerClassBookAnimation::Clone()
{
	return new CLowerClassBookAnimation(*this);
}

