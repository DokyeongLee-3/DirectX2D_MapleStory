
#include "DeathSideAnimation.h"
#include "Render/RenderManager.h"
#include "Scene/Scene.h"
#include "../Object/Player2D.h"
#include "Scene/SceneManager.h"

CDeathSideAnimation::CDeathSideAnimation()
{
}

CDeathSideAnimation::CDeathSideAnimation(const CDeathSideAnimation& Anim)	:
	CAnimationSequence2DInstance(Anim)
{
}

CDeathSideAnimation::~CDeathSideAnimation()
{
}

bool CDeathSideAnimation::Init()
{
	if (!CAnimationSequence2DInstance::Init())
		return false;

	AddAnimation(TEXT("DeathSide.sqc"), ANIMATION_PATH, "DeathSide", false, 1.5f);

	SetEndFunction<CDeathSideAnimation>("DeathSide", this, &CDeathSideAnimation::EndSkillEffect);
	
	
	CRenderManager::GetInst()->SetFadeInLimit(0.3f);
	CSceneManager::GetInst()->ClearFadeInEndCallback();
	CRenderManager::GetInst()->SetStartFadeIn(true);

	m_OriginFadeInSpeed = CRenderManager::GetInst()->GetFadeInSpeed();
	m_OriginFadeOutSpeed = CRenderManager::GetInst()->GetFadeOutSpeed();

	CRenderManager::GetInst()->SetFadeInSpeed(2.f);
	CRenderManager::GetInst()->SetFadeOutSpeed(2.f);

	return true;
}

CDeathSideAnimation* CDeathSideAnimation::Clone()
{
	return new CDeathSideAnimation(*this);
}

void CDeathSideAnimation::EndSkillEffect()
{
	CPlayer2D* Player = (CPlayer2D*)m_Scene->GetPlayerObject();

	Player->ReturnFadeApply();

	CRenderManager::GetInst()->SetStartFadeOut(true);
	CRenderManager::GetInst()->SetFadeInLimit(0.f);

	CSceneManager::GetInst()->SetFadeOutEndCallback<CDeathSideAnimation>(this, &CDeathSideAnimation::RestoreFadeInOutSpeed);

	SetCurrentAnimation(nullptr);
}

void CDeathSideAnimation::RestoreFadeInOutSpeed()
{
	CRenderManager::GetInst()->SetFadeInSpeed(m_OriginFadeInSpeed);
	CRenderManager::GetInst()->SetFadeOutSpeed(m_OriginFadeOutSpeed);

	m_Owner->GetGameObject()->Destroy();

	CSceneManager::GetInst()->ClearFadeOutEndCallback();
}
