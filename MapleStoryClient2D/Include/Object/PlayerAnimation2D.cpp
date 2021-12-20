
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

	// AddAnimation�ϸ� SpriteComponent�� ���� �ִ�
	// Material�� Texture�� Animation�� ���� Texture��
	// �ٲ㼭 Load�ϰ� �� �ؽ��Ŀ� ���� Shader Resource View��
	// �˾Ƽ� ���ε����ش�

	CSpriteComponent* PlayerSprite = GetOwner();
	CPlayer2D* Player = (CPlayer2D*)PlayerSprite->GetGameObject();

	AddAnimation("PlayerStandLeft", "PlayerStandLeft");
	AddAnimation("PlayerStandRight", "PlayerStandRight", true, 1.f, 1.f, true);
	AddAnimation("PlayerWalkLeft", "PlayerWalkLeft");
	AddAnimation("PlayerWalkRight", "PlayerWalkRight", true, 1.f, 1.f, true);
	AddAnimation("PlayerSkillActionLeft", "PlayerSkillActionLeft",
		false, 0.6f, 1.f, false);
	AddAnimation("PlayerSkillActionRight", "PlayerSkillActionRight",
		false, 0.6f, 1.f, true);

	CAnimationSequence2DData* Data = FindAnimation("PlayerSkillActionLeft");

	Data->SetEndFunction<CPlayer2D>(Player, &CPlayer2D::Stand);
	Data->AddNotify<CPlayerAnimation2D>("PlayerSkillActionLeft", 2,
		this, &CPlayerAnimation2D::CreateSylphideLancerEffect);


	Data = FindAnimation("PlayerSkillActionRight");

	Data->SetEndFunction<CPlayer2D>(Player, &CPlayer2D::Stand);
	Data->AddNotify<CPlayerAnimation2D>("PlayerSkillActionRight", 0,
		this, &CPlayerAnimation2D::CreateSylphideLancerEffect);

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
