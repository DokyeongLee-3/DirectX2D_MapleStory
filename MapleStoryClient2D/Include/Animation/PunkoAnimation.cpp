
#include "PunkoAnimation.h"
#include "Component/SpriteComponent.h"
#include "../Object/Punko.h"

CPunkoAnimation::CPunkoAnimation()
{
	SetTypeID<CPunkoAnimation>();
}

CPunkoAnimation::CPunkoAnimation(const CPunkoAnimation& Anim)	:
	CAnimationSequence2DInstance(Anim)
{
}

CPunkoAnimation::~CPunkoAnimation()
{
}

bool CPunkoAnimation::Init()
{
	if (!CAnimationSequence2DInstance::Init())
		return false;

	AddAnimation(TEXT("ZakumSummon.sqc"), ANIMATION_PATH, "ZakumSummon", false, 1.3f);
	AddAnimation(TEXT("PunkoFly.sqc"), ANIMATION_PATH, "PunkoFly", true);
	AddAnimation(TEXT("PunkoDie.sqc"), ANIMATION_PATH, "PunkoDie", false, 1.2f);

	SetEndFunction<CPunkoAnimation>("ZakumSummon", this, &CPunkoAnimation::ReturnFly);

	CMonster* Owner = (CMonster*)m_Owner->GetGameObject();

	SetEndFunction<CMonster>("PunkoDie", Owner, &CMonster::Die);

	return true;
}

CPunkoAnimation* CPunkoAnimation::Clone()
{
	return new CPunkoAnimation(*this);
}

void CPunkoAnimation::ReturnFly()
{
	ChangeAnimation("PunkoFly");

	CGameObject* Object = m_Owner->GetGameObject();

	if (Object)
	{
		((CPunko*)Object)->GetBody()->Enable(true);
	}
}
