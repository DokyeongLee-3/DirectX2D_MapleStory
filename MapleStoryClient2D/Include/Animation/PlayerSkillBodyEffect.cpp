
#include "PlayerSkillBodyEffect.h"
#include "Component/SpriteComponent.h"
#include "../Object/Player2D.h"

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
	AddAnimation("PlayerLevelUpEffect", "PlayerLevelUpEffect", false, 1.8f);

	SetCurrentAnimation(nullptr);

	SetEndFunction<CPlayerSkillBodyEffect>("SylphideLancerBodyEffectLeft", this, &CPlayerSkillBodyEffect::EndSkillEffect);
	SetEndFunction<CPlayerSkillBodyEffect>("LightTransformingLeft", this, &CPlayerSkillBodyEffect::EndSkillEffect);
	
	SetEndFunction<CPlayerSkillBodyEffect>("PlayerLevelUpEffect", this, &CPlayerSkillBodyEffect::LevelUpEndEffect);

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

void CPlayerSkillBodyEffect::LevelUpEndEffect()
{
	CPlayer2D* Player = (CPlayer2D*)m_Owner->GetGameObject();
	Player->SetChanging(false);

	SetCurrentAnimation(nullptr);
	// 원래 위치로 복귀
	//m_Owner->SetRelativePos(0.f, 11.f, 0.f);
}


