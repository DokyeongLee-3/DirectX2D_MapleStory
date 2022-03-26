#pragma once

#include "GameObject/GameObject.h"
#include "Component/SpriteComponent.h"
#include "Component/ColliderBox2D.h"

class CItemOnion :
    public CGameObject
{
    friend class CScene;

protected:
    CItemOnion();
    CItemOnion(const CItemOnion& obj);
    virtual ~CItemOnion();

private:
    CSharedPtr<CSpriteComponent>    m_Sprite;
    CSharedPtr<CColliderBox2D>       m_Body;

    int                             m_Money;
    float                           m_JumpForce;
    Item_Category                   m_ItemCategory;
    bool                            m_EatByPlayer;
    float                           m_AccTime;
    float                           m_FloatingDir;

public:
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
    virtual CItemOnion* Clone();

public:
    void CollisionBeginCallback(const CollisionResult& Result);

public:
    void GetByPlayer();
    void SetSpriteTexture(const std::string& Name, const TCHAR* FileName);
};

