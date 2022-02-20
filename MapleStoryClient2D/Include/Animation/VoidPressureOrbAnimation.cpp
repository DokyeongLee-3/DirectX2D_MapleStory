
#include "VoidPressureOrbAnimation.h"
#include "Animation/AnimationSequence2DInstance.h"
#include "Component/SpriteComponent.h"
#include "../Object/Player2D.h"

CVoidPressureOrbAnimation::CVoidPressureOrbAnimation()
{
}

CVoidPressureOrbAnimation::CVoidPressureOrbAnimation(const CVoidPressureOrbAnimation& Anim)	: 
	CAnimationSequence2DInstance(Anim)
{
}

CVoidPressureOrbAnimation::~CVoidPressureOrbAnimation()
{
}

bool CVoidPressureOrbAnimation::Init()
{
	if (!CAnimationSequence2DInstance::Init())
		return false;

	AddAnimation("VoidPressureOrb", "VoidPressureOrb", true, 1.5f);
	AddAnimation("VoidPressureOrbDestroy", "VoidPressureOrbDestroy", false, 0.6f);
	SetCurrentAnimation(nullptr);

	SetEndFunction<CVoidPressureOrbAnimation>("VoidPressureOrbDestroy", this, &CVoidPressureOrbAnimation::EndSkillEffect);


	return true;
}

CVoidPressureOrbAnimation* CVoidPressureOrbAnimation::Clone()
{
	return new CVoidPressureOrbAnimation(*this);
}

void CVoidPressureOrbAnimation::EndSkillEffect()
{
	//CPlayer2D* Player = (CPlayer2D*)GetOwner()->GetScene()->GetPlayerObject();

	//Player->SetVoidPressureOrb(nullptr);

	//SetCurrentAnimation(nullptr);

	GetOwner()->GetGameObject()->Enable(false);
}
