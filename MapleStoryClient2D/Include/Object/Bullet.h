#pragma once

#include "GameObject/GameObject.h"
#include "Component/ColliderBox2D.h"

class CBullet :
    public CGameObject
{
    friend class CScene;

protected:
    CBullet();
    CBullet(const CBullet& obj);
    virtual ~CBullet();

private:
    CSharedPtr<class CSpriteComponent>    m_Sprite;
    CSharedPtr<CColliderBox2D>       m_Body;
    float   m_Distance;
    float   m_Speed;


public:
    virtual bool Init();
    virtual void Update(float DeltaTime);
    virtual void PostUpdate(float DeltaTime);
    virtual CBullet* Clone();

public:
    void SetCollisionProfile(const std::string& Name);

public:
    void FlipAll(float DeltaTime);
    void CollisionBeginCallback(const CollisionResult& result);
    void CollisionEndCallback(const CollisionResult& result);
};


