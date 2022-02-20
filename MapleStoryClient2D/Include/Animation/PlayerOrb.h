

#include "Animation/AnimationSequence2DInstance.h"

class CPlayerOrb :
    public CAnimationSequence2DInstance
{
public:
	CPlayerOrb();
	CPlayerOrb(const CPlayerOrb& Anim);
	~CPlayerOrb();

public:
	virtual bool Init();
	virtual CPlayerOrb* Clone();
};
