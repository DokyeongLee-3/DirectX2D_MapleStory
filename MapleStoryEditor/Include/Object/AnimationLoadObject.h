#pragma once

#include "GameObject/GameObject.h"

class CAnimationLoadObject :
    public CGameObject
{
    friend class CScene;

protected:
    CAnimationLoadObject();
    CAnimationLoadObject(const CAnimationLoadObject& obj);
    virtual ~CAnimationLoadObject();

private:
    CSharedPtr<class CSpriteComponent>    m_Sprite;

public:
    class CSpriteComponent* GetSpriteComponent()    const
    {
        return m_Sprite;
    }

public:
    virtual bool Init();
    virtual void Update(float DeltaTime);
    virtual void PostUpdate(float DeltaTime);
    virtual CAnimationLoadObject* Clone();
};

