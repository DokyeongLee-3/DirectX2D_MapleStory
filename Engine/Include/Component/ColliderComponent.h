#pragma once

#include "SceneComponent.h"

class CColliderComponent :
    public CSceneComponent
{
    friend class CGameObject;

protected:
    CColliderComponent();
    CColliderComponent(const CColliderComponent& com);
    virtual ~CColliderComponent();

protected:
    Collider_Type   m_ColliderType;

public:
    Collider_Type GetColliderType() const
    {
        return m_ColliderType;
    }

public:
    virtual void Start();
    virtual bool Init();
    virtual void Update(float DeltaTime);
    virtual void PostUpdate(float DeltaTime);
    virtual void PrevRender();
    virtual void Render();
    virtual void PostRender();
    virtual CColliderComponent* Clone();
    virtual void Save(FILE* File);
    virtual void Load(FILE* File);
};

