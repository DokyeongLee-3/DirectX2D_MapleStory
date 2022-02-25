#pragma once

#include "GameObject/GameObject.h"
#include "Component/SpriteComponent.h"
#include "../Component/DragCollider.h"

class CVerticalLampLight :
    public CGameObject
{
    friend class CScene;

protected:
    CVerticalLampLight();
    CVerticalLampLight(const CVerticalLampLight& obj);
    virtual ~CVerticalLampLight();

private:
    CSharedPtr<CSceneComponent>     m_RootComponent;
    CSharedPtr<CSpriteComponent>    m_LeftSprite;
    CSharedPtr<CSpriteComponent>    m_RightSprite;
    CSharedPtr<CDragCollider>      m_DragCollider;

public:
    virtual void Start();
    virtual bool Init();
    virtual void Update(float DeltaTime);
    virtual void PostUpdate(float DeltaTime);
    virtual CVerticalLampLight* Clone();
    virtual void Save(FILE* File);
    virtual void Load(FILE* File);
};

