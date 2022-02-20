
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

	//AddAnimation("Blank", "Blank", false, 1.f);
	AddAnimation("SylphideLancerBodyEffectLeft", "SylphideLancerBodyEffectLeft", false, 0.4f);
	AddAnimation("LightTransformingLeft", "LightTransformingLeft", false, 0.4f);

	SetCurrentAnimation(nullptr);

	SetEndFunction<CPlayerSkillBodyEffect>("SylphideLancerBodyEffectLeft", this, &CPlayerSkillBodyEffect::EndSkillEffect);
	SetEndFunction<CPlayerSkillBodyEffect>("LightTransformingLeft", this, &CPlayerSkillBodyEffect::EndSkillEffect);

	return true;
}

CPlayerSkillBodyEffect* CPlayerSkillBodyEffect::Clone()
{
	return new CPlayerSkillBodyEffect(*this);
}

void CPlayerSkillBodyEffect::EndSkillEffect()
{
	SetCurrentAnimation(nullptr);
}
