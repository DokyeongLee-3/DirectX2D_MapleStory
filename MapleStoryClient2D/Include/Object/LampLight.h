#pragma once

#include "GameObject/GameObject.h"
#include "Component/SpriteComponent.h"

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


public:
    virtual void Start();
    virtual bool Init();
    virtual void Update(float DeltaTime);
    virtual void PostUpdate(float DeltaTime);
    virtual CLampLight* Clone();
    virtual void Save(FILE* File);
    virtual void Load(FILE* File);
};

