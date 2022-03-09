
#include "PlayerSkillSet.h"
#include "Animation/AnimationSequence2DInstance.h"
#include "Input.h"
#include "../EditorManager.h"
#include "../Window/SpriteWindow.h"

CPlayerSkillSet::CPlayerSkillSet() :
	m_Sprite(nullptr),
	m_Body(nullptr)
{
	SetTypeID<CPlayerSkillSet>();
}

CPlayerSkillSet::CPlayerSkillSet(const CPlayerSkillSet& obj) :
	CGameObject(obj)
{
	m_Sprite = (CSpriteComponent*)FindComponent("PlayerSkillSprite");
	m_Body = (CColliderBox2D*)FindComponent("Body");
}

CPlayerSkillSet::~CPlayerSkillSet()
{
}

void CPlayerSkillSet::Start()
{
	CGameObject::Start();
}

bool CPlayerSkillSet::Init()
{
	m_Sprite = CreateComponent<CSpriteComponent>("PlayerSkillSprite");
	m_Body = CreateComponent<CColliderBox2D>("Body");

	m_Body->SetCollisionProfile("PlayerAttack");

	SetRootComponent(m_Sprite);
	m_Sprite->AddChild(m_Body);

	m_Sprite->SetTransparency(true);

	m_Sprite->CreateAnimationInstance<CAnimationSequence2DInstance>();

	m_Sprite->SetWorldScale(100.f, 100.f, 1.f);
	m_Sprite->SetRelativePos(500.f, 300.f, 0.f);
	m_Sprite->SetPivot(0.5f, 0.5f, 0.f);




	CAnimationSequence2DInstance* Anim = m_Sprite->GetAnimationInstance();

	Anim->AddAnimation(TEXT("PlayerOrb.sqc"), ANIMATION_PATH, "PlayerOrb", true, 0.8f);

	Anim->AddAnimation(TEXT("SylphideLancerArrowLeft.sqc"), ANIMATION_PATH, "SylphideLancerArrowLeft", true, 0.3f);
	Anim->AddAnimation(TEXT("SylphideLancerArrowPurple.sqc"), ANIMATION_PATH, "SylphideLancerArrowPurple", true, 0.3f);
	Anim->AddAnimation(TEXT("SylphideLancerHit.sqc"), ANIMATION_PATH, "SylphideLancerHit", true, 0.7f);
	Anim->AddAnimation(TEXT("SylphideLancerHitPurple.sqc"), ANIMATION_PATH, "SylphideLancerHitPurple", true, 0.7f);

	Anim->AddAnimation(TEXT("SylphideLancerBodyEffectLeft.sqc"), ANIMATION_PATH, "SylphideLancerBodyEffectLeft", true, 0.6f);
	Anim->AddAnimation(TEXT("SylphideLancerMuzzleLeft.sqc"), ANIMATION_PATH, "SylphideLancerMuzzleLeft", true, 0.9f);

	Anim->AddAnimation(TEXT("VoidPressureReadyLeft.sqc"), ANIMATION_PATH, "VoidPressureReadyLeft", true, 1.f);
	Anim->AddAnimation(TEXT("VoidPressureStartLeft.sqc"), ANIMATION_PATH, "VoidPressureStartLeft", true, 1.f);
	Anim->AddAnimation(TEXT("VoidPressureOrb.sqc"), ANIMATION_PATH, "VoidPressureOrb", true, 8.f);
	Anim->AddAnimation(TEXT("VoidPressureDestroy.sqc"), ANIMATION_PATH, "VoidPressureDestroy", true, 0.7f);
	Anim->AddAnimation(TEXT("VoidPressureOrbDestroy.sqc"), ANIMATION_PATH, "VoidPressureOrbDestroy", true, 0.4f);
	Anim->AddAnimation(TEXT("VoidPressureHit.sqc"), ANIMATION_PATH, "VoidPressureHit", true, 0.5f);

	Anim->AddAnimation(TEXT("VoidPressureDestroy.sqc"), ANIMATION_PATH, "VoidPressureDestroy", true, 0.7f);

	Anim->AddAnimation(TEXT("LightTransformingLeft.sqc"), ANIMATION_PATH, "LightTransformingLeft", true, 0.7f);

	Anim->AddAnimation(TEXT("NoxSpearBottomEffect.sqc"), ANIMATION_PATH, "NoxSpearBottomEffect", true, 2.f);
	Anim->AddAnimation(TEXT("NoxSpearHitEffectLeft.sqc"), ANIMATION_PATH, "NoxSpearHitEffectLeft", true, 2.f);
	Anim->AddAnimation(TEXT("NoxSpearHitEffectMiddle.sqc"), ANIMATION_PATH, "NoxSpearHitEffectMiddle", true, 2.f);
	Anim->AddAnimation(TEXT("NoxSpearTopEffect.sqc"), ANIMATION_PATH, "NoxSpearTopEffect", true, 2.f);

	Anim->AddAnimation(TEXT("DeathSide.sqc"), ANIMATION_PATH, "DeathSide", true, 2.f);
	Anim->AddAnimation(TEXT("DeathSideHitEffect.sqc"), ANIMATION_PATH, "DeathSideHitEffect", true, 2.f);

	Anim->AddAnimation(TEXT("PlayerLevelUpEffect.sqc"), ANIMATION_PATH, "PlayerLevelUpEffect", true, 2.f);
	Anim->AddAnimation(TEXT("FloatingBill.sqc"), ANIMATION_PATH, "FloatingBill", true, 0.8f);


	CInput::GetInst()->CreateKey("Flip", 'F');
	CInput::GetInst()->SetKeyCallback<CPlayerSkillSet>("Flip", KeyState_Down, this, &CPlayerSkillSet::FlipAll);
	CInput::GetInst()->CreateKey("NextAnimation", 'N');
	CInput::GetInst()->SetKeyCallback<CPlayerSkillSet>("NextAnimation", KeyState_Down, this, &CPlayerSkillSet::NextAnimation);

	return true;
}

void CPlayerSkillSet::Update(float DeltaTime)
{
	CGameObject::Update(DeltaTime);
}

void CPlayerSkillSet::PostUpdate(float DeltaTime)
{
	CGameObject::PostUpdate(DeltaTime);
}

CPlayerSkillSet* CPlayerSkillSet::Clone()
{
	return new CPlayerSkillSet(*this);
}

void CPlayerSkillSet::Save(FILE* File)
{
	CGameObject::Save(File);
}

void CPlayerSkillSet::Load(FILE* File)
{
	CGameObject::Load(File);

	m_Sprite = (CSpriteComponent*)FindComponent("PlayerSkillSprite");
}

void CPlayerSkillSet::FlipAll(float DeltaTime)
{
	m_Sprite->Flip();
}

void CPlayerSkillSet::NextAnimation(float DeltaTime)
{
	CAnimationSequence2DInstance* Anim = m_Sprite->GetAnimationInstance();

	Anim->NextAnimation();

	float PlayTime = Anim->GetCurrentAnimation()->GetPlayTime();

	CEditorManager::GetInst()->GetSpriteWindow()->SetPlayTime(PlayTime);
}