#pragma once

#include "GameObject/GameObject.h"
#include "Component/SpriteComponent.h"

class CDeathSideHitEffect :
    public CGameObject
{
    friend class CScene;

protected:
    CDeathSideHitEffect();
    CDeathSideHitEffect(const CDeathSideHitEffect& obj);
    virtual ~CDeathSideHitEffect();

private:
    CSpriteComponent* m_Sprite;


public:
    virtual bool Init();
    virtual void Start();
    virtual void Update(float DeltaTime);
    virtual void PostUpdate(float DeltaTime);
    virtual CDeathSideHitEffect* Clone();

public:
    CSpriteComponent* GetSpriteComponent()    const
    {
        return m_Sprite;
    }

public:
    void EffectEnd();
};

