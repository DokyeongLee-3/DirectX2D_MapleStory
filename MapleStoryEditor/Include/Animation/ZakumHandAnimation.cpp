
#include "ZakumHandAnimation.h"
#include "Component/SpriteComponent.h"

CZakumHandAnimation::CZakumHandAnimation()
{
	SetTypeID<CZakumHandAnimation>();
}

CZakumHandAnimation::CZakumHandAnimation(const CZakumHandAnimation& Anim)	:
	CAnimationSequence2DInstance(Anim)
{
}

CZakumHandAnimation::~CZakumHandAnimation()
{
}

bool CZakumHandAnimation::Init()
{
	if (!CAnimationSequence2DInstance::Init())
		return false;

	AddAnimation(TEXT("ZakumLeftHand.sqc"), ANIMATION_PATH, "ZakumLeftHand", false, 0.5f);
	AddAnimation(TEXT("ZakumRightHand.sqc"), ANIMATION_PATH, "ZakumRightHand", false, 0.5f);
	AddAnimation(TEXT("ZakumLeftHandSmash.sqc"), ANIMATION_PATH, "ZakumLeftHandSmash", false, 0.5f);

	AddAnimation(TEXT("ZakumArmReturnLeft1.sqc"), ANIMATION_PATH, "ZakumArmReturnLeft1", false, 0.4f);
	AddAnimation(TEXT("ZakumArmReturnLeft2.sqc"), ANIMATION_PATH, "ZakumArmReturnLeft2", false, 0.4f);

	return true;
}

CZakumHandAnimation* CZakumHandAnimation::Clone()
{
	return new CZakumHandAnimation(*this);
}
