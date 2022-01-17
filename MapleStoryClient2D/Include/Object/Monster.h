#pragma once

#include "GameObject/GameObject.h"
#include "Component/SpriteComponent.h"
#include "Component/ColliderCircle.h"
#include "Component/WidgetComponent.h"


class CMonster :
    public CGameObject
{
    friend class CScene;

protected:
    CMonster();
    CMonster(const CMonster& obj);
    virtual ~CMonster();

private:
    CSharedPtr<CSpriteComponent>    m_Sprite;
    CSharedPtr<CColliderCircle>       m_Body;
    float   m_HP;

    CSharedPtr<CWidgetComponent>     m_SimpleHUDWidget;
    class CSimpleHUD* m_SimpleHUD;

public:
    virtual bool Init();
    virtual void Update(float DeltaTime);
    virtual void PostUpdate(float DeltaTime);
    virtual CMonster* Clone();

public:
    CSpriteComponent* GetSpriteComponent()    const
    {
        return m_Sprite;
    }

public:
    void CollisionCallback(const CollisionResult& result);


public:
    void Die();
    void ReturnIdle();
};

