#pragma once

#include "GameObject/GameObject.h"
#include "Component/ColliderBox2D.h"

class CDeathSide :
    public CGameObject
{
    friend class CScene;

protected:
    CDeathSide();
    CDeathSide(const CDeathSide& obj);
    virtual ~CDeathSide();

private:
    CSharedPtr<class CSpriteComponent>      m_Sprite;
    CSharedPtr<CColliderBox2D>              m_Body;
    int                                     m_HitCount;

public:
    virtual bool Init();
    virtual void Update(float DeltaTime);
    virtual void PostUpdate(float DeltaTime);
    virtual CDeathSide* Clone();

public:
    void SetCollisionProfile(const std::string& Name);
    void CollisionBeginCallback(const CollisionResult& result);
    void EnableCollider();
};

