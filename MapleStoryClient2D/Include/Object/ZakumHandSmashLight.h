#pragma once

#include "GameObject/GameObject.h"
#include "Component/SpriteComponent.h"

class CZakumHandSmashLight :
    public CGameObject
{
    friend class CScene;

protected:
    CZakumHandSmashLight();
    CZakumHandSmashLight(const CZakumHandSmashLight& obj);
    virtual ~CZakumHandSmashLight();

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
    virtual CZakumHandSmashLight* Clone();
    
public:
    void SetLightSpriteTexture(const std::string& Name, const TCHAR* FileName);
};

