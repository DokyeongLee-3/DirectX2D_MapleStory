#pragma once

#include "GameObject/GameObject.h"
#include "Component/SpriteComponent.h"

class CZakumClapPinkShockWave :
    public CGameObject
{
    friend class CScene;

protected:
    CZakumClapPinkShockWave();
    CZakumClapPinkShockWave(const CZakumClapPinkShockWave& obj);
    virtual ~CZakumClapPinkShockWave();

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

    CSpriteComponent* GetSprite()   const
    {
        return m_Sprite;
    }

public:
    virtual void Start();
    virtual bool Init();
    virtual void Update(float DeltaTime);
    virtual void PostUpdate(float DeltaTime);
    virtual CZakumClapPinkShockWave* Clone();

public:
    void SetSpriteTexture(const std::string& Name, const TCHAR* FileName);
    virtual void Destroy();
};

