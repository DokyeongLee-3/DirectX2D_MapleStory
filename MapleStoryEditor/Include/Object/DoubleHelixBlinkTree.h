#pragma once

#include "GameObject/GameObject.h"
#include "Component/SpriteComponent.h"

class CDoubleHelixBlinkTree :
    public CGameObject
{
    friend class CScene;

protected:
    CDoubleHelixBlinkTree();
    CDoubleHelixBlinkTree(const CDoubleHelixBlinkTree& obj);
    virtual ~CDoubleHelixBlinkTree();

private:
    CSharedPtr<CSpriteComponent>    m_Sprite;

public:
    virtual void Start();
    virtual bool Init();
    virtual void Update(float DeltaTime);
    virtual void PostUpdate(float DeltaTime);
    virtual CDoubleHelixBlinkTree* Clone();
    virtual void Save(FILE* File);
    virtual void Load(FILE* File);

};

