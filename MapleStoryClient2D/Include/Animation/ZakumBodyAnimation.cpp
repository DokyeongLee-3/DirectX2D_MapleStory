
#include "ZakumBodyAnimation.h"

CZakumBodyAnimation::CZakumBodyAnimation()
{
	SetTypeID<CZakumBodyAnimation>();
}

CZakumBodyAnimation::CZakumBodyAnimation(const CZakumBodyAnimation& Anim)	:
	CAnimationSequence2DInstance(Anim)
{
}

CZakumBodyAnimation::~CZakumBodyAnimation()
{
}

bool CZakumBodyAnimation::Init()
{
	if (!CAnimationSequence2DInstance::Init())
		return false;

	AddAnimation(TEXT("ZakumNormalIdle.sqc"), ANIMATION_PATH, "ZakumNormalIdle", true, 1.7f);
	AddAnimation(TEXT("ZakumDestroyIdle.sqc"), ANIMATION_PATH, "ZakumDestroyIdle", true, 1.7f);

	return true;
}

CZakumBodyAnimation* CZakumBodyAnimation::Clone()
{
	return new CZakumBodyAnimation(*this);
}
