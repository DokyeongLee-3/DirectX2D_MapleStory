#pragma once

#include "ColliderComponent.h"

class CColliderBox2D :
    public CColliderComponent
{
    friend class CGameObject;

protected:
    CColliderBox2D();
    CColliderBox2D(const CColliderBox2D& com);
    virtual ~CColliderBox2D();

protected:

public:
    virtual void Start();
    virtual bool Init();
    virtual void Update(float DeltaTime);
    virtual void PostUpdate(float DeltaTime);
    virtual void PrevRender();
    virtual void Render();
    virtual void PostRender();
    virtual CColliderBox2D* Clone();
    virtual void Save(FILE* File);
    virtual void Load(FILE* File);
};

