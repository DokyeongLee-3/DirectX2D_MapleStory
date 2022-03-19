#pragma once

#include "GameObject/GameObject.h"
#include "Component/SpriteComponent.h"

class CZakumClapCircleShockWave :
    public CGameObject
{
    friend class CScene;

protected:
    CZakumClapCircleShockWave();
    CZakumClapCircleShockWave(const CZakumClapCircleShockWave& obj);
    virtual ~CZakumClapCircleShockWave();

private:
    CSharedPtr<CSpriteComponent>    m_Sprite;
    class CZakumBody* m_ShockWaveOwner;
    int                 m_OwnerArmID;

public:
    void SetShockWaveOwner(class CZakumBody* Zakum);
    void SetOwnerArmID(int ID)
    {
        m_OwnerArmID = ID;
    }

public:
    virtual void Start();
    virtual bool Init();
    virtual void Update(float DeltaTime);
    virtual void PostUpdate(float DeltaTime);
    virtual CZakumClapCircleShockWave* Clone();

public:
    void SetSpriteTexture(const std::string& Name, const TCHAR* FileName);
    virtual void Destroy();
};

