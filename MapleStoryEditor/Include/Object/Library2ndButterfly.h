#pragma once

#include "GameObject/GameObject.h"
#include "Component/SpriteComponent.h"

class CLibrary2ndButterfly :
    public CGameObject
{
    friend class CScene;

protected:
    CLibrary2ndButterfly();
    CLibrary2ndButterfly(const CLibrary2ndButterfly& obj);
    virtual ~CLibrary2ndButterfly();

private:
    CSharedPtr<CSpriteComponent>    m_Sprite;

public:
    virtual void Start();
    virtual bool Init();
    virtual void Update(float DeltaTime);
    virtual void PostUpdate(float DeltaTime);
    virtual CLibrary2ndButterfly* Clone();
    virtual void Save(FILE* File);
    virtual void Load(FILE* File);


private:
    void FlipAll(float DeltaTime);
};

