
#include "ZakumBody.h"
#include "../Animation/ZakumBodyAnimation.h"
#include "../Animation/ZakumHandAnimation.h"
#include "../EditorManager.h"
#include "../Window/SpriteWindow.h"
#include "Input.h"

CZakumBody::CZakumBody()	:
	m_AfterUpdate(false)
{
	SetTypeID<CZakumBody>();
}

CZakumBody::CZakumBody(const CZakumBody& obj)	:
	CMonster(obj)
{
	m_Sprite = (CSpriteComponent*)FindComponent("ZakumBodySprite");
	m_LeftArm1Root = (CSceneComponent*)FindComponent("ZakumLeftArm1Root");
	m_LeftArm1Lower = (CSpriteComponent*)FindComponent("ZakumLeftArm1Lower");
	m_LeftArm1Upper = (CSpriteComponent*)FindComponent("ZakumLeftArm1Upper");
	m_LeftArm1Hand = (CSpriteComponent*)FindComponent("ZakumLeftArm1Hand");

	m_Body = (CColliderBox2D*)FindComponent("Body");
}

CZakumBody::~CZakumBody()
{
}

void CZakumBody::Start()
{

	//m_RightArm1Root->SetPivot(0.f, 0.f, 0.f);
	//m_RightArm1Lower->SetPivot(0.f, 0.f, 0.f);
	//m_RightArm1Upper->SetPivot(0.f, 0.f, 0.f);
	//m_RightArm1Hand->SetPivot(0.f, 0.f, 0.f);


	CInput::GetInst()->CreateKey("NextAnimation", 'N');
	CInput::GetInst()->SetKeyCallback<CZakumBody>("NextAnimation", KeyState_Down, this, &CZakumBody::NextAnimation);

}

bool CZakumBody::Init()
{
	m_Sprite = CreateComponent<CSpriteComponent>("ZakumBodySprite");
	//m_LeftArm1 = CreateComponent<CSpriteComponent>("ZakumLeftArm1Sprite");
	//m_RightArm1 = CreateComponent<CSpriteComponent>("ZakumRightArm1Sprite");

	m_Body = CreateComponent<CColliderBox2D>("Body");

	m_Sprite->AddChild(m_Body);
	m_Sprite->SetPivot(0.5f, 0.5f, 0.f);
	m_Sprite->SetWorldScale(300.f, 350.f, 1.f);

	m_Sprite->SetTransparency(true);


	m_Sprite->CreateAnimationInstance<CZakumBodyAnimation>();


	/*m_Body->SetRelativeScale(100.f, 100.f, 1.f);
	m_Body->SetRelativePos(500.f, 300.f, 0.f);
	m_Body->SetPivot(0.5f, 0.5f, 0.f);*/

	return true;
}

void CZakumBody::Update(float DeltaTime)
{
	if (!m_AfterUpdate)
	{
		m_Sprite = (CSpriteComponent*)FindComponent("ZakumBodySprite");

		m_LeftArm1Root = (CSceneComponent*)FindComponent("ZakumLeftArm1Root");
		m_LeftArm1Lower = (CSpriteComponent*)FindComponent("ZakumLeftArm1Lower");
		m_LeftArm1Upper = (CSpriteComponent*)FindComponent("ZakumLeftArm1Upper");
		m_LeftArm1Hand = (CSpriteComponent*)FindComponent("ZakumLeftArm1Hand");

		m_LeftArm2Root = (CSceneComponent*)FindComponent("ZakumLeftArm2Root");
		m_LeftArm2Lower = (CSpriteComponent*)FindComponent("ZakumLeftArm2Lower");
		m_LeftArm2Upper = (CSpriteComponent*)FindComponent("ZakumLeftArm2Upper");
		m_LeftArm2Hand = (CSpriteComponent*)FindComponent("ZakumLeftArm2Hand");

		m_LeftArm3Root = (CSceneComponent*)FindComponent("ZakumLeftArm3Root");
		m_LeftArm3Lower = (CSpriteComponent*)FindComponent("ZakumLeftArm3Lower");
		m_LeftArm3Upper = (CSpriteComponent*)FindComponent("ZakumLeftArm3Upper");
		m_LeftArm3Hand = (CSpriteComponent*)FindComponent("ZakumLeftArm3Hand");

		m_LeftArm4Root = (CSceneComponent*)FindComponent("ZakumLeftArm4Root");
		m_LeftArm4Lower = (CSpriteComponent*)FindComponent("ZakumLeftArm4Lower");
		m_LeftArm4Upper = (CSpriteComponent*)FindComponent("ZakumLeftArm4Upper");
		m_LeftArm4Hand = (CSpriteComponent*)FindComponent("ZakumLeftArm4Hand");

		m_RightArm1Root = (CSceneComponent*)FindComponent("ZakumRightArm1Root");
		m_RightArm1Lower = (CSpriteComponent*)FindComponent("ZakumRightArm1Lower");
		m_RightArm1Upper = (CSpriteComponent*)FindComponent("ZakumRightArm1Upper");
		m_RightArm1Hand = (CSpriteComponent*)FindComponent("ZakumRightArm1Hand");

		m_RightArm2Root = (CSceneComponent*)FindComponent("ZakumRightArm2Root");
		m_RightArm2Lower = (CSpriteComponent*)FindComponent("ZakumRightArm2Lower");
		m_RightArm2Upper = (CSpriteComponent*)FindComponent("ZakumRightArm2Upper");
		m_RightArm2Hand = (CSpriteComponent*)FindComponent("ZakumRightArm2Hand");

		m_RightArm3Root = (CSceneComponent*)FindComponent("ZakumRightArm3Root");
		m_RightArm3Lower = (CSpriteComponent*)FindComponent("ZakumRightArm3Lower");
		m_RightArm3Upper = (CSpriteComponent*)FindComponent("ZakumRightArm3Upper");
		m_RightArm3Hand = (CSpriteComponent*)FindComponent("ZakumRightArm3Hand");

		m_RightArm4Root = (CSceneComponent*)FindComponent("ZakumRightArm4Root");
		m_RightArm4Lower = (CSpriteComponent*)FindComponent("ZakumRightArm4Lower");
		m_RightArm4Upper = (CSpriteComponent*)FindComponent("ZakumRightArm4Upper");
		m_RightArm4Hand = (CSpriteComponent*)FindComponent("ZakumRightArm4Hand");

		m_Body = (CColliderBox2D*)FindComponent("Body");
		m_Body->SetWorldScale(100.f, 120.f, 1.f);

		if(m_Sprite)
			m_Sprite->SetTransparency(true);

		m_LeftArm1Root->SetPivot(1.f, 0.f, 0.f);
		m_LeftArm1Lower->SetPivot(1.f, 0.f, 0.f);
		m_LeftArm1Upper->SetPivot(1.f, 0.f, 0.f);

		if (m_LeftArm1Hand && !m_LeftArm1Hand->GetAnimationInstance())
		{
			m_LeftArm1Hand->CreateAnimationInstance<CZakumHandAnimation>();
			m_LeftArm1Hand->GetCurrentAnimation()->SetPlayScale(0.f);
		}

		if (m_RightArm1Hand && !m_RightArm1Hand->GetAnimationInstance())
		{
			m_RightArm1Hand->CreateAnimationInstance<CZakumHandAnimation>();
			m_RightArm1Hand->ChangeAnimation("ZakumRightHand");
			m_RightArm1Hand->GetCurrentAnimation()->SetPlayScale(0.f);
		}

		if (m_LeftArm2Hand && !m_LeftArm2Hand->GetAnimationInstance())
		{
			m_LeftArm2Hand->CreateAnimationInstance<CZakumHandAnimation>();
			m_LeftArm2Hand->GetCurrentAnimation()->SetPlayScale(0.f);
			m_LeftArm2Hand->GetCurrentAnimation()->SetCurrentAnimationFrame(1);
		}

		if (m_RightArm2Hand && !m_RightArm2Hand->GetAnimationInstance())
		{
			m_RightArm2Hand->CreateAnimationInstance<CZakumHandAnimation>();
			m_RightArm2Hand->ChangeAnimation("ZakumRightHand");
			m_RightArm2Hand->GetCurrentAnimation()->SetPlayScale(0.f);
			m_RightArm2Hand->GetCurrentAnimation()->SetCurrentAnimationFrame(1);
		}

		if (m_LeftArm3Hand && !m_LeftArm3Hand->GetAnimationInstance())
		{
			m_LeftArm3Hand->CreateAnimationInstance<CZakumHandAnimation>();
			m_LeftArm3Hand->GetCurrentAnimation()->SetPlayScale(0.f);
			m_LeftArm3Hand->GetCurrentAnimation()->SetCurrentAnimationFrame(2);
		}

		if (m_RightArm3Hand && !m_RightArm3Hand->GetAnimationInstance())
		{
			m_RightArm3Hand->CreateAnimationInstance<CZakumHandAnimation>();
			m_RightArm3Hand->ChangeAnimation("ZakumRightHand");
			m_RightArm3Hand->GetCurrentAnimation()->SetPlayScale(0.f);
			m_RightArm3Hand->GetCurrentAnimation()->SetCurrentAnimationFrame(2);
		}

		if (m_LeftArm4Hand && !m_LeftArm4Hand->GetAnimationInstance())
		{
			m_LeftArm4Hand->CreateAnimationInstance<CZakumHandAnimation>();
			m_LeftArm4Hand->GetCurrentAnimation()->SetPlayScale(0.f);
			m_LeftArm4Hand->GetCurrentAnimation()->SetCurrentAnimationFrame(1);
		}

		if (m_RightArm4Hand && !m_RightArm4Hand->GetAnimationInstance())
		{
			m_RightArm4Hand->CreateAnimationInstance<CZakumHandAnimation>();
			m_RightArm4Hand->ChangeAnimation("ZakumRightHand");
			m_RightArm4Hand->GetCurrentAnimation()->SetPlayScale(0.f);
			m_RightArm4Hand->GetCurrentAnimation()->SetCurrentAnimationFrame(1);
		}

		m_LeftArm1Hand->SetPivot(0.5f, 0.5f, 0.f);
		m_LeftArm2Root->SetPivot(1.f, 0.f, 0.f);
		m_LeftArm2Lower->SetPivot(1.f, 0.f, 0.f);
		m_LeftArm2Upper->SetPivot(1.f, 0.f, 0.f);

		if (m_LeftArm2Hand)
			m_LeftArm2Hand->SetPivot(0.5f, 0.5f, 0.f);

		m_LeftArm3Root->SetPivot(1.f, 0.f, 0.f);
		m_LeftArm3Lower->SetPivot(1.f, 0.f, 0.f);
		m_LeftArm3Upper->SetPivot(1.f, 0.f, 0.f);

		if (m_LeftArm3Hand)
			m_LeftArm3Hand->SetPivot(0.5f, 0.5f, 0.f);

		m_LeftArm4Root->SetPivot(1.f, 0.f, 0.f);
		m_LeftArm4Lower->SetPivot(1.f, 0.f, 0.f);
		m_LeftArm4Upper->SetPivot(1.f, 0.f, 0.f);
		if (m_LeftArm4Hand)
			m_LeftArm4Hand->SetPivot(0.5f, 0.5f, 0.f);

		if (m_RightArm1Hand)
			m_RightArm1Hand->SetPivot(0.5f, 0.5f, 0.f);

		if (m_RightArm2Hand)
			m_RightArm2Hand->SetPivot(0.5f, 0.5f, 0.f);

		if (m_RightArm3Hand)
			m_RightArm3Hand->SetPivot(0.5f, 0.5f, 0.f);

		if (m_RightArm4Hand)
			m_RightArm4Hand->SetPivot(0.5f, 0.5f, 0.f);

		/*m_LeftArm1Lower->SetInheritRotZ(true);
		m_LeftArm1Upper->SetInheritRotZ(true);
		if (m_LeftArm1Hand)
		m_LeftArm1Hand->SetInheritRotZ(true);

		m_LeftArm2Lower->SetInheritRotZ(true);
		m_LeftArm2Upper->SetInheritRotZ(true);
		if (m_LeftArm2Hand)
		m_LeftArm2Hand->SetInheritRotZ(true);

		m_LeftArm3Lower->SetInheritRotZ(true);
		m_LeftArm3Upper->SetInheritRotZ(true);
		if (m_LeftArm3Hand)
		m_LeftArm3Hand->SetInheritRotZ(true);

		m_LeftArm4Lower->SetInheritRotZ(true);
		m_LeftArm4Upper->SetInheritRotZ(true);
		if (m_LeftArm4Hand)
		m_LeftArm4Hand->SetInheritRotZ(true);

		m_RightArm1Lower->SetInheritRotZ(true);
		m_RightArm1Upper->SetInheritRotZ(true);
		if (m_RightArm1Hand)
		m_RightArm1Hand->SetInheritRotZ(true);

		m_RightArm2Lower->SetInheritRotZ(true);
		m_RightArm2Upper->SetInheritRotZ(true);
		if (m_RightArm2Hand)
		m_RightArm2Hand->SetInheritRotZ(true);

		m_RightArm3Lower->SetInheritRotZ(true);
		m_RightArm3Upper->SetInheritRotZ(true);
		if (m_RightArm3Hand)
		m_RightArm3Hand->SetInheritRotZ(true);

		m_RightArm4Lower->SetInheritRotZ(true);
		m_RightArm4Upper->SetInheritRotZ(true);
		if (m_RightArm4Hand)
		m_RightArm4Hand->SetInheritRotZ(true);*/

		m_Sprite->CreateAnimationInstance<CZakumBodyAnimation>();

		m_AfterUpdate = true;
	}

	CMonster::Update(DeltaTime);
}

void CZakumBody::PostUpdate(float DeltaTime)
{
	CMonster::PostUpdate(DeltaTime);
}

CZakumBody* CZakumBody::Clone()
{
	return new CZakumBody(*this);
}

void CZakumBody::Save(FILE* File)
{
	CMonster::Save(File);
}

void CZakumBody::Load(FILE* File)
{
	CMonster::Load(File);

	m_Sprite = (CSpriteComponent*)FindComponent("ZakumBodySprite");

	m_Sprite->SetTransparency(true);
}

void CZakumBody::NextAnimation(float DeltaTime)
{
	CZakumBodyAnimation* Anim = (CZakumBodyAnimation*)m_Sprite->GetAnimationInstance();

	Anim->NextAnimation();

	float PlayTime = Anim->GetCurrentAnimation()->GetPlayTime();

	CEditorManager::GetInst()->GetSpriteWindow()->SetPlayTime(PlayTime);
}