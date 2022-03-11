#pragma once

#include "GameObject/GameObject.h"
#include "Component/SpriteComponent.h"
#include "Component/ColliderBox2D.h"

class CTomb :
    public CGameObject
{
    friend class CScene;

protected:
    CTomb();
    CTomb(const CTomb& obj);
    virtual ~CTomb();

private:
    CSharedPtr<CSpriteComponent>    m_Sprite;
    CSharedPtr<CColliderBox2D>       m_Body;


public:
    virtual void Start();
    virtual bool Init();
    virtual void Update(float DeltaTime);
    virtual void PostUpdate(float DeltaTime);
    virtual CTomb* Clone();

public:
    void CollisionBeginCallback(const CollisionResult& Result);
};

