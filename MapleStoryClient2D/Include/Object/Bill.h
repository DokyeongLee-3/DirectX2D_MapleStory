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

public:
    int GetMoney()  const
    {
        return m_Money;
    }

public:
    virtual void Start();
    virtual bool Init();
    virtual void Update(float DeltaTime);
    virtual void PostUpdate(float DeltaTime);
    virtual CBill* Clone();
    virtual void Save(FILE* File);
    virtual void Load(FILE* File);

public:
    void CollisionBeginCallback(const CollisionResult& Result);
};

