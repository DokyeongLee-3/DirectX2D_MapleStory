#pragma once

#include "GameObject/GameObject.h"
#include "Component/SpriteComponent.h"

class CSylphideLancerHitEffect :
    public CGameObject
{
    friend class CScene;

protected:
    CSylphideLancerHitEffect();
    CSylphideLancerHitEffect(const CSylphideLancerHitEffect& obj);
    virtual ~CSylphideLancerHitEffect();

private:
   CSpriteComponent*   m_Sprite;


public:
    virtual bool Init();
    virtual void Start();
    virtual void Update(float DeltaTime);
    virtual void PostUpdate(float DeltaTime);
    virtual CSylphideLancerHitEffect* Clone();

public:
    CSpriteComponent* GetSpriteComponent()    const
    {
        return m_Sprite;
    }

public:
    void EffectEnd();

};

