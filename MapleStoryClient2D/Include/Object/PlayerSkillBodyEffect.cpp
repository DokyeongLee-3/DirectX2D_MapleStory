
#include "PlayerSkillBodyEffect.h"

CPlayerSkillBodyEffect::CPlayerSkillBodyEffect()
{
}

CPlayerSkillBodyEffect::CPlayerSkillBodyEffect(const CPlayerSkillBodyEffect& Anim)	:
	CAnimationSequence2DInstance(Anim)
{
}

CPlayerSkillBodyEffect::~CPlayerSkillBodyEffect()
{
}

bool CPlayerSkillBodyEffect::Init()
{
	if (!CAnimationSequence2DInstance::Init())
		return false;

	AddAnimation("Blank", "Blank", false, 1.f);
	AddAnimation("SylphideLancerBodyEffectLeft", "SylphideLancerBodyEffectLeft", false, 0.7f);


	SetEndFunction<CPlayerSkillBodyEffect>("SylphideLancerBodyEffectLeft", this, &CPlayerSkillBodyEffect::EndSkillEffect);


	return true;
}

CPlayerSkillBodyEffect* CPlayerSkillBodyEffect::Clone()
{
	return new CPlayerSkillBodyEffect(*this);
}

void CPlayerSkillBodyEffect::EndSkillEffect()
{
	ChangeAnimation("Blank");
}
