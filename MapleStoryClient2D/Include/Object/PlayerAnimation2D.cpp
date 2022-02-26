
#include "PlayerAnimation2D.h"
#include "Player2D.h"
#include "Scene/Scene.h"

CPlayerAnimation2D::CPlayerAnimation2D()
{
}

CPlayerAnimation2D::CPlayerAnimation2D(const CPlayerAnimation2D& Anim) :
	CAnimationSequence2DInstance(Anim)
{
}

CPlayerAnimation2D::~CPlayerAnimation2D()
{
}

bool CPlayerAnimation2D::Init()
{
	if (!CAnimationSequence2DInstance::Init())
		return false;

	// AddAnimation하면 SpriteComponent가 갖고 있는
	// Material의 Texture를 Animation에 대한 Texture로
	// 바꿔서 Load하고 그 텍스쳐에 대한 Shader Resource View로
	// 알아서 바인딩해준다
	AddAnimation(TEXT("PlayerIdleLeft.sqc"), ANIMATION_PATH, "IdleLeft", true);
	AddAnimation(TEXT("PlayerWalkLeft.sqc"), ANIMATION_PATH, "WalkLeft", true);
	AddAnimation(TEXT("PlayerHealLeft.sqc"), ANIMATION_PATH, "HealLeft", false, 0.5f);
	AddAnimation(TEXT("PlayerRope.sqc"), ANIMATION_PATH, "Rope", false);
	AddAnimation(TEXT("PlayerJumpLeft.sqc"), ANIMATION_PATH, "JumpLeft", false);

	//CAnimationSequence2DData* Data = FindAnimation("HealLeft");
	//Data->SetEndFunction<CPlayerAnimation2D>(this, &CPlayerAnimation2D::ReturnToIdle);


	return true;
}

CPlayerAnimation2D* CPlayerAnimation2D::Clone()
{
	return new CPlayerAnimation2D(*this);
}



void CPlayerAnimation2D::ReturnToIdle()
{
	ChangeAnimation("IdleLeft");
}
