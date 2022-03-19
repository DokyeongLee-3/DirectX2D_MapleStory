#pragma once

#include "GameObject/GameObject.h"
#include "Component/SpriteComponent.h"

class CZakumClapLightEffect :
    public CGameObject
{
    friend class CScene;

protected:
    CZakumClapLightEffect();
    CZakumClapLightEffect(const CZakumClapLightEffect& obj);
    virtual ~CZakumClapLightEffect();

private:
    CSharedPtr<CSpriteComponent>    m_Sprite;
    class CZakumBody* m_SmashLightOwner;
    int                 m_OwnerArmID;
public:
    void SetSmashLightOwner(class CZakumBody* Zakum);
    void SetOwnerArmID(int ID)
    {
        m_OwnerArmID = ID;
    }

public:
    virtual void Start();
    virtual bool Init();
    virtual void Update(float DeltaTime);
    virtual void PostUpdate(float DeltaTime);
    virtual CZakumClapLightEffect* Clone();

public:
    void SetLightSpriteTexture(const std::string& Name, const TCHAR* FileName);
};

