#pragma once

#include "GameObject/GameObject.h"
#include "Component/SpriteComponent.h"

class CZakumFogEffect :
    public CGameObject
{
    friend class CScene;

protected:
    CZakumFogEffect();
    CZakumFogEffect(const CZakumFogEffect& obj);
    virtual ~CZakumFogEffect();

private:
    CSharedPtr<CSpriteComponent>    m_Sprite;


public:
    virtual void Start();
    virtual bool Init();
    virtual void Update(float DeltaTime);
    virtual void PostUpdate(float DeltaTime);
    virtual CZakumFogEffect* Clone();

public:
    CSpriteComponent* GetSpriteComponent()   const
    {
        return m_Sprite;
    }
};

