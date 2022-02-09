#pragma once

#include "GameObject/GameObject.h"
#include "Component/ParticleComponent.h"

class CRainParticle :
    public CGameObject
{
    friend class CScene;

protected:
    CRainParticle();
    CRainParticle(const CRainParticle& obj);
    virtual ~CRainParticle();

private:
    CSharedPtr<class CParticleComponent>    m_Particle;

public:
    void SetParticle(const std::string& Name)
    {
        m_Particle->SetParticle(Name);
    }

public:
    virtual bool Init();
    virtual CRainParticle* Clone();
};

