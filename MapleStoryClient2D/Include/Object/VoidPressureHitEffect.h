#pragma once

#include "GameObject/GameObject.h"
#include "Component/SpriteComponent.h"

class CVoidPressureHitEffect :
    public CGameObject
{
    friend class CScene;

protected:
    CVoidPressureHitEffect();
    CVoidPressureHitEffect(const CVoidPressureHitEffect& obj);
    virtual ~CVoidPressureHitEffect();

private:
    CSpriteComponent* m_Sprite;


public:
    virtual bool Init();
    virtual void Start();
    virtual void Update(float DeltaTime);
    virtual void PostUpdate(float DeltaTime);
    virtual CVoidPressureHitEffect* Clone();

public:
    CSpriteComponent* GetSpriteComponent()    const
    {
        return m_Sprite;
    }

public:
    void EffectEnd();
};

