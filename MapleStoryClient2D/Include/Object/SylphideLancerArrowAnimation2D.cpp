
#include "SylphideLancerArrowAnimation2D.h"

CSylphideLancerArrowAnimation2D::CSylphideLancerArrowAnimation2D()
{
}

CSylphideLancerArrowAnimation2D::CSylphideLancerArrowAnimation2D(const CSylphideLancerArrowAnimation2D& Anim) :
	CAnimationSequence2DInstance(Anim)
{
}

CSylphideLancerArrowAnimation2D::~CSylphideLancerArrowAnimation2D()
{
}

bool CSylphideLancerArrowAnimation2D::Init()
{
	if (!CAnimationSequence2DInstance::Init())
		return false;


	AddAnimation("SylphideLancerArrowLeft", "SylphideLancerArrowLeft", true, 0.35f);
	AddAnimation("SylphideLancerArrowRight", "SylphideLancerArrowRight", true, 0.35f, 1.f, true);

	return true;
}

CSylphideLancerArrowAnimation2D* CSylphideLancerArrowAnimation2D::Clone()
{
	return new CSylphideLancerArrowAnimation2D(*this);
}