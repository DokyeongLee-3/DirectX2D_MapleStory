#pragma once

#include "GameObject/GameObject.h"
#include "Component/SpriteComponent.h"
#include "../Component/DragCollider.h"

class CLobbySmallLamp :
    public CGameObject
{
    friend class CScene;

protected:
    CLobbySmallLamp();
    CLobbySmallLamp(const CLobbySmallLamp& obj);
    virtual ~CLobbySmallLamp();

private:
    CSharedPtr<CSpriteComponent>    m_Sprite;
    CSharedPtr<CSpriteComponent>    m_HelixSprite;
    CSharedPtr<CSpriteComponent>    m_ButterflySprite;
    CSharedPtr<CSpriteComponent>    m_VerticalLightSprite;
    CSharedPtr<CSpriteComponent>    m_LampBrightImage;
    CSharedPtr<CDragCollider>      m_DragCollider;

public:
    virtual void Start();
    virtual bool Init();
    virtual void Update(float DeltaTime);
    virtual void PostUpdate(float DeltaTime);
    virtual CLobbySmallLamp* Clone();
    virtual void Save(FILE* File);
    virtual void Load(FILE* File);
};

