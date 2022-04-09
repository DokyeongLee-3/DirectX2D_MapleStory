#pragma once

#include "Monster.h"
#include "Component/SpriteComponent.h"
#include "Component/ColliderBox2D.h"

class CCuzco :
    public CMonster
{
    friend class CScene;

protected:
    CCuzco();
    CCuzco(const CCuzco& obj);
    virtual ~CCuzco();

private:
    CSharedPtr<CSpriteComponent>    m_Sprite;
    CSharedPtr<CColliderBox2D>     m_Body;

    class CPlayer2D* m_Target;

    Vector3     m_RotDirVector;

public:
    void SetTarget(class CPlayer2D* Target);

public:
    virtual void SetDamage(float Damage, bool Critical = false);

public:
    virtual bool Init();
    virtual void Start();
    virtual void Update(float DeltaTime);
    virtual void PostUpdate(float DeltaTime);
    virtual CCuzco* Clone();
    virtual void Save(FILE* File);
    virtual void Load(FILE* File);

public:
    CSpriteComponent* GetSpriteComponent()    const
    {
        return m_Sprite;
    }

    CColliderBox2D* GetBody()   const
    {
        return m_Body;
    }

public:
    void CollisionBeginCallback(const CollisionResult& Result);
    void CollisionEndCallback(const CollisionResult& Result);
};

