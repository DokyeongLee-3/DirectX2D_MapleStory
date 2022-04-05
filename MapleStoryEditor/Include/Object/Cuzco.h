#pragma once

#include "Monster.h"
#include "Component/SpriteComponent.h"
#include "Component/ColliderBox2D.h"

class CCuzco :
    public CMonster
{
    friend class CScene;

protected:
    CCuzco();
    CCuzco(const CCuzco& obj);
    virtual ~CCuzco();

private:
    CSharedPtr<CSpriteComponent>    m_Sprite;
    CSharedPtr<CColliderBox2D>     m_Body;

public:
    virtual bool Init();
    virtual void Update(float DeltaTime);
    virtual void PostUpdate(float DeltaTime);
    virtual CCuzco* Clone();
    virtual void Save(FILE* File);
    virtual void Load(FILE* File);

public:
    CSpriteComponent* GetSpriteComponent()    const
    {
        return m_Sprite;
    }
};

