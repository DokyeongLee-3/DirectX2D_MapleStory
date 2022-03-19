#pragma once

#include "GameObject/GameObject.h"
#include "Component/SpriteComponent.h"

class CZakumClapSmallCircleShockWave :
    public CGameObject
{
    friend class CScene;

protected:
    CZakumClapSmallCircleShockWave();
    CZakumClapSmallCircleShockWave(const CZakumClapSmallCircleShockWave& obj);
    virtual ~CZakumClapSmallCircleShockWave();

private:
    CSharedPtr<CSpriteComponent>    m_Sprite;
    class CZakumBody* m_SmashLightOwner;
    int                 m_OwnerArmID;
    float               m_AccTime;
    float               m_AlternativeColorTime;

public:

    void SetOwnerArmID(int ID)
    {
        m_OwnerArmID = ID;
    }

public:
    virtual void Start();
    virtual bool Init();
    virtual void Update(float DeltaTime);
    virtual void PostUpdate(float DeltaTime);
    virtual CZakumClapSmallCircleShockWave* Clone();

public:
    void SetSpriteTexture(const std::string& Name, const TCHAR* FileName);
};

