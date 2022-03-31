
#include "ZakumBody.h"
#include "../Animation/ZakumBodyAnimation.h"
#include "Input.h"
#include "../Animation/ZakumHandAnimation.h"
#include "Player2D.h"
#include "ZakumAttackWarningSpot.h"
#include "ZakumDustEffect.h"
#include "ZakumHandSmashLight.h"
#include "ZakumFogEffect.h"
#include "ZakumFlame.h"
#include "ClapWarning.h"
#include "ZakumClapLightEffect.h"
#include "ZakumClapCircleShockWave.h"
#include "ZakumClapSmallCircleShockWave.h"
#include "ZakumClapPinkShockWave.h"
#include "Bill.h"
#include "SylphideLancer.h"
#include "VoidPressure.h"
#include "DeathSide.h"
#include "../Widget/DamageFont.h"
#include "../Widget/BossInfoWindow.h"
#include "Scene/SceneManager.h"

#include <algorithm>

CZakumBody::CZakumBody() :
	m_AccTime(0.f),
	m_SmashCount(0),
	m_SmashCountLimit(2),
	m_ClapCount(0),
	m_ClapCountLimit(2),
	m_ClapArmNum(-1),
	m_HandMeet(false),
	m_AliveArmCount(8),
	m_ReturnClapPosComplete(false),
	m_CreateClapEffectComplete(false)
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
	m_vecClapRotUpperBound.resize(3);

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

	m_vecClapRotUpperBound[0] = 15.f;
	m_vecClapRotUpperBound[1] = 20.f;
	m_vecClapRotUpperBound[2] = 27.f;

	m_vecShakeRotBound[0] = 8.f;
	m_vecShakeRotBound[1] = 3.f;
	m_vecShakeRotBound[2] = 5.f;
	m_vecShakeRotBound[3] = 2.f;

	m_vecShakeRotBound[4] = 8.f;
	m_vecShakeRotBound[5] = 3.f;
	m_vecShakeRotBound[6] = 6.f;
	m_vecShakeRotBound[7] = 2.f;

	m_vecShakeRotSign[0] = -1.f;
	m_vecShakeRotSign[1] = -1.f;
	m_vecShakeRotSign[2] = -1.f;
	m_vecShakeRotSign[3] = 1.f;

	m_vecShakeRotSign[4] = 1.f;
	m_vecShakeRotSign[5] = 1.f;
	m_vecShakeRotSign[6] = -1.f;
	m_vecShakeRotSign[7] = -1.f;

	m_vecAccClapStretchRot.resize(3);

	m_ZakumInfo = CClientManager::GetInst()->GetZakumInfo();

	m_MonsterInfo.Level = m_ZakumInfo.Level;
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

	m_LeftArm1HandAttackCollider = CreateComponent<CColliderBox2D>("LeftArm1HandAttackCollider");
	m_LeftArm1Hand->AddChild(m_LeftArm1HandAttackCollider);
	m_LeftArm1HandAttackCollider->Enable(false);
	m_LeftArm1HandAttackCollider->SetWorldScale(60.f, 60.f, 1.f);
	m_LeftArm1HandAttackCollider->SetCollisionProfile("MonsterAttack");
	m_LeftArm1HandAttackCollider->SetRelativePos(0.f, -30.f, 0.f);
	m_LeftArm1HandAttackCollider->AddCollisionCallback<CZakumBody>(Collision_State::Begin, this, &CZakumBody::CollisionBeginCallback);
	m_LeftArm1HandAttackCollider->Start();

	m_LeftArm2HandAttackCollider = CreateComponent<CColliderBox2D>("LeftArm2HandAttackCollider");
	m_LeftArm2Hand->AddChild(m_LeftArm2HandAttackCollider);
	m_LeftArm2HandAttackCollider->Enable(false);
	m_LeftArm2HandAttackCollider->SetWorldScale(60.f, 60.f, 1.f);
	m_LeftArm2HandAttackCollider->SetCollisionProfile("MonsterAttack");
	m_LeftArm2HandAttackCollider->SetRelativePos(0.f, -30.f, 0.f);
	m_LeftArm2HandAttackCollider->AddCollisionCallback<CZakumBody>(Collision_State::Begin, this, &CZakumBody::CollisionBeginCallback);
	m_LeftArm2HandAttackCollider->Start();

	m_LeftArm3HandAttackCollider = CreateComponent<CColliderBox2D>("LeftArm3HandAttackCollider");
	m_LeftArm3Hand->AddChild(m_LeftArm3HandAttackCollider);
	m_LeftArm3HandAttackCollider->Enable(false);
	m_LeftArm3HandAttackCollider->SetWorldScale(60.f, 60.f, 1.f);
	m_LeftArm3HandAttackCollider->SetCollisionProfile("MonsterAttack");
	m_LeftArm3HandAttackCollider->SetRelativePos(0.f, -30.f, 0.f);
	m_LeftArm3HandAttackCollider->AddCollisionCallback<CZakumBody>(Collision_State::Begin, this, &CZakumBody::CollisionBeginCallback);
	m_LeftArm3HandAttackCollider->Start();

	m_LeftArm4HandAttackCollider = CreateComponent<CColliderBox2D>("LeftArm4HandAttackCollider");
	m_LeftArm4Hand->AddChild(m_LeftArm4HandAttackCollider);
	m_LeftArm4HandAttackCollider->Enable(false);
	m_LeftArm4HandAttackCollider->SetWorldScale(60.f, 60.f, 1.f);
	m_LeftArm4HandAttackCollider->SetCollisionProfile("MonsterAttack");
	m_LeftArm4HandAttackCollider->SetRelativePos(0.f, -30.f, 0.f);
	m_LeftArm4HandAttackCollider->AddCollisionCallback<CZakumBody>(Collision_State::Begin, this, &CZakumBody::CollisionBeginCallback);
	m_LeftArm4HandAttackCollider->Start();

	m_RightArm1HandAttackCollider = CreateComponent<CColliderBox2D>("RightArm1HandAttackCollider");
	m_RightArm1Hand->AddChild(m_RightArm1HandAttackCollider);
	m_RightArm1HandAttackCollider->Enable(false);
	m_RightArm1HandAttackCollider->SetWorldScale(60.f, 60.f, 1.f);
	m_RightArm1HandAttackCollider->SetCollisionProfile("MonsterAttack");
	m_RightArm1HandAttackCollider->SetRelativePos(0.f, -30.f, 0.f);
	m_RightArm1HandAttackCollider->AddCollisionCallback<CZakumBody>(Collision_State::Begin, this, &CZakumBody::CollisionBeginCallback);
	m_RightArm1HandAttackCollider->Start();

	m_RightArm2HandAttackCollider = CreateComponent<CColliderBox2D>("RightArm2HandAttackCollider");
	m_RightArm2Hand->AddChild(m_RightArm2HandAttackCollider);
	m_RightArm2HandAttackCollider->Enable(false);
	m_RightArm2HandAttackCollider->SetWorldScale(60.f, 60.f, 1.f);
	m_RightArm2HandAttackCollider->SetCollisionProfile("MonsterAttack");
	m_RightArm2HandAttackCollider->SetRelativePos(0.f, -30.f, 0.f);
	m_RightArm2HandAttackCollider->AddCollisionCallback<CZakumBody>(Collision_State::Begin, this, &CZakumBody::CollisionBeginCallback);
	m_RightArm2HandAttackCollider->Start();

	m_RightArm3HandAttackCollider = CreateComponent<CColliderBox2D>("RightArm3HandAttackCollider");
	m_RightArm3Hand->AddChild(m_RightArm3HandAttackCollider);
	m_RightArm3HandAttackCollider->Enable(false);
	m_RightArm3HandAttackCollider->SetWorldScale(60.f, 60.f, 1.f);
	m_RightArm3HandAttackCollider->SetCollisionProfile("MonsterAttack");
	m_RightArm3HandAttackCollider->SetRelativePos(0.f, -30.f, 0.f);
	m_RightArm3HandAttackCollider->AddCollisionCallback<CZakumBody>(Collision_State::Begin, this, &CZakumBody::CollisionBeginCallback);
	m_RightArm3HandAttackCollider->Start();

	m_RightArm4HandAttackCollider = CreateComponent<CColliderBox2D>("RightArm4HandAttackCollider");
	m_RightArm4Hand->AddChild(m_RightArm4HandAttackCollider);
	m_RightArm4HandAttackCollider->Enable(false);
	m_RightArm4HandAttackCollider->SetWorldScale(60.f, 60.f, 1.f);
	m_RightArm4HandAttackCollider->SetCollisionProfile("MonsterAttack");
	m_RightArm4HandAttackCollider->SetRelativePos(0.f, -30.f, 0.f);
	m_RightArm4HandAttackCollider->AddCollisionCallback<CZakumBody>(Collision_State::Begin, this, &CZakumBody::CollisionBeginCallback);
	m_RightArm4HandAttackCollider->Start();

	///// 플레이어에 의해 공격 당할때 쓰이는 충돌체 /////

	m_LeftArm1HandDefenseCollider = CreateComponent<CColliderBox2D>("LeftArm1HandDefenseCollider");
	m_LeftArm1Hand->AddChild(m_LeftArm1HandDefenseCollider);
	m_LeftArm1HandDefenseCollider->Enable(true);
	m_LeftArm1HandDefenseCollider->SetWorldScale(30.f, 120.f, 1.f);
	m_LeftArm1HandDefenseCollider->SetCollisionProfile("Monster");
	m_LeftArm1HandDefenseCollider->SetRelativePos(0.f, -20.f, 0.f);
	m_LeftArm1HandDefenseCollider->AddCollisionCallback<CZakumBody>(Collision_State::Begin, this, &CZakumBody::CollisionBeginCallback);
	m_LeftArm1HandDefenseCollider->Start();

	m_LeftArm2HandDefenseCollider = CreateComponent<CColliderBox2D>("LeftArm2HandDefenseCollider");
	m_LeftArm2Hand->AddChild(m_LeftArm2HandDefenseCollider);
	m_LeftArm2HandDefenseCollider->Enable(true);
	m_LeftArm2HandDefenseCollider->SetWorldScale(30.f, 120.f, 1.f);
	m_LeftArm2HandDefenseCollider->SetCollisionProfile("Monster");
	m_LeftArm2HandDefenseCollider->SetRelativePos(0.f, -20.f, 0.f);
	m_LeftArm2HandDefenseCollider->AddCollisionCallback<CZakumBody>(Collision_State::Begin, this, &CZakumBody::CollisionBeginCallback);
	m_LeftArm2HandDefenseCollider->Start();

	m_LeftArm3HandDefenseCollider = CreateComponent<CColliderBox2D>("LeftArm3HandDefenseCollider");
	m_LeftArm3Hand->AddChild(m_LeftArm3HandDefenseCollider);
	m_LeftArm3HandDefenseCollider->Enable(true);
	m_LeftArm3HandDefenseCollider->SetWorldScale(30.f, 120.f, 1.f);
	m_LeftArm3HandDefenseCollider->SetCollisionProfile("Monster");
	m_LeftArm3HandDefenseCollider->SetRelativePos(0.f, -20.f, 0.f);
	m_LeftArm3HandDefenseCollider->AddCollisionCallback<CZakumBody>(Collision_State::Begin, this, &CZakumBody::CollisionBeginCallback);
	m_LeftArm3HandDefenseCollider->Start();

	m_LeftArm4HandDefenseCollider = CreateComponent<CColliderBox2D>("LeftArm4HandDefenseCollider");
	m_LeftArm4Hand->AddChild(m_LeftArm4HandDefenseCollider);
	m_LeftArm4HandDefenseCollider->Enable(true);
	m_LeftArm4HandDefenseCollider->SetWorldScale(30.f, 120.f, 1.f);
	m_LeftArm4HandDefenseCollider->SetCollisionProfile("Monster");
	m_LeftArm4HandDefenseCollider->SetRelativePos(0.f, -20.f, 0.f);
	m_LeftArm4HandDefenseCollider->AddCollisionCallback<CZakumBody>(Collision_State::Begin, this, &CZakumBody::CollisionBeginCallback);
	m_LeftArm4HandDefenseCollider->Start();

	m_RightArm1HandDefenseCollider = CreateComponent<CColliderBox2D>("RightArm1HandDefenseCollider");
	m_RightArm1Hand->AddChild(m_RightArm1HandDefenseCollider);
	m_RightArm1HandDefenseCollider->Enable(true);
	m_RightArm1HandDefenseCollider->SetWorldScale(30.f, 120.f, 1.f);
	m_RightArm1HandDefenseCollider->SetCollisionProfile("Monster");
	m_RightArm1HandDefenseCollider->SetRelativePos(0.f, -20.f, 0.f);
	m_RightArm1HandDefenseCollider->AddCollisionCallback<CZakumBody>(Collision_State::Begin, this, &CZakumBody::CollisionBeginCallback);
	m_RightArm1HandDefenseCollider->Start();

	m_RightArm2HandDefenseCollider = CreateComponent<CColliderBox2D>("RightArm2HandDefenseCollider");
	m_RightArm2Hand->AddChild(m_RightArm2HandDefenseCollider);
	m_RightArm2HandDefenseCollider->Enable(true);
	m_RightArm2HandDefenseCollider->SetWorldScale(30.f, 120.f, 1.f);
	m_RightArm2HandDefenseCollider->SetCollisionProfile("Monster");
	m_RightArm2HandDefenseCollider->SetRelativePos(0.f, -20.f, 0.f);
	m_RightArm2HandDefenseCollider->AddCollisionCallback<CZakumBody>(Collision_State::Begin, this, &CZakumBody::CollisionBeginCallback);
	m_RightArm2HandDefenseCollider->Start();

	m_RightArm3HandDefenseCollider = CreateComponent<CColliderBox2D>("RightArm3HandDefenseCollider");
	m_RightArm3Hand->AddChild(m_RightArm3HandDefenseCollider);
	m_RightArm3HandDefenseCollider->Enable(true);
	m_RightArm3HandDefenseCollider->SetWorldScale(30.f, 120.f, 1.f);
	m_RightArm3HandDefenseCollider->SetCollisionProfile("Monster");
	m_RightArm3HandDefenseCollider->SetRelativePos(0.f, -20.f, 0.f);
	m_RightArm3HandDefenseCollider->AddCollisionCallback<CZakumBody>(Collision_State::Begin, this, &CZakumBody::CollisionBeginCallback);
	m_RightArm3HandDefenseCollider->Start();

	m_RightArm4HandDefenseCollider = CreateComponent<CColliderBox2D>("RightArm4HandDefenseCollider");
	m_RightArm4Hand->AddChild(m_RightArm4HandDefenseCollider);
	m_RightArm4HandDefenseCollider->Enable(true);
	m_RightArm4HandDefenseCollider->SetWorldScale(30.f, 120.f, 1.f);
	m_RightArm4HandDefenseCollider->SetCollisionProfile("Monster");
	m_RightArm4HandDefenseCollider->SetRelativePos(0.f, -20.f, 0.f);
	m_RightArm4HandDefenseCollider->AddCollisionCallback<CZakumBody>(Collision_State::Begin, this, &CZakumBody::CollisionBeginCallback);
	m_RightArm4HandDefenseCollider->Start();

	////// 8개 팔의 각자 DamageFont 용도의 Widget Component //////

	m_DamageWidgetComponent = CreateComponent<CWidgetComponent>("DamageFont");
	m_DamageWidgetComponent->UseAlphaBlend(true);

	CDamageFont* DamageFont = m_DamageWidgetComponent->CreateWidgetWindow<CDamageFont>("DamageFontWidget");

	m_BodySprite->AddChild(m_DamageWidgetComponent);

	m_DamageWidgetComponent->SetRelativePos(-10.f, 0.f, 0.f);
	m_DamageWidgetComponent->SetLayerName("ScreenWidgetComponent");

	m_LeftArm1HandDamageWidgetComponent = CreateComponent<CWidgetComponent>("LeftArm1DamageFont");
	m_LeftArm1HandDamageWidgetComponent->UseAlphaBlend(true);
	DamageFont = m_LeftArm1HandDamageWidgetComponent->CreateWidgetWindow<CDamageFont>("DamageFontWidget");
	m_LeftArm1Hand->AddChild(m_LeftArm1HandDamageWidgetComponent);
	m_LeftArm1HandDamageWidgetComponent->SetRelativePos(-10.f, 0.f, 0.f);
	m_LeftArm1HandDamageWidgetComponent->SetLayerName("ScreenWidgetComponent");
	m_LeftArm1HandDamageWidgetComponent->SetInheritRotZ(false);

	m_LeftArm2HandDamageWidgetComponent = CreateComponent<CWidgetComponent>("LeftArm2DamageFont");
	m_LeftArm2HandDamageWidgetComponent->UseAlphaBlend(true);
	DamageFont = m_LeftArm2HandDamageWidgetComponent->CreateWidgetWindow<CDamageFont>("DamageFontWidget");
	m_LeftArm2Hand->AddChild(m_LeftArm2HandDamageWidgetComponent);
	m_LeftArm2HandDamageWidgetComponent->SetRelativePos(-10.f, 0.f, 0.f);
	m_LeftArm2HandDamageWidgetComponent->SetLayerName("ScreenWidgetComponent");
	m_LeftArm2HandDamageWidgetComponent->SetInheritRotZ(false);

	m_LeftArm3HandDamageWidgetComponent = CreateComponent<CWidgetComponent>("LeftArm3DamageFont");
	m_LeftArm3HandDamageWidgetComponent->UseAlphaBlend(true);
	DamageFont = m_LeftArm3HandDamageWidgetComponent->CreateWidgetWindow<CDamageFont>("DamageFontWidget");
	m_LeftArm3Hand->AddChild(m_LeftArm3HandDamageWidgetComponent);
	m_LeftArm3HandDamageWidgetComponent->SetRelativePos(-10.f, 0.f, 0.f);
	m_LeftArm3HandDamageWidgetComponent->SetLayerName("ScreenWidgetComponent");
	m_LeftArm3HandDamageWidgetComponent->SetInheritRotZ(false);

	m_LeftArm4HandDamageWidgetComponent = CreateComponent<CWidgetComponent>("LeftArm4DamageFont");
	m_LeftArm4HandDamageWidgetComponent->UseAlphaBlend(true);
	DamageFont = m_LeftArm4HandDamageWidgetComponent->CreateWidgetWindow<CDamageFont>("DamageFontWidget");
	m_LeftArm4Hand->AddChild(m_LeftArm4HandDamageWidgetComponent);
	m_LeftArm4HandDamageWidgetComponent->SetRelativePos(-10.f, 0.f, 0.f);
	m_LeftArm4HandDamageWidgetComponent->SetLayerName("ScreenWidgetComponent");
	m_LeftArm4HandDamageWidgetComponent->SetInheritRotZ(false);

	m_RightArm1HandDamageWidgetComponent = CreateComponent<CWidgetComponent>("RightArm1DamageFont");
	m_RightArm1HandDamageWidgetComponent->UseAlphaBlend(true);
	DamageFont = m_RightArm1HandDamageWidgetComponent->CreateWidgetWindow<CDamageFont>("DamageFontWidget");
	m_RightArm1Hand->AddChild(m_RightArm1HandDamageWidgetComponent);
	m_RightArm1HandDamageWidgetComponent->SetRelativePos(-10.f, 0.f, 0.f);
	m_RightArm1HandDamageWidgetComponent->SetLayerName("ScreenWidgetComponent");
	m_RightArm1HandDamageWidgetComponent->SetInheritRotZ(false);

	m_RightArm2HandDamageWidgetComponent = CreateComponent<CWidgetComponent>("RightArm2DamageFont");
	m_RightArm2HandDamageWidgetComponent->UseAlphaBlend(true);
	DamageFont = m_RightArm2HandDamageWidgetComponent->CreateWidgetWindow<CDamageFont>("DamageFontWidget");
	m_RightArm2Hand->AddChild(m_RightArm2HandDamageWidgetComponent);
	m_RightArm2HandDamageWidgetComponent->SetRelativePos(-10.f, 0.f, 0.f);
	m_RightArm2HandDamageWidgetComponent->SetLayerName("ScreenWidgetComponent");
	m_RightArm2HandDamageWidgetComponent->SetInheritRotZ(false);

	m_RightArm3HandDamageWidgetComponent = CreateComponent<CWidgetComponent>("RightArm3DamageFont");
	m_RightArm3HandDamageWidgetComponent->UseAlphaBlend(true);
	DamageFont = m_RightArm3HandDamageWidgetComponent->CreateWidgetWindow<CDamageFont>("DamageFontWidget");
	m_RightArm3Hand->AddChild(m_RightArm3HandDamageWidgetComponent);
	m_RightArm3HandDamageWidgetComponent->SetRelativePos(-10.f, 0.f, 0.f);
	m_RightArm3HandDamageWidgetComponent->SetLayerName("ScreenWidgetComponent");
	m_RightArm3HandDamageWidgetComponent->SetInheritRotZ(false);

	m_RightArm4HandDamageWidgetComponent = CreateComponent<CWidgetComponent>("RightArm4DamageFont");
	m_RightArm4HandDamageWidgetComponent->UseAlphaBlend(true);
	DamageFont = m_RightArm4HandDamageWidgetComponent->CreateWidgetWindow<CDamageFont>("DamageFontWidget");
	m_RightArm4Hand->AddChild(m_RightArm4HandDamageWidgetComponent);
	m_RightArm4HandDamageWidgetComponent->SetRelativePos(-10.f, 0.f, 0.f);
	m_RightArm4HandDamageWidgetComponent->SetLayerName("ScreenWidgetComponent");
	m_RightArm4HandDamageWidgetComponent->SetInheritRotZ(false);
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
			//m_AccTime = 0.f;
			CheckUpHandAttackArm(DeltaTime);
		}
	}

	float ZRot = 0.f;

	if (m_LeftArm1Root && m_LeftArm1Root->IsActive() && m_vecArmState[0] == Zakum_ArmState::Idle)
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

	if (m_LeftArm2Root && m_LeftArm2Root->IsActive() && m_vecArmState[1] == Zakum_ArmState::Idle)
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

	if (m_LeftArm3Root && m_LeftArm3Root->IsActive() && m_vecArmState[2] == Zakum_ArmState::Idle)
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


	if (m_LeftArm4Root && m_LeftArm4Root->IsActive() && m_vecArmState[3] == Zakum_ArmState::Idle)
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

	if (m_RightArm1Root && m_RightArm1Root->IsActive() && m_vecArmState[4] == Zakum_ArmState::Idle)
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

	if (m_RightArm2Root && m_RightArm2Root->IsActive() && m_vecArmState[5] == Zakum_ArmState::Idle)
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

	if (m_RightArm3Root && m_RightArm3Root->IsActive() && m_vecArmState[6] == Zakum_ArmState::Idle)
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

	if (m_RightArm4Root && m_RightArm4Root->IsActive() && m_vecArmState[7] == Zakum_ArmState::Idle)
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

	m_Body = (CColliderBox2D*)FindComponent("Body");
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

	CPlayer2D* Player = (CPlayer2D*)m_Scene->GetPlayerObject();

	if (m_vecAccClapStretchRot[ArmID] >= m_vecClapRotUpperBound[ArmID])
	{
		switch (ArmID)
		{
		case 0:
			if (!m_LeftArm1Hand || !m_LeftArm1Hand->IsActive() || !m_RightArm1Hand || !m_RightArm1Hand->IsActive())
			{
				m_ZakumState = Zakum_State::Idle;
				m_ClapArmNum = -1;
				m_AccTime = 0.f;
				for (int i = 0; i < 8; ++i)
				{
					m_vecArmState[i] = Zakum_ArmState::Idle;
				}

				for (int i = 0; i < 4; ++i)
				{
					char FlameName[256] = {};
					sprintf_s(FlameName, "Flame%d", i);
					std::string StrFlameName = FlameName;
					CZakumFlame* Flame = (CZakumFlame*)m_Scene->FindObject(StrFlameName);

					if (Flame)
						Flame->Destroy();

				}

				// 늘린 팔 위치 되돌려주기?
			}
			break;
		case 1:
			if (!m_LeftArm2Hand || !m_LeftArm2Hand->IsActive() || !m_RightArm2Hand || !m_RightArm2Hand->IsActive())
			{
				m_ZakumState = Zakum_State::Idle;
				m_ClapArmNum = -1;
				m_AccTime = 0.f;
				for (int i = 0; i < 8; ++i)
				{
					m_vecArmState[i] = Zakum_ArmState::Idle;
				}

				for (int i = 0; i < 4; ++i)
				{
					char FlameName[256] = {};
					sprintf_s(FlameName, "Flame%d", i);
					std::string StrFlameName = FlameName;
					CZakumFlame* Flame = (CZakumFlame*)m_Scene->FindObject(StrFlameName);

					if (Flame)
						Flame->Destroy();

				}

				// 늘린 팔 위치 되돌려주기?

			}
			break;
		case 2:
			if (!m_LeftArm3Hand || !m_LeftArm3Hand->IsActive() || !m_RightArm3Hand || !m_RightArm3Hand->IsActive())
			{
				m_ZakumState = Zakum_State::Idle;
				m_ClapArmNum = -1;
				m_AccTime = 0.f;
				for (int i = 0; i < 8; ++i)
				{
					m_vecArmState[i] = Zakum_ArmState::Idle;
				}

				for (int i = 0; i < 4; ++i)
				{
					char FlameName[256] = {};
					sprintf_s(FlameName, "Flame%d", i);
					std::string StrFlameName = FlameName;
					CZakumFlame* Flame = (CZakumFlame*)m_Scene->FindObject(StrFlameName);

					if (Flame)
						Flame->Destroy();
				}

				// 늘린 팔 위치 되돌려주기?

			}
			break;
		}
		return;
	}

	float RotAmount = 7.f * DeltaTime;
	m_vecAccClapStretchRot[ArmID] += RotAmount;

	switch (ArmID)
	{
	case 0:
		if (m_LeftArm1Upper->GetWorldRot().z < 10.f)
		{
			m_LeftArm1Upper->AddWorldPos(-4.f * DeltaTime, 0.f, 0.f);
			m_LeftArm1Upper->AddWorldRotation(0.f, 0.f, RotAmount);
		}

		if (m_RightArm1Upper->GetWorldRot().z > -10.f)
		{
			m_RightArm1Upper->AddWorldPos(4.f * DeltaTime, 0.f, 0.f);
			m_RightArm1Upper->AddWorldRotation(0.f, 0.f, -1.f * RotAmount);
		}
		break;
	case 1:
		if (m_LeftArm2Upper->GetWorldRot().z < 10.f)
		{
			m_LeftArm2Upper->AddWorldPos(-4.f * DeltaTime, 0.f, 0.f);
			m_LeftArm2Upper->AddWorldRotation(0.f, 0.f, RotAmount);
		}

		if (m_RightArm2Upper->GetWorldRot().z > -10.f)
		{
			m_RightArm2Upper->AddWorldPos(4.f * DeltaTime, 0.f, 0.f);
			m_RightArm2Upper->AddWorldRotation(0.f, 0.f, -1.f * RotAmount);
		}

		break;
	case 2:
		if (m_LeftArm3Upper->GetWorldRot().z < 40.f)
		{
			m_LeftArm3Upper->AddWorldPos(-4.f * DeltaTime, 0.f, 0.f);
			m_LeftArm3Upper->AddWorldRotation(0.f, 0.f, RotAmount);
		}

		if (m_RightArm3Upper->GetWorldRot().z > -40.f)
		{
			m_RightArm3Upper->AddWorldPos(4.f * DeltaTime, 0.f, 0.f);
			m_RightArm3Upper->AddWorldRotation(0.f, 0.f, -1.f * RotAmount);
		}

		break;
	}
}

void CZakumBody::CheckUpHandAttackArm(float DeltaTime)
{
	if (m_ListAttackArmNum.empty())
	{
		if (m_AliveArmCount == 0)
			return;

		// 8개의 팔중에 랜덤한 수의 팔을 UpHand 상태일때마다 뽑아서 뽑힌 팔은 내려치는 공격을 실행한다
		// LeftArm1,2,3,4 가 0,1,2,3번 인덱스, RightArm1,2,3,4가 4,5,6,7번 인덱스
		int AttackArmCount = rand() % m_AliveArmCount + 1;
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

			if (AttackArmID == 0 && (!m_LeftArm1Root || !m_LeftArm1Root->IsActive()))
				continue;
			else if (AttackArmID == 1 && (!m_LeftArm2Root || !m_LeftArm2Root->IsActive()))
				continue;
			else if (AttackArmID == 2 && (!m_LeftArm3Root || !m_LeftArm3Root->IsActive()))
				continue;
			else if (AttackArmID == 3 && (!m_LeftArm4Root || !m_LeftArm4Root->IsActive()))
				continue;
			else if (AttackArmID == 4 && (!m_RightArm1Root || !m_RightArm1Root->IsActive()))
				continue;
			else if (AttackArmID == 5 && (!m_RightArm2Root || !m_RightArm2Root->IsActive()))
				continue;
			else if (AttackArmID == 6 && (!m_RightArm3Root || !m_RightArm3Root->IsActive()))
				continue;
			else if (AttackArmID == 7 && (!m_RightArm4Root || !m_RightArm4Root->IsActive()))
				continue;

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

void CZakumBody::DecreaseAliveArmCount()
{
	if (m_AliveArmCount == 0)
		return;
	
	--m_AliveArmCount;
}

void CZakumBody::PickAndStretchClapHand(float DeltaTime)
{
	if (m_ClapArmNum == -1)
	{
		if (m_AliveArmCount == 0)
			return;

		// 0번은 제일 위에 왼쪽,오른쪽이 박수
		int AttackArmID = rand() % 3;
		//int AttackArmID = 0;

		// 박수칠 팔이 하나도 없을 경우
		if ((!m_LeftArm1Hand->IsActive() && !m_LeftArm2Hand->IsActive() && !m_LeftArm3Hand->IsActive())
			|| (!m_RightArm1Hand->IsActive() && !m_RightArm2Hand->IsActive() && !m_RightArm3Hand->IsActive()))
		{
			m_ClapArmNum = -1;

			CPlayer2D* Player = (CPlayer2D*)m_Scene->GetPlayerObject();

			for (int i = 0; i < 4; ++i)
			{
				char FlameName[256] = {};
				sprintf_s(FlameName, "Flame%d", i);
				std::string StrFlameName = FlameName;
				CZakumFlame* Flame = (CZakumFlame*)m_Scene->FindObject(StrFlameName);

				if (Flame)
					Flame->Destroy();

			}

			m_ZakumState = Zakum_State::Idle;
			m_ClapCount = 0;
			return;
		}

		if (AttackArmID == 0 && (!m_LeftArm1Root || !m_LeftArm1Root->IsActive() || !m_RightArm1Root || !m_RightArm1Root->IsActive()))
		{
			m_ClapArmNum = -1;
			return;
		}

		else if (AttackArmID == 1 && (!m_LeftArm2Root || !m_LeftArm2Root->IsActive() || !m_RightArm2Root || !m_RightArm2Root->IsActive()))
		{
			m_ClapArmNum = -1;
			return;
		}

		else if (AttackArmID == 2 && (!m_LeftArm3Root || !m_LeftArm3Root->IsActive() || !m_RightArm3Root || !m_RightArm3Root->IsActive()))
		{
			m_ClapArmNum = -1;
			return;
		}

		m_ClapArmNum = AttackArmID;
		
		// 왼쪽 팔 Clap상태 만들기
		m_vecArmState[AttackArmID] = Zakum_ArmState::Clap;
		// 오른쪽 팔 Clap상태 만들기
		m_vecArmState[AttackArmID + 4] = Zakum_ArmState::Clap;

		CClapWarning* LeftClapWarning = nullptr;

		CClapWarning * RightClapWarning = nullptr;

		switch(AttackArmID)
		{ 
		case 0:
			m_vecPreviousHandPos[0] = m_LeftArm1Hand->GetWorldPos();
			m_vecPreviousHandRot[0] = m_LeftArm1Hand->GetWorldRot();
			m_vecPreviousUpperPos[0] = m_LeftArm1Upper->GetWorldPos();
			m_vecPreviousUpperRot[0] = m_LeftArm1Upper->GetWorldRot();

			m_vecPreviousHandPos[4] = m_RightArm1Hand->GetWorldPos();
			m_vecPreviousHandRot[4] = m_RightArm1Hand->GetWorldRot();
			m_vecPreviousUpperPos[4] = m_RightArm1Upper->GetWorldPos();
			m_vecPreviousUpperRot[4] = m_RightArm1Upper->GetWorldRot();

			m_LeftArm1Hand->ChangeAnimation("ZakumLeftHand");
			m_RightArm1Hand->ChangeAnimation("ZakumRightHand");
			m_LeftArm1Hand->GetCurrentAnimation()->SetPlayScale(1.f);
			m_RightArm1Hand->GetCurrentAnimation()->SetPlayScale(1.f);

			LeftClapWarning = m_Scene->CreateGameObject<CClapWarning>("LeftClapWarning");
			LeftClapWarning->SetWorldPos(305.f, 475.f - AttackArmID * 104.f, 215.f);
			LeftClapWarning->SetWarningPointOwner(this);

			RightClapWarning = m_Scene->CreateGameObject<CClapWarning>("RightClapWarning");
			RightClapWarning->SetWorldPos(1075.f, 475.f - AttackArmID * 104.f, 215.f);
			RightClapWarning->SetWarningPointOwner(this);

			break;
		case 1:								   
			m_vecPreviousHandPos[1] = m_LeftArm2Hand->GetWorldPos();
			m_vecPreviousHandRot[1] = m_LeftArm2Hand->GetWorldRot();
			m_vecPreviousUpperPos[1] = m_LeftArm2Upper->GetWorldPos();
			m_vecPreviousUpperRot[1] = m_LeftArm2Upper->GetWorldRot();

			m_vecPreviousHandPos[5] = m_RightArm2Hand->GetWorldPos();
			m_vecPreviousHandRot[5] = m_RightArm2Hand->GetWorldRot();
			m_vecPreviousUpperPos[5] = m_RightArm2Upper->GetWorldPos();
			m_vecPreviousUpperRot[5] = m_RightArm2Upper->GetWorldRot();

			m_LeftArm2Hand->ChangeAnimation("ZakumLeftHand");
			m_RightArm2Hand->ChangeAnimation("ZakumRightHand");
			m_LeftArm2Hand->GetCurrentAnimation()->SetPlayScale(1.f);
			m_RightArm2Hand->GetCurrentAnimation()->SetPlayScale(1.f);

			LeftClapWarning = m_Scene->CreateGameObject<CClapWarning>("LeftClapWarning");
			LeftClapWarning->SetWorldPos(255.f, 475.f - AttackArmID * 104.f, 215.f);
			LeftClapWarning->SetWarningPointOwner(this);

			RightClapWarning = m_Scene->CreateGameObject<CClapWarning>("RightClapWarning");
			RightClapWarning->SetWorldPos(1120.f, 475.f - AttackArmID * 104.f, 215.f);
			RightClapWarning->SetWarningPointOwner(this);

			break;
		case 2:
			m_vecPreviousHandPos[2] = m_LeftArm3Hand->GetWorldPos();
			m_vecPreviousHandRot[2] = m_LeftArm3Hand->GetWorldRot();
			m_vecPreviousUpperPos[2] = m_LeftArm3Upper->GetWorldPos();
			m_vecPreviousUpperRot[2] = m_LeftArm3Upper->GetWorldRot();

			m_vecPreviousHandPos[6] = m_RightArm3Hand->GetWorldPos();
			m_vecPreviousHandRot[6] = m_RightArm3Hand->GetWorldRot();
			m_vecPreviousUpperPos[6] = m_RightArm3Upper->GetWorldPos();
			m_vecPreviousUpperRot[6] = m_RightArm3Upper->GetWorldRot();

			m_LeftArm3Hand->ChangeAnimation("ZakumLeftHand");
			m_RightArm3Hand->ChangeAnimation("ZakumRightHand");
			m_LeftArm3Hand->GetCurrentAnimation()->SetPlayScale(1.f);
			m_RightArm3Hand->GetCurrentAnimation()->SetPlayScale(1.f);

			LeftClapWarning = m_Scene->CreateGameObject<CClapWarning>("LeftClapWarning");
			LeftClapWarning->SetWorldPos(255.f, 475.f - AttackArmID * 104.f, 215.f);
			LeftClapWarning->SetWarningPointOwner(this);

			RightClapWarning = m_Scene->CreateGameObject<CClapWarning>("RightClapWarning");
			RightClapWarning->SetWorldPos(1120.f, 475.f - AttackArmID * 104.f, 215.f);
			RightClapWarning->SetWarningPointOwner(this);

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

	switch (m_ClapArmNum)
	{
	case 0:
		if (m_LeftArm1Hand && m_LeftArm1Hand->IsActive() && m_RightArm1Hand && m_RightArm1Hand->IsActive())
		{
			if (!m_LeftArm1Hand->GetAnimationInstance()->CheckCurrentAnimation("ZakumClapLeft"))
			{
				m_LeftArm1Hand->ChangeAnimation("ZakumClapLeft");

				m_LeftArm1Upper->SetWorldRotation(0.f, 0.f, 0.f);
				m_LeftArm1Hand->SetWorldRotation(0.f, 0.f, 0.f);
				m_LeftArm1Hand->SetWorldPos(635.f, 463.f, ZakumPos.z - 2.f);
			}

			if (!m_RightArm1Hand->GetAnimationInstance()->CheckCurrentAnimation("ZakumClapRight"))
			{
				m_RightArm1Hand->ChangeAnimation("ZakumClapRight");

				m_RightArm1Upper->SetWorldRotation(0.f, 0.f, 0.f);
				m_RightArm1Hand->SetWorldRotation(0.f, 0.f, 0.f);
				m_RightArm1Hand->SetWorldPos(718.f, 465.f, ZakumPos.z - 3.f);
			}

			if (m_LeftArm1Lower->IsRender())
				m_LeftArm1Lower->SetRender(false);
			if (m_RightArm1Lower->IsRender())
				m_RightArm1Lower->SetRender(false);
			if (m_LeftArm1Upper->IsRender())
				m_LeftArm1Upper->SetRender(false);
			if (m_RightArm1Upper->IsRender())
				m_RightArm1Upper->SetRender(false);

			m_LeftArm1HandAttackCollider->Enable(true);
			m_RightArm1HandAttackCollider->Enable(true);
			m_LeftArm1HandAttackCollider->SetWorldPos(310.f, 500.f, ZakumPos.z);
			m_RightArm1HandAttackCollider->SetWorldPos(1070.f, 500.f, ZakumPos.z);
			m_LeftArm1HandAttackCollider->SetWorldScale(200.f, 30.f, 1.f);
			m_RightArm1HandAttackCollider->SetWorldScale(200.f, 30.f, 1.f);
		}
		break;

	case 1:
		if (m_LeftArm2Hand && m_LeftArm2Hand->IsActive() && m_RightArm2Hand && m_RightArm2Hand->IsActive())
		{
			if (!m_LeftArm2Hand->GetAnimationInstance()->CheckCurrentAnimation("ZakumClapLeft"))
			{
				m_LeftArm2Hand->ChangeAnimation("ZakumClapLeft");

				m_LeftArm2Upper->SetWorldRotation(0.f, 0.f, 0.f);
				m_LeftArm2Hand->SetWorldRotation(0.f, 0.f, 0.f);
				m_LeftArm2Hand->SetWorldPos(636.f, 391.f, ZakumPos.z - 2.f);
			}

			if (!m_RightArm2Hand->GetAnimationInstance()->CheckCurrentAnimation("ZakumClapRight"))
			{
				m_RightArm2Hand->ChangeAnimation("ZakumClapRight");

				m_RightArm2Upper->SetWorldRotation(0.f, 0.f, 0.f);
				m_RightArm2Hand->SetWorldRotation(0.f, 0.f, 0.f);
				m_RightArm2Hand->SetWorldPos(722.f, 389.f, ZakumPos.z - 3.f);
			}

			if (m_LeftArm2Lower->IsRender())
				m_LeftArm2Lower->SetRender(false);
			if (m_RightArm2Lower->IsRender())
				m_RightArm2Lower->SetRender(false);
			if (m_LeftArm2Upper->IsRender())
				m_LeftArm2Upper->SetRender(false);
			if (m_RightArm2Upper->IsRender())
				m_RightArm2Upper->SetRender(false);

			m_LeftArm2HandAttackCollider->Enable(true);
			m_RightArm2HandAttackCollider->Enable(true);
			m_LeftArm2HandAttackCollider->SetWorldPos(250.f, 380.f, ZakumPos.z);
			m_RightArm2HandAttackCollider->SetWorldPos(1120.f, 380.f, ZakumPos.z);
			m_LeftArm2HandAttackCollider->SetWorldScale(200.f, 30.f, 1.f);
			m_RightArm2HandAttackCollider->SetWorldScale(200.f, 30.f, 1.f);
		}
		break;

	case 2:
		if (m_LeftArm3Hand && m_LeftArm3Hand->IsActive() && m_RightArm3Hand && m_RightArm3Hand->IsActive())
		{
			if (!m_LeftArm3Hand->GetAnimationInstance()->CheckCurrentAnimation("ZakumClapLeft"))
			{
				m_LeftArm3Hand->ChangeAnimation("ZakumClapLeft");

				m_LeftArm3Upper->SetWorldRotation(0.f, 0.f, 0.f);
				m_LeftArm3Hand->SetWorldRotation(0.f, 0.f, 0.f);
				m_LeftArm3Hand->SetWorldPos(628.f, 299.f, ZakumPos.z - 2.f);
			}

			if (!m_RightArm3Hand->GetAnimationInstance()->CheckCurrentAnimation("ZakumClapRight"))
			{
				m_RightArm3Hand->ChangeAnimation("ZakumClapRight");

				m_RightArm3Upper->SetWorldRotation(0.f, 0.f, 0.f);
				m_RightArm3Hand->SetWorldRotation(0.f, 0.f, 0.f);
				m_RightArm3Hand->SetWorldPos(707.f, 301.f, ZakumPos.z - 3.f);
			}

			if (m_LeftArm3Lower->IsRender())
				m_LeftArm3Lower->SetRender(false);
			if (m_RightArm3Lower->IsRender())
				m_RightArm3Lower->SetRender(false);
			if (m_LeftArm3Upper->IsRender())
				m_LeftArm3Upper->SetRender(false);
			if (m_RightArm3Upper->IsRender())
				m_RightArm3Upper->SetRender(false);

			m_LeftArm3HandAttackCollider->Enable(true);
			m_RightArm3HandAttackCollider->Enable(true);
			m_LeftArm3HandAttackCollider->SetWorldPos(250.f, 290.f, ZakumPos.z);
			m_RightArm3HandAttackCollider->SetWorldPos(1120.f, 290.f, ZakumPos.z);
			m_LeftArm3HandAttackCollider->SetWorldScale(200.f, 30.f, 1.f);
			m_RightArm3HandAttackCollider->SetWorldScale(200.f, 30.f, 1.f);
		}
		break;
	}
}

void CZakumBody::CreateClapEffect()
{
	// 왼쪽손이 ZakumClapLeft 시퀀스를 끝내고 이 함수를 EndFunction으로 호출할지 오른쪽손이 ZakumClapRight 시퀀스를 끝내고 이 함수를
	// EndFunction으로 호출할지 모르지만, 둘중 하나가 호출했으면 나머지 하나는 호출하면 안되므로 m_HandMeet이 false일때만 ClapEffect를 만들어준다
	if (!m_HandMeet)
	{
		m_HandMeet = true;
		Vector3 ZakumPos = m_RootComponent->GetWorldPos();
		CZakumClapLightEffect* ClapLightEffect = nullptr;
		CZakumClapCircleShockWave* ShockWave = nullptr;
		CZakumClapSmallCircleShockWave* SmallShockWave = nullptr;
		CZakumClapPinkShockWave* PinkRightShockWave = nullptr;
		CZakumClapPinkShockWave* PinkLeftShockWave = nullptr;
		
		switch(m_ClapArmNum)
		{
		case 0:
			ClapLightEffect = m_Scene->CreateGameObject<CZakumClapLightEffect>("ClapLightEffect");
			ClapLightEffect->SetWorldPos(675.f, 460.f, ZakumPos.z - 5.f);
			ClapLightEffect->SetSmashLightOwner(this);

			ShockWave = m_Scene->CreateGameObject<CZakumClapCircleShockWave>("ClapCircleShockWave");
			ShockWave->SetWorldPos(665.f, 470.f, ZakumPos.z - 5.f);

			SmallShockWave = m_Scene->CreateGameObject<CZakumClapSmallCircleShockWave>("ClapSmallCircleShockWave");
			SmallShockWave->SetWorldPos(665.f, 470.f, ZakumPos.z - 5.f);

			PinkRightShockWave = m_Scene->CreateGameObject<CZakumClapPinkShockWave>("ClapPinkRightShockWave");
			PinkRightShockWave->SetWorldPos(810.f, 470.f, ZakumPos.z - 5.f);

			PinkLeftShockWave = m_Scene->CreateGameObject<CZakumClapPinkShockWave>("ClapPinkLeftShockWave");
			PinkLeftShockWave->SetWorldPos(510.f, 470.f, ZakumPos.z - 5.f);
			PinkLeftShockWave->GetSprite()->Flip();
			break;
		case 1:
			ClapLightEffect = m_Scene->CreateGameObject<CZakumClapLightEffect>("ClapLightEffect");
			ClapLightEffect->SetWorldPos(677.f, 385.f, ZakumPos.z - 5.f);
			ClapLightEffect->SetSmashLightOwner(this);

			ShockWave = m_Scene->CreateGameObject<CZakumClapCircleShockWave>("ClapCircleShockWave");
			ShockWave->SetWorldPos(665.f, 363.f, ZakumPos.z - 5.f);

			SmallShockWave = m_Scene->CreateGameObject<CZakumClapSmallCircleShockWave>("ClapSmallCircleShockWave");
			SmallShockWave->SetWorldPos(665.f, 363.f, ZakumPos.z - 5.f);

			PinkRightShockWave = m_Scene->CreateGameObject<CZakumClapPinkShockWave>("ClapPinkRightShockWave");
			PinkRightShockWave->SetWorldPos(814.f, 363.f, ZakumPos.z - 5.f);

			PinkLeftShockWave = m_Scene->CreateGameObject<CZakumClapPinkShockWave>("ClapPinkLeftShockWave");
			PinkLeftShockWave->SetWorldPos(510.f, 363.f, ZakumPos.z - 5.f);
			PinkLeftShockWave->GetSprite()->Flip();
			break;
		case 2:
			ClapLightEffect = m_Scene->CreateGameObject<CZakumClapLightEffect>("ClapLightEffect");
			ClapLightEffect->SetWorldPos(670.f, 295.f, ZakumPos.z - 5.f);
			ClapLightEffect->SetSmashLightOwner(this);

			ShockWave = m_Scene->CreateGameObject<CZakumClapCircleShockWave>("ClapCircleShockWave");
			ShockWave->SetWorldPos(665.f, 285.f, ZakumPos.z - 5.f);

			SmallShockWave = m_Scene->CreateGameObject<CZakumClapSmallCircleShockWave>("ClapSmallCircleShockWave");
			SmallShockWave->SetWorldPos(665.f, 285.f, ZakumPos.z - 5.f);

			PinkRightShockWave = m_Scene->CreateGameObject<CZakumClapPinkShockWave>("ClapPinkRightShockWave");
			PinkRightShockWave->SetWorldPos(810.f, 285.f, ZakumPos.z - 5.f);

			PinkLeftShockWave = m_Scene->CreateGameObject<CZakumClapPinkShockWave>("ClapPinkLeftShockWave");
			PinkLeftShockWave->SetWorldPos(510.f, 285.f, ZakumPos.z - 5.f);
			PinkLeftShockWave->GetSprite()->Flip();
			break;
		}
		CPlayer2D* Player = (CPlayer2D*)m_Scene->GetPlayerObject();
		Player->SetYCameraShake(true);
	}
}

void CZakumBody::ClapReturnArm()
{
	switch (m_ClapArmNum)
	{
	case 0:
		if (m_LeftArm1Hand && m_LeftArm1Hand->IsActive() && m_RightArm1Hand && m_RightArm1Hand->IsActive())
		{
			m_LeftArm1Hand->ChangeAnimation("ZakumClapReturnLeft");
			m_RightArm1Hand->ChangeAnimation("ZakumClapReturnRight");
			m_LeftArm1Hand->AddWorldPos(-150.f, 0.f, 0.f);
			m_RightArm1Hand->AddWorldPos(150.f, 0.f, 0.f);
		}

		else if (!m_LeftArm1Hand || !m_LeftArm1Hand->IsActive())
		{
			m_RightArm1Hand->ChangeAnimation("ZakumClapReturnRight");
			m_RightArm1Hand->AddWorldPos(150.f, 0.f, 0.f);
		}

		else if (!m_RightArm1Hand || !m_RightArm1Hand->IsActive())
		{
			m_LeftArm1Hand->ChangeAnimation("ZakumClapReturnLeft");
			m_LeftArm1Hand->AddWorldPos(-150.f, 0.f, 0.f);
		}

		else
			int a = 3;

		break;
	case 1:
		if (m_LeftArm2Hand && m_LeftArm2Hand->IsActive() && m_RightArm2Hand && m_RightArm2Hand->IsActive())
		{
			m_LeftArm2Hand->ChangeAnimation("ZakumClapReturnLeft");
			m_RightArm2Hand->ChangeAnimation("ZakumClapReturnRight");
			m_LeftArm2Hand->AddWorldPos(-150.f, 0.f, 0.f);
			m_RightArm2Hand->AddWorldPos(150.f, 0.f, 0.f);
		}

		else if (!m_LeftArm2Hand || !m_LeftArm2Hand->IsActive())
		{
			m_RightArm2Hand->ChangeAnimation("ZakumClapReturnRight");
			m_RightArm2Hand->AddWorldPos(150.f, 0.f, 0.f);
		}

		else if (!m_RightArm2Hand || !m_RightArm2Hand->IsActive())
		{
			m_LeftArm2Hand->ChangeAnimation("ZakumClapReturnLeft");
			m_LeftArm2Hand->AddWorldPos(-150.f, 0.f, 0.f);
		}

		else
			int a = 3;


		break;
	case 2:
		if (m_LeftArm3Hand && m_LeftArm3Hand->IsActive() && m_RightArm3Hand && m_RightArm3Hand->IsActive())
		{
			m_LeftArm3Hand->ChangeAnimation("ZakumClapReturnLeft");
			m_RightArm3Hand->ChangeAnimation("ZakumClapReturnRight");
			m_LeftArm3Hand->AddWorldPos(-150.f, 0.f, 0.f);
			m_RightArm3Hand->AddWorldPos(150.f, 0.f, 0.f);
		}

		else if (!m_LeftArm3Hand || !m_LeftArm3Hand->IsActive())
		{
			m_RightArm3Hand->ChangeAnimation("ZakumClapReturnRight");
			m_RightArm3Hand->AddWorldPos(150.f, 0.f, 0.f);
		}

		else if (!m_RightArm3Hand || !m_RightArm3Hand->IsActive())
		{
			m_LeftArm3Hand->ChangeAnimation("ZakumClapReturnLeft");
			m_LeftArm3Hand->AddWorldPos(-150.f, 0.f, 0.f);
		}


		else
			int a = 3;

		break;
	}
}

void CZakumBody::ClapReturnArmOriginPos()
{
	// 왼쪽손이 ZakumClapReturnLeft 시퀀스를 끝내고 이 함수를 EndFunction으로 호출할지 오른쪽손이 ZakumClapReturnRight 시퀀스를 끝내고
	// 이 함수를 EndFunction으로 호출할지 모르지만, 둘중 하나가 호출했으면 나머지 하나는 호출하면 안되므로 m_HandMeet = true일때만 아래 코드들을
	// 실행하고, 이 함수 끝 부분에서는 m_HandMeet = false로 만들어서 왼손, 오른손 둘다 이 함수를 호출하지 않도록 하고 m_HandMeet은 
	// CreateClapEffect함수에서 true로 만들어 줄 수 있다
	if (!m_HandMeet)
		return;

	switch (m_ClapArmNum)
	{
	case 0:
		if (m_LeftArm1Hand)
		{
			m_LeftArm1Upper->SetWorldPos(m_vecPreviousUpperPos[0]);
			m_LeftArm1Upper->SetWorldRotation(m_vecPreviousUpperRot[0]);
			m_LeftArm1Hand->SetWorldPos(m_vecPreviousHandPos[0]);
			m_LeftArm1Hand->SetWorldRotation(m_vecPreviousHandRot[0]);
			m_LeftArm1Hand->ChangeAnimation("ZakumLeftHand");
			m_LeftArm1Hand->GetCurrentAnimation()->SetCurrentAnimationFrame(0);
			m_LeftArm1Hand->GetCurrentAnimation()->SetPlayScale(0.f);

			m_LeftArm1Upper->SetRender(true);
			m_LeftArm1Lower->SetRender(true);
			m_vecArmState[0] = Zakum_ArmState::Idle;

			m_LeftArm1HandAttackCollider->Enable(false);
			m_LeftArm1HandAttackCollider->SetRelativePos(0.f, -30.f, 0.f);

		}

		if (m_RightArm1Hand)
		{
			m_RightArm1Upper->SetWorldPos(m_vecPreviousUpperPos[4]);
			m_RightArm1Upper->SetWorldRotation(m_vecPreviousUpperRot[4]);
			m_RightArm1Hand->SetWorldPos(m_vecPreviousHandPos[4]);
			m_RightArm1Hand->SetWorldRotation(m_vecPreviousHandRot[4]);
			m_RightArm1Hand->ChangeAnimation("ZakumRightHand");
			m_RightArm1Hand->GetCurrentAnimation()->SetCurrentAnimationFrame(0);
			m_RightArm1Hand->GetCurrentAnimation()->SetPlayScale(0.f);

			m_RightArm1Upper->SetRender(true);
			m_RightArm1Lower->SetRender(true);
			m_vecArmState[4] = Zakum_ArmState::Idle;

			m_RightArm1HandAttackCollider->Enable(false);
			m_RightArm1HandAttackCollider->SetRelativePos(0.f, -30.f, 0.f);

		}

		break;
	case 1:
		if (m_LeftArm2Upper)
		{
			m_LeftArm2Upper->SetWorldPos(m_vecPreviousUpperPos[1]);
			m_LeftArm2Upper->SetWorldRotation(m_vecPreviousUpperRot[1]);
			m_LeftArm2Hand->SetWorldPos(m_vecPreviousHandPos[1]);
			m_LeftArm2Hand->SetWorldRotation(m_vecPreviousHandRot[1]);

			m_LeftArm2Hand->ChangeAnimation("ZakumLeftHand");
			m_LeftArm2Hand->GetCurrentAnimation()->SetCurrentAnimationFrame(1);
			m_LeftArm2Hand->GetCurrentAnimation()->SetPlayScale(0.f);

			m_LeftArm2Upper->SetRender(true);
			m_LeftArm2Lower->SetRender(true);

			m_vecArmState[1] = Zakum_ArmState::Idle;

			m_LeftArm2HandAttackCollider->Enable(false);
			m_LeftArm2HandAttackCollider->SetRelativePos(0.f, -30.f, 0.f);
		}

		if (m_RightArm2Upper)
		{
			m_RightArm2Upper->SetWorldPos(m_vecPreviousUpperPos[5]);
			m_RightArm2Upper->SetWorldRotation(m_vecPreviousUpperRot[5]);
			m_RightArm2Hand->SetWorldPos(m_vecPreviousHandPos[5]);
			m_RightArm2Hand->SetWorldRotation(m_vecPreviousHandRot[5]);

			m_RightArm2Hand->ChangeAnimation("ZakumRightHand");

			m_RightArm2Hand->GetCurrentAnimation()->SetCurrentAnimationFrame(1);

			m_RightArm2Hand->GetCurrentAnimation()->SetPlayScale(0.f);

			m_RightArm2Upper->SetRender(true);
			m_RightArm2Lower->SetRender(true);


			m_vecArmState[5] = Zakum_ArmState::Idle;

			m_RightArm2HandAttackCollider->Enable(false);
			m_RightArm2HandAttackCollider->SetRelativePos(0.f, -30.f, 0.f);
		}

		break;
	case 2:
		if (m_LeftArm3Upper)
		{
			m_LeftArm3Upper->SetWorldPos(m_vecPreviousUpperPos[2]);
			m_LeftArm3Upper->SetWorldRotation(m_vecPreviousUpperRot[2]);
			m_LeftArm3Hand->SetWorldPos(m_vecPreviousHandPos[2]);
			m_LeftArm3Hand->SetWorldRotation(m_vecPreviousHandRot[2]);

			m_LeftArm3Hand->ChangeAnimation("ZakumLeftHand");
			m_LeftArm3Hand->GetCurrentAnimation()->SetCurrentAnimationFrame(2);
			m_LeftArm3Hand->GetCurrentAnimation()->SetPlayScale(0.f);
			m_LeftArm3Upper->SetRender(true);
			m_LeftArm3Lower->SetRender(true);

			m_vecArmState[2] = Zakum_ArmState::Idle;

			m_LeftArm3HandAttackCollider->Enable(false);
			m_LeftArm3HandAttackCollider->SetRelativePos(0.f, -30.f, 0.f);
		}

		if (m_RightArm3Upper)
		{
			m_RightArm3Upper->SetWorldPos(m_vecPreviousUpperPos[6]);
			m_RightArm3Upper->SetWorldRotation(m_vecPreviousUpperRot[6]);
			m_RightArm3Hand->SetWorldPos(m_vecPreviousHandPos[6]);
			m_RightArm3Hand->SetWorldRotation(m_vecPreviousHandRot[6]);

			m_RightArm3Hand->ChangeAnimation("ZakumRightHand");
			m_RightArm3Hand->GetCurrentAnimation()->SetCurrentAnimationFrame(2);
			m_RightArm3Hand->GetCurrentAnimation()->SetPlayScale(0.f);

			m_RightArm3Upper->SetRender(true);
			m_RightArm3Lower->SetRender(true);

			m_vecArmState[6] = Zakum_ArmState::Idle;

			m_RightArm3HandAttackCollider->Enable(false);
			m_RightArm3HandAttackCollider->SetRelativePos(0.f, -30.f, 0.f);
		}

		break;
	}

	++m_ClapCount;
	m_vecAccClapStretchRot[m_ClapArmNum] = 0.f;
	m_ClapArmNum = -1;
	m_AccTime = 0.f;
	m_HandMeet = false;
	m_ReturnClapPosComplete = true;

	if (m_ClapCount == m_ClapCountLimit)
	{
		CPlayer2D* Player = (CPlayer2D*)m_Scene->GetPlayerObject();

		for (int i = 0; i < 4; ++i)
		{
			char FlameName[256] = {};
			sprintf_s(FlameName, "Flame%d", i);
			std::string StrFlameName = FlameName;
			CZakumFlame* Flame = (CZakumFlame*)m_Scene->FindObject(StrFlameName);

			if (Flame)
				Flame->Destroy();
		}

		m_ZakumState = Zakum_State::Idle;
		m_ClapCount = 0;
	}
}

void CZakumBody::CollisionBeginCallback(const CollisionResult& Result)
{
	CGameObject* DestObj = Result.Dest->GetGameObject();

	CColliderComponent* SrcComp = Result.Src;

	if (DestObj->GetTypeID() == typeid(CPlayer2D).hash_code())
	{
		// Scene전환중일때는 자쿰에 맞으면 안됨
		if (CSceneManager::GetInst()->GetNextScene())
			return;

		CPlayer2D* Player = ((CPlayer2D*)DestObj);
		
		// 자쿰이 플레이어에게 공격 할때
		if (SrcComp->GetName().find("AttackCollider") != std::string::npos)
		{
			if (Player->IsDead())
				return;

			if (m_ZakumState == Zakum_State::Clap)
			{
				PlayerInfo Info = ((CPlayer2D*)DestObj)->GetInfo();
				int CurrentHP = Info.HP;

				Player->SetDamage((float)m_ZakumInfo.ClapAttack);
			}

			else
			{
				// 여러개의 손바닥이 내려칠때 하나의 손바닥에 충돌하고나면 그 턴에는 땅에 닿아있는 다른 손바닥 충돌체랑 충돌해도 충돌하면 안됨 
				Player->SetDamage((float)m_ZakumInfo.SmashAttack);
			}

			m_LeftArm1HandAttackCollider->Enable(false);
			m_LeftArm2HandAttackCollider->Enable(false);
			m_LeftArm3HandAttackCollider->Enable(false);
			m_LeftArm4HandAttackCollider->Enable(false);
			m_RightArm1HandAttackCollider->Enable(false);
			m_RightArm2HandAttackCollider->Enable(false);
			m_RightArm3HandAttackCollider->Enable(false);
			m_RightArm4HandAttackCollider->Enable(false);
		}
	}
}

void CZakumBody::SetDamage(float Damage, bool Critical)
{
	switch (m_CurrentCollisionArmID)
	{
	case 0:
		if (!m_LeftArm1Root || !m_LeftArm1Root->IsActive())
			return;

		m_ZakumInfo.LeftArm1HP -= (int)Damage;
		if (m_ZakumInfo.LeftArm1HP <= 0)
		{
			--m_AliveArmCount;
			m_LeftArm1Root->Destroy();

			m_vecAccUpHandRot[0] = 0.f;
			m_vecArmState[0] = Zakum_ArmState::Idle;
			auto iter = std::find(m_ListAttackArmNum.begin(), m_ListAttackArmNum.end(), 0);
			auto iterEnd = m_ListAttackArmNum.end();
			if(iter != iterEnd)
				m_ListAttackArmNum.erase(iter);

			if (!m_LeftArm1HandDefenseCollider->IsEnable())
				return;

			m_LeftArm1HandDefenseCollider->Enable(false);
			m_IsChanging = true;
		}
		break;
	case 1:
		if (!m_LeftArm2Root || !m_LeftArm2Root->IsActive())
			return;

		m_ZakumInfo.LeftArm2HP -= (int)Damage;
		if (m_ZakumInfo.LeftArm2HP <= 0)
		{
			--m_AliveArmCount;
			m_LeftArm2Root->Destroy();

			m_vecAccUpHandRot[1] = 0.f;
			m_vecArmState[1] = Zakum_ArmState::Idle;
			auto iter = std::find(m_ListAttackArmNum.begin(), m_ListAttackArmNum.end(), 1);
			auto iterEnd = m_ListAttackArmNum.end();
			if (iter != iterEnd)
				m_ListAttackArmNum.erase(iter);

			if (!m_LeftArm2HandDefenseCollider->IsEnable())
				return;

			m_LeftArm2HandDefenseCollider->Enable(false);
			m_IsChanging = true;
		}
		break;
	case 2:
		if (!m_LeftArm3Root || !m_LeftArm3Root->IsActive())
			return;

		m_ZakumInfo.LeftArm3HP -= (int)Damage;
		if (m_ZakumInfo.LeftArm3HP <= 0)
		{
			--m_AliveArmCount;
			m_LeftArm3Root->Destroy();

			m_vecAccUpHandRot[2] = 0.f;
			m_vecArmState[2] = Zakum_ArmState::Idle;
			auto iter = std::find(m_ListAttackArmNum.begin(), m_ListAttackArmNum.end(), 2);
			auto iterEnd = m_ListAttackArmNum.end();
			if (iter != iterEnd)
				m_ListAttackArmNum.erase(iter);

			if (!m_LeftArm3HandDefenseCollider->IsEnable())
				return;

			m_LeftArm3HandDefenseCollider->Enable(false);
			m_IsChanging = true;
		}
		break;
	case 3:
		if (!m_LeftArm4Root || !m_LeftArm4Root->IsActive())
			return;

		m_ZakumInfo.LeftArm4HP -= (int)Damage;
		if (m_ZakumInfo.LeftArm4HP <= 0)
		{
			--m_AliveArmCount;
			m_LeftArm4Root->Destroy();

			m_vecAccUpHandRot[3] = 0.f;
			m_vecArmState[3] = Zakum_ArmState::Idle;
			auto iter = std::find(m_ListAttackArmNum.begin(), m_ListAttackArmNum.end(), 3);
			auto iterEnd = m_ListAttackArmNum.end();
			if (iter != iterEnd)
				m_ListAttackArmNum.erase(iter);

			if (!m_LeftArm4HandDefenseCollider->IsEnable())
				return;

			m_LeftArm4HandDefenseCollider->Enable(false);
			m_IsChanging = true;
		}
		break;
	case 4:
		if (!m_RightArm1Root || !m_RightArm1Root->IsActive())
			return;

		m_ZakumInfo.RightArm1HP -= (int)Damage;
		if (m_ZakumInfo.RightArm1HP <= 0)
		{
			--m_AliveArmCount;
			m_RightArm1Root->Destroy();

			m_vecAccUpHandRot[4] = 0.f;
			m_vecArmState[4] = Zakum_ArmState::Idle;
			auto iter = std::find(m_ListAttackArmNum.begin(), m_ListAttackArmNum.end(), 4);
			auto iterEnd = m_ListAttackArmNum.end();
			if (iter != iterEnd)
				m_ListAttackArmNum.erase(iter);

			if (!m_RightArm1HandDefenseCollider->IsEnable())
				return;

			m_RightArm1HandDefenseCollider->Enable(false);
			m_IsChanging = true;
		}
		break;
	case 5:
		if (!m_RightArm2Root || !m_RightArm2Root->IsActive())
			return;

		m_ZakumInfo.RightArm2HP -= (int)Damage;
		if (m_ZakumInfo.RightArm2HP <= 0)
		{
			--m_AliveArmCount;
			m_RightArm2Root->Destroy();

			m_vecAccUpHandRot[5] = 0.f;
			m_vecArmState[5] = Zakum_ArmState::Idle;
			auto iter = std::find(m_ListAttackArmNum.begin(), m_ListAttackArmNum.end(), 5);
			auto iterEnd = m_ListAttackArmNum.end();
			if (iter != iterEnd)
				m_ListAttackArmNum.erase(iter);

			if (!m_RightArm2HandDefenseCollider->IsEnable())
				return;

			m_RightArm2HandDefenseCollider->Enable(false);
			m_IsChanging = true;
		}
		break;
	case 6:
		if (!m_RightArm3Root || !m_RightArm3Root->IsActive())
			return;

		m_ZakumInfo.RightArm3HP -= (int)Damage;
		if (m_ZakumInfo.RightArm3HP <= 0)
		{
			--m_AliveArmCount;
			m_RightArm3Root->Destroy();

			m_vecAccUpHandRot[6] = 0.f;
			m_vecArmState[6] = Zakum_ArmState::Idle;
			auto iter = std::find(m_ListAttackArmNum.begin(), m_ListAttackArmNum.end(), 6);
			auto iterEnd = m_ListAttackArmNum.end();
			if (iter != iterEnd)
				m_ListAttackArmNum.erase(iter);

			if (!m_RightArm3HandDefenseCollider->IsEnable())
				return;

			m_RightArm3HandDefenseCollider->Enable(false);
			m_IsChanging = true;
		}
		break;
	case 7:
		if (!m_RightArm4Root || !m_RightArm4Root->IsActive())
			return;

		m_ZakumInfo.RightArm4HP -= (int)Damage;
		if (m_ZakumInfo.RightArm4HP <= 0)
		{
			--m_AliveArmCount;
			m_RightArm4Root->Destroy();

			m_vecAccUpHandRot[7] = 0.f;
			m_vecArmState[7] = Zakum_ArmState::Idle;
			auto iter = std::find(m_ListAttackArmNum.begin(), m_ListAttackArmNum.end(), 7);
			auto iterEnd = m_ListAttackArmNum.end();
			if (iter != iterEnd)
				m_ListAttackArmNum.erase(iter);

			if (!m_RightArm4HandDefenseCollider->IsEnable())
				return;

			m_RightArm4HandDefenseCollider->Enable(false);
			m_IsChanging = true;
		}
		break;
	case -1:
		m_ZakumInfo.BodyHP -= (int)Damage;
		CBossInfoWindow* BossInfo = (CBossInfoWindow*)m_Scene->GetViewport()->FindWidgetWindow<CBossInfoWindow>("BossInfoWindow");

		float Percent = (float)m_ZakumInfo.BodyHP / m_ZakumInfo.BodyHPMax;

		BossInfo->BossHPPercent(Percent);

		if (m_ZakumInfo.BodyHP <= 0)
		{
			Die();
			
			std::vector<CClapWarning*> vecClapWarning;
			m_Scene->FindObjectByType<CClapWarning>(vecClapWarning);

			size_t Count = vecClapWarning.size();

			for (size_t i = 0; i < Count; ++i)
			{
				vecClapWarning[i]->SetWarningPointOwner(nullptr);
			}

			std::vector<CZakumAttackWarningSpot*> vecAttackWarning;
			m_Scene->FindObjectByType<CZakumAttackWarningSpot>(vecAttackWarning);

			Count = vecAttackWarning.size();

			for (size_t i = 0; i < Count; ++i)
			{
				vecAttackWarning[i]->SetWarningPointOwner(nullptr);
			}

			std::vector<CZakumHandSmashLight*> vecHandSmashLight;
			m_Scene->FindObjectByType<CZakumHandSmashLight>(vecHandSmashLight);

			Count = vecHandSmashLight.size();

			for (size_t i = 0; i < Count; ++i)
			{
				vecHandSmashLight[i]->SetSmashLightOwner(nullptr);
			}

			m_LeftArm1Hand = nullptr;
			m_LeftArm2Hand = nullptr;
			m_LeftArm3Hand = nullptr;
			m_LeftArm4Hand = nullptr;
			m_RightArm1Hand = nullptr;
			m_RightArm2Hand = nullptr;
			m_RightArm3Hand = nullptr;
			m_RightArm4Hand = nullptr;

			if (m_Body && !m_Body->IsEnable())
				return;

			m_Body->Enable(false);
			m_IsChanging = true;

			CBill* Bill = DropBill();
			Bill->AddWorldPos(-35.f, 0.f, 0.f);

			Bill = DropBill();

			Bill = DropBill();
			Bill->AddWorldPos(35.f, 0.f, 0.f);

		}

		else if (m_ZakumInfo.BodyHP <= m_ZakumInfo.BodyHPMax * 0.4f)
		{
			m_BodySprite->ChangeAnimation("ZakumDestroyIdle");
		}

		break;
	}

	PushDamageFont(Damage, Critical);

}

void CZakumBody::PushDamageFont(float Damage, bool Critical)
{
	switch (m_CurrentCollisionArmID)
	{
	case 0:
		((CDamageFont*)m_LeftArm1HandDamageWidgetComponent->GetWidgetWindow())->PushDamageNumber((int)Damage, Critical);
		break;
	case 1:
		((CDamageFont*)m_LeftArm2HandDamageWidgetComponent->GetWidgetWindow())->PushDamageNumber((int)Damage, Critical);
		break;
	case 2:
		((CDamageFont*)m_LeftArm3HandDamageWidgetComponent->GetWidgetWindow())->PushDamageNumber((int)Damage, Critical);
		break;
	case 3:
		((CDamageFont*)m_LeftArm4HandDamageWidgetComponent->GetWidgetWindow())->PushDamageNumber((int)Damage, Critical);
		break;
	case 4:
		((CDamageFont*)m_RightArm1HandDamageWidgetComponent->GetWidgetWindow())->PushDamageNumber((int)Damage, Critical);
		break;
	case 5:
		((CDamageFont*)m_RightArm2HandDamageWidgetComponent->GetWidgetWindow())->PushDamageNumber((int)Damage, Critical);
		break;
	case 6:
		((CDamageFont*)m_RightArm3HandDamageWidgetComponent->GetWidgetWindow())->PushDamageNumber((int)Damage, Critical);
		break;
	case 7:
		((CDamageFont*)m_RightArm4HandDamageWidgetComponent->GetWidgetWindow())->PushDamageNumber((int)Damage, Critical);
		break;
	case -1:
		((CDamageFont*)m_DamageWidgetComponent->GetWidgetWindow())->PushDamageNumber((int)Damage, Critical);
		break;
	}
}


void CZakumBody::ArmSmash(int ArmID)
{
	bool Smash = true;

	switch (ArmID)
	{
	case 0:
		if (!m_LeftArm1Hand || !m_LeftArm1Hand->IsActive())
		{
			m_ZakumState = Zakum_State::Idle;
			m_AccTime = 0.f;

			m_vecArmState[ArmID] = Zakum_ArmState::Idle;
			Smash = false;
		}
		break;
	case 1:
		if (!m_LeftArm2Hand || !m_LeftArm2Hand->IsActive())
		{
			m_ZakumState = Zakum_State::Idle;
			m_AccTime = 0.f;

			m_vecArmState[ArmID] = Zakum_ArmState::Idle;
			Smash = false;
		}
		break;
	case 2:
		if (!m_LeftArm3Hand || !m_LeftArm3Hand->IsActive())
		{
			m_ZakumState = Zakum_State::Idle;
			m_AccTime = 0.f;

			m_vecArmState[ArmID] = Zakum_ArmState::Idle;
			Smash = false;
		}
		break;
	case 3:
		if (!m_LeftArm4Hand || !m_LeftArm4Hand->IsActive())
		{
			m_ZakumState = Zakum_State::Idle;
			m_AccTime = 0.f;

			m_vecArmState[ArmID] = Zakum_ArmState::Idle;
			Smash = false;
		}
		break;
	case 4:
		if (!m_RightArm1Hand || !m_RightArm1Hand->IsActive())
		{
			m_ZakumState = Zakum_State::Idle;
			m_AccTime = 0.f;

			m_vecArmState[ArmID] = Zakum_ArmState::Idle;
			Smash = false;
		}
		break;
	case 5:
		if (!m_RightArm2Hand || !m_RightArm2Hand->IsActive())
		{
			m_ZakumState = Zakum_State::Idle;
			m_AccTime = 0.f;

			m_vecArmState[ArmID] = Zakum_ArmState::Idle;
			Smash = false;
		}
		break;
	case 6:
		if (!m_RightArm3Hand || !m_RightArm3Hand->IsActive())
		{
			m_ZakumState = Zakum_State::Idle;
			m_AccTime = 0.f;

			m_vecArmState[ArmID] = Zakum_ArmState::Idle;
			Smash = false;
		}
		break;
	case 7:
		if (!m_RightArm4Hand || !m_RightArm4Hand->IsActive())
		{
			m_ZakumState = Zakum_State::Idle;
			m_AccTime = 0.f;

			m_vecArmState[ArmID] = Zakum_ArmState::Idle;
			Smash = false;
		}
		break;
	}

	if (!Smash)
		return;


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

		m_LeftArm1HandAttackCollider->Enable(true);
		m_LeftArm1HandAttackCollider->SetWorldScale(110.f, 40.f, 1.f);
		m_LeftArm1HandAttackCollider->SetRelativePos(0.f, -30.f, 0.f);
		break;
	case 1:
		m_LeftArm2Lower->SetRender(false);
		m_LeftArm2Upper->SetRender(false);
		//m_LeftArm2Hand->SetPivot(0.5f, 0.5f, 0.f);
		m_LeftArm2Hand->SetWorldPos(Vector3(m_vecAttackWarningPos[1].x, m_vecAttackWarningPos[1].y + 40.f, m_vecAttackWarningPos[1].z + 10.f));
		m_LeftArm2Hand->SetWorldRotation(0.f, 0.f, 0.f);
		m_LeftArm2Hand->ChangeAnimation("ZakumLeftHandSmash");

		Light->SetLightSpriteTexture("LeftLight2", TEXT("Monster/LeftHandSmashLightEffect2.png"));

		m_LeftArm2HandAttackCollider->Enable(true);
		m_LeftArm2HandAttackCollider->SetWorldScale(110.f, 40.f, 1.f);
		m_LeftArm2HandAttackCollider->SetRelativePos(0.f, -30.f, 0.f);
		break;
	case 2:
		m_LeftArm3Lower->SetRender(false);
		m_LeftArm3Upper->SetRender(false);
		//m_LeftArm3Hand->SetPivot(0.5f, 0.5f, 0.f);
		m_LeftArm3Hand->SetWorldPos(Vector3(m_vecAttackWarningPos[2].x, m_vecAttackWarningPos[2].y + 40.f, m_vecAttackWarningPos[2].z + 10.f));
		m_LeftArm3Hand->SetWorldRotation(0.f, 0.f, 0.f);
		m_LeftArm3Hand->ChangeAnimation("ZakumLeftHandSmash2");

		Light->SetLightSpriteTexture("LeftLight1", TEXT("Monster/LeftHandSmashLightEffect1.png"));

		m_LeftArm3HandAttackCollider->Enable(true);
		m_LeftArm3HandAttackCollider->SetWorldScale(110.f, 40.f, 1.f);
		m_LeftArm3HandAttackCollider->SetRelativePos(0.f, -30.f, 0.f);
		break;
	case 3:
		m_LeftArm4Lower->SetRender(false);
		m_LeftArm4Upper->SetRender(false);
		//m_LeftArm4Hand->SetPivot(0.5f, 0.5f, 0.f);
		m_LeftArm4Hand->SetWorldPos(Vector3(m_vecAttackWarningPos[3].x, m_vecAttackWarningPos[3].y + 40.f, m_vecAttackWarningPos[3].z + 10.f));
		m_LeftArm4Hand->SetWorldRotation(0.f, 0.f, 0.f);
		m_LeftArm4Hand->ChangeAnimation("ZakumLeftHandSmash2");

		Light->SetLightSpriteTexture("LeftLight1", TEXT("Monster/LeftHandSmashLightEffect1.png"));

		m_LeftArm4HandAttackCollider->Enable(true);
		m_LeftArm4HandAttackCollider->SetWorldScale(110.f, 40.f, 1.f);
		m_LeftArm4HandAttackCollider->SetRelativePos(0.f, -30.f, 0.f);
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
		m_RightArm1HandAttackCollider->Enable(true);
		m_RightArm1HandAttackCollider->SetWorldScale(110.f, 40.f, 1.f);
		m_RightArm1HandAttackCollider->SetRelativePos(0.f, -30.f, 0.f);

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

		m_RightArm2HandAttackCollider->Enable(true);
		m_RightArm2HandAttackCollider->SetWorldScale(110.f, 40.f, 1.f);
		m_RightArm2HandAttackCollider->SetRelativePos(0.f, -30.f, 0.f);
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

		m_RightArm3HandAttackCollider->Enable(true);
		m_RightArm3HandAttackCollider->SetWorldScale(110.f, 40.f, 1.f);
		m_RightArm3HandAttackCollider->SetRelativePos(0.f, -30.f, 0.f);
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
		m_RightArm4HandAttackCollider->Enable(true);
		m_RightArm4HandAttackCollider->SetWorldScale(110.f, 40.f, 1.f);
		m_RightArm4HandAttackCollider->SetRelativePos(0.f, -30.f, 0.f);
		break;
	}
}

void CZakumBody::SmashReturnArm(int ArmID)
{
	switch (ArmID)
	{
	case 0:
		if (m_LeftArm1Hand && m_LeftArm1Hand->IsActive())
			m_LeftArm1Hand->ChangeAnimation("ZakumArmReturnLeft1");
		break;
	case 1:
		if (m_LeftArm2Hand && m_LeftArm2Hand->IsActive())
			m_LeftArm2Hand->ChangeAnimation("ZakumArmReturnLeft1");
		break;
	case 2:
		if (m_LeftArm3Hand && m_LeftArm3Hand->IsActive())
			m_LeftArm3Hand->ChangeAnimation("ZakumArmReturnLeft2");
		break;
	case 3:
		if (m_LeftArm4Hand && m_LeftArm4Hand->IsActive())
			m_LeftArm4Hand->ChangeAnimation("ZakumArmReturnLeft2");
		break;
	case 4:
		if (m_RightArm1Hand && m_RightArm1Hand->IsActive())
			m_RightArm1Hand->ChangeAnimation("ZakumArmReturnLeft2");
		//m_RightArm1Hand->Flip();
		break;
	case 5:
		if (m_RightArm2Hand && m_RightArm2Hand->IsActive())
			m_RightArm2Hand->ChangeAnimation("ZakumArmReturnLeft2");
		//m_RightArm2Hand->Flip();
		break;
	case 6:
		if (m_RightArm3Hand && m_RightArm3Hand->IsActive())
			m_RightArm3Hand->ChangeAnimation("ZakumArmReturnLeft1");
		//m_RightArm3Hand->Flip();
		break;
	case 7:
		if (m_RightArm4Hand && m_RightArm4Hand->IsActive())
			m_RightArm4Hand->ChangeAnimation("ZakumArmReturnLeft1");
		//m_RightArm4Hand->Flip();
		break;
	}

}

void CZakumBody::SmashReturnArmOriginPos()
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

			m_LeftArm1HandAttackCollider->Enable(false);
			m_LeftArm1HandAttackCollider->SetRelativePos(0.f, -30.f, 0.f);

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
			
			m_LeftArm2HandAttackCollider->Enable(false);
			m_LeftArm2HandAttackCollider->SetRelativePos(0.f, -30.f, 0.f);
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

			m_LeftArm3HandAttackCollider->Enable(false);
			m_LeftArm3HandAttackCollider->SetRelativePos(0.f, -30.f, 0.f);
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

			m_LeftArm4HandAttackCollider->Enable(false);
			m_LeftArm4HandAttackCollider->SetRelativePos(0.f, -30.f, 0.f);
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

			m_RightArm1HandAttackCollider->Enable(false);
			m_RightArm1HandAttackCollider->SetRelativePos(0.f, -30.f, 0.f);
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

			m_RightArm2HandAttackCollider->Enable(false);
			m_RightArm2HandAttackCollider->SetRelativePos(0.f, -30.f, 0.f);
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

			m_RightArm3HandAttackCollider->Enable(false);
			m_RightArm3HandAttackCollider->SetRelativePos(0.f, -30.f, 0.f);
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

			m_RightArm4HandAttackCollider->Enable(false);
			m_RightArm4HandAttackCollider->SetRelativePos(0.f, -30.f, 0.f);
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
				char FlameName[256] = {};
				sprintf_s(FlameName, "Flame%d", i);
				std::string StrFlameName = FlameName;
				CZakumFlame* Flame = m_Scene->CreateGameObject<CZakumFlame>(StrFlameName);

				if (i != 1)
					Flame->GetRootComponent()->DeleteChild("Body");

				Flame->SetWorldPos(260.f + 276.f * i, 205.f, 220.f);
				Flame->SetID(i);
			}

			m_SmashCount = 0;
			m_ZakumState = Zakum_State::Clap;
			//m_vecAccRotation.clear();
			m_vecAccUpHandRot.clear();
			//m_vecAccRotation.resize(8);
			m_vecAccUpHandRot.resize(8);
		}
	}
}
