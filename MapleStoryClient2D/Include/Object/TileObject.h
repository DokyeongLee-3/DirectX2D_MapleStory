#pragma once

#include "GameObject/GameObject.h"
#include "Component/SpriteComponent.h"
#include "../Component/DragCollider.h"

class CTileObject :
    public CGameObject
{
    friend class CScene;

protected:
    CTileObject();
    CTileObject(const CTileObject& obj);
    virtual ~CTileObject();

private:
    CColliderBox2D* m_Floor;

public:
    virtual void Start();
    virtual bool Init();
    virtual void Update(float DeltaTime);
    virtual void PostUpdate(float DeltaTime);
    virtual CTileObject* Clone();
    virtual void Save(FILE* File);
    virtual void Load(FILE* File);

};

