
#include "CuzcoAnimation.h"
#include "Component/SpriteComponent.h"
#include "../Object/Cuzco.h"

CCuzcoAnimation::CCuzcoAnimation()
{
	SetTypeID<CCuzcoAnimation>();
}

CCuzcoAnimation::CCuzcoAnimation(const CCuzcoAnimation& Anim)	:
	CAnimationSequence2DInstance(Anim)
{
}

CCuzcoAnimation::~CCuzcoAnimation()
{
}

bool CCuzcoAnimation::Init()
{
	if (!CAnimationSequence2DInstance::Init())
		return false;

	AddAnimation(TEXT("ZakumSummon.sqc"), ANIMATION_PATH, "ZakumSummon", false, 1.3f);
	AddAnimation(TEXT("CuzcoFly.sqc"), ANIMATION_PATH, "CuzcoFly", true);
	AddAnimation(TEXT("CuzcoDie.sqc"), ANIMATION_PATH, "CuzcoDie", false);

	SetEndFunction<CCuzcoAnimation>("ZakumSummon", this, &CCuzcoAnimation::ReturnFly);

	CMonster* Owner = (CMonster*)m_Owner->GetGameObject();

	SetEndFunction<CMonster>("CuzcoDie", Owner, &CMonster::Die);

	return true;
}

CCuzcoAnimation* CCuzcoAnimation::Clone()
{
	return new CCuzcoAnimation(*this);
}

void CCuzcoAnimation::ReturnFly()
{
	ChangeAnimation("CuzcoFly");

	CGameObject* Object = m_Owner->GetGameObject();

	if (Object)
	{
		((CCuzco*)Object)->GetBody()->Enable(true);
	}
}
