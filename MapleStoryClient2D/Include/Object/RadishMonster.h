#pragma once

#include "Monster.h"
#include "Component/SpriteComponent.h"
#include "Component/ColliderBox2D.h"
#include "Component/WidgetComponent.h"
#include "Component/PaperBurnComponent.h"

class CRadishMonster :
    public CMonster
{
    friend class CScene;

protected:
    CRadishMonster();
    CRadishMonster(const CRadishMonster& obj);
    virtual ~CRadishMonster();

private:
    CSharedPtr<CSpriteComponent>    m_Sprite;
    CSharedPtr<CColliderBox2D>     m_Body;

public:
    virtual void Start();
    virtual bool Init();
    virtual void Update(float DeltaTime);
    virtual void PostUpdate(float DeltaTime);
    virtual CRadishMonster* Clone();
    virtual void Save(FILE* File);
    virtual void Load(FILE* File);

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
    void ReturnIdle();
    void FiniteState(float DeltaTime);
    virtual void SetTrackState();
    class CItemRadish* DropItemRadish();

public:
    void CollisionBeginCallback(const CollisionResult& Result);
    void CollisionEndCallback(const CollisionResult& Result);
};

