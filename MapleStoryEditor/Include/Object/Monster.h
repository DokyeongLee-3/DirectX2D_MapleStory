#pragma once

#include "GameObject/GameObject.h"

class CMonster :
    public CGameObject
{
    friend class CScene;

protected:
    CMonster();
    CMonster(const CMonster& obj);
    virtual ~CMonster();


public:
    virtual void Start();
    virtual bool Init();
    virtual void Update(float DeltaTime);
    virtual void PostUpdate(float DeltaTime);
    virtual CMonster* Clone();
    virtual void Save(FILE* File);
    virtual void Load(FILE* File);
};

