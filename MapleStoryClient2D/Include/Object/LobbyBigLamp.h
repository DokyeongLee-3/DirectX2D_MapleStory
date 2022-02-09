#pragma once

#include "GameObject/GameObject.h"
#include "Component/SpriteComponent.h"
#include "../Component/DragCollider.h"

class CLobbyBigLamp :
    public CGameObject
{
    friend class CScene;

protected:
    CLobbyBigLamp();
    CLobbyBigLamp(const CLobbyBigLamp& obj);
    virtual ~CLobbyBigLamp();

private:
    CSharedPtr<CSpriteComponent>    m_Sprite;
    CSharedPtr<CSpriteComponent>    m_HelixSprite;
    CSharedPtr<CSpriteComponent>    m_ButterflySprite;

public:
    virtual void Start();
    virtual bool Init();
    virtual void Update(float DeltaTime);
    virtual void PostUpdate(float DeltaTime);
    virtual CLobbyBigLamp* Clone();
    virtual void Save(FILE* File);
    virtual void Load(FILE* File);
};

