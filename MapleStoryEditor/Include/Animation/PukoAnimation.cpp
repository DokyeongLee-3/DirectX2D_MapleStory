
#include "PukoAnimation.h"

CPukoAnimation::CPukoAnimation()
{
	SetTypeID<CPukoAnimation>();
}

CPukoAnimation::CPukoAnimation(const CPukoAnimation& Anim)	:
	CAnimationSequence2DInstance(Anim)
{
}

CPukoAnimation::~CPukoAnimation()
{
}

bool CPukoAnimation::Init()
{
	if (!CAnimationSequence2DInstance::Init())
		return false;

	//AddAnimation(TEXT("ZakumSummon.sqc"), ANIMATION_PATH, "ZakumSummon", false);
	AddAnimation(TEXT("PukoFly.sqc"), ANIMATION_PATH, "PukoFly", true);
	AddAnimation(TEXT("PukoDie.sqc"), ANIMATION_PATH, "PukoDie", true, 1.2f);

	return true;

}

CPukoAnimation* CPukoAnimation::Clone()
{
	return new CPukoAnimation(*this);
}
