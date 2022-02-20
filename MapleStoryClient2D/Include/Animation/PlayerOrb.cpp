
#include "PlayerOrb.h"

CPlayerOrb::CPlayerOrb()
{
}

CPlayerOrb::CPlayerOrb(const CPlayerOrb& Anim) :
	CAnimationSequence2DInstance(Anim)
{
}

CPlayerOrb::~CPlayerOrb()
{
}

bool CPlayerOrb::Init()
{
	if (!CAnimationSequence2DInstance::Init())
		return false;

	AddAnimation("PlayerOrb", "PlayerOrb", true, 0.8f);


	return true;
}

CPlayerOrb* CPlayerOrb::Clone()
{
	return new CPlayerOrb(*this);
}

