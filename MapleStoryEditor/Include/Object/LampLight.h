#pragma once

#include "GameObject/GameObject.h"
#include "Component/SpriteComponent.h"
#include "Component/ColliderBox2D.h"

class CLampLight :
    public CGameObject
{
    friend class CScene;

protected:
    CLampLight();
    CLampLight(const CLampLight& obj);
    virtual ~CLampLight();

private:
    CSharedPtr<CSpriteComponent>    m_Sprite;
    CSharedPtr<CColliderBox2D>       m_Body;

public:
    virtual void Start();
    virtual bool Init();
    virtual void Update(float DeltaTime);
    virtual void PostUpdate(float DeltaTime);
    virtual CLampLight* Clone();
    virtual void Save(FILE* File);
    virtual void Load(FILE* File);
};

