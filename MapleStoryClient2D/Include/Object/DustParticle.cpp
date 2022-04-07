
#include "DustParticle.h"
#include "Component/ParticleComponent.h"

CDustParticle::CDustParticle()
{
	SetTypeID<CDustParticle>();
}

CDustParticle::CDustParticle(const CDustParticle& obj) :
	CGameObject(obj)
{
	m_Particle = (CParticleComponent*)FindComponent("Particle");
}

CDustParticle::~CDustParticle()
{
}

bool CDustParticle::Init()
{
	m_Particle = CreateComponent<CParticleComponent>("Particle");

	SetRootComponent(m_Particle);

	m_Particle->SetRelativePos(300.f, 150.f, 0.f);

	m_Particle->SetParticle("Dust");

	//SetLifeSpan(5.f);
	m_Particle->SetSpawnTime(0.01f);

	return true;
}

CDustParticle* CDustParticle::Clone()
{
	return new CDustParticle(*this);
}
