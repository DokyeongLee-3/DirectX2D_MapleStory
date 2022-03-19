
#include "ZakumFlameAnimation.h"

CZakumFlameAnimation::CZakumFlameAnimation()
{
	SetTypeID<CZakumFlameAnimation>();
}

CZakumFlameAnimation::CZakumFlameAnimation(const CZakumFlameAnimation& Anim)	:
	CAnimationSequence2DInstance(Anim)
{
}

CZakumFlameAnimation::~CZakumFlameAnimation()
{
}

bool CZakumFlameAnimation::Init()
{
	if (!CAnimationSequence2DInstance::Init())
		return false;

	AddAnimation(TEXT("ZakumFlame.sqc"), ANIMATION_PATH, "ZakumFlame", false, 0.5f);

	return true;
}

CZakumFlameAnimation* CZakumFlameAnimation::Clone()
{
	return new CZakumFlameAnimation(*this);
}
