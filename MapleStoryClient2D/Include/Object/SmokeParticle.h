#pragma once

#include "GameObject/GameObject.h"
#include "Component/ParticleComponent.h"

class CSmokeParticle :
    public CGameObject
{
    friend class CScene;

protected:
    CSmokeParticle();
    CSmokeParticle(const CSmokeParticle& obj);
    virtual ~CSmokeParticle();

private:
    CSharedPtr<class CParticleComponent>    m_Particle;

public:
    void SetParticle(const std::string& Name)
    {
        m_Particle->SetParticle(Name);
    }
    
public:
    virtual bool Init();
    virtual CSmokeParticle* Clone();
};

