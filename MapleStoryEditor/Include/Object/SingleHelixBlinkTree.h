#pragma once

#include "GameObject/GameObject.h"
#include "Component/SpriteComponent.h"

class CSingleHelixBlinkTree :
    public CGameObject
{
    friend class CScene;

protected:
    CSingleHelixBlinkTree();
    CSingleHelixBlinkTree(const CSingleHelixBlinkTree& obj);
    virtual ~CSingleHelixBlinkTree();

private:
    CSharedPtr<CSpriteComponent>    m_Sprite;

public:
    virtual void Start();
    virtual bool Init();
    virtual void Update(float DeltaTime);
    virtual void PostUpdate(float DeltaTime);
    virtual CSingleHelixBlinkTree* Clone();
    virtual void Save(FILE* File);
    virtual void Load(FILE* File);

};

