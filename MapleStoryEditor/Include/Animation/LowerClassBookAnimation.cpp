
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

	AddAnimation(TEXT("ZakumLeftHand.sqc"), ANIMATION_PATH, "ZakumLeftHand", true, 1.f);
	AddAnimation(TEXT("ZakumRightHand.sqc"), ANIMATION_PATH, "ZakumRightHand", true, 1.f);
	AddAnimation(TEXT("ZakumLeftHandSmash.sqc"), ANIMATION_PATH, "ZakumLeftHandSmash", true, 1.f);
	AddAnimation(TEXT("ZakumLeftHandSmash2.sqc"), ANIMATION_PATH, "ZakumLeftHandSmash2", true, 1.f);
	AddAnimation(TEXT("ZakumFlame.sqc"), ANIMATION_PATH, "ZakumFlame", true, 0.8f);

	AddAnimation(TEXT("ZakumClapLeft.sqc"), ANIMATION_PATH, "ZakumClapLeft", true, 0.5f);
	AddAnimation(TEXT("ZakumClapRight.sqc"), ANIMATION_PATH, "ZakumClapRight", true, 0.5f);
	AddAnimation(TEXT("ZakumClapCircleShockWave.sqc"), ANIMATION_PATH, "ZakumClapCircleShockWave", true, 0.8f);
	AddAnimation(TEXT("ZakumPinkShockWave.sqc"), ANIMATION_PATH, "ZakumPinkShockWave", true, 0.8f);

	return true;
}

CLowerClassBookAnimation* CLowerClassBookAnimation::Clone()
{
	return new CLowerClassBookAnimation(*this);
}

