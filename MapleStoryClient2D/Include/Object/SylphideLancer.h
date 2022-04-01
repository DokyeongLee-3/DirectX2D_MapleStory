#pragma once

#include "GameObject/GameObject.h"
#include "Component/ColliderBox2D.h"

class CSylphideLancer :
    public CGameObject
{
    friend class CScene;

protected:
    CSylphideLancer();
    CSylphideLancer(const CSylphideLancer& obj);
    virtual ~CSylphideLancer();

private:
    CSharedPtr<class CSpriteComponent>      m_Sprite;
    CSharedPtr<CColliderBox2D>              m_Body;
    float   m_Distance;
    float   m_Speed;
    int     m_LancerID;
    bool    m_ProduceLatterGroupCheck;

public:
    void SetLancerID(int ID)
    {
        m_LancerID = ID;
    }

    int GetLancerID()   const
    {
        return m_LancerID;
    }
    
    void SetSpeed(float Speed)
    {
        m_Speed = Speed;
    }

    float GetSpeed()    const
    {
        return m_Speed;
    }

public:
    virtual bool Init();
    virtual void Update(float DeltaTime);
    virtual void PostUpdate(float DeltaTime);
    virtual CSylphideLancer* Clone();

public:
    void SetCollisionProfile(const std::string& Name);

public:
    void FlipAll(float DeltaTime);
    void CollisionBeginCallback(const CollisionResult& result);
    void CollisionEndCallback(const CollisionResult& result);
};

