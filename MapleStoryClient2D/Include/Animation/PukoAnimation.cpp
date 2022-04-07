
#include "PukoAnimation.h"
#include "Component/SpriteComponent.h"
#include "../Object/Puko.h"

CPukoAnimation::CPukoAnimation()
{
	SetTypeID<CPukoAnimation>();
}

CPukoAnimation::CPukoAnimation(const CPukoAnimation& Anim)	:
	CAnimationSequence2DInstance(Anim)
{
}

CPukoAnimation::~CPukoAnimation()
{
}

bool CPukoAnimation::Init()
{
	if (!CAnimationSequence2DInstance::Init())
		return false;

	AddAnimation(TEXT("ZakumSummon.sqc"), ANIMATION_PATH, "ZakumSummon", false, 1.3f);
	AddAnimation(TEXT("PukoFly.sqc"), ANIMATION_PATH, "PukoFly", true);
	AddAnimation(TEXT("PukoDie.sqc"), ANIMATION_PATH, "PukoDie", false, 1.2f);

	SetEndFunction<CPukoAnimation>("ZakumSummon", this, &CPukoAnimation::ReturnFly);

	CMonster* Owner = (CMonster*)m_Owner->GetGameObject();

	SetEndFunction<CMonster>("PukoDie", Owner, &CMonster::Die);

	return true;

}

CPukoAnimation* CPukoAnimation::Clone()
{
	return new CPukoAnimation(*this);
}

void CPukoAnimation::ReturnFly()
{
	ChangeAnimation("PukoFly");

	CGameObject* Object = m_Owner->GetGameObject();

	if (Object)
	{
		((CPuko*)Object)->GetBody()->Enable(true);
	}
}
