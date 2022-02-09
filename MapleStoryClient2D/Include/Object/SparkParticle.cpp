
#include "SparkParticle.h"
#include "Component/ParticleComponent.h"

CSparkParticle::CSparkParticle()
{
	SetTypeID<CSparkParticle>();
}

CSparkParticle::CSparkParticle(const CSparkParticle& obj) :
	CGameObject(obj)
{
	m_Particle = (CParticleComponent*)FindComponent("Particle");
}

CSparkParticle::~CSparkParticle()
{
}

bool CSparkParticle::Init()
{
	m_Particle = CreateComponent<CParticleComponent>("Spark");

	SetRootComponent(m_Particle);

	//m_Particle->SetRelativePos(200.f, 50.f, 0.f);
	m_Particle->SetSpawnTimeMax(0.01f);

	//SetLifeSpan(0.8f);
	//m_Particle->SetSpawnTime(0.1f);

	return true;
}

CSparkParticle* CSparkParticle::Clone()
{
	return new CSparkParticle(*this);
}
