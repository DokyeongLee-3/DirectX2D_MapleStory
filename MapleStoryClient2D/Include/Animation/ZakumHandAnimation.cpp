
#include "ZakumHandAnimation.h"
#include "../Object/ZakumBody.h"

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

	AddAnimation(TEXT("ZakumLeftHandSmash.sqc"), ANIMATION_PATH, "ZakumLeftHandSmash", false, 0.3f);
	AddAnimation(TEXT("ZakumLeftHandSmash2.sqc"), ANIMATION_PATH, "ZakumLeftHandSmash2", false, 0.3f);

	AddAnimation(TEXT("ZakumArmReturnLeft1.sqc"), ANIMATION_PATH, "ZakumArmReturnLeft1", false, 0.2f);
	AddAnimation(TEXT("ZakumArmReturnLeft2.sqc"), ANIMATION_PATH, "ZakumArmReturnLeft2", false, 0.2f);

	AddAnimation(TEXT("ZakumClapLeft.sqc"), ANIMATION_PATH, "ZakumClapLeft", false, 0.25f);
	AddAnimation(TEXT("ZakumClapRight.sqc"), ANIMATION_PATH, "ZakumClapRight", false, 0.33f);

	AddAnimation(TEXT("ZakumClapReturnLeft.sqc"), ANIMATION_PATH, "ZakumClapReturnLeft", false, 0.2f);
	AddAnimation(TEXT("ZakumClapReturnRight.sqc"), ANIMATION_PATH, "ZakumClapReturnRight", false, 0.2f);

	CZakumBody* Zakum = (CZakumBody*)m_Owner->GetGameObject();

	SetEndFunction<CZakumBody>("ZakumArmReturnLeft1", Zakum, &CZakumBody::SmashReturnArmOriginPos);
	SetEndFunction<CZakumBody>("ZakumArmReturnLeft2", Zakum, &CZakumBody::SmashReturnArmOriginPos);

	//SetEndFunction<CZakumBody>("ZakumClapLeft", Zakum, &CZakumBody::CreateClapEffect);
	SetEndFunction<CZakumBody>("ZakumClapRight", Zakum, &CZakumBody::CreateClapEffect);

	SetEndFunction<CZakumBody>("ZakumClapReturnLeft", Zakum, &CZakumBody::ClapReturnArmOriginPos);
	SetEndFunction<CZakumBody>("ZakumClapReturnRight", Zakum, &CZakumBody::ClapReturnArmOriginPos);

	return true;
}

CZakumHandAnimation* CZakumHandAnimation::Clone()
{
	return new CZakumHandAnimation(*this);
}
