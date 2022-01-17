#pragma once

#include "GameObject/GameObject.h"
#include "Component/SpriteComponent.h"
#include "Component/ColliderBox2D.h"

class CMonsterRadish :
    public CGameObject
{
    friend class CScene;

protected:
    CMonsterRadish();
    CMonsterRadish(const CMonsterRadish& obj);
    virtual ~CMonsterRadish();

private:
    CSharedPtr<CSpriteComponent>    m_Sprite;
    CSharedPtr<CColliderBox2D>       m_Body;

public:
    virtual void Start();
    virtual bool Init();
    virtual void Update(float DeltaTime);
    virtual void PostUpdate(float DeltaTime);
    virtual CMonsterRadish* Clone();
    virtual void Save(FILE* File);
    virtual void Load(FILE* File);


private:
    void FlipAll(float DeltaTime);
    void NextAnimation(float DeltaTime);
    void RotationZInv(float DeltaTime);
    void RotationZ(float DeltaTime);
};

