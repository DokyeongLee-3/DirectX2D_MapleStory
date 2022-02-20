
#include "SylphideLancerMirrorAnimation.h"
#include "Animation/AnimationSequence2DInstance.h"

CSylphideLancerMirrorAnimation::CSylphideLancerMirrorAnimation()
{
}

CSylphideLancerMirrorAnimation::CSylphideLancerMirrorAnimation(const CSylphideLancerMirrorAnimation& Anim) :
	CAnimationSequence2DInstance(Anim)
{
}

CSylphideLancerMirrorAnimation::~CSylphideLancerMirrorAnimation()
{
}

bool CSylphideLancerMirrorAnimation::Init()
{
	if (!CAnimationSequence2DInstance::Init())
		return false;

	//AddAnimation("Blank", "Blank", false, 1.f);

	AddAnimation("SylphideLancerMuzzle", "SylphideLancerMuzzle", false, 0.7f);

	SetCurrentAnimation(nullptr);

	SetEndFunction<CSylphideLancerMirrorAnimation>("SylphideLancerMuzzle", this, &CSylphideLancerMirrorAnimation::EndSkillEffect);


	return true;
}

CSylphideLancerMirrorAnimation* CSylphideLancerMirrorAnimation::Clone()
{
	return new CSylphideLancerMirrorAnimation(*this);
}

void CSylphideLancerMirrorAnimation::EndSkillEffect()
{
	SetCurrentAnimation(nullptr);
}
