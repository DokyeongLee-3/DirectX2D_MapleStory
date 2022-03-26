#pragma once

#include "Monster.h"
#include "Component/SpriteComponent.h"
#include "Component/ColliderCircle.h"
#include "../ClientManager.h"

class COnionMonster :
    public CMonster
{
    friend class CScene;

protected:
    COnionMonster();
    COnionMonster(const COnionMonster& obj);
    virtual ~COnionMonster();

private:
    CSharedPtr<CSpriteComponent>    m_Sprite;
    CSharedPtr<CColliderCircle>      m_Body;

public:
    CSpriteComponent* GetSpriteComponent()    const
    {
        return m_Sprite;
    }

    CColliderCircle* GetBody()    const
    {
        return m_Body;
    }


public:
    virtual void SetDamage(float Damage, bool Critical = false);
    virtual void PushDamageFont(float Damage, bool Critical = false);

public:
    virtual void Start();
    virtual bool Init();
    virtual void Update(float DeltaTime);
    virtual void PostUpdate(float DeltaTime);
    virtual COnionMonster* Clone();
    virtual void Save(FILE* File);
    virtual void Load(FILE* File);


public:
    virtual void SetTrackState();
    void ReturnIdle();
    void FiniteState(float DeltaTime);
    void PushDeadPosition(const Vector3& Pos);
    class CItemOnion* DropItemOnion();

public:
    void CollisionBeginCallback(const CollisionResult& Result);
    void CollisionEndCallback(const CollisionResult& Result);
};

