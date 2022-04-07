#pragma once

#include "GameObject/GameObject.h"

class CDustParticle :
    public CGameObject
{
    friend class CScene;

protected:
    CDustParticle();
    CDustParticle(const CDustParticle& obj);
    virtual ~CDustParticle();

private:
    CSharedPtr<class CParticleComponent>    m_Particle;

public:
    virtual bool Init();
    virtual CDustParticle* Clone();
};

