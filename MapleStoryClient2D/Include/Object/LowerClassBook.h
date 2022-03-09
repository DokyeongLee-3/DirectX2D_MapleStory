#pragma once

#include "Monster.h"
#include "Component/WidgetComponent.h"
#include "Component/SpriteComponent.h"
#include "Component/ColliderBox2D.h"

class CLowerClassBook :
    public CMonster
{
    friend class CScene;

protected:
    CLowerClassBook();
    CLowerClassBook(const CLowerClassBook& obj);
    virtual ~CLowerClassBook();

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
    virtual void SetDamage(float Damage, bool Critical = false);
    virtual void PushDamageFont(float Damage, bool Critical = false);

public:
    virtual void Start();
    virtual bool Init();
    virtual void Update(float DeltaTime);
    virtual void PostUpdate(float DeltaTime);
    virtual CLowerClassBook* Clone();
    virtual void Save(FILE* File);
    virtual void Load(FILE* File);


private:
    void FlipAll(float DeltaTime);


public:
    void ReturnIdle();
    void FiniteState(float DeltaTime);   
    virtual void SetTrackState();


public:
    void CollisionBeginCallback(const CollisionResult& Result);
    void CollisionEndCallback(const CollisionResult& Result);

};

