#pragma once

#include "GameObject/GameObject.h"

class CParticleObject :
    public CGameObject
{
    friend class CScene;
    friend class CEditorManager;

protected:
    CParticleObject();
    CParticleObject(const CParticleObject& obj);
    virtual ~CParticleObject();

private:
    CSharedPtr<class CParticleComponent>    m_Particle;

public:
    virtual bool Init();
    virtual void Start();
    virtual CParticleObject* Clone();

public:
    void SetParticle(const std::string& Name);
};

