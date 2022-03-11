#pragma once

#include "GameObject/GameObject.h"
#include "Component/SpriteComponent.h"
#include "Component/ColliderBox2D.h"

class CBill :
    public CGameObject
{
    friend class CScene;

protected:
    CBill();
    CBill(const CBill& obj);
    virtual ~CBill();

private:
    CSharedPtr<CSpriteComponent>    m_Sprite;
    CSharedPtr<CColliderBox2D>       m_Body;

    int                             m_Money;
    float                           m_JumpForce;
    Item_Category                   m_ItemCategory;
    bool                            m_EatByPlayer;

public:
    int GetMoney()  const
    {
        return m_Money;
    }

    bool GetEatByPlayer()  const
    {
        return m_EatByPlayer;
    }

    void SetEatByPlayer(bool Eat)
    {
        m_EatByPlayer = Eat;
    }

public:
    virtual void Start();
    virtual bool Init();
    virtual void Update(float DeltaTime);
    virtual void PostUpdate(float DeltaTime);
    virtual CBill* Clone();

public:
    void CollisionBeginCallback(const CollisionResult& Result);

public:
    void GetByPlayer();
};

