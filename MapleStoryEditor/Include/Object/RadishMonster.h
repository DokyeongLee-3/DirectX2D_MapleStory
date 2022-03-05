#pragma once

#include "Monster.h"
#include "Component/SpriteComponent.h"
#include "Component/ColliderCircle.h"
#include "Component/WidgetComponent.h"
#include "Component/PaperBurnComponent.h"

class CRadishMonster :
    public CMonster
{
    friend class CScene;

protected:
    CRadishMonster();
    CRadishMonster(const CRadishMonster& obj);
    virtual ~CRadishMonster();

private:
    CSharedPtr<CSpriteComponent>    m_Sprite;
    CSharedPtr<CColliderCircle>     m_Body;

public:
    virtual bool Init();
    virtual void Update(float DeltaTime);
    virtual void PostUpdate(float DeltaTime);
    virtual CRadishMonster* Clone();
    virtual void Save(FILE* File);
    virtual void Load(FILE* File);

public:
    CSpriteComponent* GetSpriteComponent()    const
    {
        return m_Sprite;
    }
};

