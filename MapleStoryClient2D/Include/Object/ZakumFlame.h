#pragma once

#include "Monster.h"
#include "Component/SpriteComponent.h"
#include "Component/ColliderBox2D.h"

class CZakumFlame :
    public CMonster
{
    friend class CScene;

protected:
    CZakumFlame();
    CZakumFlame(const CZakumFlame& obj);
    virtual ~CZakumFlame();

private:
    CSharedPtr<CSpriteComponent>    m_Sprite;
    CSharedPtr<CColliderBox2D>      m_Body;

public:
    CSpriteComponent* GetSpriteComponent()    const
    {
        return m_Sprite;
    }

    CColliderBox2D* GetBody()    const
    {
        return m_Body;
    }


public:
   

public:
    virtual void Start();
    virtual bool Init();
    virtual void Update(float DeltaTime);
    virtual void PostUpdate(float DeltaTime);
    virtual CZakumFlame* Clone();
};

