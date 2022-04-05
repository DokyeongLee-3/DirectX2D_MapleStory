
#include "PunkoAnimation.h"

CPunkoAnimation::CPunkoAnimation()
{
	SetTypeID<CPunkoAnimation>();
}

CPunkoAnimation::CPunkoAnimation(const CPunkoAnimation& Anim)	:
	CAnimationSequence2DInstance(Anim)
{
}

CPunkoAnimation::~CPunkoAnimation()
{
}

bool CPunkoAnimation::Init()
{
	if (!CAnimationSequence2DInstance::Init())
		return false;

	//AddAnimation(TEXT("ZakumSummon.sqc"), ANIMATION_PATH, "ZakumSummon", false);
	AddAnimation(TEXT("PunkoFly.sqc"), ANIMATION_PATH, "PunkoFly", true);

	return true;
}

CPunkoAnimation* CPunkoAnimation::Clone()
{
	return new CPunkoAnimation(*this);
}
