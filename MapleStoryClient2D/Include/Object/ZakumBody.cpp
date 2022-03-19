
#include "ZakumBody.h"
#include "../Animation/ZakumBodyAnimation.h"
#include "Input.h"
#include "../Animation/ZakumHandAnimation.h"
#include "../Object/Player2D.h"
#include "../Object/ZakumAttackWarningSpot.h"
#include "../Object/ZakumDustEffect.h"
#include "../Object/ZakumHandSmashLight.h"
#include "../Object/ZakumFogEffect.h"
#include "../Object/ZakumFlame.h"
#include "../Object/ClapWarning.h"
#include "../Object/ZakumClapLightEffect.h"
#include "../Object/ZakumClapCircleShockWave.h"
#include "../Object/ZakumClapSmallCircleShockWave.h"
#include "../Object/ZakumClapPinkShockWave.h"

#include <algorithm>

CZakumBody::CZakumBody() :
	m_AccTime(0.f),
	m_SmashCount(0),
	m_SmashCountLimit(1),
	m_ClapCount(0),
	m_ClapCountLimit(4),
	m_ClapArmNum(-1),
	m_HandMeet(false)
{
	SetTypeID<CZakumBody>();

	m_ZakumState = Zakum_State::Idle;

	m_IdleStateTime = (float)(rand() % 5) + 2.f;

	m_vecUpHandRotUpperBound.resize(8);
	m_vecAccUpHandRot.resize(8);
	m_vecShakeRotBound.resize(8);
	m_vecAccRotation.resize(8);
	m_vecShakeRotSign.resize(8);
	m_vecPreviousHandRot.resize(8);
	m_vecPreviousHandPos.resize(8);
	m_vecPreviousLowerPos.resize(8);
	m_vecPreviousLowerRot.resize(8);
	m_vecPreviousUpperPos.resize(8);
	m_vecPreviousUpperRot.resize(8);
	m_vecSmashRotUpperBound.resize(3);

	for (int i = 0; i < 8; ++i)
	{
		m_vecAccUpHandRot[i] = 0.f;
		m_vecAccRotation[i] = 0.f;
		m_vecArmState.push_back(Zakum_ArmState::Idle);
	}

	for (int i = 0; i < 4; ++i)
	{
		m_vecAttackWarningPos.push_back(Vector3((150.f * i + 130.f), 205.f, 250.f));
	}

	for (int i = 4; i < 8; ++i)
	{
		m_vecAttackWarningPos.push_back(Vector3((150.f * i + 150.f), 205.f, 250.f));
	}

	m_vecUpHandRotUpperBound[0] = 20.f;
	m_vecUpHandRotUpperBound[4] = 20.f;
	m_vecUpHandRotUpperBound[1] = 30.f;
	m_vecUpHandRotUpperBound[5] = 30.f;
	m_vecUpHandRotUpperBound[2] = 40.f;
	m_vecUpHandRotUpperBound[6] = 40.f;
	m_vecUpHandRotUpperBound[3] = 50.f;
	m_vecUpHandRotUpperBound[7] = 50.f;

	m_vecSmashRotUpperBound[0] = 15.f;
	m_vecSmashRotUpperBound[1] = 10.f;
	m_vecSmashRotUpperBound[2] = 7.f;

	m_vecShakeRotBound[0] = 8.f;
	m_vecShakeRotBound[1] = 7.f;
	m_vecShakeRotBound[2] = 6.f;
	m_vecShakeRotBound[3] = 3.f;

	m_vecShakeRotBound[4] = 9.f;
	m_vecShakeRotBound[5] = 6.f;
	m_vecShakeRotBound[6] = 6.f;
	m_vecShakeRotBound[7] = 2.f;

	m_vecShakeRotSign[0] = 1.f;
	m_vecShakeRotSign[1] = -1.f;
	m_vecShakeRotSign[2] = -1.f;
	m_vecShakeRotSign[3] = 1.f;

	m_vecShakeRotSign[4] = -1.f;
	m_vecShakeRotSign[5] = 1.f;
	m_vecShakeRotSign[6] = -1.f;
	m_vecShakeRotSign[7] = -1.f;
}

CZakumBody::CZakumBody(const CZakumBody& obj)	:
	CMonster(obj)
{
	m_BodySprite = (CSpriteComponent*)FindComponent("ZakumBodySprite");
	m_Body = (CColliderBox2D*)FindComponent("Body");

	m_LeftArm1Lower = (CSpriteComponent*)FindComponent("ZakumLeftArm1Lower");
	m_LeftArm1Upper = (CSpriteComponent*)FindComponent("ZakumLeftArm1Upper");
	m_LeftArm1Hand = (CSpriteComponent*)FindComponent("ZakumLeftArm1Hand");


}

CZakumBody::~CZakumBody()
{
}

void CZakumBody::Start()
{
	CMonster::Start();

	m_LeftArm1Root = (CSceneComponent*)FindComponent("ZakumLeftArm1Root");
	m_LeftArm1Lower = (CSpriteComponent*)FindComponent("ZakumLeftArm1Lower");
	m_LeftArm1Upper = (CSpriteComponent*)FindComponent("ZakumLeftArm1Upper");
	m_LeftArm1Hand = (CSpriteComponent*)FindComponent("ZakumLeftArm1Hand");
	m_LeftArm1Hand->SetTransparency(true);
	m_LeftArm1Hand->CreateAnimationInstance<CZakumHandAnimation>();
	m_LeftArm1Hand->GetCurrentAnimation()->SetPlayScale(0.f);

	m_LeftArm2Root = (CSceneComponent*)FindComponent("ZakumLeftArm2Root");
	m_LeftArm2Lower = (CSpriteComponent*)FindComponent("ZakumLeftArm2Lower");
	m_LeftArm2Upper = (CSpriteComponent*)FindComponent("ZakumLeftArm2Upper");
	m_LeftArm2Hand = (CSpriteComponent*)FindComponent("ZakumLeftArm2Hand");
	m_LeftArm2Hand->SetTransparency(true);
	m_LeftArm2Hand->CreateAnimationInstance<CZakumHandAnimation>();
	m_LeftArm2Hand->GetCurrentAnimation()->SetPlayScale(0.f);
	m_LeftArm2Hand->GetCurrentAnimation()->SetCurrentAnimationFrame(1);

	m_LeftArm3Root = (CSceneComponent*)FindComponent("ZakumLeftArm3Root");
	m_LeftArm3Lower = (CSpriteComponent*)FindComponent("ZakumLeftArm3Lower");
	m_LeftArm3Upper = (CSpriteComponent*)FindComponent("ZakumLeftArm3Upper");
	m_LeftArm3Hand = (CSpriteComponent*)FindComponent("ZakumLeftArm3Hand");
	m_LeftArm3Hand->SetTransparency(true);
	m_LeftArm3Hand->CreateAnimationInstance<CZakumHandAnimation>();
	m_LeftArm3Hand->GetCurrentAnimation()->SetPlayScale(0.f);
	m_LeftArm3Hand->GetCurrentAnimation()->SetCurrentAnimationFrame(2);

	m_LeftArm4Root = (CSceneComponent*)FindComponent("ZakumLeftArm4Root");
	m_LeftArm4Lower = (CSpriteComponent*)FindComponent("ZakumLeftArm4Lower");
	m_LeftArm4Upper = (CSpriteComponent*)FindComponent("ZakumLeftArm4Upper");
	m_LeftArm4Hand = (CSpriteComponent*)FindComponent("ZakumLeftArm4Hand");
	m_LeftArm4Hand->SetTransparency(true);
	m_LeftArm4Hand->CreateAnimationInstance<CZakumHandAnimation>();
	m_LeftArm4Hand->GetCurrentAnimation()->SetPlayScale(0.f);
	m_LeftArm4Hand->GetCurrentAnimation()->SetCurrentAnimationFrame(2);

	m_RightArm1Root = (CSceneComponent*)FindComponent("ZakumRightArm1Root");
	m_RightArm1Lower = (CSpriteComponent*)FindComponent("ZakumRightArm1Lower");
	m_RightArm1Upper = (CSpriteComponent*)FindComponent("ZakumRightArm1Upper");
	m_RightArm1Hand = (CSpriteComponent*)FindComponent("ZakumRightArm1Hand");
	m_RightArm1Hand->SetTransparency(true);
	m_RightArm1Hand->CreateAnimationInstance<CZakumHandAnimation>();
	m_RightArm1Hand->ChangeAnimation("ZakumRightHand");
	m_RightArm1Hand->GetCurrentAnimation()->SetPlayScale(0.f);

	m_RightArm2Root = (CSceneComponent*)FindComponent("ZakumRightArm2Root");
	m_RightArm2Lower = (CSpriteComponent*)FindComponent("ZakumRightArm2Lower");
	m_RightArm2Upper = (CSpriteComponent*)FindComponent("ZakumRightArm2Upper");
	m_RightArm2Hand = (CSpriteComponent*)FindComponent("ZakumRightArm2Hand");
	m_RightArm2Hand->SetTransparency(true);
	m_RightArm2Hand->CreateAnimationInstance<CZakumHandAnimation>();
	m_RightArm2Hand->ChangeAnimation("ZakumRightHand");
	m_RightArm2Hand->GetCurrentAnimation()->SetPlayScale(0.f);
	m_RightArm2Hand->GetCurrentAnimation()->SetCurrentAnimationFrame(1);

	m_RightArm3Root = (CSceneComponent*)FindComponent("ZakumRightArm3Root");
	m_RightArm3Lower = (CSpriteComponent*)FindComponent("ZakumRightArm3Lower");
	m_RightArm3Upper = (CSpriteComponent*)FindComponent("ZakumRightArm3Upper");
	m_RightArm3Hand = (CSpriteComponent*)FindComponent("ZakumRightArm3Hand");
	m_RightArm3Hand->SetTransparency(true);
	m_RightArm3Hand->CreateAnimationInstance<CZakumHandAnimation>();
	m_RightArm3Hand->ChangeAnimation("ZakumRightHand");
	m_RightArm3Hand->GetCurrentAnimation()->SetPlayScale(0.f);
	m_RightArm3Hand->GetCurrentAnimation()->SetCurrentAnimationFrame(2);

	m_RightArm4Root = (CSceneComponent*)FindComponent("ZakumRightArm4Root");
	m_RightArm4Lower = (CSpriteComponent*)FindComponent("ZakumRightArm4Lower");
	m_RightArm4Upper = (CSpriteComponent*)FindComponent("ZakumRightArm4Upper");
	m_RightArm4Hand = (CSpriteComponent*)FindComponent("ZakumRightArm4Hand");
	m_RightArm4Hand->SetTransparency(true);
	m_RightArm4Hand->CreateAnimationInstance<CZakumHandAnimation>();
	m_RightArm4Hand->ChangeAnimation("ZakumRightHand");
	m_RightArm4Hand->GetCurrentAnimation()->SetPlayScale(0.f);
	m_RightArm4Hand->GetCurrentAnimation()->SetCurrentAnimationFrame(2);

	m_LeftArm1Root->SetInheritRotZ(false);
	m_LeftArm1Lower->SetInheritRotZ(true);
	m_LeftArm1Upper->SetInheritRotZ(true);
	m_LeftArm1Hand->SetInheritRotZ(true);

	m_LeftArm2Root->SetInheritRotZ(false);
	m_LeftArm2Lower->SetInheritRotZ(true);
	m_LeftArm2Upper->SetInheritRotZ(true);
	m_LeftArm2Hand->SetInheritRotZ(true);

	m_LeftArm3Root->SetInheritRotZ(false);
	m_LeftArm3Lower->SetInheritRotZ(true);
	m_LeftArm3Upper->SetInheritRotZ(true);
	m_LeftArm3Hand->SetInheritRotZ(true);

	m_LeftArm4Root->SetInheritRotZ(false);
	m_LeftArm4Lower->SetInheritRotZ(true);
	m_LeftArm4Upper->SetInheritRotZ(true);
	m_LeftArm4Hand->SetInheritRotZ(true);

	m_RightArm1Root->SetInheritRotZ(false);
	m_RightArm1Lower->SetInheritRotZ(true);
	m_RightArm1Upper->SetInheritRotZ(true);
	m_RightArm1Hand->SetInheritRotZ(true);

	m_RightArm2Root->SetInheritRotZ(false);
	m_RightArm2Lower->SetInheritRotZ(true);
	m_RightArm2Upper->SetInheritRotZ(true);
	m_RightArm2Hand->SetInheritRotZ(true);

	m_RightArm3Root->SetInheritRotZ(false);
	m_RightArm3Lower->SetInheritRotZ(true);
	m_RightArm3Upper->SetInheritRotZ(true);
	m_RightArm3Hand->SetInheritRotZ(true);

	m_RightArm4Root->SetInheritRotZ(false);
	m_RightArm4Lower->SetInheritRotZ(true);
	m_RightArm4Upper->SetInheritRotZ(true);
	m_RightArm4Hand->SetInheritRotZ(true);

	m_LeftArm1Root->SetPivot(1.f, 0.f, 0.f);
	m_LeftArm1Lower->SetPivot(1.f, 0.f, 0.f);
	m_LeftArm1Upper->SetPivot(1.f, 0.f, 0.f);
	m_LeftArm1Hand->SetPivot(0.5f, 0.5f, 0.f);

	m_LeftArm2Root->SetPivot(1.f, 0.f, 0.f);
	m_LeftArm2Lower->SetPivot(1.f, 0.f, 0.f);
	m_LeftArm2Upper->SetPivot(1.f, 0.f, 0.f);
	m_LeftArm2Hand->SetPivot(0.5f, 0.5f, 0.f);

	m_LeftArm3Root->SetPivot(1.f, 0.f, 0.f);
	m_LeftArm3Lower->SetPivot(1.f, 0.f, 0.f);
	m_LeftArm3Upper->SetPivot(1.f, 0.f, 0.f);
	m_LeftArm3Hand->SetPivot(0.5f, 0.5f, 0.f);

	m_LeftArm4Root->SetPivot(1.f, 0.f, 0.f);
	m_LeftArm4Lower->SetPivot(1.f, 0.f, 0.f);
	m_LeftArm4Upper->SetPivot(1.f, 0.f, 0.f);
	m_LeftArm4Hand->SetPivot(0.5f, 0.5f, 0.f);

	m_RightArm1Hand->SetPivot(0.5f, 0.5f, 0.f);
	m_RightArm2Hand->SetPivot(0.5f, 0.5f, 0.f);
	m_RightArm3Hand->SetPivot(0.5f, 0.5f, 0.f);
	m_RightArm4Hand->SetPivot(0.5f, 0.5f, 0.f);

}

bool CZakumBody::Init()
{
	m_BodySprite = CreateComponent<CSpriteComponent>("ZakumBodySprite");
	//m_LeftArm1 = CreateComponent<CSpriteComponent>("ZakumLeftArm1Sprite");
	//m_RightArm1 = CreateComponent<CSpriteComponent>("ZakumRightArm1Sprite");

	m_Body = CreateComponent<CColliderBox2D>("Body");

	m_BodySprite->AddChild(m_Body);

	m_Body->SetWorldScale(300.f, 350.f, 1.f);

	m_BodySprite->SetTransparency(true);


	m_BodySprite->CreateAnimationInstance<CZakumBodyAnimation>();


	m_BodySprite->SetRelativeScale(100.f, 100.f, 1.f);
	m_BodySprite->SetRelativePos(500.f, 300.f, 0.f);
	m_BodySprite->SetPivot(0.5f, 0.5f, 0.f);

	return true;
}

void CZakumBody::Update(float DeltaTime)
{
	CMonster::Update(DeltaTime);

	m_AccTime += DeltaTime;

	if (m_ZakumState == Zakum_State::Clap)
	{
		PickAndStretchClapHand(DeltaTime);
	}

	else
	{
		if (m_AccTime >= m_IdleStateTime)
		{
			m_ZakumState = Zakum_State::UpHand;
			m_AccTime = 0.f;
			//CheckUpHandAttackArm(DeltaTime);
		}

		float ZRot = 0.f;

		if (m_vecArmState[0] == Zakum_ArmState::Idle)
		{
			ZRot = m_vecShakeRotSign[0] * DeltaTime * 5.f;
			m_vecAccRotation[0] += abs(ZRot);
			if (m_vecAccRotation[0] >= m_vecShakeRotBound[0])
			{
				m_vecShakeRotSign[0] *= -1.f;
				m_vecAccRotation[0] = 0.f;
			}

			m_LeftArm1Root->AddWorldRotation(Vector3(0.f, 0.f, ZRot));
		}

		if (m_vecArmState[1] == Zakum_ArmState::Idle)
		{
			ZRot = m_vecShakeRotSign[1] * DeltaTime * 3.f;
			m_vecAccRotation[1] += abs(ZRot);
			if (m_vecAccRotation[1] >= m_vecShakeRotBound[1])
			{
				m_vecShakeRotSign[1] *= -1.f;
				m_vecAccRotation[1] = 0.f;
			}

			m_LeftArm2Root->AddWorldRotation(Vector3(0.f, 0.f, ZRot));
		}

		if (m_vecArmState[2] == Zakum_ArmState::Idle)
		{
			ZRot = m_vecShakeRotSign[2] * DeltaTime * 4.f;
			m_vecAccRotation[2] += abs(ZRot);
			if (m_vecAccRotation[2] >= m_vecShakeRotBound[2])
			{
				m_vecShakeRotSign[2] *= -1.f;
				m_vecAccRotation[2] = 0.f;
			}

			m_LeftArm3Root->AddWorldRotation(Vector3(0.f, 0.f, ZRot));
		}


		if (m_vecArmState[3] == Zakum_ArmState::Idle)
		{
			ZRot = m_vecShakeRotSign[3] * DeltaTime * 2.f;
			m_vecAccRotation[3] += abs(ZRot);
			if (m_vecAccRotation[3] >= m_vecShakeRotBound[3])
			{
				m_vecShakeRotSign[3] *= -1.f;
				m_vecAccRotation[3] = 0.f;
			}

			m_LeftArm4Root->AddWorldRotation(Vector3(0.f, 0.f, ZRot));
		}

		if (m_vecArmState[4] == Zakum_ArmState::Idle)
		{
			ZRot = m_vecShakeRotSign[4] * DeltaTime * 6.f;
			m_vecAccRotation[4] += abs(ZRot);
			if (m_vecAccRotation[4] >= m_vecShakeRotBound[4])
			{
				m_vecShakeRotSign[4] *= -1.f;
				m_vecAccRotation[4] = 0.f;
			}

			m_RightArm1Root->AddWorldRotation(Vector3(0.f, 0.f, ZRot));
		}

		if (m_vecArmState[5] == Zakum_ArmState::Idle)
		{
			ZRot = m_vecShakeRotSign[5] * DeltaTime * 2.f;
			m_vecAccRotation[5] += abs(ZRot);
			if (m_vecAccRotation[5] >= m_vecShakeRotBound[5])
			{
				m_vecShakeRotSign[5] *= -1.f;
				m_vecAccRotation[5] = 0.f;
			}

			m_RightArm2Root->AddWorldRotation(Vector3(0.f, 0.f, ZRot));
		}

		if (m_vecArmState[6] == Zakum_ArmState::Idle)
		{
			ZRot = m_vecShakeRotSign[6] * DeltaTime * 3.f;
			m_vecAccRotation[6] += abs(ZRot);
			if (m_vecAccRotation[6] >= m_vecShakeRotBound[6])
			{
				m_vecShakeRotSign[6] *= -1.f;
				m_vecAccRotation[6] = 0.f;
			}

			m_RightArm3Root->AddWorldRotation(Vector3(0.f, 0.f, ZRot));
		}

		if (m_vecArmState[7] == Zakum_ArmState::Idle)
		{
			ZRot = m_vecShakeRotSign[7] * DeltaTime * 2.f;
			m_vecAccRotation[7] += abs(ZRot);
			if (m_vecAccRotation[7] >= m_vecShakeRotBound[7])
			{
				m_vecShakeRotSign[7] *= -1.f;
				m_vecAccRotation[7] = 0.f;
			}

			m_RightArm4Root->AddWorldRotation(Vector3(0.f, 0.f, ZRot));
		}

		if (m_ZakumState == Zakum_State::UpHand)
		{
			CheckUpHandAttackArm(DeltaTime);
		}
	}
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

	m_BodySprite = (CSpriteComponent*)FindComponent("ZakumBodySprite");

	m_BodySprite->SetTransparency(true);
}


void CZakumBody::UpHand(int ArmID, float DeltaTime)
{
	float RotAmount = 30.f * DeltaTime;
	
	if (m_vecAccUpHandRot[ArmID] >= m_vecUpHandRotUpperBound[ArmID])
	{
		return;
	}

	m_vecAccUpHandRot[ArmID] += RotAmount;

	switch (ArmID + 1)
	{
	case 1:
		m_LeftArm1Root->AddWorldRotation(0.f, 0.f, -1.f * RotAmount);
		break;
	case 2:
		m_LeftArm2Root->AddWorldRotation(0.f, 0.f, -1.f * RotAmount);
		break;
	case 3:
		m_LeftArm3Root->AddWorldRotation(0.f, 0.f, -1.f * RotAmount);
		break;
	case 4:
		m_LeftArm4Root->AddWorldRotation(0.f, 0.f, -1.f * RotAmount);
		break;
	case 5:
		m_RightArm1Root->AddWorldRotation(0.f, 0.f, RotAmount);
		break;
	case 6:
		m_RightArm2Root->AddWorldRotation(0.f, 0.f, RotAmount);
		break;
	case 7:
		m_RightArm3Root->AddWorldRotation(0.f, 0.f, RotAmount);
		break;
	case 8:
		m_RightArm4Root->AddWorldRotation(0.f, 0.f, RotAmount);
		break;

	}
}

void CZakumBody::StretchArm(int ArmID, float DeltaTime)
{
	if (m_HandMeet)
		return;

	if (m_vecAccRotation[ArmID] >= m_vecSmashRotUpperBound[ArmID])
	{
		/*m_LeftArm1Upper->AddWorldPos(200.f * DeltaTime, 0.f, 0.f);
		m_RightArm1Upper->AddWorldPos(-250.f * DeltaTime, 0.f, 0.f);*/

		return;
	}

	float RotAmount = 7.f * DeltaTime;
	m_vecAccRotation[ArmID] += RotAmount;

	switch (ArmID)
	{
	case 0:
		m_LeftArm1Upper->AddWorldPos(-4.f * DeltaTime, 0.f, 0.f);
		m_LeftArm1Upper->AddWorldRotation(0.f, 0.f, RotAmount);
		m_RightArm1Upper->AddWorldPos(4.f * DeltaTime, 0.f, 0.f);
		m_RightArm1Upper->AddWorldRotation(0.f, 0.f, -1.f * RotAmount);
		break;
	case 1:
		break;
	case 2:
		break;
	}
}

void CZakumBody::CheckUpHandAttackArm(float DeltaTime)
{
	if (m_ListAttackArmNum.empty())
	{
		// 8개의 팔중에 랜덤한 수의 팔을 UpHand 상태일때마다 뽑아서 뽑힌 팔은 내려치는 공격을 실행한다
		// LeftArm1,2,3,4 가 0,1,2,3번 인덱스, RightArm1,2,3,4가 4,5,6,7번 인덱스
		 
		int AttackArmCount = rand() % 8 + 1;
		//int AttackArmCount = 8;

		for (int i = 0; i < AttackArmCount; ++i)
		{
			int AttackArmID = rand() % 8;
			//int AttackArmID = i;

			// 이미 공격하는 팔 ID로 뽑힌 팔을 또 뽑으면 다시 뽑기
			auto iter = std::find(m_ListAttackArmNum.begin(), m_ListAttackArmNum.end(), AttackArmID);
		
			if (iter != m_ListAttackArmNum.end())
			{
				--i;
				continue;
			}

			//int AttackArmID = i;
			m_ListAttackArmNum.push_back(AttackArmID);
			m_vecArmState[AttackArmID] = Zakum_ArmState::UpHand;

			CZakumAttackWarningSpot* Spot = m_Scene->CreateGameObject<CZakumAttackWarningSpot>("WarningSpot");
			Spot->SetWorldPos(m_vecAttackWarningPos[AttackArmID]);
			Spot->SetWarningPointOwner(this);
			Spot->SetArmID(AttackArmID);

			switch (AttackArmID)
			{
			case 0:
				m_vecPreviousHandPos[0] = m_LeftArm1Hand->GetWorldPos();
				m_vecPreviousHandRot[0] = m_LeftArm1Hand->GetWorldRot();
				m_vecPreviousUpperPos[0] = m_LeftArm1Upper->GetWorldPos();
				m_vecPreviousUpperRot[0] = m_LeftArm1Upper->GetWorldRot();
				m_vecPreviousLowerPos[0] = m_LeftArm1Lower->GetWorldPos();
				m_vecPreviousLowerRot[0] = m_LeftArm1Lower->GetWorldRot();
				m_LeftArm1Hand->GetCurrentAnimation()->SetPlayScale(1.f);
				break;
			case 1:
				m_vecPreviousHandPos[1] = m_LeftArm2Hand->GetWorldPos();
				m_vecPreviousHandRot[1] = m_LeftArm2Hand->GetWorldRot();
				m_vecPreviousUpperPos[1] = m_LeftArm2Upper->GetWorldPos();
				m_vecPreviousUpperRot[1] = m_LeftArm2Upper->GetWorldRot();
				m_vecPreviousLowerPos[1] = m_LeftArm2Lower->GetWorldPos();
				m_vecPreviousLowerRot[1] = m_LeftArm2Lower->GetWorldRot();
				m_LeftArm2Hand->GetCurrentAnimation()->SetPlayScale(1.f);
				break;
			case 2:
				m_vecPreviousHandPos[2] = m_LeftArm3Hand->GetWorldPos();
				m_vecPreviousHandRot[2] = m_LeftArm3Hand->GetWorldRot();
				m_vecPreviousUpperPos[2] = m_LeftArm3Upper->GetWorldPos();
				m_vecPreviousUpperRot[2] = m_LeftArm3Upper->GetWorldRot();
				m_vecPreviousLowerPos[2] = m_LeftArm3Lower->GetWorldPos();
				m_vecPreviousLowerRot[2] = m_LeftArm3Lower->GetWorldRot();
				m_LeftArm3Hand->GetCurrentAnimation()->SetPlayScale(1.f);
				break;
			case 3:
				m_vecPreviousHandPos[3] = m_LeftArm4Hand->GetWorldPos();
				m_vecPreviousHandRot[3] = m_LeftArm4Hand->GetWorldRot();
				m_vecPreviousUpperPos[3] = m_LeftArm4Upper->GetWorldPos();
				m_vecPreviousUpperRot[3] = m_LeftArm4Upper->GetWorldRot();
				m_vecPreviousLowerPos[3] = m_LeftArm4Lower->GetWorldPos();
				m_vecPreviousLowerRot[3] = m_LeftArm4Lower->GetWorldRot();
				m_LeftArm4Hand->GetCurrentAnimation()->SetPlayScale(1.f);
				break;
			case 4:
				m_vecPreviousHandPos[4] = m_RightArm1Hand->GetWorldPos();
				m_vecPreviousHandRot[4] = m_RightArm1Hand->GetWorldRot();
				m_vecPreviousUpperPos[4] = m_RightArm1Upper->GetWorldPos();
				m_vecPreviousUpperRot[4] = m_RightArm1Upper->GetWorldRot();
				m_vecPreviousLowerPos[4] = m_RightArm1Lower->GetWorldPos();
				m_vecPreviousLowerRot[4] = m_RightArm1Lower->GetWorldRot();
				m_RightArm1Hand->GetCurrentAnimation()->SetPlayScale(1.f);
				break;
			case 5:
				m_vecPreviousHandPos[5] = m_RightArm2Hand->GetWorldPos();
				m_vecPreviousHandRot[5] = m_RightArm2Hand->GetWorldRot();
				m_vecPreviousUpperPos[5] = m_RightArm2Upper->GetWorldPos();
				m_vecPreviousUpperRot[5] = m_RightArm2Upper->GetWorldRot();
				m_vecPreviousLowerPos[5] = m_RightArm2Lower->GetWorldPos();
				m_vecPreviousLowerRot[5] = m_RightArm2Lower->GetWorldRot();
				m_RightArm2Hand->GetCurrentAnimation()->SetPlayScale(1.f);
				break;
			case 6:
				m_vecPreviousHandPos[6] = m_RightArm3Hand->GetWorldPos();
				m_vecPreviousHandRot[6] = m_RightArm3Hand->GetWorldRot();
				m_vecPreviousUpperPos[6] = m_RightArm3Upper->GetWorldPos();
				m_vecPreviousUpperRot[6] = m_RightArm3Upper->GetWorldRot();
				m_vecPreviousLowerPos[6] = m_RightArm3Lower->GetWorldPos();
				m_vecPreviousLowerRot[6] = m_RightArm3Lower->GetWorldRot();
				m_RightArm3Hand->GetCurrentAnimation()->SetPlayScale(1.f);
				break;
			case 7:
				m_vecPreviousHandPos[7] = m_RightArm4Hand->GetWorldPos();
				m_vecPreviousHandRot[7] = m_RightArm4Hand->GetWorldRot();
				m_vecPreviousUpperPos[7] = m_RightArm4Upper->GetWorldPos();
				m_vecPreviousUpperRot[7] = m_RightArm4Upper->GetWorldRot();
				m_vecPreviousLowerPos[7] = m_RightArm4Lower->GetWorldPos();
				m_vecPreviousLowerRot[7] = m_RightArm4Lower->GetWorldRot();
				m_RightArm4Hand->GetCurrentAnimation()->SetPlayScale(1.f);
				break;

			}
		}

	}

	else
	{
		size_t Count = m_ListAttackArmNum.size();

		for (size_t i = 0; i < Count; ++i)
		{
			auto iterfront = m_ListAttackArmNum.begin();

			std::advance(iterfront, i);

			m_vecArmState[*iterfront] = Zakum_ArmState::UpHand;
			UpHand(*iterfront, DeltaTime);
		}
	}
}

void CZakumBody::PickAndStretchClapHand(float DeltaTime)
{
	if (m_ClapArmNum == -1)
	{
		// 0번은 제일 위에 왼쪽,오른쪽이 박수
		//int AttackArmID = rand() % 3;
		int AttackArmID = 0;

		m_ClapArmNum = AttackArmID;
		m_vecArmState[AttackArmID] = Zakum_ArmState::Clap;

		CClapWarning* LeftClapWarning = m_Scene->CreateGameObject<CClapWarning>("LeftClapWarning");
		LeftClapWarning->SetWorldPos(305.f, 475.f, 210.f);
		LeftClapWarning->SetWarningPointOwner(this);

		CClapWarning* RightClapWarning = m_Scene->CreateGameObject<CClapWarning>("RightClapWarning");
		RightClapWarning->SetWorldPos(1075.f, 475.f, 210.f);
		RightClapWarning->SetWarningPointOwner(this);

		switch(AttackArmID)
		{ 
		case 0:
			m_vecPreviousHandPos[0] = m_LeftArm1Hand->GetWorldPos();
			m_vecPreviousHandRot[0] = m_LeftArm1Hand->GetWorldRot();
			m_vecPreviousUpperPos[0] = m_LeftArm1Upper->GetWorldPos();
			m_vecPreviousUpperRot[0] = m_LeftArm1Upper->GetWorldRot();

			m_LeftArm1Hand->ChangeAnimation("ZakumLeftHand");
			m_RightArm1Hand->ChangeAnimation("ZakumRightHand");
			m_LeftArm1Hand->GetCurrentAnimation()->SetPlayScale(1.f);
			m_RightArm1Hand->GetCurrentAnimation()->SetPlayScale(1.f);

			break;
		case 1:								   
			m_vecPreviousHandPos[1] = m_LeftArm2Hand->GetWorldPos();
			m_vecPreviousHandRot[1] = m_LeftArm2Hand->GetWorldRot();
			m_vecPreviousUpperPos[1] = m_LeftArm2Upper->GetWorldPos();
			m_vecPreviousUpperRot[1] = m_LeftArm2Upper->GetWorldRot();

			m_LeftArm2Hand->ChangeAnimation("ZakumLeftHand");
			m_RightArm2Hand->ChangeAnimation("ZakumRightHand");
			m_LeftArm2Hand->GetCurrentAnimation()->SetPlayScale(1.f);
			m_RightArm2Hand->GetCurrentAnimation()->SetPlayScale(1.f);

			break;
		case 2:
			m_vecPreviousHandPos[2] = m_LeftArm3Hand->GetWorldPos();
			m_vecPreviousHandRot[2] = m_LeftArm3Hand->GetWorldRot();
			m_vecPreviousUpperPos[2] = m_LeftArm3Upper->GetWorldPos();
			m_vecPreviousUpperRot[2] = m_LeftArm3Upper->GetWorldRot();

			m_LeftArm3Hand->ChangeAnimation("ZakumLeftHand");
			m_RightArm3Hand->ChangeAnimation("ZakumRightHand");
			m_LeftArm3Hand->GetCurrentAnimation()->SetPlayScale(1.f);
			m_RightArm3Hand->GetCurrentAnimation()->SetPlayScale(1.f);

			break;
		}
	}

	else
	{
		StretchArm(m_ClapArmNum, DeltaTime);
	}
}

void CZakumBody::ZakumClap()
{
	Vector3 ZakumPos = m_RootComponent->GetWorldPos();

	if (m_ClapArmNum == 0)
	{
		if (!m_LeftArm1Hand->GetAnimationInstance()->CheckCurrentAnimation("ZakumClapLeft"))
		{
			m_LeftArm1Hand->ChangeAnimation("ZakumClapLeft");

			m_LeftArm1Hand->SetWorldPos(620.f, 466.f, ZakumPos.z - 2.f);
			m_LeftArm1Upper->SetWorldRotation(0.f, 0.f, 0.f);
			m_LeftArm1Hand->SetWorldRotation(0.f, 0.f, 0.f);
		}

		if (!m_RightArm1Hand->GetAnimationInstance()->CheckCurrentAnimation("ZakumClapRight"))
		{
			m_RightArm1Hand->ChangeAnimation("ZakumClapRight");

			m_RightArm1Hand->SetWorldPos(725.f, 470.f, ZakumPos.z - 3.f);
			m_RightArm1Upper->SetWorldRotation(0.f, 0.f, 0.f);
			m_RightArm1Hand->SetWorldRotation(0.f, 0.f, 0.f);
		}

		if (m_LeftArm1Lower->IsRender())
			m_LeftArm1Lower->SetRender(false);
		if (m_RightArm1Lower->IsRender())
			m_RightArm1Lower->SetRender(false);
		if (m_LeftArm1Upper->IsRender())
			m_LeftArm1Upper->SetRender(false);
		if (m_RightArm1Upper->IsRender())
			m_RightArm1Upper->SetRender(false);
	}

}

void CZakumBody::CreateClapEffect()
{
	if (!m_HandMeet)
	{
		m_HandMeet = true;
		Vector3 ZakumPos = m_RootComponent->GetWorldPos();

		CZakumClapLightEffect* ClapLightEffect = m_Scene->CreateGameObject<CZakumClapLightEffect>("ClapLightEffect");
		ClapLightEffect->SetWorldPos(670.f, 460.f, ZakumPos.z - 5.f);

		CZakumClapCircleShockWave* ShockWave = m_Scene->CreateGameObject<CZakumClapCircleShockWave>("ClapCircleShockWave");
		ShockWave->SetWorldPos(665.f, 470.f, ZakumPos.z - 5.f);

		CZakumClapSmallCircleShockWave* SmallShockWave = m_Scene->CreateGameObject<CZakumClapSmallCircleShockWave>("ClapSmallCircleShockWave");
		SmallShockWave->SetWorldPos(665.f, 470.f, ZakumPos.z - 5.f);

		CZakumClapPinkShockWave* PinkRightShockWave = m_Scene->CreateGameObject<CZakumClapPinkShockWave>("ClapPinkRightShockWave");
		PinkRightShockWave->SetWorldPos(770.f, 470.f, ZakumPos.z- 5.f);

		CZakumClapPinkShockWave* PinkLeftShockWave = m_Scene->CreateGameObject<CZakumClapPinkShockWave>("ClapPinkLeftShockWave");
		PinkLeftShockWave->SetWorldPos(510.f, 470.f, ZakumPos.z - 5.f);
		PinkLeftShockWave->GetSprite()->Flip();

		CPlayer2D* Player = (CPlayer2D*)m_Scene->GetPlayerObject();
		Player->SetCameraShake(true);
	}
}

void CZakumBody::ArmSmash(int ArmID)
{
	CZakumDustEffect* Dust = nullptr;
	CZakumHandSmashLight* Light = nullptr;
	CZakumFogEffect* Fog = nullptr;

	Dust = m_Scene->CreateGameObject<CZakumDustEffect>("ZakumDust");
	Dust->SetWorldPos(Vector3(m_vecAttackWarningPos[ArmID].x - 5.f, m_vecAttackWarningPos[ArmID].y - 10.f, m_vecAttackWarningPos[ArmID].z + 10.f));

	Fog = m_Scene->CreateGameObject<CZakumFogEffect>("ZakumFog");
	Fog->SetWorldPos(Vector3(m_vecAttackWarningPos[ArmID].x, m_vecAttackWarningPos[ArmID].y + 10.f, m_vecAttackWarningPos[ArmID].z - 10.f));

	Light = m_Scene->CreateGameObject<CZakumHandSmashLight>("HandSmashLight");
	Light->SetWorldPos(m_vecAttackWarningPos[ArmID]);
	Light->SetSmashLightOwner(this);
	Light->SetOwnerArmID(ArmID);

	switch (ArmID)
	{

	case 0:
		m_LeftArm1Lower->SetRender(false);
		m_LeftArm1Upper->SetRender(false);
		//m_LeftArm1Hand->SetPivot(0.5f, 0.5f, 0.f);
		m_LeftArm1Hand->SetWorldPos(Vector3(m_vecAttackWarningPos[0].x, m_vecAttackWarningPos[0].y + 40.f, m_vecAttackWarningPos[0].z + 10.f));
		m_LeftArm1Hand->SetWorldRotation(0.f, 0.f, 0.f);
		m_LeftArm1Hand->ChangeAnimation("ZakumLeftHandSmash");

		Light->SetLightSpriteTexture("LeftLight2", TEXT("Monster/LeftHandSmashLightEffect2.png"));
		break;
	case 1:
		m_LeftArm2Lower->SetRender(false);
		m_LeftArm2Upper->SetRender(false);
		//m_LeftArm2Hand->SetPivot(0.5f, 0.5f, 0.f);
		m_LeftArm2Hand->SetWorldPos(Vector3(m_vecAttackWarningPos[1].x, m_vecAttackWarningPos[1].y + 40.f, m_vecAttackWarningPos[1].z + 10.f));
		m_LeftArm2Hand->SetWorldRotation(0.f, 0.f, 0.f);
		m_LeftArm2Hand->ChangeAnimation("ZakumLeftHandSmash");

		Light->SetLightSpriteTexture("LeftLight2", TEXT("Monster/LeftHandSmashLightEffect2.png"));

		break;
	case 2:
		m_LeftArm3Lower->SetRender(false);
		m_LeftArm3Upper->SetRender(false);
		//m_LeftArm3Hand->SetPivot(0.5f, 0.5f, 0.f);
		m_LeftArm3Hand->SetWorldPos(Vector3(m_vecAttackWarningPos[2].x, m_vecAttackWarningPos[2].y + 40.f, m_vecAttackWarningPos[2].z + 10.f));
		m_LeftArm3Hand->SetWorldRotation(0.f, 0.f, 0.f);
		m_LeftArm3Hand->ChangeAnimation("ZakumLeftHandSmash2");

		Light->SetLightSpriteTexture("LeftLight1", TEXT("Monster/LeftHandSmashLightEffect1.png"));

		break;
	case 3:
		m_LeftArm4Lower->SetRender(false);
		m_LeftArm4Upper->SetRender(false);
		//m_LeftArm4Hand->SetPivot(0.5f, 0.5f, 0.f);
		m_LeftArm4Hand->SetWorldPos(Vector3(m_vecAttackWarningPos[3].x, m_vecAttackWarningPos[3].y + 40.f, m_vecAttackWarningPos[3].z + 10.f));
		m_LeftArm4Hand->SetWorldRotation(0.f, 0.f, 0.f);
		m_LeftArm4Hand->ChangeAnimation("ZakumLeftHandSmash2");

		Light->SetLightSpriteTexture("LeftLight1", TEXT("Monster/LeftHandSmashLightEffect1.png"));
		break;
	case 4:
		m_RightArm1Lower->SetRender(false);
		m_RightArm1Upper->SetRender(false);
		//m_RightArm1Hand->SetPivot(0.5f, 0.5f, 0.f);
		m_RightArm1Hand->SetWorldPos(Vector3(m_vecAttackWarningPos[4].x, m_vecAttackWarningPos[4].y + 40.f, m_vecAttackWarningPos[4].z + 10.f));
		m_RightArm1Hand->SetWorldRotation(0.f, 0.f, 0.f);
		m_RightArm1Hand->ChangeAnimation("ZakumLeftHandSmash2");
		m_RightArm1Hand->Flip();

		Light->SetLightSpriteTexture("RightLight1", TEXT("Monster/RightHandSmashLightEffect1.png"));
		break;
	case 5:
		m_RightArm2Lower->SetRender(false);
		m_RightArm2Upper->SetRender(false);
		//m_RightArm2Hand->SetPivot(0.5f, 0.5f, 0.f);
		m_RightArm2Hand->SetWorldPos(Vector3(m_vecAttackWarningPos[5].x, m_vecAttackWarningPos[5].y + 40.f, m_vecAttackWarningPos[5].z + 10.f));
		m_RightArm2Hand->SetWorldRotation(0.f, 0.f, 0.f);
		m_RightArm2Hand->ChangeAnimation("ZakumLeftHandSmash2");
		m_RightArm2Hand->Flip();

		Light->SetLightSpriteTexture("RightLight1", TEXT("Monster/RightHandSmashLightEffect1.png"));

		break;
	case 6:
		m_RightArm3Lower->SetRender(false);
		m_RightArm3Upper->SetRender(false);
		//m_RightArm3Hand->SetPivot(0.5f, 0.5f, 0.f);
		m_RightArm3Hand->SetWorldPos(Vector3(m_vecAttackWarningPos[6].x, m_vecAttackWarningPos[6].y + 40.f, m_vecAttackWarningPos[6].z + 10.f));
		m_RightArm3Hand->SetWorldRotation(0.f, 0.f, 0.f);
		m_RightArm3Hand->ChangeAnimation("ZakumLeftHandSmash");
		m_RightArm3Hand->Flip();

		Light->SetLightSpriteTexture("RightLight2", TEXT("Monster/RightHandSmashLightEffect2.png"));

		break;
	case 7:
		m_RightArm4Lower->SetRender(false);
		m_RightArm4Upper->SetRender(false);
		//m_RightArm4Hand->SetPivot(0.5f, 0.5f, 0.f);
		m_RightArm4Hand->SetWorldPos(Vector3(m_vecAttackWarningPos[7].x, m_vecAttackWarningPos[7].y + 40.f, m_vecAttackWarningPos[7].z + 10.f));
		m_RightArm4Hand->SetWorldRotation(0.f, 0.f, 0.f);
		m_RightArm4Hand->ChangeAnimation("ZakumLeftHandSmash");
		m_RightArm4Hand->Flip();

		Light->SetLightSpriteTexture("RightLight2", TEXT("Monster/RightHandSmashLightEffect2.png"));

		break;
	}
}

void CZakumBody::ReturnArm(int ArmID)
{
	switch (ArmID)
	{
	case 0:
		//m_LeftArm1Hand->AddWorldPos(50.f, 30.f, 0.f);
		m_LeftArm1Hand->ChangeAnimation("ZakumArmReturnLeft1");
		break;
	case 1:
		//m_LeftArm2Hand->AddWorldPos(30.f, 30.f, 0.f);
		m_LeftArm2Hand->ChangeAnimation("ZakumArmReturnLeft1");
		break;
	case 2:
		//m_LeftArm3Hand->AddWorldPos(10.f, 30.f, 0.f);
		m_LeftArm3Hand->ChangeAnimation("ZakumArmReturnLeft2");
		break;
	case 3:
		//m_LeftArm4Hand->AddWorldPos(0.f, 40.f, 0.f);
		m_LeftArm4Hand->ChangeAnimation("ZakumArmReturnLeft2");
		break;
	case 4:
		//m_RightArm1Hand->AddWorldPos(-10.f, 30.f, 0.f);
		m_RightArm1Hand->ChangeAnimation("ZakumArmReturnLeft2");
		//m_RightArm1Hand->Flip();
		break;
	case 5:
		//m_RightArm2Hand->AddWorldPos(-20.f, 30.f, 0.f);
		m_RightArm2Hand->ChangeAnimation("ZakumArmReturnLeft2");
		//m_RightArm2Hand->Flip();
		break;
	case 6:
		//m_RightArm3Hand->AddWorldPos(-30.f, 30.f, 0.f);
		m_RightArm3Hand->ChangeAnimation("ZakumArmReturnLeft1");
		//m_RightArm3Hand->Flip();
		break;
	case 7:
		//m_RightArm4Hand->AddWorldPos(-50.f, 30.f, 0.f);
		m_RightArm4Hand->ChangeAnimation("ZakumArmReturnLeft1");
		//m_RightArm4Hand->Flip();
		break;
	}

	
}

void CZakumBody::ReturnArmOriginPos()
{
	auto iter = m_ListAttackArmNum.begin();
	auto iterEnd = m_ListAttackArmNum.end();

	m_ZakumState = Zakum_State::Idle;

	for (; iter != iterEnd; )
	{
		int ArmID = *iter;
		iter = m_ListAttackArmNum.erase(iter);
		iterEnd = m_ListAttackArmNum.end();

		m_vecArmState[ArmID] = Zakum_ArmState::Idle;
		m_vecAccUpHandRot[ArmID] = 0.f;

		switch (ArmID)
		{
		case 0:
			m_LeftArm1Lower->SetWorldPos(m_vecPreviousLowerPos[0]);
			m_LeftArm1Lower->SetWorldRotation(m_vecPreviousLowerRot[0]);
			m_LeftArm1Upper->SetWorldPos(m_vecPreviousUpperPos[0]);
			m_LeftArm1Upper->SetWorldRotation(m_vecPreviousUpperRot[0]);
			m_LeftArm1Hand->SetWorldPos(m_vecPreviousHandPos[0]);
			m_LeftArm1Hand->SetWorldRotation(m_vecPreviousHandRot[0]);

			m_LeftArm1Hand->ChangeAnimation("ZakumLeftHand");
			m_LeftArm1Hand->GetCurrentAnimation()->SetCurrentAnimationFrame(0);
			m_LeftArm1Hand->GetCurrentAnimation()->SetPlayScale(0.f);
			m_LeftArm1Lower->SetRender(true);
			m_LeftArm1Upper->SetRender(true);

			break;
		case 1:
			m_LeftArm2Lower->SetWorldPos(m_vecPreviousLowerPos[1]);
			m_LeftArm2Lower->SetWorldRotation(m_vecPreviousLowerRot[1]);
			m_LeftArm2Upper->SetWorldPos(m_vecPreviousUpperPos[1]);
			m_LeftArm2Upper->SetWorldRotation(m_vecPreviousUpperRot[1]);
			m_LeftArm2Hand->SetWorldPos(m_vecPreviousHandPos[1]);
			m_LeftArm2Hand->SetWorldRotation(m_vecPreviousHandRot[1]);

			m_LeftArm2Hand->ChangeAnimation("ZakumLeftHand");
			m_LeftArm2Hand->GetCurrentAnimation()->SetCurrentAnimationFrame(1);
			m_LeftArm2Hand->GetCurrentAnimation()->SetPlayScale(0.f);
			m_LeftArm2Lower->SetRender(true);
			m_LeftArm2Upper->SetRender(true);
			break;
		case 2:
			m_LeftArm3Lower->SetWorldPos(m_vecPreviousLowerPos[2]);
			m_LeftArm3Lower->SetWorldRotation(m_vecPreviousLowerRot[2]);
			m_LeftArm3Upper->SetWorldPos(m_vecPreviousUpperPos[2]);
			m_LeftArm3Upper->SetWorldRotation(m_vecPreviousUpperRot[2]);
			m_LeftArm3Hand->SetWorldPos(m_vecPreviousHandPos[2]);
			m_LeftArm3Hand->SetWorldRotation(m_vecPreviousHandRot[2]);

			m_LeftArm3Hand->ChangeAnimation("ZakumLeftHand");
			m_LeftArm3Hand->GetCurrentAnimation()->SetCurrentAnimationFrame(2);
			m_LeftArm3Hand->GetCurrentAnimation()->SetPlayScale(0.f);
			m_LeftArm3Lower->SetRender(true);
			m_LeftArm3Upper->SetRender(true);
			break;
		case 3:
			m_LeftArm4Lower->SetWorldPos(m_vecPreviousLowerPos[3]);
			m_LeftArm4Lower->SetWorldRotation(m_vecPreviousLowerRot[3]);
			m_LeftArm4Upper->SetWorldPos(m_vecPreviousUpperPos[3]);
			m_LeftArm4Upper->SetWorldRotation(m_vecPreviousUpperRot[3]);
			m_LeftArm4Hand->SetWorldPos(m_vecPreviousHandPos[3]);
			m_LeftArm4Hand->SetWorldRotation(m_vecPreviousHandRot[3]);

			m_LeftArm4Hand->ChangeAnimation("ZakumLeftHand");
			m_LeftArm4Hand->GetCurrentAnimation()->SetCurrentAnimationFrame(2);
			m_LeftArm4Hand->GetCurrentAnimation()->SetPlayScale(0.f);
			m_LeftArm4Lower->SetRender(true);
			m_LeftArm4Upper->SetRender(true);
			break;
		case 4:
			m_RightArm1Lower->SetWorldPos(m_vecPreviousLowerPos[4]);
			m_RightArm1Lower->SetWorldRotation(m_vecPreviousLowerRot[4]);
			m_RightArm1Upper->SetWorldPos(m_vecPreviousUpperPos[4]);
			m_RightArm1Upper->SetWorldRotation(m_vecPreviousUpperRot[4]);
			m_RightArm1Hand->SetWorldPos(m_vecPreviousHandPos[4]);
			m_RightArm1Hand->SetWorldRotation(m_vecPreviousHandRot[4]);

			m_RightArm1Hand->ChangeAnimation("ZakumRightHand");
			m_RightArm1Hand->GetCurrentAnimation()->SetCurrentAnimationFrame(0);
			m_RightArm1Hand->GetCurrentAnimation()->SetPlayScale(0.f);
			m_RightArm1Hand->Flip();
			m_RightArm1Lower->SetRender(true);
			m_RightArm1Upper->SetRender(true);
			break;
		case 5:
			m_RightArm2Lower->SetWorldPos(m_vecPreviousLowerPos[5]);
			m_RightArm2Lower->SetWorldRotation(m_vecPreviousLowerRot[5]);
			m_RightArm2Upper->SetWorldPos(m_vecPreviousUpperPos[5]);
			m_RightArm2Upper->SetWorldRotation(m_vecPreviousUpperRot[5]);
			m_RightArm2Hand->SetWorldPos(m_vecPreviousHandPos[5]);
			m_RightArm2Hand->SetWorldRotation(m_vecPreviousHandRot[5]);

			m_RightArm2Hand->ChangeAnimation("ZakumRightHand");
			m_RightArm2Hand->GetCurrentAnimation()->SetCurrentAnimationFrame(1);
			m_RightArm2Hand->GetCurrentAnimation()->SetPlayScale(0.f);
			m_RightArm2Hand->Flip();
			m_RightArm2Lower->SetRender(true);
			m_RightArm2Upper->SetRender(true);
			break;
		case 6:
			m_RightArm3Lower->SetWorldPos(m_vecPreviousLowerPos[6]);
			m_RightArm3Lower->SetWorldRotation(m_vecPreviousLowerRot[6]);
			m_RightArm3Upper->SetWorldPos(m_vecPreviousUpperPos[6]);
			m_RightArm3Upper->SetWorldRotation(m_vecPreviousUpperRot[6]);
			m_RightArm3Hand->SetWorldPos(m_vecPreviousHandPos[6]);
			m_RightArm3Hand->SetWorldRotation(m_vecPreviousHandRot[6]);

			m_RightArm3Hand->ChangeAnimation("ZakumRightHand");
			m_RightArm3Hand->GetCurrentAnimation()->SetCurrentAnimationFrame(2);
			m_RightArm3Hand->GetCurrentAnimation()->SetPlayScale(0.f);
			m_RightArm3Hand->Flip();
			m_RightArm3Lower->SetRender(true);
			m_RightArm3Upper->SetRender(true);
			break;
		case 7:
			m_RightArm4Lower->SetWorldPos(m_vecPreviousLowerPos[7]);
			m_RightArm4Lower->SetWorldRotation(m_vecPreviousLowerRot[7]);
			m_RightArm4Upper->SetWorldPos(m_vecPreviousUpperPos[7]);
			m_RightArm4Upper->SetWorldRotation(m_vecPreviousUpperRot[7]);
			m_RightArm4Hand->SetWorldPos(m_vecPreviousHandPos[7]);
			m_RightArm4Hand->SetWorldRotation(m_vecPreviousHandRot[7]);

			m_RightArm4Hand->ChangeAnimation("ZakumRightHand");
			m_RightArm4Hand->GetCurrentAnimation()->SetCurrentAnimationFrame(2);
			m_RightArm4Hand->GetCurrentAnimation()->SetPlayScale(0.f);
			m_RightArm4Hand->Flip();
			m_RightArm4Lower->SetRender(true);
			m_RightArm4Upper->SetRender(true);
			break;
		}
	}

	m_AccTime = 0.f;

	if (m_ListAttackArmNum.empty())
	{
		++m_SmashCount;

		if (m_SmashCount == m_SmashCountLimit)
		{
			for (int i = 0; i < 4; ++i)
			{
				CZakumFlame* Flame = m_Scene->CreateGameObject<CZakumFlame>("Flame");
				Flame->SetWorldPos(260.f + 276.f * i, 205.f, 220.f);
			}

			m_SmashCount = 0;
			m_ZakumState = Zakum_State::Clap;
			m_vecAccRotation.clear();
			m_vecAccUpHandRot.clear();
			m_vecAccRotation.resize(8);
			m_vecAccUpHandRot.resize(8);
		}
	}
}
