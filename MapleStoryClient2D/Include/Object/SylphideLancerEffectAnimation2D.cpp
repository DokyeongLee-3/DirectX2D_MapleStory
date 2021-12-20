#include "SylphideLancerEffectAnimation2D.h"

CSylphideLancerEffectAnimation2D::CSylphideLancerEffectAnimation2D()
{
}

CSylphideLancerEffectAnimation2D::CSylphideLancerEffectAnimation2D(const CSylphideLancerEffectAnimation2D& Anim) :
	CAnimationSequence2DInstance(Anim)
{
}

CSylphideLancerEffectAnimation2D::~CSylphideLancerEffectAnimation2D()
{
}

bool CSylphideLancerEffectAnimation2D::Init()
{
	if (!CAnimationSequence2DInstance::Init())
		return false;

	AddAnimation("SylphideLancerMirrorLeft", "SylphideLancerMirrorLeft", false, 1.f);
	AddAnimation("SylphideLancerMirrorRight", "SylphideLancerMirrorRight", false, 1.f, 1.f, true);

	AddAnimation("SylphideLancerHitEffect", "SylphideLancerHitEffect", false, 1.f, 1.f, true);

	AddAnimation("Blank", "Blank", false, 1.f);

	SetEndFunction<CSylphideLancerEffectAnimation2D>("SylphideLancerMirrorLeft", this, &CSylphideLancerEffectAnimation2D::EndSkillEffect);
	SetEndFunction<CSylphideLancerEffectAnimation2D>("SylphideLancerMirrorRight", this, &CSylphideLancerEffectAnimation2D::EndSkillEffect);

	SetEndFunction<CSylphideLancerEffectAnimation2D>("SylphideLancerHitEffect", this, &CSylphideLancerEffectAnimation2D::EndSkillEffect);

	return true;
}

CSylphideLancerEffectAnimation2D* CSylphideLancerEffectAnimation2D::Clone()
{
	return new CSylphideLancerEffectAnimation2D(*this);
}

void CSylphideLancerEffectAnimation2D::EndSkillEffect()
{
	ChangeAnimation("Blank");
}
