#pragma once

#include "GameObject/GameObject.h"
#include "Component/SpriteComponent.h"
#include "Component/ColliderCircle.h"

class CVoidPressure :
    public CGameObject
{
    friend class CScene;

protected:
    CVoidPressure();
    CVoidPressure(const CVoidPressure& obj);
    virtual ~CVoidPressure();

private:
    CSharedPtr<class CSpriteComponent>      m_Sprite;
    CSharedPtr<CColliderCircle>             m_Body;
   
    float                                   m_Distance; // 공격하는 구체가 움직일 수 있는 폭의 크기
    Vector3                                 m_OriginPos;

    float                                   m_CollisionFrequency;
    float                                   m_AccCollisionFrequency;
    float                                   m_VoidPressureLifeSpan; // CGameObject의 m_LifeSpan은 아예 수명이 다 되면 Destroy하게 돼서 VoidPressure만의 LifeSpan 멤버 사용
    bool                                    m_OnDestroy;

public:
    virtual bool Init();
    virtual void Update(float DeltaTime);
    virtual void PostUpdate(float DeltaTime);
    virtual CVoidPressure* Clone();

public:
    void SetOriginPos(const Vector3& Pos)
    {
        m_OriginPos = Pos;
    }

    void SetOriginPos(float X, float Y, float Z)
    {
        m_OriginPos = Vector3(X, Y, Z);
    }

    Vector3 GetOriginPos()  const
    {
        return m_OriginPos;
    }

    void SetVoidPressureLifeSpan(float LifeSpan)
    {
        m_VoidPressureLifeSpan = LifeSpan;
    }

    bool GetOnDestroy() const
    {
        return m_OnDestroy;
    }

    void SetOnDestroy(bool OnDestroy)
    {
        m_OnDestroy = OnDestroy;
    }

public:
    void SetCollisionProfile(const std::string& Name);

public:
    virtual void FlipAll(float DeltaTime);
    void CollisionBeginCallback(const CollisionResult& result);
    void CollisionEndCallback(const CollisionResult& result);
};

