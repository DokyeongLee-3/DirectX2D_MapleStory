#pragma once

#include "GameObject/GameObject.h"
#include "Component/SpriteComponent.h"
#include "Component/ColliderBox2D.h"

class CNPCAdobis :
    public CGameObject
{
    friend class CScene;

protected:
    CNPCAdobis();
    CNPCAdobis(const CNPCAdobis& obj);
    virtual ~CNPCAdobis();

private:
    CSharedPtr<CSpriteComponent>    m_Sprite;
    CSharedPtr<CColliderBox2D>       m_Body;


public:
    virtual void Start();
    virtual bool Init();
    virtual void Update(float DeltaTime);
    virtual void PostUpdate(float DeltaTime);
    virtual CNPCAdobis* Clone();

public:
    void CollisionMouseBeginCallback(const CollisionResult& Result);

public:
    virtual void Save(FILE* File);
    virtual void Load(FILE* File);
};


