
#include "RainParticle.h"
#include "Component/ParticleComponent.h"

CRainParticle::CRainParticle()
{
	SetTypeID<CRainParticle>();
}

CRainParticle::CRainParticle(const CRainParticle& obj) :
	CGameObject(obj)
{
	m_Particle = (CParticleComponent*)FindComponent("Particle");
}

CRainParticle::~CRainParticle()
{
}

bool CRainParticle::Init()
{
	m_Particle = CreateComponent<CParticleComponent>("Rain");

	SetRootComponent(m_Particle);

	m_Particle->SetRelativePos(300.f, 400.f, 0.f);
	m_Particle->SetSpawnTimeMax(1.f);

	//SetLifeSpan(0.8f);
	//m_Particle->SetSpawnTime(0.1f);

	return true;
}

CRainParticle* CRainParticle::Clone()
{
	return new CRainParticle(*this);
}
