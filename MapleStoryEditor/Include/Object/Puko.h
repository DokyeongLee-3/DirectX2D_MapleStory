#pragma once

#include "Monster.h"
#include "Component/SpriteComponent.h"
#include "Component/ColliderBox2D.h"


class CPuko :
    public CMonster
{
    friend class CScene;

protected:
    CPuko();
    CPuko(const CPuko& obj);
    virtual ~CPuko();

private:
    CSharedPtr<CSpriteComponent>    m_Sprite;
    CSharedPtr<CColliderBox2D>     m_Body;

public:
    virtual bool Init();
    virtual void Update(float DeltaTime);
    virtual void PostUpdate(float DeltaTime);
    virtual CPuko* Clone();
    virtual void Save(FILE* File);
    virtual void Load(FILE* File);

public:
    CSpriteComponent* GetSpriteComponent()    const
    {
        return m_Sprite;
    }
};

