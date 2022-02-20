
#include "VoidPressureAttackSphere.h"
#include "Animation/AnimationSequence2DInstance.h"
#include "Component/SpriteComponent.h"
#include "../Object/Player2D.h"
#include"../Object/VoidPressureOrb.h"

CVoidPressureAttackSphere::CVoidPressureAttackSphere()
{
}

CVoidPressureAttackSphere::CVoidPressureAttackSphere(const CVoidPressureAttackSphere& Anim)	: 
	CAnimationSequence2DInstance(Anim)
{
}

CVoidPressureAttackSphere::~CVoidPressureAttackSphere()
{
}

bool CVoidPressureAttackSphere::Init()
{
	if (!CAnimationSequence2DInstance::Init())
		return false;

	AddAnimation("VoidPressureReadyLeft", "VoidPressureReadyLeft", false, 0.4f);
	AddAnimation("VoidPressureStartLeft", "VoidPressureStartLeft", true, 1.4f);
	AddAnimation("VoidPressureDestroy", "VoidPressureDestroy", false, 0.6f);

	AddNotify<CVoidPressureAttackSphere>("VoidPressureDestroy", "VoidPressureDestroy", 0, this, &CVoidPressureAttackSphere::DisableColliderComponent);

	SetEndFunction<CVoidPressureAttackSphere>("VoidPressureReadyLeft", this, &CVoidPressureAttackSphere::EndReadyEffect);
	SetEndFunction<CVoidPressureAttackSphere>("VoidPressureDestroy", this, &CVoidPressureAttackSphere::EndSkillEffect);


	return true;
}

CVoidPressureAttackSphere* CVoidPressureAttackSphere::Clone()
{
	return new CVoidPressureAttackSphere(*this);
}

void CVoidPressureAttackSphere::EndSkillEffect()
{
	/*CPlayer2D* Player = (CPlayer2D*)GetOwner()->GetScene()->GetPlayerObject();

	Player->SetVoidPressure(nullptr);*/

	// SetCurrentAnimation(nullptr);

	GetOwner()->GetGameObject()->Enable(false);
}

void CVoidPressureAttackSphere::DisableColliderComponent()
{
	CSceneComponent* Body = GetOwner()->FindComponent("VoidPressureSpriteBody");
	Body->Enable(false);
}

void CVoidPressureAttackSphere::EndReadyEffect()
{
	ChangeAnimation("VoidPressureStartLeft");

	CSceneComponent* Body = GetOwner()->FindComponent("VoidPressureSpriteBody");
	Body->Enable(true);

	CPlayer2D* Player = (CPlayer2D*)GetOwner()->GetScene()->GetPlayerObject();

	CVoidPressureOrb* Orb = Player->GetVoidPressureOrb();

	CSpriteComponent* Root = (CSpriteComponent *)(Orb->GetRootComponent());
	Root->ChangeAnimation("VoidPressureOrb");
}
