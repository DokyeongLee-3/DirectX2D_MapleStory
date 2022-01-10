#pragma once

#include "GameObject/GameObject.h"
#include "Component/SpriteComponent.h"
#include "Component/ColliderBox2D.h"

class CMonsterOnion :
    public CGameObject
{
    friend class CScene;

protected:
    CMonsterOnion();
    CMonsterOnion(const CMonsterOnion& obj);
    virtual ~CMonsterOnion();

private:
    CSharedPtr<CSpriteComponent>    m_Sprite;
    CSharedPtr<CColliderBox2D>       m_Body;

public:
    virtual void Start();
    virtual bool Init();
    virtual void Update(float DeltaTime);
    virtual void PostUpdate(float DeltaTime);
    virtual CMonsterOnion* Clone();
    virtual void Save(FILE* File);
    virtual void Load(FILE* File);


private:
    void FlipAll(float DeltaTime);
    void NextAnimation(float DeltaTime);
};

