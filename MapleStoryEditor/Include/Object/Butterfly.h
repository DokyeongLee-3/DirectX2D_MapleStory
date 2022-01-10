#pragma once

#include "GameObject/GameObject.h"
#include "Component/SpriteComponent.h"

class CButterfly :
    public CGameObject
{
    friend class CScene;

protected:
    CButterfly();
    CButterfly(const CButterfly& obj);
    virtual ~CButterfly();

private:
    CSharedPtr<CSpriteComponent>    m_Sprite;

public:
    virtual void Start();
    virtual bool Init();
    virtual void Update(float DeltaTime);
    virtual void PostUpdate(float DeltaTime);
    virtual CButterfly* Clone();
    virtual void Save(FILE* File);
    virtual void Load(FILE* File);

};

