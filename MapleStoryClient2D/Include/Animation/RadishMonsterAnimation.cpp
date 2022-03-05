
#include "RadishMonsterAnimation.h"

CRadishMonsterAnimation::CRadishMonsterAnimation()
{
	SetTypeID<CRadishMonsterAnimation>();
}

CRadishMonsterAnimation::CRadishMonsterAnimation(const CRadishMonsterAnimation& Anim) :
	CAnimationSequence2DInstance(Anim)
{
}

CRadishMonsterAnimation::~CRadishMonsterAnimation()
{
}

bool CRadishMonsterAnimation::Init()
{
	if (!CAnimationSequence2DInstance::Init())
		return false;

	AddAnimation(TEXT("RadishIdleLeft.sqc"), ANIMATION_PATH, "RadishIdleLeft", true);
	AddAnimation(TEXT("RadishWalkLeft.sqc"), ANIMATION_PATH, "RadishWalkLeft", true);
	AddAnimation(TEXT("RadishHitLeft.sqc"), ANIMATION_PATH, "RadishHitLeft", false, 0.6f);
	AddAnimation(TEXT("RadishDieLeft.sqc"), ANIMATION_PATH, "RadishDieLeft", false, 1.5f);

	return true;
}

CRadishMonsterAnimation* CRadishMonsterAnimation::Clone()
{
	return new CRadishMonsterAnimation(*this);
}

