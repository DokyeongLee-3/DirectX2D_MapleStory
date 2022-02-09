#pragma once

#include "GameObject/GameObject.h"
#include "Component/ParticleComponent.h"

class CSparkParticle :
    public CGameObject
{
    friend class CScene;

protected:
    CSparkParticle();
    CSparkParticle(const CSparkParticle& obj);
    virtual ~CSparkParticle();

private:
    CSharedPtr<class CParticleComponent>    m_Particle;

public:
    void SetParticle(const std::string& Name)
    {
        m_Particle->SetParticle(Name);
    }

public:
    virtual bool Init();
    virtual CSparkParticle* Clone();
};

