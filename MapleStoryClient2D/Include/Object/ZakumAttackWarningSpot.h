#pragma once

#include "GameObject/GameObject.h"
#include "Component/SpriteComponent.h"

class CZakumAttackWarningSpot :
    public CGameObject
{
    friend class CScene;

protected:
    CZakumAttackWarningSpot();
    CZakumAttackWarningSpot(const CZakumAttackWarningSpot& obj);
    virtual ~CZakumAttackWarningSpot();

private:
    CSharedPtr<CSpriteComponent>    m_Sprite;
    float                           m_ColorChangeTime;
    float                           m_AccTime;
    bool                            m_IsRedBaseColor;
    class CZakumBody*               m_WarningPointOwner;
    int                             m_ArmID;

public:
    void SetWarningPointOwner(class CZakumBody* Zakum);
    void SetArmID(int ID)
    {
        m_ArmID = ID;
    }

public:
    virtual void Start();
    virtual bool Init();
    virtual void Update(float DeltaTime);
    virtual void PostUpdate(float DeltaTime);
    virtual CZakumAttackWarningSpot* Clone();
};

