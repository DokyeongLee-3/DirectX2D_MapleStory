
#include "ParticleObject.h"
#include "Component/ParticleComponent.h"
#include "../Window/ParticleSystemWindow.h"
#include "IMGUIManager.h"

CParticleObject::CParticleObject()
{
	SetTypeID<CParticleObject>();
}

CParticleObject::CParticleObject(const CParticleObject& obj) :
	CGameObject(obj)
{
	m_Particle = (CParticleComponent*)FindComponent("Particle");
}

CParticleObject::~CParticleObject()
{
}

bool CParticleObject::Init()
{
	m_Particle = CreateComponent<CParticleComponent>("Particle");

	SetRootComponent(m_Particle);

	return true;
}

void CParticleObject::Start()
{
	CGameObject::Start();

	CParticleSystemWindow* ParticleWindow = (CParticleSystemWindow*)CIMGUIManager::GetInst()->FindIMGUIWindow("ParticleSystemWindow");

	if (ParticleWindow)
	{
		Select_ParticleMaterial SelectMaterial = ParticleWindow->GetSelectParticle();

		switch (SelectMaterial)
		{
		case Select_ParticleMaterial::Rain:
			SetParticle("Rain");
			m_Particle->SetSpawnTimeMax(0.08f);
			break;
		case Select_ParticleMaterial::Dust:
			SetParticle("Dust");
			m_Particle->SetSpawnTimeMax(0.1f);
			break;
		case Select_ParticleMaterial::Smoke:
			SetParticle("Smoke");
			m_Particle->SetSpawnTimeMax(0.1f);
			break;
		}
	}

	if(m_Particle)
		m_Particle->SetRelativePos(100.f, 500.f, 0.f);

	//m_Particle->SetParticle("Rain");

	//SetLifeSpan(5.f);
}

CParticleObject* CParticleObject::Clone()
{
	return new CParticleObject(*this);
}

void CParticleObject::SetParticle(const std::string& Name)
{
	m_Particle->SetParticle(Name);
}
