
#include "MonsterAnimation.h"

CMonsterAnimation::CMonsterAnimation()
{
	SetTypeID<CMonsterAnimation>();
}

CMonsterAnimation::CMonsterAnimation(const CMonsterAnimation& Anim) :
	CAnimationSequence2DInstance(Anim)
{
}

CMonsterAnimation::~CMonsterAnimation()
{
}

bool CMonsterAnimation::Init()
{
	if (!CAnimationSequence2DInstance::Init())
		return false;

	AddAnimation(TEXT("RadishIdleLeft.sqc"), ANIMATION_PATH, "RadishIdleLeft", true);
	AddAnimation(TEXT("RadishWalkLeft.sqc"), ANIMATION_PATH, "RadishWalkLeft", true);
	AddAnimation(TEXT("RadishHitLeft.sqc"), ANIMATION_PATH, "RadishHitLeft", false, 0.3f);
	AddAnimation(TEXT("RadishDieLeft.sqc"), ANIMATION_PATH, "RadishDieLeft", false);

	return true;
}

CMonsterAnimation* CMonsterAnimation::Clone()
{
	return new CMonsterAnimation(*this);
}

