
#include "PlayerAnimation2D.h"
#include "Player2D.h"
#include "SylphideLancerArrow.h"
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
	AddAnimation(TEXT("PlayerHealLeft.sqc"), ANIMATION_PATH, "HealLeft", true);
	AddAnimation(TEXT("PlayerRope.sqc"), ANIMATION_PATH, "Rope", true);
	AddAnimation(TEXT("PlayerJumpLeft.sqc"), ANIMATION_PATH, "JumpLeft", true);

	/*CAnimationSequence2DData* Data = FindAnimation("PlayerSkillActionLeft");

	Data->SetEndFunction<CPlayer2D>(Player, &CPlayer2D::Stand);
	Data->AddNotify<CPlayerAnimation2D>("PlayerSkillActionLeft", 2,
		this, &CPlayerAnimation2D::CreateSylphideLancerEffect);


	Data = FindAnimation("PlayerSkillActionRight");

	Data->SetEndFunction<CPlayer2D>(Player, &CPlayer2D::Stand);
	Data->AddNotify<CPlayerAnimation2D>("PlayerSkillActionRight", 0,
		this, &CPlayerAnimation2D::CreateSylphideLancerEffect);*/

	return true;
}

CPlayerAnimation2D* CPlayerAnimation2D::Clone()
{
	return new CPlayerAnimation2D(*this);
}

void CPlayerAnimation2D::CreateSylphideLancerEffect()
{
	CSpriteComponent* PlayerSprite = GetOwner();
	CPlayer2D* Player = (CPlayer2D*)PlayerSprite->GetGameObject();
	CSceneComponent* PlayerSkillMuzzle = Player->GetSkillMuzzle();

	Vector3 MuzzlePos = PlayerSkillMuzzle->GetWorldPos();

	CSylphideLancerArrow* Arrow = Player->GetScene()->CreateGameObject<CSylphideLancerArrow>("SylphideLancerArrow");
	Arrow->SetWorldPos(PlayerSkillMuzzle->GetWorldPos());

	std::string CurAnim = Player->GetRootSpriteComponent()->GetCurrentAnimationName();

	if (CurAnim.find("Left") != std::string::npos)
	{
		Arrow->ChangeRootSpriteAnimation("SylphideLancerArrowLeft");
	}

	else
	{
		Arrow->ChangeRootSpriteAnimation("SylphideLancerArrowRight");
	}
}
