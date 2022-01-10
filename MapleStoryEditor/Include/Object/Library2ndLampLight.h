#pragma once

#include "GameObject/GameObject.h"
#include "Component/SpriteComponent.h"

class CLibrary2ndLampLight :
    public CGameObject
{
    friend class CScene;

protected:
    CLibrary2ndLampLight();
    CLibrary2ndLampLight(const CLibrary2ndLampLight& obj);
    virtual ~CLibrary2ndLampLight();

private:
    CSharedPtr<CSpriteComponent>    m_Sprite;

public:
    virtual void Start();
    virtual bool Init();
    virtual void Update(float DeltaTime);
    virtual void PostUpdate(float DeltaTime);
    virtual CLibrary2ndLampLight* Clone();
    virtual void Save(FILE* File);
    virtual void Load(FILE* File);


private:
    void FlipAll(float DeltaTime);
};

