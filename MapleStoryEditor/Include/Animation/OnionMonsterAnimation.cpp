
#include "OnionMonsterAnimation.h"

COnionMonsterAnimation::COnionMonsterAnimation()
{
	SetTypeID<COnionMonsterAnimation>();
}

COnionMonsterAnimation::COnionMonsterAnimation(const COnionMonsterAnimation& Anim) :
	CAnimationSequence2DInstance(Anim)
{
}

COnionMonsterAnimation::~COnionMonsterAnimation()
{
}

bool COnionMonsterAnimation::Init()
{
	if (!CAnimationSequence2DInstance::Init())
		return false;

	AddAnimation(TEXT("OnionIdleLeft.sqc"), ANIMATION_PATH, "OnionIdleLeft", true);	
	AddAnimation(TEXT("OnionWalkLeft.sqc"), ANIMATION_PATH, "OnionWalkLeft", true);
	AddAnimation(TEXT("OnionHitLeft.sqc"), ANIMATION_PATH, "OnionHitLeft", false, 0.6f);
	AddAnimation(TEXT("OnionDieLeft.sqc"), ANIMATION_PATH, "OnionDieLeft", false, 1.5f);

	return true;
}

COnionMonsterAnimation* COnionMonsterAnimation::Clone()
{
	return new COnionMonsterAnimation(*this);
}

