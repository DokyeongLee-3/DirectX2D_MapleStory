#pragma once

#include "GameObject/GameObject.h"
#include "Component/SpriteComponent.h"

class CZakumDustEffect :
    public CGameObject
{
    friend class CScene;

protected:
    CZakumDustEffect();
    CZakumDustEffect(const CZakumDustEffect& obj);
    virtual ~CZakumDustEffect();

private:
    CSharedPtr<CSpriteComponent>    m_Sprite;


public:
    virtual void Start();
    virtual bool Init();
    virtual void Update(float DeltaTime);
    virtual void PostUpdate(float DeltaTime);
    virtual CZakumDustEffect* Clone();

public:
    CSpriteComponent* GetSpriteComponent()   const
    {
        return m_Sprite;
    }

public:
    void SetDestSpriteTexture(const std::string& Name, const TCHAR* FileName);

};

