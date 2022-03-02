#pragma once

#include "GameObject\GameObject.h"

class CLopeTileObject :
    public CGameObject
{
    friend class CScene;

protected:
    CLopeTileObject();
    CLopeTileObject(const CLopeTileObject& obj);
    virtual ~CLopeTileObject();


public:
    virtual void Start();
    virtual bool Init();
    virtual void Update(float DeltaTime);
    virtual void PostUpdate(float DeltaTime);
    virtual CLopeTileObject* Clone();
    virtual void Save(FILE* File);
    virtual void Load(FILE* File);

public:
    void DeleteAllDragCollider();
};

