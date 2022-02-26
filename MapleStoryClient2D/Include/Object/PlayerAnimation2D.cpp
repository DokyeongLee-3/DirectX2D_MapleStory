
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

	// AddAnimation�ϸ� SpriteComponent�� ���� �ִ�
	// Material�� Texture�� Animation�� ���� Texture��
	// �ٲ㼭 Load�ϰ� �� �ؽ��Ŀ� ���� Shader Resource View��
	// �˾Ƽ� ���ε����ش�
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
