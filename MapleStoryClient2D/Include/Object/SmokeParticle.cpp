
#include "SmokeParticle.h"
#include "Component/ParticleComponent.h"

CSmokeParticle::CSmokeParticle()
{
	SetTypeID<CSmokeParticle>();
}

CSmokeParticle::CSmokeParticle(const CSmokeParticle& obj) :
	CGameObject(obj)
{
	m_Particle = (CParticleComponent*)FindComponent("Particle");
}

CSmokeParticle::~CSmokeParticle()
{
}

bool CSmokeParticle::Init()
{
	m_Particle = CreateComponent<CParticleComponent>("Smoke");

	SetRootComponent(m_Particle);

	//m_Particle->SetRelativePos(200.f, 50.f, 0.f);
	m_Particle->SetSpawnTimeMax(0.01f);
	
	//SetLifeSpan(0.8f);
	//m_Particle->SetSpawnTime(0.1f);

	return true;
}

CSmokeParticle* CSmokeParticle::Clone()
{
	return new CSmokeParticle(*this);
}
