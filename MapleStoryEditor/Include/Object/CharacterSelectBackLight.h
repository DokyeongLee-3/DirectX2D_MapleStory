#pragma once

#include "GameObject/GameObject.h"
#include "Component/SpriteComponent.h"

class CCharacterSelectBackLight :
    public CGameObject
{
    friend class CScene;

protected:
    CCharacterSelectBackLight();
    CCharacterSelectBackLight(const CCharacterSelectBackLight& obj);
    virtual ~CCharacterSelectBackLight();

private:
    CSharedPtr<CSpriteComponent>    m_Sprite;


public:
    virtual void Start();
    virtual bool Init();
    virtual void Update(float DeltaTime);
    virtual void PostUpdate(float DeltaTime);
    virtual CCharacterSelectBackLight* Clone();
    virtual void Save(FILE* File);
    virtual void Load(FILE* File);

public:
    void NextAnimation(float DeltaTime);
};

