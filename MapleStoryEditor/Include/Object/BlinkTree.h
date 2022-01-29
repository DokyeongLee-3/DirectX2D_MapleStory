#pragma once

#include "GameObject/GameObject.h"
#include "Component/SpriteComponent.h"
#include "../Component/DragCollider.h"

class CBlinkTree :
    public CGameObject
{
    friend class CScene;

protected:
    CBlinkTree();
    CBlinkTree(const CBlinkTree& obj);
    virtual ~CBlinkTree();

private:
    CSharedPtr<CSpriteComponent>    m_Sprite;
    CSharedPtr<CDragCollider>      m_DragCollider;

public:
    virtual void Start();
    virtual bool Init();
    virtual void Update(float DeltaTime);
    virtual void PostUpdate(float DeltaTime);
    virtual CBlinkTree* Clone();
    virtual void Save(FILE* File);
    virtual void Load(FILE* File);


private:
    void FlipAll(float DeltaTime);
};

